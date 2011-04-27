#ifndef MAPSUPLOADTHREAD_H
#define MAPSUPLOADTHREAD_H

#include <QObject>
#include <QPoint>
#include <QPixmap>
#include <QNetworkAccessManager>
#include <QNetworkReply>
#include <QUrl>
#include <QFile>
#include <QDir>
#include <QThread>
#include "MapsUploader.h"

class MapsUploadThread : public QThread
{
    Q_OBJECT

    QVector<TilePoint> m_tiles;

public:
    explicit MapsUploadThread(QVector<TilePoint> & tiles, QObject * parent = 0);
    void run();

private slots:
    void uploadingFinished();
};

#endif // MAPSUPLOADTHREAD_H
