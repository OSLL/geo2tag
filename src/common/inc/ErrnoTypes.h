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

enum geo2tagError
{
  SUCCESS ,                             // 0
  WRONG_TOKEN_ERROR ,                   // 1
  USER_ALREADY_EXIST_ERROR ,            // 2
  USER_DOES_NOT_EXIST_ERROR ,           // 3
  CHANNEL_ALREADY_EXIST_ERROR ,         // 4
  CHANNEL_DOES_NOT_EXIST_ERROR ,        // 5
  SUBSCRIPTION_ALREADY_EXIST ,          // 6
  INTERNAL_DB_ERROR ,                   // 7
  INCORRECT_QUERY_NAME_ERROR ,          // 8
  INCORRECT_JSON_ERROR ,                // 9
  INCORRECT_CREDENTIALS_ERROR ,         // 10
  CHANNEL_NOT_SUBCRIBED_ERROR ,         // 11
  CHANNEL_ALREADY_SUBSCRIBED_ERROR ,    // 12
  TAG_DOES_NOT_EXIST_ERROR ,            // 13
  TAG_ALREADY_EXIST_ERROR ,             // 14
  NULL_TIMESLOT_ERROR ,                 // 15
  UNKNOWN_ERROR,                        // 16
  TMP_USER_ALREADY_EXIST_ERROR,         // 17
  NETWORK_ERROR                         // 18
};

const QString getErrorByCode(int error);
#endif                                  //_ErrnoTypes_H_9465846A_3F83_4512_8C8B_EE2A40A357F9_INCLUDED_
