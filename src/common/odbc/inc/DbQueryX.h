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
#ifndef DBQUERYX_H
#define DBQUERYX_H

#include "DbQuery.h"

namespace ODBC
{
    /*!
     * \brief абстрактный базовый класс параметризованных запросов с исключениями
     */
    class CDbQueryX : public CDbQuery
    {
    protected:

      void bind(int colSet=0);
      void bindParam();

    public:
      CDbQueryX(const CDbConn &dbConn);

      /*!
       * \brief чтение строчки из курсора в текущий набор колонок
       * \return прочитано ли
       */
      bool fetch();

      /*!
       * \brief чтение строчки из курсора в текущий набор колонок
       * отсутствие данных рассматривается как ошибка
       */
      void fetchNoEmpty();

      /*!
       * \brief чтение строчки из курсора в заданный набор колонок
       * \param colSet: [in] набор колонок, который становится текущим
       * \return прочитано ли
       */
      bool fetch(int colSet);

      /*!
       * \brief чтение строчки из курсора в заданный набор колонок
       * отсутствие данных рассматривается как ошибка
       * \param colSet: [in] набор колонок, который становится текущим
       */
      void fetchNoEmpty(int colSet);

      /*!
       * \brief подготовка запроса к выполнению: prepare, bind, bindParam
       * \param pSql: [in] запрос SQL (опустить для запроса по умолчанию)
       */
      void prepare(const char *pSql=NULL);

      /*
       * обертки функций нижнего уровня возбуждающие исключения по ошибкам
       */
      void execute();

      void execDirect(const char *sql);

      void close();
  };

};

#endif // DBQUERYX_H
