#ifndef VIEWOPTSWIDGET_H
#define VIEWOPTSWIDGET_H

#include <QWidget>
#include <QLineEdit>
#include <QCheckBox>
#include <QPushButton>
#include <QHBoxLayout>
#include <QScrollArea>
#include <QSpinBox>
#include <QComboBox>
#include <QSettings>
#include "defines.h"
class ViewOptsWidget : public QScrollArea
{
    Q_OBJECT
    QSpinBox * m_marksCountEdit;
    QSpinBox * m_timeFilterEdit;
    QWidget * m_widg;
    QPushButton * m_doneButton;
    QPushButton * m_cancelButton ;
    //void initSettings();


public:
	ViewOptsWidget(QWidget *parent = 0);
	int getTimeLimit() const; 
	int getMarksCount() const;
public slots:
	void onDoneClicked();
	void onCancelClicked();
signals:
	void cancel();
	void done();

};
#endif
