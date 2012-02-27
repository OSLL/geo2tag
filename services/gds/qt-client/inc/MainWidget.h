#ifndef MAINWIDGET_H
#define MAINWIDGET_H

#include <QWidget>
#include <QLabel>

class MainWidget : public QWidget
{
    Q_OBJECT

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
