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
    CHandlePtr<DataMarks> m_marks; //!< Marks for this channel
    
    bool m_isDisplayed; //!< Displayed on the UI

  protected:
    Channel(const std::string &name, const std::string &description, const CHandlePtr<DataMarks> &marks);

  public:

    const std::string& getDescription() const;

    const std::string& getName() const;

    virtual void setDescription(const std::string& description);

    const CHandlePtr<DataMarks> getMarks() const;

    virtual void addData(CHandlePtr<DataMark> mark);

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
