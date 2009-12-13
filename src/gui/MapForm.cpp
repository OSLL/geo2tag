/*! ---------------------------------------------------------------
 *  
 *
 * \file MapForm.cpp
 * \brief MapForm implementation
 *
 * File description
 *
 *  PROJ: OSLL/geo2tag 
 * ---------------------------------------------------------------- */

#include "MapForm.h"
#include <QVBoxLayout>
#include <QPushButton>
#include <QSlider>


#include "MapWidget.h"
#include "TabWidget.h"

namespace GUI
{

    MapForm::MapForm(QWidget * parent) : QDialog(parent)
    {
			setLayout(new QVBoxLayout(this));
			
			m_b = new QLineEdit(QString::number(DEFAULT_POSITION_B),this);
			m_l = new QLineEdit(QString::number(DEFAULT_POSITION_L),this);
			m_updateBtn = new QPushButton("show",this);

			layout()->addWidget(m_b);
			layout()->addWidget(m_l);
			layout()->addWidget(m_updateBtn);



      m_tabs = new TabWidget(this);

//      m_tabs->addTab(m_picture,"map");

      layout()->addWidget(m_tabs);

			connect(m_updateBtn,SIGNAL(pressed()), this, SLOT(updateMap()));

    }

		void MapForm::updateMap()
		{
      QPointF coord(m_b->text().toDouble(), m_l->text().toDouble());

      m_tabs->setPosition(coord);
		}

} // namespace GUI

/* ===[ End of file  ]=== */
