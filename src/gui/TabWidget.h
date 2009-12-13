/*  */
/*!
 * \file TabWidget.h
 * \brief Header of TabWidget
 * \todo add comment here
 *
 * File description
 *
 *  PROJ: OSLL/geo2tag 
 * ---------------------------------------------------------------- */


#ifndef _TabWidget_H_E7458A72_E034_4BA3_9B2F_0E40DB02B1E7_INCLUDED_
#define _TabWidget_H_E7458A72_E034_4BA3_9B2F_0E40DB02B1E7_INCLUDED_

#include <QtGui/QTabWidget>
#include <QWidget>
#include <QVBoxLayout>
#include "MapWidget.h"
#include <QSlider>
#include <QListView>

namespace GUI
{


  class MapPane : public QWidget
  {
    Q_OBJECT;

    MapWidget *m_picture;
    QSlider   *m_slider;

  public:
    MapPane(QWidget * parent) : QWidget(parent)
    {
  	  QHBoxLayout *vbl = new QHBoxLayout(this);
      m_picture = new MapWidget(this);  
  		vbl->addWidget(m_picture);
			
  		m_slider = new QSlider(Qt::Vertical,this);
  		m_slider->setMinimum(SCALE_MIN*SCALE_RATIO);
  		m_slider->setMaximum(SCALE_MAX*SCALE_RATIO);
  		m_slider->setValue(SCALE_DEFAULT*SCALE_RATIO);

  		vbl->addWidget(m_slider);
  
      setLayout(vbl);
  		connect(m_slider, SIGNAL(valueChanged(int)), this, SLOT(updateMap())); 
    }

    virtual ~MapPane()
    {
    }

    void setPosition(const QPointF& pos)
    {
      m_picture->setB(pos.x());
      m_picture->setL(pos.y());
      m_picture->updateMap();
    }

    public slots:
    
    void updateMap()
    {
	    m_picture->setScale(m_slider->value()/SCALE_RATIO);
      m_picture->updateMap();
    }

  };

 /*!
   * Class description. May use HTML formatting
   *
     */
  class TabWidget : public QTabWidget
  {

    Q_OBJECT;

    MapPane *m_map;


  public:
    TabWidget(QWidget *parent = NULL);

    virtual ~TabWidget()
    {
    }

  void setPosition(const QPointF& pos)
  {
    m_map->setPosition(pos);
  }
    
  private:    
    TabWidget(const TabWidget& obj);
    TabWidget& operator=(const TabWidget& obj);

  }; // class TabWidget
  
} // namespace GUI

#endif //_TabWidget_H_E7458A72_E034_4BA3_9B2F_0E40DB02B1E7_INCLUDED_

/* ===[ End of file  ]=== */
