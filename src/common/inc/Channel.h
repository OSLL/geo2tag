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
/*!
 * \file Channel.h
 * \brief Header of Channel
 *
 * File description
 *
 * PROJ: OSLL/geo2tag
 * ---------------------------------------------------------------- */


#ifndef _Channel_H_480D4E41_537B_41D1_A67C_326A700DDC2D_INCLUDED_
#define _Channel_H_480D4E41_537B_41D1_A67C_326A700DDC2D_INCLUDED_

#include <string>
#include <vector>
#include "DataMarks.h"
#include "Handle.h"

namespace common
{
  /*!
   * 
   */
  class Channel
  {
    std::string m_name; //!< channel name  
    std::string m_description; //!< Description for channel
    std::string m_url; //!< URL for mark

    CHandlePtr<DataMarks> m_marks; //!< Marks for this channel
    
    bool m_isDisplayed; //!< Displayed on the UI

    double m_activeRadius; //< Radius for visible marks

  protected:
    Channel(const std::string &name, const std::string &description, const std::string& url, const CHandlePtr<DataMarks> &marks);

  public:

    const std::string& getDescription() const;

    const std::string& getName() const;

    const std::string& getUrl() const;

    void setDescription(const std::string& description);
    
    void setUrl(const std::string& url);

    const CHandlePtr<DataMarks> getMarks() const;

    virtual void addData(CHandlePtr<DataMark> mark);

    void setRadius(const double &radius);
    double getRadius() const;

    bool isDisplayed() const;

    void setDisplayed(bool);

    virtual ~Channel();
  }; // class Channel
 
  class Channels: public std::vector<CHandlePtr<Channel> >
  {
  public:
    Channels();

    virtual ~Channels();
  }; // class Channels
} // namespace common

#endif //_Channel_H_480D4E41_537B_41D1_A67C_326A700DDC2D_INCLUDED_

/* ===[ End of file ]=== */
