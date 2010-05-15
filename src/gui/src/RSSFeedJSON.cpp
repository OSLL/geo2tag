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

/*! ---------------------------------------------------------------
 * $Id$ 
 *
 * \file RSSFeedJSON.cpp
 * \brief RSSFeedJSON implementation
 *
 * File description
 *
 * PROJ: OSLL/geo2tag
 * ---------------------------------------------------------------- */

#include "RSSFeedJSON.h"
#include "User.h"
#include "OnLineInformation.h"
#include "qjson/parser.h"
#include <QVariant>
#include <QDebug>
#include "Time.h"
#include <time.h>
static std::map<std::string, CHandlePtr<common::User> > s_users;

namespace stub
{
    class User : public common::User
    {
    public:
        User(const std::string& name) : common::User(name, name)
        {
        }

        virtual ~User()
        {
        }
    };

    class Channel: public common::Channel
    {
    public:
      Channel(const std::string& name): common::Channel(name,"undefined channel", "not available", makeHandle(new common::DataMarks()))
      {
      }

      virtual ~Channel()
      {
      }
    };

    class DataMark: public common::DataMark
    {
    public:
        DataMark (double latitude, double longitude, const std::string &label,
                  const std::string &description, const std::string &link,
                  const std::string &channel, const CHandlePtr<common::User>& user, const CTime Time)
                      : common::DataMark(latitude, longitude, label, description, link, Time, user )
        {
            CHandlePtr<common::Channel> foundChannel = GUI::OnLineInformation::getInstance().findChannel(channel);
            if (foundChannel == 0)
            {
                m_channel = makeHandle(new stub::Channel(std::string("undefined channel")));
            }
            else
            {
                m_channel = foundChannel;
            }
        }

        virtual ~DataMark()
        {
        }
    };
}

RSSFeedJSON::RSSFeedJSON(const std::stringstream& json)
{
    m_marks=makeHandle(new common::DataMarks);
    m_json << json.str();
    convertInMarks();
}

RSSFeedJSON::~RSSFeedJSON()
{

}

void RSSFeedJSON::convertInMarks()
{
    QJson::Parser parser;
    bool ok;
    QByteArray json(m_json.str().c_str());
    QVariantMap result = parser.parse(json, &ok).toMap();
    if (!ok)
    {
        qFatal("An error occured during parsing json with rss feed");
        return;
    }
    else
    {
        QVariantMap rss = result["rss"].toMap();
        QVariantMap channelVariant = rss["channel"].toMap();
        QVariantList marksList = channelVariant["item"].toList();
        int size = marksList.size();
        for (int i = 0; i < size; i++)
        {
            QVariantMap markMap = marksList.at(i).toMap();

            std::string title = markMap["title"].toString().toStdString();
            std::string link = markMap["link"].toString().toStdString();
            std::string description = markMap["description"].toString().toStdString();
            double latitude = markMap["latitude"].toString().toDouble();
            double longitude = markMap["longitude"].toString().toDouble();
            //const json::Array& tags = interpreter["rss"]["channel"]["item"][i]["tags"];
            std::string guid = markMap["user"].toString().toStdString();
            std::string login = markMap["user"].toString().toStdString();
	    std::string timeStr =  markMap["pubDate"].toString().toStdString();
            struct tm timeTm;
            strptime(timeStr.c_str(),"%d %b %Y %H:%M:%S",&timeTm);
	    CTime Time(timeTm);    
            CHandlePtr<common::User> user;
            if(s_users.count(guid) == 0)
            {
                // We recieved new user, we have to add it to our containers
                user = makeHandle(new stub::User(login));
                s_users[login]=user;
                //OnLineInformation::getInstance().getUsers().push_back(user); // <= This is bug. I think we can find another way.
                // Or does we realy need to store users in OnLineInformation instance
            }
            else
            {
                user = s_users.find(login)->second;
            }
            std::string channel = markMap["channel"].toString().toStdString();

            CHandlePtr<common::DataMark> mark =
                    makeHandle(new stub::DataMark(latitude, longitude, title, description, link, channel, user,Time));
            m_marks->push_back(mark);
            qDebug() << "converted using QJson";
        }

    }

}

//void RSSFeedJSON::convertInMarks()
//{
//    json::Element elemRoot;
//    try
//    {
//        json::Reader::Read(elemRoot,m_json);
//        json::QuickInterpreter interpreter(elemRoot);
//        const json::String& title = interpreter["rss"]["channel"]["title"];
//        const json::String& link = interpreter["rss"]["channel"]["link"];
//        const json::String& description = interpreter["rss"]["channel"]["description"];
//        const json::String& language = interpreter["rss"]["channel"]["language"];
//        const json::String& pubDate = interpreter["rss"]["channel"]["pubDate"];
//        const json::Array& marks = interpreter["rss"]["channel"]["item"];
//
//        int length=marks.Size();
//        for (int i = 0; i < length; i++)
//        {
//            const json::String& title = interpreter["rss"]["channel"]["item"][i]["title"];
//            const json::String& link = interpreter["rss"]["channel"]["item"][i]["link"];
//            const json::String& description = interpreter["rss"]["channel"]["item"][i]["description"];
//            const json::Number& latitude = interpreter["rss"]["channel"]["item"][i]["latitude"];
//            const json::Number& longitude = interpreter["rss"]["channel"]["item"][i]["longitude"];
//            //const json::Array& tags = interpreter["rss"]["channel"]["item"][i]["tags"];
//            const json::String& guid = interpreter["rss"]["channel"]["item"][i]["user"];
//            std::string login((const json::String&)(interpreter["rss"]["channel"]["item"][i]["user"]));
//            CHandlePtr<common::User> user;
//            if(s_users.count(guid) == 0)
//            {
//              // We recieved new user, we have to add it to our containers
//              user = makeHandle(new stub::User(login));
//              s_users[login]=user;
//              //OnLineInformation::getInstance().getUsers().push_back(user); // <= This is bug. I think we can find another way.
//                                                                             // Or does we realy need to store users in OnLineInformation instance
//            }
//            else
//            {
//              user = s_users.find(login)->second;
//            }
//            const json::String& channel = interpreter["rss"]["channel"]["item"][i]["channel"];
//
//            CHandlePtr<common::DataMark> mark =
//                    makeHandle(new stub::DataMark(latitude, longitude, title, description, link, channel, user));
//            m_marks->push_back(mark);
//            qDebug() << "converted";
//        }
//    }
//    catch (const json::Exception& x)
//    {
//
//    }
//
//}

const CHandlePtr<common::DataMarks>& RSSFeedJSON::getMarks() const
{
        return m_marks;
}

/* ===[ End of file $HeadURL$ ]=== */
