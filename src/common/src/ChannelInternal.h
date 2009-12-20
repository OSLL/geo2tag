/*!
 * \file ChannelInternal.h
 * \brief Header of ChannelInternal
 * \todo add comment here
 *
 * File description
 *
 * PROJ: OSLL/geo2tag
 * ---------------------------------------------------------------- */


#ifndef _ChannelInternal_H_F613BD25_03A0_4229_9305_260275C911A1_INCLUDED_
#define _ChannelInternal_H_F613BD25_03A0_4229_9305_260275C911A1_INCLUDED_

#include "Channel.h"

namespace loader
{
  class Channel: public common::Channel
  {
    unsigned long long m_id;

  public:
    Channel(unsigned long long id, std::string description, const CHandlePtr<common::DataMarks> &marks=makeHandle(new common::DataMarks()));
    
    unsigned long long getId() const;
    
    void setId(unsigned long long id);

    virtual void addData(CHandlePtr<common::DataMark> mark);

    virtual ~Channel();
  };
} // namespace loader

#endif //_ChannelInternal_H_F613BD25_03A0_4229_9305_260275C911A1_INCLUDED_

/* ===[ End of file ]=== */
