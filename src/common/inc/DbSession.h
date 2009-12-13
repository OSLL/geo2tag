/*!
 * \file DbSession.h
 * \brief Header of DbSession
 * \todo add comment here
 *
 * File description
 *
 * PROJ: OSLL/geo2tag
 * ---------------------------------------------------------------- */


#ifndef _DbSession_H_9BF6A8FE_DA47_4F7A_B008_2EA2842C490F_INCLUDED_
#define _DbSession_H_9BF6A8FE_DA47_4F7A_B008_2EA2842C490F_INCLUDED_

#include "Db.h"
#include "DataMarks.h"

namespace common
{
  /*!
   * \brief session with database
   */
  class DbSession: protected ODBC::CDbEnv, public ODBC::CDbConn
  {
  public:
    DbSession();

    ~DbSession();
    
    void loadMarks(DataMarks& marks);

    void storeMarks(DataMarks& marks);

  private:    
    DbSession(const DbSession& obj);
    DbSession& operator=(const DbSession& obj);

  }; // class DbSession
  
} // namespace common

#endif //_DbSession_H_9BF6A8FE_DA47_4F7A_B008_2EA2842C490F_INCLUDED_

/* ===[ End of file ]=== */
