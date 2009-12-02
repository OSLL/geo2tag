#ifndef __Position__h__
#define __Position__h__
#include <string>

class Position
{
  double m_latitude;
  double m_longitude;

public:
  Position();
  Position(const char* pos);
  Position(std::string pos);
  Position(double latitude, double longitude);
  const std::string& toStr() const;
  const double getLatitude() const;
  const double getLongitude() const;
  const double distance(const Position& target) const;

};


#endif

