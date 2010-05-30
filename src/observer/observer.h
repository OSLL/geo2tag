#ifndef OBSERVER_H
#define OBSERVER_H

#include <QDialog>
#include "ui_ObserverForm.h"
#include "defs.h"
#include "RSSFeedQuery.h"
#include "LoginQuery.h"
#include <QStatusBar>
#include <QStringList>
#include <QString>

class Observer : public QDialog
{
    Q_OBJECT;
    Ui::Form           m_ui;
    // last user login
    QString m_lastLogin;
    // dataMarks
    CHandlePtr<common::DataMarks> m_marks;
    //optimal scale
    int m_optScale;
    GUI::RSSFeedQuery  *rssFeedQuery;
    GUI::LoginQuery *loginQuery;
public:

    Observer();

signals:
    void dataMarksGotten(CHandlePtr<common::DataMarks>& marks);
public slots:
    void tokenRecieved(QString status,QString auth_token);
    void updateData(CHandlePtr<common::DataMarks>& marks);
    void showAllMarks();
    void buttonPushed();
    void doRequest();
};

#endif // OBSERVER_H
