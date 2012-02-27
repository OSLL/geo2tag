/*
 * Copyright 2010-2011  OSLL osll@osll.spb.ru
 *
 * Redistribution and use in source and binary forms, with or without
 * modification, are permitted provided that the following conditions
 * are met:
 *
 * 1. Redistributions of source code must retain the above copyright
 *    notice, this list of conditions and the following disclaimer.
 * 2. Redistributions in binary form must reproduce the above copyright
 *    notice, this list of conditions and the following disclaimer in the
 *    documentation and/or other materials provided with the distribution.
 *
 * THIS SOFTWARE IS PROVIDED BY THE AUTHOR ``AS IS'' AND ANY EXPRESS OR
 * IMPLIED WARRANTIES, INCLUDING, BUT NOT LIMITED TO, THE IMPLIED WARRANTIES
 * OF MERCHANTABILITY AND FITNESS FOR A PARTICULAR PURPOSE ARE DISCLAIMED.
 * IN NO EVENT SHALL THE AUTHOR BE LIABLE FOR ANY DIRECT, INDIRECT,
 * INCIDENTAL, SPECIAL, EXEMPLARY, OR CONSEQUENTIAL DAMAGES (INCLUDING, BUT
 * NOT LIMITED TO, PROCUREMENT OF SUBSTITUTE GOODS OR SERVICES; LOSS OF USE,
 * DATA, OR PROFITS; OR BUSINESS INTERRUPTION) HOWEVER CAUSED AND ON ANY
 * THEORY OF LIABILITY, WHETHER IN CONTRACT, STRICT LIABILITY, OR TORT
 * (INCLUDING NEGLIGENCE OR OTHERWISE) ARISING IN ANY WAY OUT OF THE USE OF
 * THIS SOFTWARE, EVEN IF ADVISED OF THE POSSIBILITY OF SUCH DAMAGE.
 *
 * 3. The name of the author may not be used to endorse or promote
 *    products derived from this software without specific prior written
 *    permission.
 *
 * The advertising clause requiring mention in adverts must never be included.
 */
/*----------------------------------------------------------------- !
 * PROJ: OSLL/geo2tag
 * ---------------------------------------------------------------- */

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
m_background_mode(false),
m_replies_in_work(0)
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
    if(m_replies_in_work == 0)
      emit this->uploadingFinished();
    return;
  }

  TilePoint tp =  m_tiles.back();
  m_tiles.pop_back();
  //qDebug() << m_tiles.size() << "left in thread" << this->thread();
  this->downloadTile(tp);
}


void MapsUploader::handleNetworkData(QNetworkReply *reply)
{
  m_replies_in_work--;
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
    qDebug() << reply->errorString();
    //this->downloadTile(qMakePair(tp,zoom));
    this->popNextTile();
    return;
  }
  reply->deleteLater();

  if(!img.isNull())
  {
    QString file_name = m_file_store_dir.path() + "/"
      + QString::number(zoom) + "/"
      + QString::number(tp.x()) + "_"
      + QString::number(tp.y());
    if(!QFile(file_name).exists())
    {
      QByteArray bytes;
      QBuffer buffer(&bytes);
      buffer.open(QIODevice::ReadWrite);
      img.save(&buffer, "PNG");         // writes pixmap into bytes in PNG format
      bytes = qCompress(bytes,9);
      QFile file(file_name);
      file.open(QIODevice::ReadWrite);
      file.write(bytes);
      file.flush();
      file.close();
    }

    if(!m_background_mode)
      emit this->tileUploaded(QPixmap::fromImage(img), qMakePair(tp,zoom));
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
    m_replies_in_work++;
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
