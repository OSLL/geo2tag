#include "Client.h"
#include <QDebug>
#include <QEventLoop>
#include "JsonDataMark.h"
#include "JsonChannel.h"
#include "JsonUser.h"
#include "GpsInfo.h"

#include <QTimer>
#include <QVariant>
#include "Settings.h"

// Interval for different requests
#define PAUSE_INTERVAL 250
#define DEFAULT_CHANNEL "default"

Client::Client(ContactModel *contactModel, QObject *parent) :
QObject(parent),m_trackInterval(5),
m_authentificated(0), m_trackingPermitted(Settings::getInstance().getPermission()),
m_isHavingOwnChannel(true), m_contactModel(contactModel)
{
  m_timer = new QTimer(this);
  connect(m_timer, SIGNAL(timeout()), SLOT(track()));

  m_additionalTimer = new QTimer(this);
  connect(m_additionalTimer, SIGNAL(timeout()),SLOT(getTagsRequest()));

  m_loginQuery = new LoginQuery(this);
  connect(m_loginQuery, SIGNAL(connected()), SLOT(onAuthentificated()));
  connect(m_loginQuery, SIGNAL(errorOccured(QString)), SLOT(onError(QString)));
  connect(m_loginQuery, SIGNAL(errorOccured(int)),SLOT(onError(int)));

  m_RegisterUserQuery = new RegisterUserQuery(this);
  connect(m_RegisterUserQuery, SIGNAL(connected()), SLOT(onRegistered()));
  connect(m_RegisterUserQuery, SIGNAL(errorOccured(int)),SLOT(onError(int)));

  m_netManager = new QNetworkConfigurationManager(this);

  m_history = new MarksHistory(this);
  connect(m_history,SIGNAL(isFull()),SLOT(onHistoryFull()));
  m_history->setHistoryLimit(Settings::getInstance().getTimeInterval()/m_trackInterval);

  m_addNewMarkQuery = new WriteTagQuery(this);
  connect(m_addNewMarkQuery,SIGNAL(tagAdded()),SLOT(onMarkAdded()));
  //  connect(m_addNewMarkQuery, SIGNAL(errorOccured(QString)), SIGNAL(error(QString)));

  m_applyChannelQuery = new ApplyChannelQuery(this);
  connect(m_applyChannelQuery, SIGNAL(channelAdded(QSharedPointer<Channel>)),SLOT(subscribeToOwnChannel()));
  connect(m_applyChannelQuery, SIGNAL(errorOccured(int)), SLOT(onError(int)));

  m_subscribeChannelQuery = new SubscribeChannelQuery(this);
  connect(m_subscribeChannelQuery,SIGNAL(channelSubscribed(QSharedPointer<Channel>)),SLOT(onChannelSubscribed(QSharedPointer<Channel>)));
  connect(m_subscribeChannelQuery, SIGNAL(errorOccured(int)), SLOT(onError(int)));

  m_subscibedChannelsQuery = new SubscribedChannelsQuery(this);
  //connect(m_subscibedChannelsQuery,SIGNAL(responseReceived()),SLOT(constructContactModel()));
  connect(m_subscibedChannelsQuery,SIGNAL(errorOccured(int)), SLOT(onError(int)));

  m_loadTagsQuery = new LoadTagsQuery(this);
  connect(m_loadTagsQuery, SIGNAL(tagsReceived()), SLOT(onGetTags()));
  connect(m_loadTagsQuery,SIGNAL(errorOccured(int)), SLOT(onError(int)));

  m_unsubscribeChannelQuery = new UnsubscribeChannelQuery(this);
  connect(m_unsubscribeChannelQuery, SIGNAL(channelUnsubscribed()),SLOT(onChannelUnsubscribed()));
  connect(m_unsubscribeChannelQuery,SIGNAL(errorOccured(int)), SLOT(onError(int)));

  qDebug()<<Settings::getInstance().getLogin();
  qDebug()<<Settings::getInstance().getPassword();

  if (Settings::getInstance().isHavingAuthData())
    auth(Settings::getInstance().getLogin(),Settings::getInstance().getPassword());

}


void Client::setPermission(bool permission)
{
  m_trackingPermitted = permission;
  if ((permission) && (!isTracking()))
    startTrack();
  else
  if ((!permission) && (isTracking()))
    stopTrack();
}


bool Client::isTrackingPermitted()
{
  return m_trackingPermitted;
}


void Client::auth(QString user, QString pass)
{
  m_loginQuery->setQuery(user,pass);
  m_loginQuery->doRequest();

}


void Client::registration(const QString &email, const QString &user, const QString &pass)
{
  Settings::getInstance().setLogin(user);
  Settings::getInstance().setPassword(pass);
  m_RegisterUserQuery->setQuery(email, user,pass);
  m_RegisterUserQuery->doRequest();
}


void Client::onError(QString err)
{

  emit error(QVariant(err));
  m_lastError = err;
  if(!m_authentificated)
  {
    // Retry authentification
    qDebug() << "Error during authentification";
    pause(1000);
    m_loginQuery->doRequest();

  }
  else
  {
    qDebug() << "Error during mark sending";
  }
}


void Client::onError(int err)
{
  qDebug()<<"err="<<err;
  if (err==INCORRECT_CREDENTIALS_ERROR)
    emit error(QVariant("Incorrect login or password"));
  else
  if (err==USER_ALREADY_EXIST_ERROR)
    emit error(QVariant("User with that name already exist"));
  else
  if (err==CHANNEL_DOES_NOT_EXIST_ERROR)
    emit error(QVariant("User with that name isn't existed"));

}


bool Client::isAuthentificated()
{
  return m_authentificated;
}


QString Client::getLastError()
{
  return m_lastError;
}


// Return internete state
bool Client::isOnline()
{
  return m_netManager->isOnline();
}


void Client::sendHistory()
{
  while(!m_history->isEmpty() && isOnline())
  {
    sendLastCoordinate();
    pause(250);
  }
}


void Client::onAuthentificated()
{
  qDebug() << "Authentificated " <<  m_loginQuery->getUser()->getToken();

  m_user =  m_loginQuery->getUser();
  Settings::getInstance().setLogin(m_user->getLogin());
  Settings::getInstance().setPassword(m_user->getPassword());

  m_authentificated = true;
  if (!m_isHavingOwnChannel)
  {
    QSharedPointer<Channel> channel = QSharedPointer<Channel>(new Channel(m_user->getLogin(),m_user->getLogin() + "'s channel"));
    channel->setRadius(40000000);
    m_applyChannelQuery->setQuery(channel, m_user);
    m_applyChannelQuery->doRequest();
  }
  getTagsRequest();
  startTrack();

  emit authentificated(QVariant(m_user->getLogin()));
}


void Client::process(QNetworkReply *)
{
  m_isHavingOwnChannel =false;
  //pause(10000);
  auth(Settings::getInstance().getLogin(), Settings::getInstance().getPassword());
}


void Client::onRegistered()
{
  qDebug() << "Registered " <<  m_RegisterUserQuery->getConfirmUrl();
  QNetworkAccessManager *manager = new QNetworkAccessManager(this);
  QNetworkRequest request;
  request.setUrl(m_RegisterUserQuery->getConfirmUrl());
  manager->get(request);
  connect(manager, SIGNAL(finished(QNetworkReply*)),this, SLOT(process(QNetworkReply*)));
  // connect(manager, SIGNAL(sslErrors(QNetworkReply*,QList<QSslError>)), this, SLOT(handleError()));

}


void Client::subscribeChannel(const QString &channel)
{
  QSharedPointer<Channel> m_channel = QSharedPointer<Channel>(new Channel(channel,channel));
  qDebug()<<"channel: "<<m_channel->getName();
  m_subscribeChannelQuery->setQuery(m_channel, m_user);
  m_subscribeChannelQuery->doRequest();
}


void Client::subscribeToOwnChannel()
{
  subscribeChannel(m_user->getLogin());
}


void Client::onChannelSubscribed(QSharedPointer<Channel> channel)
{
  qDebug()<<"Channel is subscribed ="<<channel->getName();
  m_channels.insert(channel,"");
  if (channel->getName()!=m_user->getLogin())
    m_contactModel->addContact(QSharedPointer<Contact>(new Contact(channel->getName(),channel->getName())));
  Settings::getInstance().setCustomName(channel->getName(),channel->getName());
  pause(1000);
  getTagsRequest();
}


void Client::pause(int msecs)
{
  QEventLoop eventLoop;
  QTimer::singleShot(msecs, &eventLoop, SLOT(quit()));
  eventLoop.exec();
}


void Client::startTrack()
{
  if (!m_authentificated) emit authRequest();
  else
  if (!m_timer->isActive())
  {
    m_timer->start(m_trackInterval*1000);
    if (m_additionalTimer->isActive())
      m_additionalTimer->stop();
  }

}


void Client::onMarkAdded()
{
  qDebug()<<"Mark added";
}


void Client::track()
{
  // Primitive stub for position source
  double lat=common::GpsInfo::getInstance().getLatitude();
  double lon=common::GpsInfo::getInstance().getLongitude();
  /*  double lat=30+0.01*(qrand()%100);
    double lon=60+0.01*(qrand()%100);*/
  QSharedPointer<DataMark> mark(new JsonDataMark(0,lat,lon,"m_name",
    "this tag was generated","unknown",QDateTime::currentDateTime()));

  QSharedPointer<Channel> channel(new JsonChannel(m_user->getLogin(),""));
  mark->setChannel(channel);
  m_history->pushMark(mark);
}


void Client::onHistoryFull()
{
  if (isOnline() && isAuthentificated())
  {
    sendHistory();
    getTagsRequest();
  }
}


void Client::onGoOffEvent()
{
  sendHistory();
}


void Client::onNetworkEvent(bool state)
{
  if (state) sendHistory();
}


void Client::sendLastCoordinate()
{
  m_addNewMarkQuery->setTag(m_history->popMark());
  m_addNewMarkQuery->getTag()->setUser(m_user);
  m_addNewMarkQuery->doRequest();

}


bool Client::isTracking()
{
  return m_timer->isActive();
}


void Client::stopTrack()
{
  if (m_timer->isActive())
  {
    m_timer->stop();
    m_additionalTimer->start(m_history->getHistoryLimit()*m_trackInterval*1000);
  }
  if (isOnline() && isAuthentificated() && !m_history->isEmpty()) sendHistory();
}


void Client::setHistoryLimit(int sec)
{
  m_history->setHistoryLimit(sec/m_trackInterval);

}


void Client::getTagsRequest()
{
  m_loadTagsQuery->setQuery(m_user,0,0,40000000);
  m_loadTagsQuery->doRequest();
}


void Client::onGetTags()
{
  QList<QSharedPointer <Channel> > channels = m_loadTagsQuery->getData().uniqueKeys();
  for (int i=0; i<channels.size(); i++)
  {
    if (channels.at(i)->getName()!=m_user->getLogin())
    {
      QList<QSharedPointer<DataMark> > data = m_loadTagsQuery->getData().values(channels.at(i));
      if (!data.isEmpty())
      {
        QSharedPointer<DataMark> mark = data.at(data.size()-1);
        STATUS_TYPE type = LOST;
        m_contactModel->getContactByName(channels.at(i)->getName())->setLastDataMark(mark);
        qDebug()<<"---------------------------------";
        qDebug()<<"channel name="<<channels.at(i)->getName();
        qDebug()<<"mark lng="<<mark->getLongitude();
        qDebug()<<"mark lat="<<mark->getLatitude();
        qDebug()<<"---------------------------------";
      }

    }

  }

}


void Client::unSubscribeChannelRequest(const QString &channelName)
{
  QSharedPointer<Channel> channel = QSharedPointer<Channel>(new Channel(channelName, channelName + "'s channel"));
  m_unsubscribeChannelQuery->setQuery(m_user,channel);
  m_unsubscribeChannelQuery->doRequest();
}


void Client::onChannelUnsubscribed()
{
  m_contactModel->removeContact(m_unsubscribeChannelQuery->getChannel()->getName());

}


void Client::logout()
{
  if (isTracking())
    stopTrack();
  else
  if (m_additionalTimer->isActive())
    m_additionalTimer->stop();
  Settings::getInstance().setLogin("unknown");
  Settings::getInstance().setPassword("unknown");
  int size = m_contactModel->getContacts().size();
  for (int i=0;i<size;i++)
    m_contactModel->removeContact(m_contactModel->getContacts().at(0)->getChannelName());

}
