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
#include <QMessageBox>

namespace GUI
{
    CentralWidget::CentralWidget(QWidget *parent) : QWidget(parent)
    {
      m_palette = new QPalette(Qt::black);
      setPalette(*m_palette);
      m_layout = new QStackedLayout(this);

      m_mapView  = new MapPane(this);
      m_feedView = new MarkPane(this);
      m_channelView = new ChannelPane(this);
      m_editor = new MarkEditor(this);
      
      m_layout->addWidget(m_mapView);
      m_layout->addWidget(m_feedView);
      m_layout->addWidget(m_channelView);
      m_layout->addWidget(m_editor);
      
      switchMap();
      setLayout(m_layout);

      
    }

    void CentralWidget::switchMap()
    {
      m_layout->setCurrentWidget(m_mapView); 
    }

    void CentralWidget::switchFeed()
    {
      m_feedView->refresh();      
      m_layout->setCurrentWidget(m_feedView); 
    }

    void CentralWidget::switchChannel()
    {
      m_layout->setCurrentWidget(m_channelView); 
    }

    void CentralWidget::switchEditor()
    {
      m_layout->setCurrentWidget(m_editor); 
    }
    
    void CentralWidget::setRadius()
    {
//TODO: ask real raduis
      QMessageBox::information(this,"Set channel raduis","You cannot change radius for this channel");
    }

    void CentralWidget::showChannelInfo()
    {
//TODO: get real channel info
      QMessageBox::information(this,"Channel information","This is a public channel with common city information about roads, events, building reconstructions. You cannot add new information to this channel");
    }
    
} // namespace GUI

/* ===[ End of file ]=== */
