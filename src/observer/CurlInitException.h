/* $Id$ */
/*!
 * \file CurlInitException.h
 * \brief Header of CurlInitException
 * \todo add comment here
 *
 * File description
 *
 * PROJ: OSLL/geoblog
 * ---------------------------------------------------------------- */


#ifndef _CurlInitException_H_9F2740EC_D8F2_46E4_809B_F48BFDC4638B_INCLUDED_
#define _CurlInitException_H_9F2740EC_D8F2_46E4_809B_F48BFDC4638B_INCLUDED_

namespace exception
{
 /*!
   * Class description. May use HTML formatting
   *
   */
  class CurlInitException
  {
  public:
    CurlInitException();
    
    virtual std::string getDescription() const;

    virtual ~CurlInitException();
    
  }; // class CurlInitException
  
} // namespace exception

#endif //_CurlInitException_H_9F2740EC_D8F2_46E4_809B_F48BFDC4638B_INCLUDED_

/* ===[ End of file $HeadURL$ ]=== */
