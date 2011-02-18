#include <QApplication>
#include <QDebug>
#include <QColor>


QColor getColor()
{
  static int counter = 0; // times of getting color
  QColor color; 

   ///

  return color;
}



int main()
{

  for(int i=0;i<9999;++i)
    qDebug() << getColor().name();

  return 0;
}
