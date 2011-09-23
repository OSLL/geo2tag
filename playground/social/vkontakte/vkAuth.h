#ifndef VKAUTH_H
#define VKAUTH_H
#include <QWebView>
#include <QNetworkAccessManager>
#include <QNetworkReply>
#include <QNetworkRequest>


class vkAuth:public QWebView
{
    Q_OBJECT
private:
    QString token;
    QUrl url;
    int expires;
    QString userId;
    QNetworkAccessManager manager;
    QNetworkReply *reply;
public:
    vkAuth(QString appid, QWebView *parent=0);
    QString & getToken();
    QString getUserId();

private slots:
    void slotChanged(const QUrl & url);
    void getReplyFinished();
};
#endif // VKAUTH_H
