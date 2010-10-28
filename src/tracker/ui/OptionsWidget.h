#ifndef OPTIONSWIDGET_H
#define OPTIONSWIDGET_H

#include <QWidget>
#include <QLineEdit>
#include <QPushButton>
#include <QtXml/QDomElement>
#include "tracker.h"

class OptionsWidget : public QWidget
{
    Q_OBJECT

    Settings m_settings;
    Settings m_backupSettings;

    QLineEdit *m_nameEdit;
    QLineEdit *m_passwordEdit;
    QLineEdit *m_channelEdit;

    QString m_name;
    QString m_password;
    QString m_channel;

    QPushButton *m_doneButton;

public:
    OptionsWidget(QWidget *parent = 0);

    QString name();
    QString password();
    QString channel();

    void initSettings();
    void readSettings();
    void createSettings();
    void updateWidgets();

public slots:
    void onDoneClicked();

signals:
    /* is emitted when "ok" button is pressed */
    void done();

};

#endif // OPTIONSWIDGET_H
