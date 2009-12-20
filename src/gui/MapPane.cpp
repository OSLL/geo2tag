/*! ---------------------------------------------------------------
 *  
 *
 * \file MapPane.cpp
 * \brief MapPane implementation
 *
 * File description
 *
 * PROJ: OSLL/geoblog
 * ---------------------------------------------------------------- */

#include "MapPane.h"
#include <QtGui/QHBoxLayout>

namespace GUI
{
    MapPane::MapPane(QWidget * parent) : QWidget(parent)
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

    void MapPane::setPosition(const QPointF& pos)
    {
      m_picture->setB(pos.x());
      m_picture->setL(pos.y());
      m_picture->updateMap();
    }

    void MapPane::updateMap()
    {
	    m_picture->setScale(m_slider->value()/SCALE_RATIO);
      m_picture->updateMap();
    }

} // namespace GUI

/* ===[ End of file ]=== */
