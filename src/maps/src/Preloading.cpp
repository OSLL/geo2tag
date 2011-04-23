#include "Preloading.h"
#include <QtAlgorithms>

Preloading::Preloading(int max_threads, QObject *parent) :
        QObject(parent), m_max_threads(max_threads), m_max_tiles_for_thread(100)
{
}

Preloading::~Preloading()
{
    for(int i = 0; i < m_threads.size(); i++)
    {
        if(m_threads[i]->isRunning())
            m_threads[i]->quit();
    }
}

int Preloading::maxThreads() const
{
    return m_max_threads;
}

void Preloading::loadingCircle()
{
    QVector<TilePoint> tiles_for_upload;

    while(m_zoom <= 18)
    {
        for(int x = m_x ; x <= m_bottom_right.x(); x++)
        {
            for(int y = m_y ; y <= m_bottom_right.y(); y++)
            {
                TilePoint tp = qMakePair(QPoint(x, y), m_zoom);
                tiles_for_upload.push_back(tp);

                if(tiles_for_upload.size() >= m_max_tiles_for_thread)
                {
                    QThread * thread = new MapsUploadThread(tiles_for_upload);
                    connect(thread, SIGNAL(finished()), this, SLOT(threadFinished()));
                    m_threads.push_back(thread);
                    thread->start();

                    tiles_for_upload.clear();

                    if(m_threads.size() >= m_max_threads)
                    {
                        m_x = x+1;
                        m_y = y+1;
                        return;
                    }
                }
            }
        }

        m_zoom++;

        m_top_left.setX(m_top_left.x()*2);
        m_top_left.setY(m_top_left.y()*2);
        m_x = m_top_left.x();
        m_y = m_top_left.y();

        m_bottom_right.setX(m_bottom_right.x()*2+1);
        m_bottom_right.setY(m_bottom_right.y()*2+1);
    }

    QThread * thread = new MapsUploadThread(tiles_for_upload);
    connect(thread, SIGNAL(finished()), this, SLOT(threadFinished()));
    m_threads.push_back(thread);
    thread->start();

    tiles_for_upload.clear();
}

void Preloading::setMaxThreads(const int & max_threads)
{
    m_max_threads = max_threads;
}

void Preloading::load(const QPoint & top_left, const QPoint & bottom_right, const int & zoom)
{
    m_top_left = top_left;
    m_bottom_right = bottom_right;
    m_zoom = zoom;

    m_x = m_top_left.x();
    m_y = m_top_left.y();

    this->loadingCircle();
}

void Preloading::threadFinished()
{
    QThread * thread = (QThread *)QObject::sender();
    QVector<QThread *>::Iterator it = qFind(m_threads.begin(), m_threads.end(), thread);
    m_threads.erase(it);

    this->loadingCircle();
}
