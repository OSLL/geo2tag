#include "Position.h"
#include <math.h>
Position::Position(){
    m_latitude=0;
    m_longitude=0;
}

Position::Position(double latitude, double longitude){
    m_latitude=latitude;
    m_longitude=longitude;
}

const double Position::getLatitude() const{
	return m_latitude;
}

const double Position::getLongitude() const{
	return m_longitude;
}

const double Position::distance(const Position& target) const{//This function must calc distance betvine two points but it doesnt work
        double R=6.4e6;
        double z=pow(R*(cos(m_longitude)-cos(target.m_longitude)),2.0);
        double y=pow(R*(sin(m_longitude)*sin(m_latitude)-sin(target.m_longitude)*sin(target.m_latitude)),2.0);
        double x=pow(R*(sin(m_longitude)*cos(m_latitude)-sin(target.m_longitude)*cos(target.m_latitude)),2.0);
        return sqrt(x+y+z);
}
