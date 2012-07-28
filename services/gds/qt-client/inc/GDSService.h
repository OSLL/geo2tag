#ifndef GDSSERVICE_H
#define GDSSERVICE_H

#include <QObject>

class GDSService : public QObject
{
  Q_OBJECT

    private:

  public:
    explicit GDSService(QObject *parent = 0);

    void startTracking();
    void stopTracking();
    bool isTracking();

  public slots:
    void settingsUpdated();

};
#endif                                  // GDSSERVICE_H
