#include "LoginThread.h"
#include "TrackThread.h"
#include <time.h>
#include <QDebug>
#include "JsonUser.h"
#include "JsonDataMark.h"
#include <QCoreApplication>
#include "ThreadCleaner.h"

int main(int argc,char** argv)
{
  QCoreApplication app(argc,argv);
  if (argc >= 2)
  {
    QEventLoop loop;

    if (QString(argv[1]) == "login") //perform login testing
    {
      srand ( time(NULL) );

      for (int i=0;i<4;i++)
      {
	LoginThread * thread = new LoginThread();
        QObject::connect(thread,SIGNAL(terminated()),&loop,SLOT(quit()));
        QObject::connect(thread,SIGNAL(finished()),&loop,SLOT(quit()));
        thread->start(); 
      }
    }else if (QString(argv[1]) == "track") // perform track testing
    {
      QSharedPointer<DataMark> tag(new JsonDataMark(0.0,0.0,0.0,"","","",QDateTime::currentDateTime()));
      QSharedPointer<common::User> user(new JsonUser("","",argv[2]));
      tag->setUser(user);
      for (int i=0;i<4;i++)
      {
	TrackThread * thread = new TrackThread(tag);
        QObject::connect(thread,SIGNAL(terminated()),&loop,SLOT(quit()));
        QObject::connect(thread,SIGNAL(finished()),&loop,SLOT(quit()));
        thread->start(); 
      }
    } else 
    {
       qDebug() <<"unknown request";
       return 1;
    }

    loop.exec();
    return 0;

  }else    qDebug() << "not enough args!";

  return 1;

} 
