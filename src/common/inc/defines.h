/*
 * Copyright 2010 OSLL osll@osll.spb.ru
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

/*!
 * \file defines.h
 * \brief Header with defines
 *
 * File description
 *
 * PROJ: OSLL/wikigps
 * ---------------------------------------------------------------- */

#ifndef _defines_H_CB67F287_A4BC_4745_9700_14421ED3AE3D_INCLUDED_
#define _defines_H_CB67F287_A4BC_4745_9700_14421ED3AE3D_INCLUDED_
#include <QString>
#include <QPoint>

QString getServerUrl();
void setServerUrl(QString serverUrl);
int getServerPort();
void setServerPort(int port);

// "demo"
#define SETTINGS_STORAGE_FILENAME "/opt/geo2tag/geo2tag.conf"

//#define GEO2TAG_VERSION "0.16"
//#define DATABASE_NAME "geo2tag"

#define SUBSCRIBE_HTTP_URL        getServerUrl()+QString("service/subscribe")
#define UNSUBSCRIBE_HTTP_URL      getServerUrl()+QString("service/unsubscribe")
#define APPLY_HTTP_URL            getServerUrl()+QString("service/writeTag")
#define FEED_HTTP_URL             getServerUrl()+QString("service/loadTags")
#define SUBSCRIBED_LIST_HTTP_URL  getServerUrl()+QString("service/subscribed")
#define AVAILABLE_LIST_HTTP_URL   getServerUrl()+QString("service/channels")
#define REGISTER_USER_HTTP_URL    getServerUrl()+QString("service/registerUser")
#define LOGIN_HTTP_URL            getServerUrl()+QString("service/login")
#define APPLY_CHANNEL_HTTP_URL    getServerUrl()+QString("service/addChannel")
#define ADD_USER_HTTP_URL         getServerUrl()+QString("service/addUser")
#define VERSION_HTTP_URL          getServerUrl()+QString("service/version")
#define DELETE_USER_HTTP_URL      getServerUrl()+QString("service/deleteUser")

#define GPS_MODELLER_FILE         "/usr/share/wikigps/helsinki.gpx"
//#define CONFIG_FILE               "/usr/share/wikigps/wikigps.conf"

// Default values
#define DEFAULT_SERVER "http://tracks.osll.spb.ru:81/"
#define DEFAULT_PORT 81
#define PASSWORD_SALT "geo2tag_salt"

#define DEFAULT_RADIUS            30
#define DEFAULT_USER_NAME         "Mark"
#define DEFAULT_USER_PASSWORD     "test"
#define DEFAULT_CHANNEL           "default"
#define DEFAULT_TOKEN             "2154086390Ivan1128557755"

#define DEFAULT_TMP_USER_TIMELIFE "2 days"
#define DEFAULT_SESSION_TIMELIFE  3     // days

#define DEFAULT_LATITUDE   ((double) 59.91446)
#define DEFAULT_LONGITUDE  ((double) 30.489442)

#define DEFAULT_PASSWORD_QUALITY_CHECK false
#define MINIMAL_PASSWORD_LENGTH 6

//#define TRACKER_TAG_LABEL   "tracker's tag"
//#define TRACKER_MSECS_TIMEOUT 500

#define DEFAULT_EMAIL_SUBJECT     "Registration confirmation"
#define DEFAULT_EMAIL_BODY        "This will go into the body of the mail."

#define DEFAULT_DB_UPDATE_INTERVAL 10000
#define DEFAULT_TRANSACTION_DIFF_TO_SYNC 0

#ifndef Q_WS_SYMBIAN
extern uint qHash(const QPoint & p);
#endif
//_defines_H_CB67F287_A4BC_4745_9700_14421ED3AE3D_INCLUDED_
#endif

/* ===[ End of file $HeadURL$ ]=== */
