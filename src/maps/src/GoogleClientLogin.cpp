/*! ---------------------------------------------------------------
 *  
 *
 * \file GoogleClientLogin.cpp
 * \brief GoogleClientLogin implementation
 *
 * File description
 *
 *  PROJ: OSLL/geo2tag 
 * ---------------------------------------------------------------- */

#include <iostream>
#include <sstream>
#include "GoogleClientLogin.h"
#include "CurlInitException.h"

namespace maps
{
  GoogleClientLogin::GoogleClientLogin(std::string email, std::string password):m_email(email), m_password(password), m_authToken("")
  {
    m_curl = curl_easy_init();
    if(!m_curl)
      throw exception::CurlInitException();
  }

  void GoogleClientLogin::login()
  {
    std::ostringstream ss;
    ss << "Email="<< m_email << "&Passwd=" << m_password << "&service=local&source=TestLogin";
    std::string s = ss.str();
    curl_easy_setopt(m_curl, CURLOPT_URL, "https://www.google.com/accounts/ClientLogin");
    curl_easy_setopt(m_curl, CURLOPT_VERBOSE, 1L);
    curl_easy_setopt(m_curl, CURLOPT_SSL_VERIFYPEER, 0L);
    curl_easy_setopt(m_curl, CURLOPT_SSL_VERIFYHOST, 0L);
    curl_easy_setopt(m_curl, CURLOPT_POST, 1L);
    curl_easy_setopt(m_curl, CURLOPT_POSTFIELDS, s.c_str());
    curl_easy_setopt(m_curl, CURLOPT_POSTFIELDSIZE, s.size());
    curl_easy_setopt(m_curl, CURLOPT_WRITEFUNCTION, write);
    /* Set a pointer to our struct to pass to the callback */
    std::string buf;
    curl_easy_setopt(m_curl, CURLOPT_WRITEDATA, &buf);

    CURLcode res = curl_easy_perform(m_curl);
    
    // Parsing Buffer for auth substring
    int authToken = buf.find("Auth",0);
    
    m_authToken = buf.substr(authToken,buf.size()-authToken);
  }

  std::string GoogleClientLogin::getAuthToken() const
  {
    return m_authToken;
  }

  size_t GoogleClientLogin::write(void *buffer, size_t size, size_t nmemb, void *stream)
  {
      std::string *data = reinterpret_cast<std::string *>(stream);
      if(data==NULL)
      {
         // Epic FAIL
         return -1;
      }
      for(int i=0; i<size*nmemb; ++i)
      {
         (*data)+=(((char*)buffer)[i]);
         std::cout <<(((char*)buffer)[i]);
      }
      return size*nmemb;
  }

  GoogleClientLogin::~GoogleClientLogin()
  {
    curl_easy_cleanup(m_curl);
  }
} // namespace maps

/* ===[ End of file  ]=== */
