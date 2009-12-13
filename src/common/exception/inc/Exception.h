/*  */
/*!
 * \file Exception.h
 * \brief заголовок класса CException
 *
 * File description
 *
 *  PROJ: OSLL/geo2tag
 *
 * 
 * ------------------------------------------------------------------------ */


#ifndef _CException_H_71DA1F16_33DC_432E_8F07_BBEBB4297269_INCLUDED_
#define _CException_H_71DA1F16_33DC_432E_8F07_BBEBB4297269_INCLUDED_

#include <string>
#include <iostream>
#include "Streamable.h"
#include "Trace.h"

/*!
  \class CException
  \brief Базовый класс для всех исключений в библиотеке CLib
  \todo добавить описание правил создания исключений нижнего уровня
*/
class CException: public IStreamable
{
  unsigned short m_errcode;  /*!< уникальный идентификатор ошибки */
  unsigned char  m_package;  /*!< идентификатор пакета - источника исключения */

public:

  /*! конструктор исключения
    \param  errorcode идентификатор ошибки
    \param  package_id идентификатор пакета-источника исключения
  */
  CException(unsigned short errorcode, unsigned char package_id)
  {
    m_errcode=errorcode;
    m_package=package_id;
  }

  /*! виртуальный деструктор */
  virtual ~CException()
  {
  }

  /*! получить код ошибки
    \return код ошибки, определенный в \ref errcodes.h
  */
  const unsigned short& getErrorCode() const
  {
    return m_errcode;
  }

  /*! получить идентификатор пакета- источника ошибки
    \return идентификатор пакета-источника исключения
  */
  const unsigned char& getPackage() const
  {
    return m_package;
  }

  /*! сформировать уникальный дескриптор ошибки

    Уникальный дескриптор ошибки представляет собой беззнаковое целое содержащее код
    ошибки и идентификатор пакета-источника. Может использоваться в качестве ключа в протоколах
    работы или при записи в таблицы.
    \code
    формат

    31         23         15          7         0
    |==3 байт==|==2 байт==|==1 байт==|==0 байт==|
    |----------+----------+----------+----------|
    | package  | reserved |    error code       |
    |----------+----------+----------+----------|
    \endcode

    \return уникальный дескриптор ошибки
  */
  unsigned long getErrorHandle() const
  {
   return (((unsigned long)m_package) << 24 ) + m_errcode;
  }

  virtual std::ostream& outToStream(std::ostream& stm) const;

  /*!
   * \brief вывести в поток текстовое описание исключения.
   *
   * описание не содержит никакой служебной информации генерируемой автоматически.
   * следует использовать для передачи строки, описывающей, что именно произошло.
   *
   * \param stm: поток для вывода описания
   * \return поток после вывода
   */
  virtual std::ostream& outDescription(std::ostream& stm) const
  {
    return stm;
  }

  /*!
   * \brief получить текстовое описание исключения.
   *
   * описание не содержит никакой служебной информации генерируемой автоматически.
   * следует использовать для передачи строки, описывающей, что именно произошло.
   *
   */
  virtual std::string getDescription() const;

}; // class CException


//=======================[ class CExceptionSource ]======================================


/*!
  \class CExceptionSource
  \brief Класс исключений с информацией о месте исключения и ссылками на исходный текст
*/
class CExceptionSource : public CException
{
  CSource m_src;  /*!< ссылка на источник исключения */
  CStackTrace m_stack;

public:

  /*!
    Конструктор инициализации
    \param errorcode код ошибки, определенный в файле errcodes.h
    \param source описатель точки возникновения исключения (см. описание \ref SRC, \ref CSource)
    \param package_id идентификатор пакета
  */
  CExceptionSource(unsigned short errorcode, const CSource& src,unsigned char package_id):
    CException(errorcode,package_id), m_src(src), m_stack(CStackTrace::getInstance())
  {
  }

  /*! получить строку с именем файла
    \return строка с именем файла, в котором сгенерировано исключение
  */
  std::string getSource() const { return m_src.m_file; }

  /*! получить имя функции, сгенерировашей исключение
  */
  std::string getSourceFunction() const { return m_src.m_function; }

  /*! получить номер строки в которой сгенерировано исключение
    \return номер строки-источника исключения
  */
  unsigned int getLine() const { return m_src.m_line; }

  /*!
   * вывод объекта в поток
   * \param stm: [in] поток для вывода
   * \return поток stm после вывода
   */
  virtual std::ostream& outToStream(std::ostream& stm) const;
}; // class CExceptionSource


//=======================[ class CNotImplementedException ]======================================

/*!
  \class CNotImplementedException
  \brief Исключение отсутствия реализации

  Данный тип исключений предназначен для описания ошибок связанных с
  вызовом методов с неполной или отсутствующей реализацией
*/
class CNotImplementedException: public CExceptionSource
{
  std::string m_description;
  
public:
  /*!
   * \brief конструктор инициализации
   * \param source: [in] описатель точки возникновения исключения (см. описание \ref SRC, \ref CSource)
   * \param description: [in] описание
   */
  CNotImplementedException(const CSource& source,const std::string& description):
    CExceptionSource(0,source,0),m_description(description)
  {
  }

  virtual std::string getDescription() const;
};


#ifndef NOT_IMPLEMENTED
/*!
 * \brief генерация исключения о нереализованной функциональности
 * \param description: [safe] описание
 */
#define NOT_IMPLEMENTED(description) throw CNotImplementedException(SRC(),(description))
#endif

#endif //_CException_H_71DA1F16_33DC_432E_8F07_BBEBB4297269_INCLUDED_

/* ===[ End of file  ]=== */
