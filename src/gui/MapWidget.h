/*  */
/*!
 * \file MapWidget.h
 * \brief Header of MapWidget
 * \todo add comment here
 *
 * File description
 *
 *  PROJ: OSLL/geo2tag 
 * ---------------------------------------------------------------- */


#ifndef _MapWidget_H_065385CA_5A72_4063_8F11_A6EB4798CB6D_INCLUDED_
#define _MapWidget_H_065385CA_5A72_4063_8F11_A6EB4798CB6D_INCLUDED_

#include <QtGui/QWidget>
#include <QtGui/QPainter>
#include <QtGui/QPaintEvent>
#include <QDebug>
#include <QtGui/QImage>
#include <QtGui/QSlider>
#include <QtCore/QTimer>

namespace GUI
{
	const double DEFAULT_POSITION_B = 59.939145;
	const double DEFAULT_POSITION_L = 30.311642;
	const int    SCALE_DEFAULT      = 10;
	const int    SCALE_MIN          = 2;
	const int    SCALE_MAX          = 20;
  const int    SCALE_RATIO        = 10;

 /*!
   * Class description. May use HTML formatting
   *
   */
  class MapWidget : public QWidget 
  {
		double m_B;
		double m_L;
		double m_scale;
    QPointF m_mousePosition;
    bool m_moving;
    QTimer m_updateGpsDataTimer;
    
    Q_OBJECT

	public:

		MapWidget(QWidget *parent, double b = DEFAULT_POSITION_B , double l = DEFAULT_POSITION_L, double scale = SCALE_DEFAULT);

		virtual ~MapWidget()
		{
		}

		void setB(double b);
		void setL(double l);
		void setScale(int scale);

  protected:
		void paintEvent(QPaintEvent *pe);
    void mouseMoveEvent ( QMouseEvent * event );
    void mousePressEvent( QMouseEvent * event );
    void mouseReleaseEvent(QMouseEvent * event );

	public slots:
		void updateMap();
    void updateGpsData();

  private:    
    MapWidget(const MapWidget& obj);
    MapWidget& operator=(const MapWidget& obj);
  }; // class MapWidget
  
} // namespace GUI

#endif //_MapWidget_H_065385CA_5A72_4063_8F11_A6EB4798CB6D_INCLUDED_

/* ===[ End of file  ]=== */
