#ifndef _MapWidget_H_0i065385CA_5A72_4063_8F11_A6EB4798CB6D_INCLUDED_
#define _MapWidget_H_0i065385CA_5A72_4063_8F11_A6EB4798CB6D_INCLUDED_

#include <QtGui/QWidget>

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
  QSharedPointer<DataMarks> m_marks;

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
	void updated(QSharedPointer<DataMarks>& marks);
	
};
#endif
