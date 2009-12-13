/**************************************************
 *
 *
 **************************************************
 * This code was created by Peter Harvey @ CodeByDesign.
 * Released under GPL 18.FEB.99
 *
 * Contributions from...
 * -----------------------------------------------
 * Peter Harvey         - pharvey@codebydesign.com
 **************************************************/
#ifndef classODBC_included
#define classODBC_included

#include "classNode.h"
#include "classCanvas.h"
class classDrivers ;
class classDataSources ;
#include <qstring.h>
#include <qcursor.h>
#include <qnamespace.h>
#include <qmessagebox.h>
#include <qlistview.h>
#include <sqlext.h>

#define MAX_COLUMN_WIDTH 1024

// ******************************
//  Common Utility Classes
// ******************************

// Use to display ODBC error messages
void my_msgBox( const QString & className,
             const QString & actionName,
             SQLRETURN rc   = 0,
             SQLHENV  hEnv  = NULL,
             SQLHDBC  hDbc  = NULL,
             SQLHSTMT hStmt = NULL,
             const QString & moreInfo = "",
             QMessageBox::Icon icon = QMessageBox::Critical ) ;

// Cursor scoper class
class CursorScoper: public QWidget
{
 public:
  CursorScoper( QWidget * w ) : w_(w)
    { w_->setCursor(waitCursor) ; }
 ~CursorScoper()
    { w_->unsetCursor()         ; }
 private:
   QWidget *w_ ;
} ;

// SQL ENV scoper class
class EnvironmentScoper
{
 public:
  EnvironmentScoper( const QString & className = "" ) : hEnv_( NULL ) , className_( className )
  {
    SQLRETURN nReturn ;
    if (!SQL_SUCCEEDED(nReturn=SQLAllocEnv( &hEnv_ ) ) )
      my_msgBox( className_, "SQLAllocEnv", nReturn ) ;
  }

  HENV operator()() const
  {
    return hEnv_ ;
  }

 ~EnvironmentScoper()
  {
    SQLRETURN nReturn ;
    if (hEnv_)
      if (!SQL_SUCCEEDED(nReturn=SQLFreeEnv( hEnv_ ) ) )
        my_msgBox( className_, "SQLFreeEnv", nReturn ) ;
    hEnv_ = NULL ;
  }

 private:
  HENV hEnv_ ;
  QString className_ ;

} ;

// SQL DBC scoper class
class ConnectionScoper
{
 public:
  ConnectionScoper( HENV hEnv, const QString & className = "" ) : hDbc_( NULL ) , className_( className ), active_( false )
  {
    SQLRETURN nReturn ;
    if (!SQL_SUCCEEDED(nReturn=SQLAllocConnect( hEnv, &hDbc_ ) ) )
      my_msgBox( className_, "SQLAllocConnect", nReturn, hEnv ) ;
  }

  HDBC operator()() const
  {
    return hDbc_ ;
  }

  bool connect( const QString & dataSourceName, const QString & uid, const QString & pwd )
  {
    SQLRETURN nReturn ;
    if (!active_ && !SQL_SUCCEEDED(nReturn=SQLConnect( hDbc_, (SQLCHAR*)dataSourceName.data(), SQL_NTS, (SQLCHAR*)uid.data(), SQL_NTS, (SQLCHAR*)pwd.data(), SQL_NTS ) ) )
      my_msgBox( className_, "SQLConnect", nReturn, NULL, hDbc_, NULL, dataSourceName ) ;
    else
      active_ = true ;

    return active_ ;
  }

  void disconnect()
  {
    SQLRETURN nReturn ;
    if (active_ && !SQL_SUCCEEDED(nReturn=SQLDisconnect( hDbc_ ) ) )
        my_msgBox( className_, "SQLDisconnect", nReturn, NULL, hDbc_ ) ;
    active_ = false ;
  }

  bool active() const
  {
    return active_ ;
  }

 ~ConnectionScoper()
  {
    SQLRETURN nReturn ;
    if (hDbc_)
    {
      if (active_ && !SQL_SUCCEEDED(nReturn=SQLDisconnect( hDbc_ ) ) )
        my_msgBox( className_, "SQLDisconnect", nReturn, NULL, hDbc_ ) ;

      if (!SQL_SUCCEEDED(nReturn=SQLFreeConnect( hDbc_ ) ) )
        my_msgBox( className_, "SQLFreeConnect", nReturn, NULL, hDbc_ ) ;

      hDbc_ = NULL ;
      active_ = false ;
    }
  }

 private:
  HDBC hDbc_ ;
  bool active_ ;
  QString className_ ;

} ;

// Statement scoper class
class StatementScoper
{
 public:
  StatementScoper( HDBC hDbc, const QString & className = "" ) : hStmt_( NULL ) , className_( className )
  {
    SQLRETURN nReturn ;
    if (!SQL_SUCCEEDED(nReturn=SQLAllocStmt( hDbc, &hStmt_ ) ) )
      my_msgBox( className_, "SQLAllocStmt", nReturn, NULL, hDbc ) ;
  }

  HSTMT operator()() const
  {
    return hStmt_ ;
  }

  ~StatementScoper()
  {
    SQLRETURN nReturn ;
    if (hStmt_)
      if (!SQL_SUCCEEDED(nReturn=SQLFreeStmt( hStmt_, SQL_DROP ) ) )
        my_msgBox( className_, "SQLFreeStmt", nReturn, NULL, NULL, hStmt_ ) ;
    hStmt_ = NULL ;
  }

 private:
  HSTMT hStmt_ ;
  QString className_ ;

} ;

// ******************************
// ******************************

class classODBC: public classNode
{
public:
    enum DSType
    {
      User,
      System
    };

    classODBC( QListView *pParent, classCanvas *pCanvas );
   ~classODBC() {}

    void setOpen( bool );
    void selectionChanged ( QListViewItem * );

private:
    classDrivers      *pDrivers;
    classDataSources  *pDataSourcesUser;
    classDataSources  *pDataSourcesSystem;

};
#endif

