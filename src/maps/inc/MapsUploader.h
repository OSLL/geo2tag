#ifndef MAPSUPLOADER_H
#define MAPSUPLOADER_H

#include <QNetworkAccessManager>
#include <QNetworkReply>
#include <QUrl>

#include <QDir>
#include <QFile>

#include <QPixmap>

#include "OSMCoordinatesConverter.h"

class MapsUploader : public QObject
{
    Q_OBJECT

    QNetworkAccessManager m_manager;
    const QString m_url;
    QDir m_file_store_dir;
    QVector<TilePoint> m_loaded;
    QVector<TilePoint> m_tiles;
    bool m_background_mode;

public:
    explicit MapsUploader(QObject *parent = 0);

private:
    void checkHomePath();
    void popNextTile();

signals:
    void tileUploaded(const QPixmap & pixmap, const TilePoint & point);
    void uploadingFinished();

private slots:
    void handleNetworkData(QNetworkReply *reply);
    void replyError(QNetworkReply::NetworkError code);
    void downloadTile(const TilePoint & point);

public slots:
    void uploadTiles(QVector<TilePoint> & tiles_to_upload);
    void uploadTilesBG(QVector<TilePoint> & tiles_to_upload);
};

#endif // MAPSUPLOADER_H
