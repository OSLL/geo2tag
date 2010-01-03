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

#include <iostream>
#include <QWebView>
#include <QUrl>
#include <QVBoxLayout>
#include <QHBoxLayout>
#include <QLabel>
#include <QDialog>
#include <QSpinBox>
#include <QDialogButtonBox>
#include "MarkPane.h"
#include "MarkTableDelegat.h"

namespace GUI
{
    MarkDetailsDialog::MarkDetailsDialog(QWidget *parent, CHandlePtr<common::DataMark> mark): QDialog(parent), m_mark(mark)
    {
      setWindowTitle(mark->getLabel().c_str());
      QHBoxLayout *hbox = new QHBoxLayout();
      if(m_mark->getUrl() != "")
      {
        QWebView *picture = new QWebView();
        picture->load(QUrl(m_mark->getUrl().c_str()));
        picture->setMaximumSize(200,200);
        hbox->addWidget(picture);
      }
      QLabel *description = new QLabel(m_mark->getDescription().c_str());
      hbox->addWidget(description);

      setLayout(hbox);
    }

    MarkPane::MarkPane(QWidget * parent) : QWidget(parent)
    {
  	  QHBoxLayout *vbl = new QHBoxLayout(this);

      m_list = new ListView(this);
  		vbl->addWidget(m_list);
      setLayout(vbl);

      connect(m_list,    SIGNAL(      clicked(const QModelIndex&)), this, SLOT(showUrl(const QModelIndex&)));
    }
    
    void MarkPane::showUrl(const QModelIndex &index)
    {
      //TODO:: here's image dialog, please 
      // QMessageBox::information(this,"MarkImage","IIMAGE DBG IMAGE DBG IMAGE DBG IMAGE DBG MAGE DBG ");
      // We are looking for real index of selected mark ( we store all marks and show only for current channel)
      CHandlePtr<common::DataMarks> marks = common::DbSession::getInstance().getMarks();
      size_t c=0,i=0;
      for(; i<marks->size(); i++)
      {
        if((*marks)[i]->getChannel() == dynamic_cast<GUI::ListModel*>(m_list->model())->getCurrentChannel())
          c++;
        if((c-1)==index.row())
          break;
      }
      // now "i" is a real mark index
      // We should draw dialog with our mark.
      MarkDetailsDialog dialog(this, (*marks)[i]);
      dialog.exec();
    }


    class RadiusEditor: public QDialog
    {
      CHandlePtr<common::Channel> m_channel;
      QSpinBox *m_value;
    public:
      RadiusEditor(QWidget *parent, CHandlePtr<common::Channel> channel):QDialog(parent), m_channel(channel)
      {
        setWindowTitle(QString("Editing radius for channel: ") + QString(channel->getName().c_str())); 
        QVBoxLayout *vbox = new QVBoxLayout();
        m_value = new QSpinBox();
        m_value->setMinimum(200);
        m_value->setMaximum(15000);
        m_value->setValue(m_channel->getRadius()*1000);
        m_value->setSingleStep(200);
        QHBoxLayout *hbox = new QHBoxLayout();
        QLabel *label = new QLabel("Radius: ");
        QLabel *met = new QLabel(" metres");
        hbox->addWidget(label);
        hbox->addWidget(m_value);
        hbox->addWidget(met);
        hbox->addStretch();
        QDialogButtonBox *bbox = new QDialogButtonBox(QDialogButtonBox::Ok | QDialogButtonBox::Cancel);
        vbox->addLayout(hbox);
        vbox->addWidget(bbox);
        setLayout(vbox);
        connect(bbox, SIGNAL(accepted()), this, SLOT(accept()));
        connect(bbox, SIGNAL(rejected()), this, SLOT(reject()));
      }
    public slots:
      virtual void accept()
      {
        m_channel->setRadius(m_value->value()/1000.);
        QDialog::accept();
      }
    };
    
    void MarkPane::updateCurrentChannelRadius()
    {
        RadiusEditor dialog(this, dynamic_cast<GUI::ListModel*>(m_list->model())->getCurrentChannel());
        dialog.exec();   
        refresh(dynamic_cast<GUI::ListModel*>(m_list->model())->getCurrentChannel());
    }

} // namespace GUI

/* ===[ End of file ]=== */
