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
  class Channel;
  class User;

  class DataMark
  {
    double m_latitude;
    double m_longitude;

    std::string m_label;
    std::string m_description;

    std::string m_url;

    CTime m_time;

    CHandlePtr<User> m_user;
    
  protected:
    CHandlePtr<Channel> m_channel;

    DataMark(double latitude, double longitude, const std::string& label, 
             const std::string& description, const std::string& url, const CTime &time,
             const CHandlePtr<User> & user);

  public:

    void setDescription(const std::string&);
    const std::string& getDescription() const;

    double getLatitude() const;
    void setLatitude(const double&);

    double getLongitude() const;
    void setLongitude(const double&);

    const std::string& getLabel() const;
    void setLabel(const std::string&);

    const std::string& getUrl() const;
    void setUrl(const std::string&);

    const CTime& getTime() const;
    void setTime(const CTime& time=CTime::now());

    CHandlePtr<User> getUser() const;

    CHandlePtr<Channel> getChannel() const;

    static CHandlePtr<DataMark> createMark(double latitude=0., double longitude=0., const std::string& label="Default label", 
             const std::string& description="No descrption", const std::string& url="", const CTime &time=CTime::now(), const CHandlePtr<Channel>&channel=CHandlePtr<Channel>());
    
    static double getDistance(double lat1, double lon1, double lat2, double lon2);

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
