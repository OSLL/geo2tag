/*  */
/*!
 * \file MapForm.h
 * \brief Header of MapForm
 * \todo add comment here
 *
 * MapForm  
 *
 *  PROJ: OSLL/geo2tag 
 * ---------------------------------------------------------------- */


#ifndef _MapForm_H_A0FE75DA_351E_46D6_ACCD_B7D69937A703_INCLUDED_
#define _MapForm_H_A0FE75DA_351E_46D6_ACCD_B7D69937A703_INCLUDED_

#include <QDialog>
#include <QLineEdit>
#include <QSlider>
#include "MapWidget.h"
#include "TabWidget.h"

namespace GUI
{

 /*!
   * Class description. May use HTML formatting
   
   */
  class MapForm : public QDialog
  {
		Q_OBJECT;

  public:
    MapForm(QWidget * parent = NULL);

    virtual ~MapForm()
    {
    }

	public slots:
		void updateMap();
    
  private:    
    MapForm(const MapForm& obj);
    MapForm& operator=(const MapForm& obj);


		QLineEdit     *m_b; 
		QLineEdit     *m_l;
		QPushButton   *m_updateBtn;
    TabWidget     *m_tabs;


  }; // class MapForm
  
} // namespace GUI

#endif //_MapForm_H_A0FE75DA_351E_46D6_ACCD_B7D69937A703_INCLUDED_

/* ===[ End of file  ]=== */
