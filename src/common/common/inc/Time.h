/*  */
/*!
 * \file Time.h
 * \brief заголовок класса CTime
 *
 * File description
 *
 *  PROJ: OSLL/geo2tag
 *
 * 
 * ------------------------------------------------------------------------ */


#ifndef _CTime_H_401ED276_9F52_4594_9406_0977B06293A2_INCLUDED_
#define _CTime_H_401ED276_9F52_4594_9406_0977B06293A2_INCLUDED_

#include <time.h>
#include "Init.h"
#include "Exception.h"

std::ostream& operator << (std::ostream& stm,const tm& a);

/*!
 * общий родитель всех исключений связанных со временем
 */
class CExceptionTime: public CExceptionSource
{
public:
  CExceptionTime(const CSource& src):
    CExceptionSource(0,src,0)
  {
  }  
};


/*!
 * исключение недействительного типа времени
 */
class CExceptionTimeType: public CExceptionTime
{
  int m_type;
  
public:
  CExceptionTimeType(int type,const CSource& src):
    CExceptionTime(src),m_type(type)
  {
  }
  
  virtual std::ostream& outDescription(std::ostream& stm) const;
};


/*!
 * исключение ошибки преобразования времени
 */
class CExceptionTimeConv: public CExceptionTime
{
  tm m_time;
public:
  CExceptionTimeConv(const tm& time,const CSource& src):
    CExceptionTime(src),m_time(time)
  {
  }
  
  virtual std::ostream& outDescription(std::ostream& stm) const;
};


/*!
 * \brief класс работы со временем и датой
 */
class CTime: public CInit
{
  time_t m_time;  /*!< время */
  double m_addt;  /*!< добавка нецелых секунд */

public:
  enum CType
  {
    UTC,  /*!< время UTC */
    MDV,  /*!< время МДВ */
    LOCAL,/*!< локальное время */
    ERR   /*!< неизвестное */
  };

protected:

  /*! 
   * \param year: год (AD, e.g. 2005)
   * \param month: месяц (1-12 [+/-12])
   * \param day: день (1-31 [+/-366])
   * \param hour: час (0-23 [+/-366*24])
   * \param min: минуты (0-59+)
   * \param sec: секунды (0-59 [+/-366*86400])
   * \param type: тип
   * \param isDst: признак летнего времени,
   *  0 -- зимнее, 1 -- летнее, -1 -- подходящее.
   *  для зон без летнего времени (UTC, MDV) игнорируется
   *
   * \exception CExceptionTimeType, CExceptionTimeConv
   */
  void init(int year, int month, int day,
            int hour, int min, double sec, 
            CType type, int isDst=0);

public:

  CTime()
  {
    m_time=-1;m_addt=0;
  }
  
  /*! 
   * \param year: год (AD, e.g. 2005)
   * \param month: месяц (1-12 [+/-12])
   * \param day: день (1-31 [+/-366])
   * \param hour: час (0-23 [+/-366*24])
   * \param min: минуты (0-59+)
   * \param sec: секунды (0-59 [+/-366*86400])
   * \param type: тип (по умолчанию CTime::MDV)
   * \param isDst: признак летнего времени,
   *  0 -- зимнее, 1 -- летнее, -1 -- подходящее.
   *  для зон без летнего времени (UTC, MDV) игнорируется
   *
   * \exception CExceptionTimeType, CExceptionTimeConv
   */
  CTime(short year, short month, short day,
        short hour, short min, double sec, 
        CType type=MDV, int isDst=0): CInit(true)
  {
    init(year,month,day,hour,min,sec,type,isDst);
  }
  
  /*!
   * \param time дата и время
   * \param type: тип (по умолчанию CTime::MDV)   
   * \exception CExceptionTimeType, CExceptionTimeConv
   */   
  CTime(const tm& time, CType type=MDV): CInit(true)
  {
    init(time.tm_year+1900, time.tm_mon+1, time.tm_mday, 
         time.tm_hour, time.tm_min, time.tm_sec, type, time.tm_isdst);
  }
  
  /*!
   * \param time дата и время
   * \param type: тип (по умолчанию CTime::MDV)
   * \exception CExceptionTimeType, CExceptionTimeConv
   */   
  explicit CTime(time_t time): CInit(true), m_time(time), m_addt()
  {
  }

  /*!
   * текущее время
   */
  static CTime now();

  /*! получить время
   * \param type: тип (по умолчанию CTime::MDV)
   * \return структура содержащая время в заданной системе
   */
  tm getTime(CType type=MDV) const;
  
  /*!
   * \brief проверка правильности времени внутри объекта
   * \return \b true = время установлено; \b false = время не установлено; 
   */
  bool isValid() const;

  /*! получить нецелое число секунд
    \param type: тип (по умолчанию CTime::MDV)
    \return число секунд
  */
  double getSec(CType type=MDV) const;

  /*! прибавить время
    \param sec: интервал в секундах
  */
  const CTime& operator +=(double sec);

  /*! отнять время
    \param sec: интервал в секундах
  */
  const CTime& operator -=(double sec)
  {
    return (*this)+=(-sec);
  }

  /*! прибавить время
    \param sec: интервал в секундах
  */
  CTime operator+(double sec) const
  {
    return CTime(*this)+=sec;
  }

  /*! отнять время
   * \param sec: интервал в секундах
   */
  CTime operator -(double sec) const
  {
    return CTime(*this)-=sec;
  }

  /*! Разница во времени
   * \param t: вычитаемое
   */
  double operator -(const CTime &t) const;

  /*! оператор проверки равенства
   * \param t: сравниваемое время
   */
  bool operator ==(const CTime &t) const
  {
    return (*this-t)==0.0;
  }
  
  bool operator !=(const CTime &t) const
  {
    return !(*this==t);
  }

  /*! оператор сравнения "меньше"
   * \param t: сравниваемое время
   */
  bool operator <(const CTime &t) const
  {
    return (*this-t)<0.0;
  }

  /*! оператор сравнения "больше"
   * \param t: сравниваемое время
   */
  bool operator >(const CTime &t) const 
  {
    return (*this-t)>0.0;
  }

  /*! оператор сравнения "меньше или равно"
   * \param t: сравниваемое время
   */
  bool operator <=(const CTime &t) const
  {
    return !(*this>t);
  }

  /*! оператор сравнения "больше или равно"
   * \param t: сравниваемое время
   */
  bool operator >=(const CTime &t) const
  {
    return !(*this<t);
  }

  /*! получить количество секунд от начала суток
   * \param type: тип (по умолчанию CTime::MDV)
   * \return количество секунд от начала суток
   */
  double getSec24(CType type=MDV) const;
  
  friend std::ostream& operator << (std::ostream& stm,const CTime& t);
  
  static const CTime& min();
  static const CTime& max();
};

/*!
 * \brief отформатировать время заданным форматом (см. strftime)
 * \param a: [in] время
 * \param fmt: [in] формат
 * \return строку со временем
 */
std::string formatTime(const tm& a,const char *fmt);

#endif //_CTime_H_401ED276_9F52_4594_9406_0977B06293A2_INCLUDED_

/* ===[ End of file  ]=== */
