/*! ---------------------------------------------------------------
 *  
 *
 * \file MarkPane.cpp
 * \brief MarkPane implementation
 *
 * File description
 *
 * PROJ: OSLL/geoblog
 * ---------------------------------------------------------------- */

#include "MarkPane.h"

namespace GUI
{
    MarkPane::MarkPane(QWidget * parent) : QWidget(parent)
    {
  	  QHBoxLayout *vbl = new QHBoxLayout(this);

      m_list = new ListView(this);
  		vbl->addWidget(m_list);
      setLayout(vbl);

      connect(m_list,    SIGNAL(      clicked(const QModelIndex&)), this, SLOT(showUrl(const QModelIndex&)));
    }


} // namespace GUI

/* ===[ End of file ]=== */
