#ifndef VKAUTH_H
#define VKAUTH_H
#include <QWebView>

class vkAuth:public QWebView
{
  Q_OBJECT
    private:
    QString token;
    QUrl url;
    int expires;
    QString userId;

  public:
    vkAuth(QString appid, QWidget *parent=0);
    QString & getToken();
    QString getUserId();
    signals:
    void success(const QString &token, const QString &userId, int expires);
  private slots:
    void slotChanged(const QUrl & url);

};
#endif                                  // VKAUTH_H
