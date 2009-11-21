#ifndef __Position__h__
#define __Position__h__


class Position
{
  double m_latitude;
  double m_longitude;

public:
  Position();
  Position(double latitude, double longitude);
  const double getLatitude() const;
  const double getLongitude() const;
  const double distance(const Position& target) const;

};


#endif

