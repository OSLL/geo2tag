/*
 * Copyright 2012  Kirill Krinkin  kirill.krinkin@gmail.com
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
 * THIS SOFTWARE IS PROVIDED BY THE AUTHOR AS IS'' AND ANY EXPRESS OR
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
 * \file Log.h
 * \brief Header of Log
 * \todo add comment here
 *
 * File description
 *
 * PROJ: OSLL/geo2tag
 * ---------------------------------------------------------------- */


#ifndef _Log_H_15CB69AA_AD4A_4920_9586_22AD73133FD5_INCLUDED_
#define _Log_H_15CB69AA_AD4A_4920_9586_22AD73133FD5_INCLUDED_

#include <stdio.h>

namespace
{

typedef enum
{
    DEFAULT=0,
    MAX_FACILITY_VALUE
} FACILITY;

typedef enum
{
    FATAL,
    ERROR,
    WARNING,
    INFO,
    MAX_SEVERIY_VALUE
} SEVERITY;


const char* ___log_facility_name[MAX_FACILITY_VALUE] = {"Default"};
const char* ___log_severity_name[MAX_SEVERIY_VALUE] = {
    "F",
    "E",
    "W",
    "I",
};

/*!
   * Class description. May use HTML formatting
   *
   */
class Log
{

    SEVERITY m_severity;
    FACILITY m_facility;
    const char *m_file;
    const char *m_func;
    int m_line;

public:
    Log(const char *file, int line, const char *func, SEVERITY severity = INFO, FACILITY facility = DEFAULT) :
        m_severity(severity),
        m_facility(facility),
        m_file(file),
        m_func(func),
        m_line(line) {}

    void write(const char *msg)
    {
        //will be fixed
        printf("%s: [%s] %s +%d :%s msg=\"%s\"\n",
               ___log_severity_name[m_severity], ___log_facility_name[m_facility],
               m_file,m_line,m_func,msg);
    }

private:
    Log(const Log& obj);
    Log& operator=(const Log& obj);

}; // class Log

//should be in one line
//#define LOG(msg,severity,facility) { Log __l(__FILE__,__LINE__,__FUNCSIG__,(severity),(facility)); __l.write(msg); }

//should be in one line
#define LOG(msg) { Log __l(__FILE__,__LINE__,__FUNCTION__,INFO,DEFAULT); __l.write(msg); }


} // namespace Geo

#endif //_Log_H_15CB69AA_AD4A_4920_9586_22AD73133FD5_INCLUDED_
