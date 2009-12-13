/*! ---------------------------------------------------------------
 *  
 *
 * \file TabWidget.cpp
 * \brief TabWidget implementation
 *
 * File description
 *
 *  PROJ: OSLL/geo2tag 
 * ---------------------------------------------------------------- */

#include "TabWidget.h"
#include "MarkTableDelegat.h"

namespace GUI
{

  TabWidget::TabWidget(QWidget *parent) : QTabWidget(parent)
  {
    m_map = new MapPane(this);
    addTab(m_map,"map");
    addTab(new MarkPane(this),"marks");
  }

} // namespace GUI

/* ===[ End of file  ]=== */
