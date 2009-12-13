/*  */
/*!
 * \file Lock.h
 * \brief заголовок класса CLock
 *
 * File description
 *
 *  PROJ: OSLL/geo2tag
 *
 * 
 * ------------------------------------------------------------------------ */


#ifndef _CLock_H_959A7C4C_E612_45E0_AC97_F3480BE01AA5_INCLUDED_
#define _CLock_H_959A7C4C_E612_45E0_AC97_F3480BE01AA5_INCLUDED_

namespace Thread
{
  /*!
   * \brief автоматический объект блокировки
   *
   * снимает блокировку при разрушении
   *
   * \param T: [typename] класс объекта синхронизации
   * \param Lock: функция-член класса T устанавливающая блокировку
   * \param Unlock: функция-член класса T снимающая блокировку
   */
  template<typename T,void (T::*Lock)() const=&T::lock,void (T::*Unlock)() const=&T::unlock>
  class CLock
  {
    /*! заблокированный объект */
    const T* m_lock;
    
  public:
    /*!
     * \brief заблокировать объект
     */
    explicit CLock(const T& o): m_lock(&o)
    {
      (m_lock->*Lock)();
    }

    /*!
     * \brief снять блокировку, если она еще не снята
     */
    void unlock()
    {
      if(m_lock)
      {
        (m_lock->*Unlock)();
        m_lock=NULL;
      }
    }
    
    ~CLock()
    {
      unlock();
    }
  }; // class CLock

}//namespace Thread

#endif //_CLock_H_959A7C4C_E612_45E0_AC97_F3480BE01AA5_INCLUDED_

/* ===[ End of file  ]=== */
