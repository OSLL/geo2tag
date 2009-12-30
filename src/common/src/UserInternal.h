/*!
 * \file UserInternal.h
 * \brief Header of UserInternal
 *
 * PROJ: OSLL/geo2tag
 * ---------------------------------------------------------------- */


#ifndef _UserInternal_H_530385AA_47B5_4EFA_92BF_1C79CEC156BC_INCLUDED_
#define _UserInternal_H_530385AA_47B5_4EFA_92BF_1C79CEC156BC_INCLUDED_

#include "User.h"

namespace loader
{
 /*!
   * 
   */
  class User: public common::User
  {
    unsigned long m_id;

  public:
    User(const std::string& login, const std::string& pass, unsigned long id);
    
    unsigned long getId() const;

    ~User();
  }; // class UserInternal
  
} // namespace loader

#endif //_UserInternal_H_530385AA_47B5_4EFA_92BF_1C79CEC156BC_INCLUDED_

/* ===[ End of file ]=== */
