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
 * \file DbSession.h
 * \brief Header of DbSession
 * \todo add comment here
 *
 * File description
 *
 * PROJ: OSLL/geo2tag
 * ---------------------------------------------------------------- */


#ifndef _DbSession_H_9BF6A8FE_DA47_4F7A_B008_2EA2842C490F_INCLUDED_
#define _DbSession_H_9BF6A8FE_DA47_4F7A_B008_2EA2842C490F_INCLUDED_

#include <syslog.h>
#include "DbConn.h"
#include "DataMarks.h"
#include "Channel.h"
#include "Thread.h"
#include "SwMr.h"
#include "Sleep.h"

namespace common
{
  template<typename T>
  class UpdateThread: public Thread::CThread
  {
    bool m_needExit;
    
    T*  m_instance;

    void thread()
    {
      while(!m_needExit)
      {
        syslog(LOG_INFO, "update thread: updating data");
        m_instance->loadData();
        mSleep(10000);
      }
      syslog(LOG_INFO, "update thread: exiting...");
    }
    public:
      UpdateThread(T* instance):m_needExit(false), m_instance(instance)
      {
        start();
      }

      void shutdown()
      {
        m_needExit = true;
      }
  };
  
  /*!
   * \brief session with database
   */
  class DbSession: protected ODBC::CDbEnv, public ODBC::CDbConn
  {
    friend class UpdateThread<DbSession>;

    CHandlePtr<DataMarks> m_marks;
    CHandlePtr<Channels> m_channels;
    CHandlePtr<std::vector<CHandlePtr<common::User> > > m_users;
    CHandlePtr<Thread::CThread> m_updateThread;

    void storeChannel(CHandlePtr<common::Channel> c); //! this routine will be in private area

    void loadUsers();
    void loadChannels();
    void loadMarks();
    void loadRelations();

    void saveChannels();
    void saveMarks();
    void saveRelations();
  
  protected:

    DbSession();
    
    void loadData();

    void saveData();

  public:
 

    ~DbSession();
    
    CHandlePtr<DataMarks> getMarks() const;

    CHandlePtr<Channels> getChannels() const;

    void storeMark(CHandlePtr<common::DataMark> m);
    
    CHandlePtr<std::vector<CHandlePtr<common::User> > > getUsers() const;
    
    void subscribe(const std::string& userName, const std::string &channelName);

    void updateChannel(unsigned long long channel_id,  CHandlePtr<common::DataMark> m); 

    static DbSession& getInstance();

  private:    
    DbSession(const DbSession& obj);
    DbSession& operator=(const DbSession& obj);

  }; // class DbSession
  
} // namespace common

#endif //_DbSession_H_9BF6A8FE_DA47_4F7A_B008_2EA2842C490F_INCLUDED_

/* ===[ End of file ]=== */
