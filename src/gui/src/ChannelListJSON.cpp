/*
 * Copyright 2010  Open Source & Linux Lab (OSLL) osll@osll.spb.ru
 *
 * Redistribution and use in source and binary forms, with or without
 * modification, are permitted provided that the following conditions
 * are met:
 *
 * 1. Redistributions of source code must retain the above copyright
 *    notice, this list of conditions and the following disclaimer.
 * 2. Redistributions in binary form must reproduce the above copyright
 *    notice, this list of conditions and the following disclaimer in the
 *    documentation and/or other materials provided with the distribution.
 *
 * THIS SOFTWARE IS PROVIDED BY THE AUTHOR ``AS IS'' AND ANY EXPRESS OR
 * IMPLIED WARRANTIES, INCLUDING, BUT NOT LIMITED TO, THE IMPLIED WARRANTIES
 * OF MERCHANTABILITY AND FITNESS FOR A PARTICULAR PURPOSE ARE DISCLAIMED.
 * IN NO EVENT SHALL THE AUTHOR BE LIABLE FOR ANY DIRECT, INDIRECT,
 * INCIDENTAL, SPECIAL, EXEMPLARY, OR CONSEQUENTIAL DAMAGES (INCLUDING, BUT
 * NOT LIMITED TO, PROCUREMENT OF SUBSTITUTE GOODS OR SERVICES; LOSS OF USE,
 * DATA, OR PROFITS; OR BUSINESS INTERRUPTION) HOWEVER CAUSED AND ON ANY
 * THEORY OF LIABILITY, WHETHER IN CONTRACT, STRICT LIABILITY, OR TORT
 * (INCLUDING NEGLIGENCE OR OTHERWISE) ARISING IN ANY WAY OUT OF THE USE OF
 * THIS SOFTWARE, EVEN IF ADVISED OF THE POSSIBILITY OF SUCH DAMAGE.
 *
 * 3. The name of the author may not be used to endorse or promote
 *    products derived from this software without specific prior written
 *    permission.
 *
 * The advertising clause requiring mention in adverts must never be included.
 */
/*  */
/*!
 * \file ChannelListJSON.cpp
 * \brief implementation of Channels2JSON
 *
 * File description
 *
 *  PROJ: OSLL/geo2tag
 *
 *
 * ------------------------------------------------------------------------ */

#include "ChannelListJSON.h"


namespace GUI
{
    namespace stub
    {
        class Channel: public common::Channel
        {
        public:
            Channel(const std::string& name, const std::string& description): common::Channel(name,description, "not available", makeHandle(new common::DataMarks()))
            {
            }

            virtual ~Channel()
            {
            }
        };
    }

    ChannelListJSON::ChannelListJSON(const CHandlePtr<common::Channels>& channels):m_channels(channels)
    {
        convertInJSON();
    }

    ChannelListJSON::ChannelListJSON(const std::stringstream& json)
    {
        m_channels=makeHandle(new common::Channels);
        m_json << json.str();
        convertInChannels();
    }

    void ChannelListJSON::convertInJSON()
    {
        assert(m_channels);

        QVariantList channelsList;
        int j=0;
        for (common::Channels::iterator i=m_channels->begin();i!=m_channels->end();i++ )
        {
            QVariantMap channelMap;
            channelMap.insert("name", QString((*i)->getName().c_str()));
            channelMap.insert("description", QString((*i)->getDescription().c_str()));
            QVariantList tagsList;
            channelMap.insert("tags", tagsList);
            channelsList << channelMap;
            j++;
        }

        QVariantMap channelsMap;
        channelsMap.insert("channels", channelsList);

        QJson::Serializer serializer;
        QByteArray jsonArray = serializer.serialize(channelsMap);
        QString jsonString(jsonArray);
        qDebug() << " json channels list: " << jsonString;
        m_json << jsonString.toStdString();
    }

    void ChannelListJSON::convertInChannels()
    {
        QJson::Parser parser;
        bool ok;
        QByteArray json(m_json.str().c_str());
        QVariantMap result = parser.parse(json, &ok).toMap();
        if (!ok)
        {
            qFatal("An error occured during parsing json with channel list");
            return;
        }
        else
        {
            QVariantList channels = result["channels"].toList();
            int size = channels.size();
            for (int i = 0; i < size; i++)
            {
                QVariantMap channelVariant = channels.at(i).toMap();
                std::string name = channelVariant["name"].toString().toStdString();
                std::string description = channelVariant["description"].toString().toStdString();
                CHandlePtr<common::Channel> channel=makeHandle(new stub::Channel(std::string(name), std::string(description)));
                m_channels->push_back(channel);
            }
        }
        convertInJSON();
    }

    const std::stringstream& ChannelListJSON::getJSON() const
    {
        return m_json;
    }

    const CHandlePtr<common::Channels>& ChannelListJSON::getChannels() const
    {
        return m_channels;
    }
} // namespace GUI

/* ===[ End of file $HeadURL$ ]=== */
