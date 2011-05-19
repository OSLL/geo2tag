#include "MaemoDaemon.h"

MaemoDaemon::MaemoDaemon:
m_daemon(new QTcpSocket()),
m_device(new QTextStream(m_daemon)),
m_started(false)
{
  connect(m_daemon,SIGNAL(readyRead()), SLOT(readData()));
  m_daemon->connectToHost("127.0.0.1", 31234);
}


void MaemoDaemon::readData()
{

  QString message = m_device->readAll();

  // split recieved message for many parts, process last and clean
  QStringList commands=message.split(" ",QString::SkipEmptyParts);
  qDebug() << "recieved from trackerDaemon " << message;
  if (commands.last().indexOf("lastCoords_")!=-1)
  {
    QString buf=commands.last();
    QStringList lastCoord = buf.right(buf.size()-QString("lastCoords_").size()).split(",",QString::SkipEmptyParts);

    qDebug() << "REcieved coordinates" << lastCoord.at(0) << " " lastCoord.at(1);
    m_lastCoordinates.setX(lastCoord.at(0).toDouble());
    m_lastCoordinates.setY(lastCoord.at(1).toDouble());
    m_started = true;
  }
  if (commands.last()=="stoped")
  {
    m_started = false;
  }
}


void MaemoDaemon::start()
{
  (*m_device) << "start ";
}


void MaemoDaemon::stop()
{
  (*m_device) << "stop ";
}


void MaemoDaemon::reload()
{
  (*m_device) << "reload ";
}


void MaemoDaemon::restart()
{
  system("pkill  \'wikigpsTracker$\'");
  system("/usr/bin/wikigpsTracker > /dev/null 2>&1 &");
}


QPointF MaemoDaemon::getLastCoordinates() const
{
  return m_lastCoordinates;
}


bool MaemoDaemon::isConnected() const
{
  return (m_daemon->state() >= QAbstractSocket::ConnectedState);
}


bool MaemoDaemon::isStarted() const
{
  return m_started;
}
