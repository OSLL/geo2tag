#include <sstream>
#include <QApplication>
#include <QDebug>
#include <QMessageBox>
#include "MainWindow.h" 
#include <curl/curl.h>
#include "DbSession.h"

int main(int c, char **v)
{
  QApplication 	app(c,v);

  try
  {
    common::DbSession::getInstance().loadData("test0","test");
  }
  catch(ODBC::CException &x)
  {
    std::ostringstream s;
    s << x;
    qDebug() << s.str().c_str();
    QMessageBox::critical(NULL, QObject::tr("Error"), QObject::tr("No database connection!!! Exiting..."));
    return 0;
  }
  catch(CException &x)
  {
    std::ostringstream s;
    s << x;
    qDebug() << s.str().c_str();
    QMessageBox::critical(NULL, QObject::tr("Error"), QObject::tr(x.getDescription().c_str()));
    return 0;
  }
  

  if(curl_global_init(CURL_GLOBAL_ALL)!=0)
  {
    QMessageBox::critical(NULL, QObject::tr("Error"), QObject::tr("Error during initialization! Maps will not be able to able.  Exiting..."));
    return 0;
  }

  GUI::MainWindow window;

#ifdef DESKTOP_STYLE
  window.show();
#else  
  window.showMaximized();
#endif  

  app.exec();
  curl_global_cleanup();
  try
  {
    common::DbSession::getInstance().saveData();
  }
  catch(ODBC::CException &x)
  {
    std::ostringstream s;
    s << x;
    qDebug() << s.str().c_str();
    QMessageBox::critical(NULL, QObject::tr("Error"), QObject::tr("Error during saving data... Some data can be lost."));
    return 0;
  }
  catch(CException &x)
  {
    std::ostringstream s;
    s << x;
    qDebug() << s.str().c_str();
    QMessageBox::critical(NULL, QObject::tr("Error"), QObject::tr(x.getDescription().c_str()));
    return 0;
  }

  return 0;

}

