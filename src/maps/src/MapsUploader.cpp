#include "MapsUploader.h"
#include <math.h>
#include <QNetworkReply>
#include <QNetworkDiskCache>
#include <QDesktopServices>
#include <QVector>
#include <QVectorIterator>
#include <QDebug>
#include <QDir>
#include <QBuffer>


/*
 * For use QPoint in QHash
 */
uint qHash(const QPoint & p)
{
	return p.x() * pow( 17, p.y() );
}


QPointF tileForCoordinate(qreal lat, qreal lng, int zoom)
{
    qreal zn = static_cast<qreal>(1 << zoom);
    qreal tx = (lng + 180.0) / 360.0;
    qreal ty = (1.0 - log(tan(lat * M_PI / 180.0) +
                          1.0 / cos(lat * M_PI / 180.0)) / M_PI) / 2.0;
    return QPointF(tx * zn, ty * zn);
}

qreal longitudeFromTile(qreal tx, int zoom)
{
    qreal zn = static_cast<qreal>(1 << zoom);
    qreal lat = tx / zn * 360.0 - 180.0;
    return lat;
}

qreal latitudeFromTile(qreal ty, int zoom)
{
    qreal zn = static_cast<qreal>(1 << zoom);
    qreal n = M_PI - 2 * M_PI * ty / zn;
    qreal lng = 180.0 / M_PI * atan(0.5 * (exp(n) - exp(-n)));
    return lng;
}

MapsUploader::MapsUploader(QObject *parent) :
    QObject(parent)
{
//    QNetworkDiskCache *cache = new QNetworkDiskCache;
//    cache->setCacheDirectory(QDesktopServices::storageLocation
//                             (QDesktopServices::CacheLocation));
//    m_manager.setCache(cache);

	this->loadCachedFiles();

    connect(&m_manager, SIGNAL(finished(QNetworkReply*)),
            this, SLOT(handleNetworkData(QNetworkReply*)));
}

MapsUploader::~MapsUploader()
{
//	for(QVectorIterator<QFile *> it = m_files.begin(); it != m_files.end(); ++it)
//	{
//		if(*it != 0) it->unmap();
//	}
}

void MapsUploader::loadCachedFiles()
{
	QDir home_dir = QDir::home();

	if(!home_dir.exists(".geo2tag/uploaded_maps/"))
		home_dir.mkpath(".geo2tag/uploaded_maps/");
}


void MapsUploader::handleNetworkData(QNetworkReply *reply)
{
    QImage img;
    QList<QVariant> data = reply->request().attribute(QNetworkRequest::User).toList();
    QPoint tp = data.front().toPoint();
    int zoom = data.back().toInt();
    QUrl url = reply->url();
    if (!reply->error())
        if (!img.load(reply, 0))
            img = QImage();
    reply->deleteLater();

    if(!img.isNull())
    {
        QPixmap pixmap = QPixmap::fromImage(img);
		QDir file_store_path = QDir().home();
		if(!file_store_path.exists(".geo2tag/uploaded_maps/" + QString::number(zoom) + "/"))
			file_store_path.mkpath(".geo2tag/uploaded_maps/" + QString::number(zoom) + "/");
		file_store_path.cd(".geo2tag/uploaded_maps/" + QString::number(zoom) + "/");
		QString file_name = file_store_path.path() + "/" + QString::number(tp.x()) + "_" + QString::number(tp.y());// + ".png";
		if(!QFile(file_name).exists())
		{
			//qDebug() << file_name << "\n";
			QByteArray bytes;
			QBuffer buffer(&bytes);
			buffer.open(QIODevice::WriteOnly);
			pixmap.save(&buffer, "PNG"); // writes pixmap into bytes in PNG format
			bytes = qCompress(bytes,9);
			QFile file(file_name);
			file.open(QIODevice::WriteOnly);
			file.write(bytes);
			file.flush();
			file.close();
			//pixmap.save(file_name, 0, 100);
		}
        emit this->tileUploaded(pixmap, tp, zoom);
    }
}

void MapsUploader::downloadTile(int lat, int lg, int zoom)
{
    QPoint grab(lg, lat);

	QDir file_store_path = QDir().home();
	if(!file_store_path.exists(".geo2tag/uploaded_maps/" + QString::number(zoom) + "/"))
		file_store_path.mkpath(".geo2tag/uploaded_maps/" + QString::number(zoom) + "/");
	file_store_path.cd(".geo2tag/uploaded_maps/" + QString::number(zoom) + "/");
	QString file_name = file_store_path.path() + "/" + QString::number(lg) + "_" + QString::number(lat);// + ".png";
	if(QFile::exists(file_name))
	{
		//qDebug() << file_name << "\n";

		QFile pixmap_file(file_name, this);
		//m_files.push_back(pixmap_file);
		pixmap_file.open(QFile::ReadOnly);
		QPixmap pixmap;
		if( pixmap.loadFromData( qUncompress(pixmap_file.readAll()) ) )
		{
			//qDebug() << "File loaded from cache\n";
			emit this->tileUploaded(pixmap, grab, zoom);
		}
		else
		{
			qDebug() << "Error!\n";
		}
		pixmap_file.close();
	}
	else
	{
		QString path = "http://tile.openstreetmap.org/%1/%2/%3.png";
		m_url = QUrl(path.arg(zoom).arg(lg).arg(lat));
		//qDebug() << m_url << "\n";
		QNetworkRequest request;
		request.setUrl(m_url);
		request.setRawHeader("User-Agent", "OSLL Observer");
		QList<QVariant> data;
		data.push_back(QVariant(grab));
		data.push_back(QVariant(zoom));
		request.setAttribute(QNetworkRequest::User, QVariant(data));
		m_manager.get(request);
	}
}


void MapsUploader::uploadTiles(QVector<TilePoint> & tiles_to_upload)
{
    for(int i = 0; i < tiles_to_upload.size(); i++)
    {
        TilePoint tp = tiles_to_upload.at(i);
        if(m_loaded.contains(tp))
            continue;

        QPoint p = tp.first;
        int zoom = tp.second;
        this->downloadTile(p.x(), p.y(), zoom);

        m_loaded.push_back(tp);
    }
}
