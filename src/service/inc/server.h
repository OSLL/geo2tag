#ifndef _SERVER_H_
#define _SERVER_H_

#include <QThread>
#include <fcgiapp.h>

class Server: private QThread
{

  FCGX_Request m_cgi;
  QString extractRESTQuery();
  void extractIncomingData(const FCGX_Request& request, QString& queryString, QByteArray& queryBody);
  public:

    QByteArray process( const QByteArray&);

    void run();

  public:

    Server();

    void serve();

    ~Server();
};
#endif
