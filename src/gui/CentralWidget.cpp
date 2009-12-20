/*! ---------------------------------------------------------------
 *  
 *
 * \file CentralWidget.cpp
 * \brief CentralWidget implementation
 *
 * File description
 *
 * PROJ: OSLL/geoblog
 * ---------------------------------------------------------------- */

#include "CentralWidget.h"

namespace GUI
{
    CentralWidget::CentralWidget(QWidget *parent) : QWidget(parent)
    {
      m_layout = new QStackedLayout(this);

      m_mapView  = new MapPane(this);
      m_feedView = new MarkPane(this);
      
      m_layout->addWidget(m_mapView);
      m_layout->addWidget(m_feedView);
      
      setLayout(m_layout);

    }

    void CentralWidget::switchMap()
    {
      m_layout->setCurrentWidget(m_mapView); 
    }

    void CentralWidget::switchFeed()
    {
      m_layout->setCurrentWidget(m_feedView); 
    }

} // namespace GUI

/* ===[ End of file ]=== */
