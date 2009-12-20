/*!
 * \file MapPane.h
 * \brief Header of MapPane
 * \todo add comment here
 *
 * File description
 *
 * PROJ: OSLL/geo2tag
 * ---------------------------------------------------------------- */


#ifndef _MapPane_H_974943CF_0D73_468D_AD1A_AE6CDCE44E04_INCLUDED_
#define _MapPane_H_974943CF_0D73_468D_AD1A_AE6CDCE44E04_INCLUDED_

#include "MapWidget.h"

namespace GUI
{
 /*!
   * Class description. May use HTML formatting
   *
   */
  class MapPane : public QWidget
  {
    Q_OBJECT;

    MapWidget *m_picture;
    QSlider   *m_slider;

  public:
    MapPane(QWidget * parent);

    virtual ~MapPane()
    {
    }

    void setPosition(const QPointF& pos);

    public slots:
    
    void updateMap();
    
  private:    
    MapPane(const MapPane& obj);
    MapPane& operator=(const MapPane& obj);

  }; // class MapPane
  
} // namespace GUI

#endif //_MapPane_H_974943CF_0D73_468D_AD1A_AE6CDCE44E04_INCLUDED_

/* ===[ End of file ]=== */
