#ifndef STATUS_H
#define STATUS_H

#include <QDateTime>
#include <QString>

struct Status
{
  QDateTime datetime;
  QString status;
  QString description;
  bool valid;
};
/* STATUS_H */
#endif
