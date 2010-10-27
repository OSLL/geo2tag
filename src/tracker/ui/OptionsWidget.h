#ifndef OPTIONSWIDGET_H
#define OPTIONSWIDGET_H

#include <QWidget>
#include <QLineEdit>
#include <QPushButton>
#include <QtXml/QDomElement>

class OptionsWidget : public QWidget
{
    Q_OBJECT

    QLineEdit *m_name;
    QLineEdit *m_password;
    QLineEdit *m_channel;
    QPushButton *m_doneButton;

public:
    OptionsWidget(QWidget *parent = 0);

    QString name();
    QString password();
    QString channel();

public slots:
    void onDoneClicked();

signals:
    /* is emitted when "ok" button is pressed */
    void done();

};

#endif // OPTIONSWIDGET_H
