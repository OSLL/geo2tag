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
  
    std::string m_description; //!< Description for channel
    CHandlePtr<DataMarks> m_marks; //!< Marks for this channel

  //CODE INSPECTION: protected:
  public:
    Channel(const std::string &description="", const CHandlePtr<DataMarks> &marks=CHandlePtr<DataMarks>());

  public:

    const std::string& getDescription() const;

    const CHandlePtr<DataMarks> getMarks() const;

    virtual void addData(CHandlePtr<DataMark> mark);

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
