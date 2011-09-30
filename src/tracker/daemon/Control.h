#ifndef CONTROL_H
#define CONTROL_H

#include <QStringList>

class Control: public QObject
{
  public:
    virtual void startTracking() = 0;
    virtual void stopTracking() = 0;
    virtual QStringList getLog() const = 0;
    virtual bool isTracking() const = 0;
};
// CONTROL_H
#endif
