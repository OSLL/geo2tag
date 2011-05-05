/*
 * Copyright 2010  OSLL osll@osll.spb.ru
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
/*!
 * \file Query.cpp
 * \brief implmentation of query
 *
 * File description
 *
 *  PROJ: OSLL/geo2tag
 *
 *
 * ------------------------------------------------------------------------ */

#include <iostream>
#include <sstream>
#include <syslog.h>
#include <string.h>
#include "query.h"

Query::Query(const QString &s/*,Stream& stm*/)
//{
//    enum States {
//        READ_NAME,
//        READ_VALUE
//    } state;

//    size_t j = 0;
//    QString paramName(""), paramValue("");
//    state = READ_NAME;
//    for (size_t i = 0; i < s.size(); ++i) {
//        if (s[i] == '=') {
//            paramName = s.substr(j, i - j);
//            j = i + 1;
//            state=READ_VALUE;
//        }
//        if (s[i] == '&') {
//            if (state == READ_NAME) {
//                // variable without value
//                m_params[s.substr(j, i - j)]="";
//                j = i + 1;
//                continue;
//            }
//            paramValue = s.substr(j, i - j );
//            j = i + 1;
//            m_params[paramName] = paramValue;
//            state=READ_NAME;
//        }
//    }
//    if (paramName.size()) {
//        paramValue = s.substr(j, s.size() - j);
//        m_params[paramName] = paramValue;
//    }
//    QString tmp="";
//    stm >> tmp;
//    m_query << tmp;
//    std::ostringstream ss;
//    for(std::map<QString, QString>::const_iterator it=m_params.begin();it!=m_params.end(); ++it)
//      ss << it->first << "=" << it->second << " &&& ";
//    // syslog(LOG_INFO, ss.str().c_str());

//    if (getParam("query")==QString("subscribe")) m_type=SUBSCRIBE;
//    else if (getParam("query")==QString("channels")) m_type=AVAILABLE_LIST;
//    else if (getParam("query")==QString("subscribed")) m_type=SUBSCRIBED_LIST;
//    else if (getParam("query")==QString("applymark")) m_type=APPLYMARK;
//    else if (getParam("query")==QString("rss"))m_type=RSSFEED;
//    else if (getParam("query")==QString("unsubscribe"))m_type=UNSUBSCRIBE;
//    else if (getParam("query")==QString("login"))m_type=LOGIN;
//    else if (getParam("query")==QString("addChannel"))m_type=APPLYCHANNEL;
//    else if (getParam("query")==QString("addUser"))m_type=APPLYUSER;
//    else  m_type=UNKNOWN;
}


const QString& Query::getParam(const QString& param) const
{
  static QString nothing("");
  if(m_params.count(param))
    return m_params.find(param)->second;
  return nothing;
}


QueryType Query::getType() const
{
  return m_type;
}


const QStringstream& Query::getStream() const
{
  return m_query;
}


/* ===[ End of file  ]=== */
