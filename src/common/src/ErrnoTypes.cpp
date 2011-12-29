/*
 * Copyright 2011  bac1ca  bac1ca89@gmail.com
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
 *
 * \file ErrnoTypes.cpp
 * \brief ErrnoTypes implementation
 *
 * File description
 *
 * PROJ: OSLL/geo2tag
 * ---------------------------------------------------------------- */

#include "ErrnoTypes.h"
#include <QString>

const QString getErrorByCode(int error)
{

  switch (error)
  {
    case SUCCESS: return "SUCCESS";
    case WRONG_TOKEN_ERROR: return "WRONG_TOKEN_ERROR";
    case USER_ALREADY_EXIST_ERROR: return "USER_ALREADY_EXIST_ERROR";
    case USER_DOES_NOT_EXIST_ERROR: return "USER_DOES_NOT_EXIST_ERROR";
    case CHANNEL_ALREADY_EXIST_ERROR: return "CHANNEL_ALREADY_EXIST_ERROR";
    case CHANNEL_DOES_NOT_EXIST_ERROR: return "CHANNEL_DOES_NOT_EXIST_ERROR";
    case INTERNAL_DB_ERROR: return "INTERNAL_DB_ERROR";
    case UNKNOWN_ERROR: return "UNKNOWN_ERROR";
  }
  return "UNDEFINED_ERROR";
}
