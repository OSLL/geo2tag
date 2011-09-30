#ifndef VKREQUEST_H
#define VKREQUEST_H

#include <QObject>
#include <QNetworkAccessManager>
#include <QNetworkReply>
#include <QNetworkRequest>

class vkRequest : public QObject
{
  Q_OBJECT
    private:
    QNetworkAccessManager manager;
    QNetworkReply *replyProfile;
  public:
    explicit vkRequest(QObject *parent = 0);

  public slots:
    void getProfile(const QString &token,const QString userId);
    void getReplyFinished();

};
#endif                                  // VKREQUEST_H
