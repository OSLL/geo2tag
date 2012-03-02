#ifndef MAINWIDGET_H
#define MAINWIDGET_H

#include <QWidget>
#include <QLabel>
#include "Settings.h"

class MainWidget : public QWidget
{
    Q_OBJECT

    Settings m_settings;

    QLabel *m_label;

public:
    explicit MainWidget(QWidget *parent = 0);

signals:

public slots:
    void signIn(const QString& authToken);
    void signOut();

private:
    void initGUI();

};

#endif // MAINWIDGET_H
