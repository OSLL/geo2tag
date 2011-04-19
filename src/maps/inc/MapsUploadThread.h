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

typedef QPair<QPoint,int> TilePoint;

class MapsUploadThread : public QThread
{
    Q_OBJECT

    QNetworkAccessManager * m_manager;
    QUrl m_url;
	QDir m_file_store_dir;
    QVector<TilePoint> m_tiles;
	QVector<QFile *> m_files;

private:
	void checkHomePath();
	void popNextTile();

public:
    explicit MapsUploadThread(QVector<TilePoint> & tiles, QObject *parent = 0);
	void run();

private slots:
    void handleNetworkData(QNetworkReply *reply);
	void replyError(QNetworkReply::NetworkError code);
    void downloadTile(int lat, int lg, int zoom);
};

#endif // MAPSUPLOADTHREAD_H
