/*! ---------------------------------------------------------------
 *  
 *
 * \file MarkEditor.cpp
 * \brief MarkEditor implementation
 *
 * File description
 *
 * PROJ: OSLL/geoblog
 * ---------------------------------------------------------------- */

#include "MarkEditor.h"
#include <defs.h>
#include <QtGui/QVBoxLayout>

namespace GUI
{
  MarkEditor::MarkEditor(QWidget *parent) : QWidget(parent)
  {
    m_ok = new QPushButton("Add mark", this);
    m_combo = new QComboBox(this);
    m_text = new QTextEdit("Enter new mark",this);


    QVBoxLayout *layout = new QVBoxLayout(this);

    layout->addWidget(m_combo);
    layout->addWidget(m_text);
    layout->addWidget(m_ok);

    setLayout(layout);

    connect(m_ok, SIGNAL(triggered()), this, SLOT(applyMark()));
  }

  void MarkEditor::applyMark()
  {
    qDebug() << "sending new mark";
  }

} // namespace GUI

/* ===[ End of file ]=== */
