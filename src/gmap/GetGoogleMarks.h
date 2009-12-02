#ifndef __GetGoogleMarks__h__
#define __GetGoogleMarks__h__

//#include "reply.h"
#include "Position.h"
#include <QStringList>
#include <QtNetwork/QHttp>
#include <QtXml/QDomDocument>

class GetGoogleMarks:public QObject
{
    Q_OBJECT

private:
    QHttp *client;

    void execute();

    QString m_map_url;

    Position m_user_pos;

    double m_radius;

    QList<QDomNode> m_marks;

    QStringList m_cookies;



public: 

    GetGoogleMarks(QString map_url,Position user_pos,double radius,QStringList  cookies);

    const QString& getMapUrl() const;
    const QString& setMapUrl(const QString& mapUrl);

    const Position& getUserPos() const;    
    const Position& setUserPos(const Position& pos);

    const double getRadius() const;
    const double setRadius(double radius);

    const QStringList&  getCookies()const;
    const QStringList& setCookies(const QStringList& cookies);

    const QList<QDomNode>& getMarks()const;

private slots:
    void getXml();
signals:
    void done();

};

#endif
