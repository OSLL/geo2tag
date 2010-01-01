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

      connect(m_channelView, SIGNAL(      clicked(const QModelIndex&)), this, SLOT(switchFeed(const QModelIndex&)));


      setLayout(m_layout);
    }

    void CentralWidget::switchMap()
    {
      m_layout->setCurrentWidget(m_mapView); 
    }

    void CentralWidget::switchFeed(const QModelIndex& index)
    {
//TODO: get real channel info
// qobject_cast<model>->getChannelDescription
      QMessageBox::information(this,"Channel information",(*common::DbSession::getInstance().getChannels())[index.row()]->getDescription().c_str());
      // TODO get active channel from index, switch to m_feedView
      m_feedView->refresh((*common::DbSession::getInstance().getChannels())[index.row()]
);      
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
      if(m_layout->currentWidget()!=m_feedView)
      {
        QMessageBox::information(this,"Set channel raduis","You cannot change radius for this channel. You should select channel before.");
      }
      else
      {
        m_feedView->updateCurrentChannelRadius();  
      }
    }



} // namespace GUI

/* ===[ End of file ]=== */
