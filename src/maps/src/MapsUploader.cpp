#include "MapsUploader.h"
#include <QNetworkReply>
#include <QNetworkDiskCache>
#include <QDesktopServices>
#include <QVector>
#include <QVectorIterator>
#include <QDebug>
#include <QDir>
#include <QBuffer>


MapsUploader::MapsUploader(QObject *parent) :
        QObject(parent),
        m_url("http://tile.openstreetmap.org/%1/%2/%3.png"),
        m_background_mode(false)
{
    this->checkHomePath();

    connect(&m_manager, SIGNAL(finished(QNetworkReply*)),
            this, SLOT(handleNetworkData(QNetworkReply*)));
}

void MapsUploader::checkHomePath()
{
    this->m_file_store_dir = QDir::home();

    if(!this->m_file_store_dir.exists(".geo2tag/uploaded_maps/"))
        this->m_file_store_dir.mkpath(".geo2tag/uploaded_maps/");

    this->m_file_store_dir.cd(".geo2tag/uploaded_maps/");

    for(int zoom = 0; zoom < 19; zoom++)
    {
        if( !this->m_file_store_dir.exists( QString::number(zoom) + "/" ) )
            this->m_file_store_dir.mkpath( QString::number(zoom) + "/" );
    }
}

void MapsUploader::popNextTile()
{
    if(m_tiles.isEmpty())
    {
        qDebug() << "No more tiles!\n";
        return;
    }

    TilePoint tp =  m_tiles.back();
    m_tiles.pop_back();
    this->downloadTile(tp);
}


void MapsUploader::handleNetworkData(QNetworkReply *reply)
{
    if(reply == 0)
    {
        qDebug() << "Got EMPTY reply!\n";
        return;
    }

    QImage img;
    QList<QVariant> data = reply->request().attribute(QNetworkRequest::User).toList();
    QPoint tp = data.front().toPoint();
    int zoom = data.back().toInt();
    if (!reply->error())
    {
        if (!img.load(reply, 0))
            img = QImage();
    }
    else
    {
        this->downloadTile(qMakePair(tp,zoom));
        return;
    }
    reply->deleteLater();

    if(!img.isNull())
    {
        QPixmap pixmap = QPixmap::fromImage(img);
        QString file_name = m_file_store_dir.path() + "/"
                            + QString::number(zoom) + "/"
                            + QString::number(tp.x()) + "_"
                            + QString::number(tp.y());
        if(!QFile(file_name).exists())
        {
            qDebug() << "Saving... (" << file_name << ")";
            QByteArray bytes;
            QBuffer buffer(&bytes);
            buffer.open(QIODevice::ReadWrite);
            pixmap.save(&buffer, "PNG"); // writes pixmap into bytes in PNG format
            bytes = qCompress(bytes,9);
            QFile file(file_name);
            file.open(QIODevice::ReadWrite);
            file.write(bytes);
            file.flush();
            file.close();

            qDebug() << "Saved! (" << file_name << ")";
        }

        if(!m_background_mode)
            emit this->tileUploaded(pixmap, qMakePair(tp,zoom));
        else
        {
            if(m_tiles.isEmpty())
            {
                emit this->uploadingFinished();
                return;
            }

            this->popNextTile();
        }
    }
}

void MapsUploader::replyError(QNetworkReply::NetworkError code)
{
    qDebug() << "Network error: " << code << "\n";
    if(m_background_mode)
        this->popNextTile();
}

void MapsUploader::downloadTile(const TilePoint & point)
{   
    QString file_name = m_file_store_dir.path() + "/"
                        + QString::number(point.second) + "/"
                        + QString::number(point.first.x()) + "_"
                        + QString::number(point.first.y());

    //qDebug() << file_name;
    if(QFile::exists(file_name))
    {
        if(!m_background_mode)
        {
            QFile pixmap_file(file_name, this);
            pixmap_file.open(QFile::ReadOnly);
            QPixmap pixmap;
            if( pixmap.loadFromData( qUncompress(pixmap_file.readAll()) ) )
                emit this->tileUploaded(pixmap, point);
            else
                qDebug() << "Error!\n";
            pixmap_file.close();
        }
        else
            this->popNextTile();
    }
    else
    {
        QNetworkRequest request;
        request.setUrl(
                QUrl( m_url.arg(point.second).arg(point.first.x()).arg(point.first.y()) )
                );
        request.setRawHeader("User-Agent", "OSLL Observer");
        QList<QVariant> data;
        data.push_back(QVariant(point.first));
        data.push_back(QVariant(point.second));
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

        this->downloadTile(tp);

        m_loaded.push_back(tp);
    }
}

void MapsUploader::uploadTilesBG(QVector<TilePoint> & tiles_to_upload)
{
    this->m_background_mode = true;
    this->m_tiles = tiles_to_upload;
    this->popNextTile();
}
