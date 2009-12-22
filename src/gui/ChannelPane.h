/*!
 * \file ChannelPane.h
 * \brief Header of ChannelPane
 * \todo add comment here
 *
 * File description
 *
 * PROJ: OSLL/geo2tag
 * ---------------------------------------------------------------- */


#ifndef _ChannelPane_H_A4534852_66A8_4982_975D_0D67B6F33B44_INCLUDED_
#define _ChannelPane_H_A4534852_66A8_4982_975D_0D67B6F33B44_INCLUDED_

#include <QtGui/QListView>
#include <QAbstractItemModel>

namespace GUI
{
 /*!
   * Class description. May use HTML formatting
   *
   */
  class ChannelPane : public QListView
  {  
    Q_OBJECT;

    QAbstractItemModel *m_model;

  public:
    ChannelPane(QWidget *parent);

    virtual ~ChannelPane()
    {
    }
    
  private:    
    ChannelPane(const ChannelPane& obj);
    ChannelPane& operator=(const ChannelPane& obj);

  }; // class ChannelPane
  
} // namespace GUI

#endif //_ChannelPane_H_A4534852_66A8_4982_975D_0D67B6F33B44_INCLUDED_

/* ===[ End of file ]=== */
