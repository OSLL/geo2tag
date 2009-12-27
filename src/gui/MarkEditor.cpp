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
#include <sstream>
#include "MarkEditor.h"
#include <QDebug>
#include <QMessageBox>
#include <QtGui/QVBoxLayout>
#include "DataMarks.h"
#include "Channel.h"
#include "DbSession.h"
#include "Handle.h"
#include "GpsInfo.h"

namespace GUI
{
  MarkEditor::MarkEditor(QWidget *parent) : QWidget(parent)
  {
    m_ok = new QPushButton("Add mark", this);
    m_combo = new QComboBox(this);

    CHandlePtr<common::Channels> channels = common::DbSession::getInstance().getChannels();
    int i=0;
    for(common::Channels::iterator it = channels->begin(); it != channels->end(); it++)
    {
      m_combo->insertItem(i++,QObject::tr((*it)->getDescription().c_str()));
    }
    m_text = new QTextEdit("Enter new mark",this);

    m_text->selectAll();


    QVBoxLayout *layout = new QVBoxLayout(this);

    layout->addWidget(m_combo);
    layout->addWidget(m_text);
    layout->addWidget(m_ok);

    setLayout(layout);

    connect(m_ok, SIGNAL(pressed()), this, SLOT(applyMark()));
  }

  void MarkEditor::applyMark()
  {
    QString text = m_text->toPlainText(); 
    qDebug() << "sending new mark " << text;
    
    CHandlePtr<common::DataMark> mark = common::DataMark::createMark(common::GpsInfo::getInstance().getLatitude(),
                                                     common::GpsInfo::getInstance().getLongitude(), 
                                                     "P", 
                                                     text.toStdString(),
						     "" /* unknown/undefined url*/);
    try
    {
      (*common::DbSession::getInstance().getChannels())[m_combo->currentIndex()]->addData(mark);
    }
    catch(ODBC::CException &x)
    {
      std::ostringstream s;
      s << x;
      qDebug() << s.str().c_str();
      QMessageBox::critical(this, QObject::tr("Error"), QObject::tr("Error during save your message."));
    }

  //  common::DbSession::getInstance().saveData();
  //  common::DbSession::getInstance().loadData();
  }

} // namespace GUI

/* ===[ End of file ]=== */
