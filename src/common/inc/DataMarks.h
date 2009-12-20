/*  */
/*!
 * \file DataMarks.h
 * \brief Header of DataMarks
 * \todo add comment here
 *
 * File description
 *
 *  PROJ: OSLL/geo2tag 
 * ---------------------------------------------------------------- */


#ifndef _DataMarks_H_E8A2619E_0BF6_4AE8_BB61_F09B92F73637_INCLUDED_
#define _DataMarks_H_E8A2619E_0BF6_4AE8_BB61_F09B92F73637_INCLUDED_

#include <string>
#include <vector>
#include "Time.h"
#include "Handle.h"

namespace common
{
  class DataMark
  {
    double m_latitude;
    double m_longitude;

    std::string m_label;
    std::string m_description;

    CTime m_time;

  protected:
    DataMark(double latitude=0., double longitude=0., const std::string& label="Default label", 
             const std::string& description="No descrption", const CTime &time=CTime::now());

  public:

    void setDescription(const std::string&);
    const std::string& getDescription() const;

    double getLatitude() const;
    void setLatitude(const double&);

    double getLongitude() const;
    void setLongitude(const double&);

    const std::string& getLabel() const;
    void setLabel(const std::string&);

    const CTime& getTime() const;
    void setTime(const CTime& time=CTime::now());

    static CHandlePtr<DataMark> createMark(double latitude=0., double longitude=0., const std::string& label="Default label", 
             const std::string& description="No descrption", const CTime &time=CTime::now());

    virtual ~DataMark();
  };

  /*!
   * Class description. May use HTML formatting
   *
   */
  class DataMarks: public std::vector<CHandlePtr<DataMark> >
  {
  public:
    DataMarks();

    virtual ~DataMarks();

  }; // class DataMarks
  
} // namespace common

#endif //_DataMarks_H_E8A2619E_0BF6_4AE8_BB61_F09B92F73637_INCLUDED_

/* ===[ End of file  ]=== */
