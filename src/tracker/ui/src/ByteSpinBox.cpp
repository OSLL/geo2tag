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
/*!
 * PROJ: OSLL/geo2tag
 * ---------------------------------------------------------------- */

#include "inc/ByteSpinBox.h"
#include "math.h"

ByteSpinBox::ByteSpinBox(QWidget *parent) :
QSpinBox(parent),
m_max(UINT_MAX)
{
  /*
  this->setMinimum(0);
  this->setMaximum(m_max);
  this->setButtonSymbols(QSpinBox::PlusMinus);

  m_suffix.insert(10, " B");
  m_suffix.insert(20, " Kb");
  m_suffix.insert(30, " Mb");
  m_suffix.insert(32, " Gb");

  m_border.insert(10, pow(2,10));
  m_border.insert(20, pow(2,20));
  m_border.insert(30, pow(2,30));
  */
}


QString ByteSpinBox::textFromValue(int val) const
{
  /*
  uint key = (uint)log2(val);
  QString text = "";
  if(key < m_border.value(10))
  {
      text = QString(val, 10) + m_suffix.value(10);
  }
  else if(key < m_border.value(20))
  {
      text = QString(val/m_border.value(10), 10) + m_suffix.value(20);
  }
  else if(key < m_border.value(30))
  {
      text = QString(val/m_border.value(20), 10) + m_suffix.value(30);
  }
  else
  {
      text = QString(val/m_border.value(30), 10) + m_suffix.value(40);
  }
  */
  val = 0;
  return "";
}


int ByteSpinBox::valueFromText(const QString &text) const
{
  QString t = text;
  t = "";
  return 0;
}
