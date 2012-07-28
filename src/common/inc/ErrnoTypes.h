/*
 * Copyright 2011  bac1ca  bac1ca89@gmail.com
 *
 * Redistribution and use in source and binary forms , with or without
 * modification , are permitted provided that the following conditions
 * are met:
 *
 * 1. Redistributions of source code must retain the above copyright
 *    notice , this list of conditions and the following disclaimer.
 * 2. Redistributions in binary form must reproduce the above copyright
 *    notice , this list of conditions and the following disclaimer in the
 *    documentation and/or other materials provided with the distribution.
 *
 * THIS SOFTWARE IS PROVIDED BY THE AUTHOR AS IS'' AND ANY EXPRESS OR
 * IMPLIED WARRANTIES , INCLUDING , BUT NOT LIMITED TO , THE IMPLIED WARRANTIES
 * OF MERCHANTABILITY AND FITNESS FOR A PARTICULAR PURPOSE ARE DISCLAIMED.
 * IN NO EVENT SHALL THE AUTHOR BE LIABLE FOR ANY DIRECT , INDIRECT ,
 * INCIDENTAL , SPECIAL , EXEMPLARY , OR CONSEQUENTIAL DAMAGES (INCLUDING , BUT
 * NOT LIMITED TO , PROCUREMENT OF SUBSTITUTE GOODS OR SERVICES; LOSS OF USE ,
 * DATA , OR PROFITS; OR BUSINESS INTERRUPTION) HOWEVER CAUSED AND ON ANY
 * THEORY OF LIABILITY , WHETHER IN CONTRACT , STRICT LIABILITY , OR TORT
 * (INCLUDING NEGLIGENCE OR OTHERWISE) ARISING IN ANY WAY OUT OF THE USE OF
 * THIS SOFTWARE , EVEN IF ADVISED OF THE POSSIBILITY OF SUCH DAMAGE.
 *
 * 3. The name of the author may not be used to endorse or promote
 *    products derived from this software without specific prior written
 *    permission.
 *
 * The advertising clause requiring mention in adverts must never be included.
 */

/*! ---------------------------------------------------------------
 * \file ErrnoTypes.h
 * \brief Header of ErrnoTypes
 * \todo add comment here
 *
 * File description
 *
 * PROJ: OSLL/geo2tag
 * ---------------------------------------------------------------- */

#ifndef _ErrnoTypes_H_9465846A_3F83_4512_8C8B_EE2A40A357F9_INCLUDED_
#define _ErrnoTypes_H_9465846A_3F83_4512_8C8B_EE2A40A357F9_INCLUDED_

#include <QString>
#include <QMap>
#include <QString>

#define SUCCESS                             0
#define WRONG_TOKEN_ERROR                   1
#define USER_ALREADY_EXIST_ERROR            2
#define USER_DOES_NOT_EXIST_ERROR           3
#define CHANNEL_ALREADY_EXIST_ERROR         4
#define CHANNEL_DOES_NOT_EXIST_ERROR        5
#define SUBSCRIPTION_ALREADY_EXIST          6
#define INTERNAL_DB_ERROR                   7
#define INCORRECT_QUERY_NAME_ERROR          8
#define INCORRECT_JSON_ERROR                9
#define INCORRECT_CREDENTIALS_ERROR         10
#define CHANNEL_NOT_SUBCRIBED_ERROR         11
#define CHANNEL_ALREADY_SUBSCRIBED_ERROR    12
#define TAG_DOES_NOT_EXIST_ERROR            13
#define TAG_ALREADY_EXIST_ERROR             14
#define NULL_TIMESLOT_ERROR                 15
#define UNKNOWN_ERROR                       16
#define TMP_USER_ALREADY_EXIST_ERROR        17
#define NETWORK_ERROR                       18
#define EMAIL_ALREADY_EXIST_ERROR           19
#define WEAK_PASSWORD_ERROR                 20

class Errno
{
  public:
    static QMap<int,QString> initErrnoMap()
    {
      QMap<int, QString> emap;

      /*
       * After adding new error code, please fill item in this map
       */
                                        // 0
      emap.insert(SUCCESS                            ,"SUCCESS");
                                        // 1
      emap.insert(WRONG_TOKEN_ERROR                  ,"WRONG_TOKEN_ERROR");
                                        // 2
      emap.insert(USER_ALREADY_EXIST_ERROR           ,"USER_ALREADY_EXIST_ERROR");
                                        // 3
      emap.insert(USER_DOES_NOT_EXIST_ERROR          ,"USER_DOES_NOT_EXIST_ERROR");
                                        // 4
      emap.insert(CHANNEL_ALREADY_EXIST_ERROR        ,"CHANNEL_ALREADY_EXIST_ERROR");
                                        // 5
      emap.insert(CHANNEL_DOES_NOT_EXIST_ERROR       ,"CHANNEL_DOES_NOT_EXIST_ERROR");
                                        // 6
      emap.insert(SUBSCRIPTION_ALREADY_EXIST         ,"SUBSCRIPTION_ALREADY_EXIST");
                                        // 7
      emap.insert(INTERNAL_DB_ERROR                  ,"INTERNAL_DB_ERROR");
                                        // 8
      emap.insert(INCORRECT_QUERY_NAME_ERROR         ,"INCORRECT_QUERY_NAME_ERROR");
                                        // 9
      emap.insert(INCORRECT_JSON_ERROR               ,"INCORRECT_JSON_ERROR");
                                        // 10
      emap.insert(INCORRECT_CREDENTIALS_ERROR        ,"INCORRECT_CREDENTIALS_ERROR");
                                        // 11
      emap.insert(CHANNEL_NOT_SUBCRIBED_ERROR        ,"CHANNEL_NOT_SUBCRIBED_ERROR");
                                        // 12
      emap.insert(CHANNEL_ALREADY_SUBSCRIBED_ERROR   ,"CHANNEL_ALREADY_SUBSCRIBED_ERROR");
                                        // 13
      emap.insert(TAG_DOES_NOT_EXIST_ERROR           ,"TAG_DOES_NOT_EXIST_ERROR");
                                        // 14
      emap.insert(TAG_ALREADY_EXIST_ERROR            ,"TAG_ALREADY_EXIST_ERROR");
                                        // 15
      emap.insert(NULL_TIMESLOT_ERROR                ,"NULL_TIMESLOT_ERROR");
                                        // 16
      emap.insert(UNKNOWN_ERROR                      ,"UNKNOWN_ERROR");
                                        // 17
      emap.insert(TMP_USER_ALREADY_EXIST_ERROR       ,"TMP_USER_ALREADY_EXIST_ERROR");
                                        // 18
      emap.insert(NETWORK_ERROR                      ,"NETWORK_ERROR");
                                        // 19
      emap.insert(EMAIL_ALREADY_EXIST_ERROR          ,"EMAIL_ALREADY_EXIST_ERROR");
                                        // 20
      emap.insert(WEAK_PASSWORD_ERROR                ,"WEAK_PASSWORD_ERROR");
      return emap;
    }
};

const QString getErrorByCode(int error);
#endif                                  //_ErrnoTypes_H_9465846A_3F83_4512_8C8B_EE2A40A357F9_INCLUDED_
