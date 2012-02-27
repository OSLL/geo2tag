/*
 * Copyright 2010-2011  OSLL osll@osll.spb.ru
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
/*----------------------------------------------------------------- !
 * PROJ: OSLL/geo2tag
 * ---------------------------------------------------------------- */

#include "Parameters.h"

#include <stdio.h>
#include <string>
#include <stdlib.h>
#include <QDebug>

namespace common
{

  void removeAllWhite (QString &str)
  {
    QString temp;
    for (unsigned int i = 0; i < str.length(); i++)
      if (str[i] != ' ') temp += str[i];
    str = temp;
  }

  Parameters::Parameters()
  {
    FILE *configFile;
    if ((configFile = fopen("geo2tag.conf", "r")) == NULL)
    {
      qDebug() << "can't open config file";
      return;
    }

    int i = 0;
    char str[255];
    while ((str[i] = getc(configFile)) != EOF)
    {
      if (str[i] == '\n')
      {
        /* reading parameter's value */
        str[i] = '\0';
        QString line(str);
        QString parameter;
        QString value;
        int found = -1;
        if ((found = line.find("=")) != QString::npos)
        {
          char buffer[255];
          line.copy(buffer, found, 0);
          buffer[found] = '\0';
          parameter = QString(buffer);
          removeAllWhite(parameter);
          line.copy(buffer, line.size() - found - 1, found + 1);
          buffer[line.size() - found - 1] = '\0';
          value = QString(buffer);
          removeAllWhite(value);
        }

        int res = strcmp(parameter.c_str(), "default_radius\0");
        if (res == 0)
        {
          m_defaultRadius = atoi(value.c_str());
        }

        i = 0;
      }
      else
      {
        i++;
      }
    }

    fclose(configFile);
  }

  Parameters& Parameters::getInstance()
  {
    static Parameters instance;
    return instance;
  }

  int Parameters::getDefaultRadius()
  {
    return m_defaultRadius;
  }

  void Parameters::setDefaultRadius(int newRadius)
  {
    m_defaultRadius = newRadius;
    updateFile();
  }

  void Parameters::updateFile()
  {
    FILE *configFile;
    if ((configFile = fopen("geo2tag.conf", "w")) == NULL)
    {
      qDebug() << "can't open config file";
      return;
    }

    fprintf(configFile, "default_radius = %d\n", m_defaultRadius);

    fclose(configFile);
  }

}                                       //namespace common
