/*!
 * \file User.h
 * \brief Header of User
 *
 * PROJ: OSLL/geo2tag
 * ---------------------------------------------------------------- */


#ifndef _User_H_83C39FC3_ECFB_41CD_8902_81D6172CD890_INCLUDED_
#define _User_H_83C39FC3_ECFB_41CD_8902_81D6172CD890_INCLUDED_

#include <string>

namespace common
{
 /*!
   * User
   */
  class User
  {
    std::string m_login;
    std::string m_password;

  protected:
    User(const std::string&, const std::string&);

  public:

    const std::string& getLogin() const;
    const std::string& getPassword() const;

    ~User();    
  }; // class User
  
} // namespace common

#endif //_User_H_83C39FC3_ECFB_41CD_8902_81D6172CD890_INCLUDED_

/* ===[ End of file ]=== */
