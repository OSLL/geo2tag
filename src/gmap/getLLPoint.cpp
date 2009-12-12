#include <math.h>
#include "Position.h"

const double radius=6371;
const double metres4pixel[]={83333.33, 41666.67, 21052.63, 10526.32, 5263.16, 2500.00, 1250.00, 625.00, 307.69, 160.00, 80.00, 40.00, 20.00, 10.00, 5.00, 2.50, 1.25, 0.63, 0.31, 0.16};


Position& getLLPoint(const Postion& currentPos,double xShift,double yShift,int scale){
  double r_latitude;
  double r_longitude;
  double tmp;
  double toRad=3.14159265/180.0;
  r_latitude=abs(meters4pixel[scale]*yShift/(radius*toRad)-currentPos.getLatitude());
  tmp=meters4pixel[scale]*xShift/(toRad*radius;
  r_longitude=abs(2.0*tmp/(cos(r_latitude*toRad)+cos(currentPos.getLatitude()*toRad)-currentPos.getLongitude());
  return Positrion(r_latitude,r_longitude);
}
