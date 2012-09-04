/*
 * Copyright 2010-2012  OSLL osll@osll.spb.ru
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
 * \file  EmailMessage.cpp
 * \brief Implementation of the class EmailMessage
 *
 * PROJ: OSLL/geo2tag
 * ---------------------------------------------------------------- */

#include <syslog.h>
#include "EmailMessage.h"
#include "defines.h"
#include "SettingsStorage.h"

EmailMessage::EmailMessage(QString email) :
m_email(email)
{
}


void EmailMessage::setEmail(QString email)
{
  m_email = email;
}


void EmailMessage::setBody(QString body)
{
  m_body = body;
}


void EmailMessage::setSubject(QString subject)
{
  m_subject = subject;
}


QString EmailMessage::getBody() const
{
  return m_body;
}


QString EmailMessage::getEmail() const
{
  return m_email;
}


QString EmailMessage::getSubject() const
{
  return m_subject;
}


void EmailMessage::send() const
{
  QString command = QString("echo \"%1\" | mail -s '%2' %3 &").arg(m_body).arg(m_subject).arg(m_email);
  system(command.toStdString().c_str());
}


void EmailMessage::sendAsRegistrationLetter(const QString& info)
{
  SettingsStorage storage(SETTINGS_STORAGE_FILENAME);
  QString serverUrl = storage.getValue("General_Settings/server_url", QVariant(DEFAULT_SERVER)).toString();
  QString subject = storage.getValue("Mail_Settings/subject", QVariant(DEFAULT_REGISTRATION_EMAIL_SUBJECT)).toString();
  QString body = storage.getValue("Mail_Settings/body", QVariant(DEFAULT_REGISTRATION_EMAIL_BODY)).toString();
  body.append(" To confirm registration, please, go to this link: ");
  body.append(serverUrl.toStdString().c_str());
  body.append("service/confirmRegistration-");
  body.append(info);
  m_subject = subject;
  m_body = body;
  syslog(LOG_INFO, "Email: %s", m_email.toStdString().c_str());
  syslog(LOG_INFO, "Subject: %s", m_subject.toStdString().c_str());
  syslog(LOG_INFO, "Body: %s", m_body.toStdString().c_str());
  send();
}


void EmailMessage::sendAsRestorePwdMessage(const QString& pwd)
{
  SettingsStorage storage(SETTINGS_STORAGE_FILENAME);
  QString subject = storage.getValue("Mail_Settings/restore_pwd_subject", QVariant(DEFAULT_RESTORE_PASSWORD_SUBJECT)).toString();
  QString body = storage.getValue("Mail_Settings/restore_pwd_body", QVariant(DEFAULT_RESTORE_PASSWORD_BODY)).toString()
    + " " + pwd;
  m_subject = subject;
  m_body = body;
  syslog(LOG_INFO, "Email: %s", m_email.toStdString().c_str());
  syslog(LOG_INFO, "Subject: %s", m_subject.toStdString().c_str());
  syslog(LOG_INFO, "Body: %s", m_body.toStdString().c_str());
  send();
}
