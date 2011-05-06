#include "testwidget.h"
#include <QPaintEvent>
#include <QPainter>
#include <QBrush>
#include <cstdlib>

const int PIXEL_SIZE=3;

namespace
{

  QColor getColor()
  {
    int r =(rand()%255 | 0x7);
    int g =(rand()%255 | 0x7);
    int b =(rand()%255 | 0x7);

    return QColor(r,g,b);
  }
}


TestWidget::TestWidget(QWidget *parent) :
QWidget(parent)
{

}


void TestWidget::paintEvent(QPaintEvent *pe)
{
  QRect r = pe->rect();
  QPainter p(this);

  for(int i = 0; i<r.width()/PIXEL_SIZE; ++i)
  {
    for( int j = 0; j<r.height()/PIXEL_SIZE; j++)
    {
      QBrush br(getColor());
      p.setBrush(br);
      p.drawRect(i*PIXEL_SIZE,j*PIXEL_SIZE,i*(PIXEL_SIZE+1),j*(PIXEL_SIZE+1));

    }
  }
}
