/*  */
/*!
 * \file DefaultException.h
 * \brief Header of DefaultException
 *
 *  PROJ: OSLL/geo2tag 
 * ---------------------------------------------------------------- */


#ifndef _DefaultException_H_B503B3CC_1500_4544_8732_F56CD8A9F61B_INCLUDED_
#define _DefaultException_H_B503B3CC_1500_4544_8732_F56CD8A9F61B_INCLUDED_

#include <string>

namespace exception
{
  /*!
   * Default exception. All exception will be ingerited from this one
   */
  class DefaultException
  {
  public:
    DefaultException();
  
    virtual std::string getDescription() const=0;
   
    virtual ~DefaultException();
    
  private:    
    DefaultException(const DefaultException& obj);
    DefaultException& operator=(const DefaultException& obj);

  }; // class DefaultException
  
} // namespace exception

#endif //_DefaultException_H_B503B3CC_1500_4544_8732_F56CD8A9F61B_INCLUDED_

/* ===[ End of file  ]=== */
