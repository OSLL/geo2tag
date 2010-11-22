#ifndef OPTIONSWIDGET_H
#define OPTIONSWIDGET_H

#include <QWidget>
#include <QLineEdit>
#include <QPushButton>
#include <QSettings>

class OptionsWidget : public QWidget
{
    Q_OBJECT

    QLineEdit *m_nameEdit;
    QLineEdit *m_passwordEdit;
    QLineEdit *m_channelEdit;

    QString m_name;
    QString m_password;
    QString m_channel;

    QPushButton *m_doneButton;
    QPushButton *m_cancelButton;

    QSettings m_settings;

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
    void onCancelClicked();

signals:
    /* is emitted when "ok" button is pressed */
    void done();
    void cancel();

};

#endif // OPTIONSWIDGET_H
