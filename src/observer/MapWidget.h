#ifndef _MapWidget_H_0i065385CA_5A72_4063_8F11_A6EB4798CB6D_INCLUDED_
#define _MapWidget_H_0i065385CA_5A72_4063_8F11_A6EB4798CB6D_INCLUDED_

#include <QtGui/QWidget>
#include <QtGui/QPainter>
#include <QtGui/QPaintEvent>
#include <QDebug>
#include <QPainter>
#include "Picture.h"
#include "GoogleMapLoader.h"
#include "MapLoaderFactory.h"
#include "MapLoader.h" 
#include "DataMarks.h"

class MapWidget : public QWidget 
{

  Q_OBJECT
  double m_scale;
  //
  bool m_moving;
  double m_B;
  double m_L;
  QPointF m_mousePosition;
  ///
  CHandlePtr<common::DataMarks> m_marks;

public:

	MapWidget(QWidget *parent);
  virtual ~MapWidget(){}
  void setB(double b);
  void setL(double l);

protected:

  void paintEvent(QPaintEvent *pe);
//  void mouseMoveEvent ( QMouseEvent * event );
  void mousePressEvent( QMouseEvent * event );
  void mouseReleaseEvent(QMouseEvent * event );

public slots:

	void scaleChanged(int newScale);
	void updated(CHandlePtr<common::DataMarks>& marks);
	
};
#endif
