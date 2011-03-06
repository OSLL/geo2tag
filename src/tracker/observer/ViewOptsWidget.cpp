#include "ViewOptsWidget.h"
#include <QLabel>
#include <QDebug>
ViewOptsWidget::ViewOptsWidget(QWidget *parent):QScrollArea(parent)
{
    m_widg=new QWidget(this);
    QVBoxLayout *layout = new QVBoxLayout(m_widg);
    layout->addWidget(new QLabel("Number of marks, shown from each channel:"));
    layout->addWidget(m_marksCountEdit = new QSpinBox(this) );
    layout->addWidget(new QLabel("Max age of mark:"));
    layout->addWidget(m_timeFilterEdit = new QSpinBox(this) );
    m_marksCountEdit->setMinimum(1);
    m_marksCountEdit->setMaximum(10);
    m_timeFilterEdit->setMaximum(60);
    m_timeFilterEdit->setMinimum(1);
    QHBoxLayout *buttonLayout = new QHBoxLayout();
    buttonLayout->addWidget(m_doneButton = new QPushButton("Done"));
    buttonLayout->addWidget(m_cancelButton = new QPushButton("Cancel"));
    layout->addLayout(buttonLayout);
    m_widg->setLayout(layout);
    this->setWidget(m_widg);
    this->setWidgetResizable(true);
    connect(m_doneButton, SIGNAL(clicked()), this, SLOT(onDoneClicked()));
    connect(m_cancelButton, SIGNAL(clicked()), this, SLOT(onCancelClicked()));

    QSettings settings("osll","libs");
/*    if (settings.value("timeLimit").toInt()>60 || settings.value("timeLimit").toInt()<1){
	    m_timeFilterEdit->setValue(4);
    }else {*/
	    m_timeFilterEdit->setValue(settings.value("timeLimit").toInt());
	   /* }
    if (settings.value("marksCount").toInt()>10 || settings.value("marksCount").toInt()<1){
	    m_marksCountEdit->setValue(10);
    }else {*/
	    m_marksCountEdit->setValue(settings.value("marksCount").toInt());
//    }
    qDebug() << "initing timeLimit as " << m_timeFilterEdit->value();
    qDebug() << "initing marksCount as " << m_marksCountEdit->value();


}


void ViewOptsWidget::onDoneClicked(){
	QSettings settings("osll","libs");
	settings.setValue("timeLimit",m_timeFilterEdit->value());
	settings.setValue("marksCount",m_marksCountEdit->value());

    qDebug() << "setting timeLimit as " << m_timeFilterEdit->value();
    qDebug() << "setting marksCount as " << m_marksCountEdit->value();
	emit done();
}

int ViewOptsWidget::getTimeLimit() const
{
	QSettings settings("osll","libs");
	qDebug() << "current timeLimit " << settings.value("timeLimit").toInt();
	return settings.value("timeLimit").toInt();

}

void ViewOptsWidget::onCancelClicked()
{
	   emit cancel();
}

int ViewOptsWidget::getMarksCount() const
{
	QSettings settings("osll","libs");
        qDebug() << "current marksCount " << settings.value("marksCount").toInt();
	return settings.value("marksCount").toInt();
}
