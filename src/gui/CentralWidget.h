/*!
 * \file CentralWidget.h
 * \brief Header of CentralWidget
 * \todo add comment here
 *
 * File description
 *
 * PROJ: OSLL/geo2tag
 * ---------------------------------------------------------------- */


#ifndef _CentralWidget_H_044270F5_8D38_4F61_9F05_96359B3BD122_INCLUDED_
#define _CentralWidget_H_044270F5_8D38_4F61_9F05_96359B3BD122_INCLUDED_

#include <QtGui/QWidget>
#include <QtGui/QStackedLayout>
#include <QDebug>
#include "MarkPane.h"
#include "MapPane.h"
#include "ChannelPane.h"
#include "MarkEditor.h"

namespace GUI
{
 /*!
   * Class description. May use HTML formatting
   *
   */
  class CentralWidget : public QWidget
  {
    Q_OBJECT;

    QStackedLayout *m_layout;
    MapPane        *m_mapView;
    MarkPane       *m_feedView;
    ChannelPane    *m_channelView;
    MarkEditor     *m_editor;
    QPalette       *m_palette;

  public:
    CentralWidget(QWidget *parent);
    
    virtual ~CentralWidget()
    {
    }

  public slots:
    void switchMap();
    void switchFeed(const QModelIndex& index);
    void switchChannel();
    void switchEditor();
    void setRadius();
    
  private:    
    CentralWidget(const CentralWidget& obj);
    CentralWidget& operator=(const CentralWidget& obj);

  }; // class CentralWidget
  
} // namespace GUI

#endif //_CentralWidget_H_044270F5_8D38_4F61_9F05_96359B3BD122_INCLUDED_

/* ===[ End of file ]=== */
