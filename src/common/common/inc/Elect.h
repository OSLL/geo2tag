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
/*  */
/*!
 * \file Elect.h
 * \brief заголовок класса CElect
 *
 * File description
 *
 *  PROJ: OSLL/geo2tag
 *
 * 
 * ------------------------------------------------------------------------ */


#ifndef _CElect_H_E510BE6F_8FC0_4D70_AD57_4937408BF239_INCLUDED_
#define _CElect_H_E510BE6F_8FC0_4D70_AD57_4937408BF239_INCLUDED_

#include <map>
#include <list>
#include <algorithm>
#include <functional>
#include "ExceptionSource.h"

class CElectException: public CExceptionSource
{
public:
  CElectException(const CSource& src): CExceptionSource(0,src,0)
  {
  }
};

/*!
 * \brief фильтр выбора простым большинством.
 * 
 * если два кандидата набирают равное число голосов, побеждает получивший последний голос.
 * \param Name: [typename] тип идентификатора кандидата
 * \param Pred: [typename] предикат сравнения имен кандидатов
 */
template<typename Name,typename Pred=std::less<Name> >
class CElect
{
  std::map<Name,long,Pred> m_voteCount;
  std::map<long,Name,std::greater<long> > m_chart;
public:
  /*!
   * \brief голосовать за кандидата
   * \param name: [in] имя кандидата
   */
  void vote(const Name& name)
  {
    m_chart[++m_voteCount[name]]=name;
  }
  /*!
   * \brief получить имя победителя
   * \return имя победителя
   * \exception CElectException если невозможно определить победителя
   */
  const Name& getWinner() const
  {
    if(!m_chart.empty())
      return m_chart.begin()->second;
      
    throw CElectException(SRC());
  }
};




#endif //_CElect_H_E510BE6F_8FC0_4D70_AD57_4937408BF239_INCLUDED_

/* ===[ End of file  ]=== */
