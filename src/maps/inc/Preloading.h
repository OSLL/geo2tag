#ifndef PRELOADING_H
#define PRELOADING_H

#include <QObject>
#include "OSMCoordinatesConverter.h"
#include "MapsUploadThread.h"

class Preloading : public QObject
{
  Q_OBJECT

    QVector<QThread *> m_threads;
  QPoint m_top_left;
  QPoint m_bottom_right;
  int m_x, m_y, m_zoom, m_max_threads, m_max_tiles_for_thread;

  public:
    explicit Preloading(int max_threads = 3, QObject *parent = 0);
    ~Preloading();

  private:
    void loadingCircle();

  public:
    int maxThreads() const;
    void setMaxThreads(const int & max_threads);
    void load(const QPoint &  top_left, const QPoint & bottom_right, const int & zoom);

    signals:

  public slots:
    void threadFinished();
    };
#endif                                                      // PRELOADING_H
