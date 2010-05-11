/* $Id$ */
/*!
 * \file Picture.h
 * \brief Header of Picture
 *
 * PROJ: OSLL/geoblog
 * ---------------------------------------------------------------- */


#ifndef _Picture_H_86992429_0AAF_42A5_9706_B8DDACCE6103_INCLUDED_
#define _Picture_H_86992429_0AAF_42A5_9706_B8DDACCE6103_INCLUDED_

#include <vector>

namespace common
{
  /*!
   * common picture. binary data + some meta information
   */
  class Picture
  {
    std::vector<char> m_data;
  public:
    
    Picture(const Picture&);

    enum Type {
      PNG,
      UNKNOWN
    };
    
    virtual const std::vector<char> getData() const;

    virtual Type getType() const;

    virtual ~Picture();
  
  protected:
    Picture(const std::vector<char>& data);

  }; // class Picture
  
} // namespace common

#endif //_Picture_H_86992429_0AAF_42A5_9706_B8DDACCE6103_INCLUDED_

/* ===[ End of file $HeadURL$ ]=== */
