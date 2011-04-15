#include "MapScene.h"
#include <QPixmap>
#include <QPaintEngine>
#include <QGraphicsSceneWheelEvent>
#include <QGraphicsSceneMouseEvent>
#include <QDebug>
#include <math.h>
#include <QGraphicsView>
#include <QScrollBar>
#include <QPolygon>
#include <QSettings>

#include <QGraphicsProxyWidget>
#include <QPushButton>

#include <QList>
#include <QSharedPointer>

#include "MapsUploadThread.h"

//Move distance for one arrow key press
#define KEY_MOVE_DIST 10


/*
 * For use Qpoint in QHash
 */
uint qHash(const QPoint& p)
{
    return p.x() * 17 ^ p.y();
}


MapScene::MapScene(QObject *parent) :
    QGraphicsScene(parent),
    m_zoom(15),
    m_latitude(59.910000),
    m_longitude(10.760000)
{
    m_maps = QHash<TilePoint, QGraphicsPixmapItem * >();

    m_uploader = new MapsUploader(this);
    connect(this, SIGNAL(uploadTiles(QVector<TilePoint>)), m_uploader, SLOT(uploadTiles(QVector<TilePoint>)));
    connect(m_uploader, SIGNAL(tileUploaded(QPixmap,QPoint,int)), this, SLOT(tileUploaded(QPixmap,QPoint,int)));
}


void MapScene::tileUploaded(const QPixmap pixmap, const QPoint point, const int zoom)
{
    TilePoint tp = qMakePair(point, zoom);

    if(m_maps.contains(tp))
        return;

    QGraphicsPixmapItem * pm = addPixmap( pixmap );
    pm->setData(0, zoom);
    m_maps.insert(tp, pm);
    m_maps.value(tp)->setPos(point.x()*256.0, point.y()*256.0);
    m_maps.value(tp)->setVisible(zoom == m_zoom);
}

void MapScene::preFetch()
{
    if(this->views().isEmpty())
        return;

    QPoint point_top_left = (this->views()[0]->mapToScene(this->views()[0]->frameRect().topLeft())).toPoint();
    QPoint point_bottom_right = (this->views()[0]->mapToScene(this->views()[0]->frameRect().bottomRight())).toPoint();

    point_top_left.setX(point_top_left.x()-256);
    point_top_left.setY(point_top_left.y()-256);

    if(point_top_left.x() < 0) point_top_left.setX(0);
    if(point_top_left.y() < 0) point_top_left.setY(0);

    point_bottom_right.setX(point_bottom_right.x()+256);
    point_bottom_right.setY(point_bottom_right.y()+256);

    if(point_bottom_right.x() > this->sceneRect().width()) point_bottom_right.setX(this->sceneRect().width());
    if(point_bottom_right.y() > this->sceneRect().width()) point_bottom_right.setY(this->sceneRect().width());

    point_top_left.setX(point_top_left.x()/256);
    point_top_left.setY(point_top_left.y()/256);

    point_bottom_right.setX(point_bottom_right.x()/256);
    point_bottom_right.setY(point_bottom_right.y()/256);

	int zoom = m_zoom;

	QVector<TilePoint> tiles_for_upload;

	while(zoom <= 18)
	{
		qDebug() << "Upload tile " << point_top_left << "\t" << point_bottom_right << "\n";

		for(int x = point_top_left.x(); x <= point_bottom_right.x(); x++)
		{
			for(int y = point_top_left.y(); y <= point_bottom_right.y(); y++)
			{
	            TilePoint tp = qMakePair(QPoint(y,x), zoom);
            	if(!m_maps.contains(tp))
            	    tiles_for_upload.push_back(tp);
        	}
    	}

		zoom++;
		point_top_left.setX(point_top_left.x()*2);
		point_top_left.setY(point_top_left.y()*2);

		point_bottom_right.setX(point_bottom_right.x()*2+1);
		point_bottom_right.setY(point_bottom_right.y()*2+1);	
		
		if(tiles_for_upload.isEmpty())
			continue;

		m_map_uploader = new MapsUploadThread(tiles_for_upload, this);
		m_map_uploader->start();
		
		tiles_for_upload.clear();
	}

/*
	if(tiles_for_upload.isEmpty())
		return;

	m_map_uploader = new MapsUploadThread(tiles_for_upload, this);
	m_map_uploader->start();
*/	
}

void MapScene::addMark(qreal latitude, qreal longitude, QVariant data)
{   
    QPixmap pixmap(20,20);
    pixmap.fill(Qt::transparent);
    QPoint center(pixmap.width()/2, pixmap.height()/2);

    QPainter painter;
    painter.begin(&pixmap);
    painter.setBrush(Qt::blue);
    painter.drawEllipse(center, pixmap.width()/2, pixmap.height()/2);
    painter.setBrush(Qt::black);
    painter.drawEllipse(center, pixmap.width()/10, pixmap.height()/10);
    painter.end();

    QGraphicsItem * mark = this->addPixmap(pixmap);
    QPointF mark_point = convertCoordinates(latitude, longitude, this->m_zoom);

    mark_point.setX(mark_point.x()*256.0);
    mark_point.setY(mark_point.y()*256.0);
    mark->setX(mark_point.x());
    mark->setY(mark_point.y());
    mark->setData(0,data);

    this->views()[0]->centerOn(mark_point);
}

void MapScene::addMark(qreal latitude, qreal longitude, QVariant data, QWidget * widget)
{
    QGraphicsProxyWidget * mark = this->addWidget(widget);
    widget->show();

    QPointF mark_point = convertCoordinates(latitude, longitude, this->m_zoom);
    mark_point.setX(mark_point.x()*256.0);
    mark_point.setY(mark_point.y()*256.0);

    mark->setX(mark_point.x());
    mark->setY(mark_point.y());
    mark->setData(0,data);

    this->views()[0]->centerOn(mark_point);
}

void MapScene::removeMark(QGraphicsItem * mark)
{
    mark->update();
    /*
    qDebug() << "pan\n";
    m_slippy_map->pan(QPoint(300,0));
    m_slippy_map->invalidate();
    */

    /*
    QList<QGraphicsItem * > check_items = this->items();
    for(int i = 0; i < check_items.size(); i++)
    {
        if(check_items.at(i)->data(0) == data)
        {
            removeItem(check_items.at(i));
            break;
        }
    }
    */
}

void MapScene::setMarks(DataChannels marks)
{
	double tdim=256.;
	QPointF pos;

	//Add here time and count filter
	QSettings settings("osll","libs");
	int maxAgeOfMark = settings.value("timeLimit").toInt();
    int marksCount = settings.value("marksCount").toInt();

	//Getting list of all channels, wich marks are in request
    QList<QSharedPointer<DataMark> > marks_to_show;

	m_marks.clear();

	QList<QSharedPointer<Channel> > channels = marks.uniqueKeys();
	for (int j = 0; j < channels.size(); j++)
	{
		marks_to_show = marks.values(channels.at(j));
		qSort(marks_to_show.begin(), marks_to_show.end(), qGreater<QSharedPointer<DataMark> >());
		for (int i = 0; i < qMin( marksCount, marks_to_show.size() ); i++)
		{
			//Check, that current mark isnt older that maxAgeOfMark minutes
			qDebug() << "Mark time " << marks_to_show.at(i)->getTime().toString("dd.MM.yyyy hh:mm:ss");
			qDebug() << "CurrTime-4min  " << QDateTime::currentDateTime().addSecs(-60 * maxAgeOfMark).toString("dd.MM.yyyy hh:mm:ss");
			if(true)//marks_to_show.at(i)->getTime().toUTC()>QDateTime::currentDateTime().addSecs(-60 * maxAgeOfMark))
			{
				pos = convertCoordinates(
										marks_to_show.at(i)->getLatitude(), 
										marks_to_show.at(i)->getLongitude(),
										m_zoom);
				pos = pos * qreal(tdim);
				this->add_mark(pos, channels.at(j)->getName());
			}
		}
	}
}

void MapScene::add_mark(QPointF pos, QString channel_name)
{
	QPointF posForPicture = QPointF(pos.x()-12.0, pos.y()-12.0);
	QPointF posForText = QPointF(pos.x()-24.0, pos.y()+24.0);
	QGraphicsPixmapItem * pi = 0;

	if(channel_name == "Fuel prices")
	{
		pi = addPixmap(QPixmap(":/img/fuel.png"));
	}
	else if(channel_name == "Public announcements")
	{
		pi = addPixmap(QPixmap(":/img/public.png"));
	}
    else if(channel_name == "ObsTestChannel")
    {
		pi = addPixmap(QPixmap(":/img/test.png"));
		//painter.drawText(posForText, "Test text");
    }
    else if(channel_name.startsWith("bus_"))
    {
		pi = addPixmap(QPixmap(":/img/bus.png"));
        //painter.drawText(posForText, channel_name.split('_').at(1));
    }
    else if(channel_name.startsWith("tram_"))
    {
        pi = addPixmap(QPixmap(":/img/tram.png"));
        //painter.drawText(posForText, channel_name.split('_').at(1));
    }
    else if(channel_name.startsWith("troll_"))
    {
        pi = addPixmap(QPixmap(":/img/trolleybus.png"));
        //painter.drawText(posForText, channel_name.split('_').at(1));
    }
    else if(channel_name.startsWith("user_"))
    {
        pi = addPixmap(QPixmap(":/img/user.png"));
    }
    else
    {
		QPixmap pixmap(24, 24);
	    pixmap.fill(Qt::transparent);
    	QPoint center(pixmap.width()/2, pixmap.height()/2);

	    QPainter painter;
    	painter.begin(&pixmap);
    	painter.setBrush(Qt::blue);
    	painter.drawEllipse(center, pixmap.width()/2, pixmap.height()/2);
    	painter.setBrush(Qt::black);
    	painter.drawEllipse(center, pixmap.width()/10, pixmap.height()/10);
    	painter.end();

    	pi = this->addPixmap(pixmap);
    }

	pi->setX(posForPicture.x());
	pi->setY(posForPicture.y());
}

void MapScene::wheelEvent(QGraphicsSceneWheelEvent *event)
{
    if (event->delta()>0 && m_zoom<18)
        m_zoom++;
    else if (event->delta()<0 && m_zoom>0)
        m_zoom--;
    else
        return;

    this->set_zoom();
}

void MapScene::set_zoom()
{
    foreach(TilePoint tp, m_maps.keys())
    {
        if(tp.second != m_zoom)
            m_maps.value(tp)->setVisible(false);
        else
            m_maps.value(tp)->setVisible(true);
    }

    qreal max_point = (pow(2,m_zoom) - 1)*256 + 256;

    QRectF zoom_rect = QRectF(
            QPointF(0.0, 0.0),
            QPointF(max_point, max_point)
            );
    this->setSceneRect(zoom_rect);
    //this->addPolygon(QPolygonF(zoom_rect));
    if(!views().isEmpty())
        this->views()[0]->setSceneRect(zoom_rect);

    QPointF center_point =  convertCoordinates(m_latitude, m_longitude, m_zoom);
    center_point.setX(center_point.x()*256);
    center_point.setY(center_point.y()*256);
    if(!views().isEmpty())
        this->views()[0]->centerOn(center_point);

//    qDebug() << center_point.x() << "\t" << center_point.y() << "\n";

    this->update_state();
}


void MapScene::mousePressEvent(QGraphicsSceneMouseEvent *event)
{
    if(event->buttons() != Qt::LeftButton)
        return;

    pressed_screen_position = event->screenPos();
}

void MapScene::mouseMoveEvent(QGraphicsSceneMouseEvent *event)
{
    if (!event->buttons())
        return;

    QPoint screen_delta = event->screenPos() - pressed_screen_position;
    pressed_screen_position = event->screenPos();

    this->views()[0]->horizontalScrollBar()->setValue(
            this->views()[0]->horizontalScrollBar()->value()
            - screen_delta.x());
    this->views()[0]->verticalScrollBar()->setValue(
            this->views()[0]->verticalScrollBar()->value()
            - screen_delta.y());

    QPointF cur_pos = event->scenePos();
    cur_pos.setX(cur_pos.x()/256);
    cur_pos.setY(cur_pos.y()/256);

    m_longitude = tilex2long(cur_pos.x(), m_zoom);
    m_latitude = tiley2lat(cur_pos.y(), m_zoom);

    this->update_state();
}

void MapScene::keyPressEvent(QKeyEvent *event)
{
    QPoint screen_delta(0,0);

	if(event->key() == Qt::Key_F)
	{
		qDebug() << "F pressed\n";
		this->preFetch();
	}
    if(event->key() == Qt::Key_Right)
        screen_delta.setX(screen_delta.x() - KEY_MOVE_DIST);
    if(event->key() == Qt::Key_Left)
        screen_delta.setX(screen_delta.x() + KEY_MOVE_DIST);
    if(event->key() == Qt::Key_Down)
        screen_delta.setY(screen_delta.y() - KEY_MOVE_DIST);
    if(event->key() == Qt::Key_Up)
        screen_delta.setY(screen_delta.y() + KEY_MOVE_DIST);

    qDebug() << "Pressed key: " << event->key() << "\n";

    this->views()[0]->horizontalScrollBar()->setValue(
            this->views()[0]->horizontalScrollBar()->value()
            - screen_delta.x());
    this->views()[0]->verticalScrollBar()->setValue(
            this->views()[0]->verticalScrollBar()->value()
            - screen_delta.y());

    /*
    QPointF cur_pos = event->scenePos();
    cur_pos.setX(cur_pos.x()/256);
    cur_pos.setY(cur_pos.y()/256);

    m_longitude = tilex2long(cur_pos.x(), m_zoom);
    m_latitude = tiley2lat(cur_pos.y(), m_zoom);
    */

    this->update_state();
}

void MapScene::update_state()
{
    if(this->views().isEmpty())
        return;

/*
	QPointF center_point =  convertCoordinates(m_latitude, m_longitude, m_zoom);
	center_point.setX(center_point.x()*256);
	center_point.setY(center_point.y()*256);
	this->views()[0]->centerOn(center_point);
*/

    QPoint point_top_left = (this->views()[0]->mapToScene(this->views()[0]->frameRect().topLeft())).toPoint();
    QPoint point_bottom_right = (this->views()[0]->mapToScene(this->views()[0]->frameRect().bottomRight())).toPoint();

    point_top_left.setX(point_top_left.x()-256);
    point_top_left.setY(point_top_left.y()-256);

    if(point_top_left.x() < 0) point_top_left.setX(0);
    if(point_top_left.y() < 0) point_top_left.setY(0);

    point_bottom_right.setX(point_bottom_right.x()+256);
    point_bottom_right.setY(point_bottom_right.y()+256);

    if(point_bottom_right.x() > this->sceneRect().width()) point_bottom_right.setX(this->sceneRect().width());
    if(point_bottom_right.y() > this->sceneRect().width()) point_bottom_right.setY(this->sceneRect().width());

    point_top_left.setX(point_top_left.x()/256);
    point_top_left.setY(point_top_left.y()/256);

    point_bottom_right.setX(point_bottom_right.x()/256);
    point_bottom_right.setY(point_bottom_right.y()/256);

    QVector<TilePoint> tiles_for_upload;

    for(int x = point_top_left.x(); x <= point_bottom_right.x(); x++)
    {
        for(int y = point_top_left.y(); y <= point_bottom_right.y(); y++)
        {
            TilePoint tp = qMakePair(QPoint(y,x), m_zoom);
            if(!m_maps.contains(tp))
                tiles_for_upload.push_back(tp);
        }
    }

    if(tiles_for_upload.size() != 0)
        emit this->uploadTiles(tiles_for_upload);
}

QPointF MapScene::convertCoordinates(qreal lat, qreal lng, int zoom)
{
    qreal zn = static_cast<qreal>(1 << zoom);
    qreal tx = (lng + 180.0) / 360.0;
    qreal ty = (1.0 - log(tan(lat * M_PI / 180.0) +
                          1.0 / cos(lat * M_PI / 180.0)) / M_PI) / 2.0;
    return QPointF(tx * zn, ty * zn);
}

int MapScene::long2tilex(qreal lon, int z)
{
        return (int)(floor((lon + 180.0) / 360.0 * pow(2.0, z)));
}

int MapScene::lat2tiley(qreal lat, int z)
{
        return (int)(floor((1.0 - log( tan(lat * M_PI/180.0) + 1.0 / cos(lat * M_PI/180.0)) / M_PI) / 2.0 * pow(2.0, z)));
}

qreal MapScene::tilex2long(int x, int z)
{
        return x / pow(2.0, z) * 360.0 - 180;
}

qreal MapScene::tiley2lat(int y, int z)
{
        qreal n = M_PI - 2.0 * M_PI * y / pow(2.0, z);
        return 180.0 / M_PI * atan(0.5 * (exp(n) - exp(-n)));
}

qreal MapScene::tilex2long(qreal x, int z)
{
        return x / pow(2.0, z) * 360.0 - 180;
}

qreal MapScene::tiley2lat(qreal y, int z)
{
        qreal n = M_PI - 2.0 * M_PI * y / pow(2.0, z);
        return 180.0 / M_PI * atan(0.5 * (exp(n) - exp(-n)));
}
