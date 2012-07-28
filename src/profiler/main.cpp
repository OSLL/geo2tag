#include "LoginThread.h"
#include "TrackThread.h"
#include "LoadTagsThread.h"
#include "ProfilerThread.h"
#include <time.h>
#include <QDebug>
#include "JsonUser.h"
#include "JsonChannel.h"
#include "JsonSession.h"
#include "JsonDataMark.h"
#include <QCoreApplication>
//#include "ThreadCleaner.h"

// Usage
/* ./profiler @number_of_iterations@ @method@ @token@ @channel@
 @number_of_iterations@ - number of iterations that should be performed(or -1 if it should work till infinity);
 @method@ - performed operation - login | writeTag | loadTags;
 @token@ - auth_token for track operation;
 @channel@ - channel for track operation;
*/

int main(int argc,char** argv)
{
  QCoreApplication app(argc,argv);
  if (argc >= 3)
  {
    QEventLoop loop;
    int number_of_op = QString(argv[1]).toInt();
    ProfilerThread::m_number_of_requests = number_of_op;
    QSharedPointer<Session> session(new JsonSession(argv[3], QDateTime::currentDateTime(), QSharedPointer<common::User>(NULL)));
    if (QString(argv[2]) == "login")    //perform login testing
    {
      srand ( time(NULL) );
      for (int i=0;i<4;i++)
      {
        LoginThread * thread = new LoginThread();
        QObject::connect(thread,SIGNAL(terminated()),&loop,SLOT(quit()));
        QObject::connect(thread,SIGNAL(finished()),&loop,SLOT(quit()));
        thread->start();
      }
                                        // perform track testing
    }else if (QString(argv[2]) == "writeTag")
    {
      QSharedPointer<DataMark> tag(new JsonDataMark(0.0,0.0,0.0,"","","",QDateTime::currentDateTime()));
      QSharedPointer<Channel> channel(new JsonChannel(argv[4],"",""));
      for (int i=0;i<4;i++)
      {
        TrackThread * thread = new TrackThread(session,channel,tag);
        QObject::connect(thread,SIGNAL(terminated()),&loop,SLOT(quit()));
        QObject::connect(thread,SIGNAL(finished()),&loop,SLOT(quit()));
        thread->start();
      }
                                        // perform track testing
    }else if (QString(argv[2]) == "loadTags")
    {
      for (int i=0;i<4;i++)
      {
        LoadTagsThread * thread = new LoadTagsThread(session);
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
