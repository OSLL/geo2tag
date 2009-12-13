/*  */
/*!
 * \file GoogleClientLogin.h
 * \brief Header of GoogleClientLogin
 *
 *  PROJ: OSLL/geo2tag 
 * ---------------------------------------------------------------- */


#ifndef _GoogleClientLogin_H_E398ED1A_0E83_42CA_BC67_5C3668729B16_INCLUDED_
#define _GoogleClientLogin_H_E398ED1A_0E83_42CA_BC67_5C3668729B16_INCLUDED_

#include <string>
#include <curl/curl.h>

namespace maps
{
  /*!
   * Google client auth
   */
  class GoogleClientLogin
  {
    CURL *m_curl;
    std::string m_email;
    std::string m_password;

    std::string m_authToken;

  public:
    GoogleClientLogin(std::string email, std::string password);

    void login();

    std::string getAuthToken() const;

    ~GoogleClientLogin();
    
  private:    
    GoogleClientLogin(const GoogleClientLogin& obj);
    GoogleClientLogin& operator=(const GoogleClientLogin& obj);

    static size_t write(void *buffer, size_t size, size_t nmemb, void *stream);
  }; // class GoogleClientLogin
  
} // namespace maps

#endif //_GoogleClientLogin_H_E398ED1A_0E83_42CA_BC67_5C3668729B16_INCLUDED_

/* ===[ End of file  ]=== */
