/*!
 * \file DataMarkInternal.h
 * \brief Header of DataMarkInternal
 *
 * File description
 *
 * PROJ: OSLL/geo2tag
 * ---------------------------------------------------------------- */


#ifndef _DataMarkInternal_H_6E8C1DBF_DF18_46D0_9119_1F2D838576EE_INCLUDED_
#define _DataMarkInternal_H_6E8C1DBF_DF18_46D0_9119_1F2D838576EE_INCLUDED_

#include "DataMarks.h"

namespace loader
{
  class DataMark: public common::DataMark
  {
    unsigned long long m_id;

  public:
    DataMark(unsigned long long id, double latitude, double longitude, 
             std::string label, std::string description, const CTime& time);
    
    unsigned long long getId() const;
    
    void setId(unsigned long long id);

    virtual ~DataMark();
  };
  
} // namespace loader

#endif //_DataMarkInternal_H_6E8C1DBF_DF18_46D0_9119_1F2D838576EE_INCLUDED_

/* ===[ End of file ]=== */
