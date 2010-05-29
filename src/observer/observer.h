#ifndef OBSERVER_H
#define OBSERVER_H

#include <QDialog>
//#include <QMainWindow>
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
    QVector<MarkInfo>  m_data;
    QString m_lastLogin;
    QStringList m_list;
    CHandlePtr<common::DataMarks> m_marks;
    int m_goodScale;
    GUI::RSSFeedQuery  *rssFeedQuery;
    GUI::LoginQuery *loginQuery;
public:

    Observer();

signals:
    void dataUpdated();
    void dataMarksGotten(CHandlePtr<common::DataMarks>& marks);
public slots:
    void tokenRecieved(QString status,QString auth_token);
    void updateData(CHandlePtr<common::DataMarks>& marks);
    void restoreGoodScale();
    void buttonPushed();
    void doRequest();
};

#endif // OBSERVER_H
