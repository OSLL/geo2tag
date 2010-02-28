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
 * \file Trace.h
 * \brief инструментарий трассировки кадров стека и ссылок на исходные тексты
 *
 * File description
 *
 *  PROJ: OSLL/geo2tag
 *
 * 
 * ------------------------------------------------------------------------ */


#ifndef _CTrace_H_A6C1533E_B988_4DB6_94D7_2217A89CEC1A_INCLUDED_
#define _CTrace_H_A6C1533E_B988_4DB6_94D7_2217A89CEC1A_INCLUDED_

#include <vector>
#include <string>
#include <iostream>
#include <cstddef>

/*!
 * \brief ссылка на место в исходниках
 */
class CSource
{
public:
  /*! строка */
  long m_line;
  /*! имя файла */
  const char *m_file;
  /*! полное имя функции */
  const char *m_function;

  CSource(int line,const char *file,const char *function) : m_line(line), m_file(file), m_function(function)
  {
  }

  /*!
   * \brief строка описывающая место в исходниках
   * \return std::string вида file:line:function
   */
  std::string getDescription() const;

  /*!
   * \brief вывод ссылки на место в исходниках в поток
   */
  friend std::ostream& operator << (std::ostream &stm,const CSource &a)
  {
    return stm << a.m_file << ":" << a.m_line << ":" << a.m_function;
  }
};

/*!
 * \brief создает CSource ссылающийся на текущую строчку
 */
#define SRC() CSource(__LINE__,__FILE__,__PRETTY_FUNCTION__)

/*!
 * \brief класс трассировки стека
 *
 * блоки программы помеченные STRACE() оставляют свой след
 * в текущей трассе стека в виде ссылки на исходник.
 *
 * в одном потоке одновременно обрабатывается одно исключение,
 * поэтому трасса стека собирается в последний созданный
 * объект этого класса.
 * 
 * thread-safe
 */
class CStackTrace
{
  /*! список оттрассированных кадров */
  typedef std::vector<CSource> CTraceList;
  CTraceList m_trace;

public:

  /*!
   * \brief получить cтек текущего потока
   */
  static const CStackTrace& getInstance();
  
  /*!
   * \brief поместить кадр на вершину стека
   * \param src: [in] ссылка на кадр стека в исходнике
   */
  static void push(const CSource& src);
  
  /*!
   * \brief снять верхний кадр стека
   */
  static void pop();
  
  /*!
   * \brief получить трассу стека
   * \return список кадров стека по убыванию уровня вложенности
   */
  const CTraceList& getTrace() const
  {
    return m_trace;
  }
};

/*!
 * \brief вывод трассы стека в поток
 * \todo обсудить общий формат текстового представления всяческих объектов
 */
std::ostream& operator << (std::ostream& stm,const CStackTrace& a);

/*!
 * \brief класс кадра стека
 */
class CStackLocation
{
public:
  CStackLocation(const CSource &src)
  {
    CStackTrace::push(src);
  }

  ~CStackLocation()
  {
    CStackTrace::pop();
  }
};

/*!
 * \brief отмечает трассируемый кадр стека
 */
#define STRACE() CStackLocation __stack_trace_location__(SRC())

#endif //_CTrace_H_A6C1533E_B988_4DB6_94D7_2217A89CEC1A_INCLUDED_

/* ===[ End of file  ]=== */
