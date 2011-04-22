#ifndef MAPSUPLOADER_H
#define MAPSUPLOADER_H

#include <QPixmap>
#include <QNetworkAccessManager>
#include <QUrl>
#include <QFile>
#include "TranslateCoordinatesOSM.h"

class MapsUploader : public QObject
{
    Q_OBJECT

    QNetworkAccessManager m_manager;
    QUrl m_url;
    QVector<TilePoint> m_loaded;
	QVector<QFile *> m_files;

private:
	void loadCachedFiles();

public:
    explicit MapsUploader(QObject *parent = 0);
	~MapsUploader();

signals:
    void tileUploaded(const QPixmap & pixmap, const QPoint & point, int zoom);

private slots:

    void handleNetworkData(QNetworkReply *reply);
    void downloadTile(int lat, int lg, int zoom);

public slots:
    void uploadTiles(QVector<TilePoint> & tiles_to_upload);

};

#endif // MAPSUPLOADER_H
