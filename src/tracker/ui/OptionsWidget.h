#ifndef OPTIONSWIDGET_H
#define OPTIONSWIDGET_H

#include <QWidget>
#include <QLineEdit>
#include <QPushButton>
#include <QSettings>

#include <QHBoxLayout>
#include <QSpinBox>
#include <QComboBox>

class OptionsWidget : public QWidget
{
    Q_OBJECT

    QHBoxLayout *m_layout_proxy_host;

    QLineEdit *m_nameEdit;
    QLineEdit *m_passwordEdit;
    QLineEdit *m_channelEdit;
    QLineEdit *m_proxyHostEdit;

    QSpinBox *m_proxyPortEdit;
    QComboBox *m_proxyType;

    QString m_name;
    QString m_password;
    QString m_channel;
    QString m_productName;

    QPushButton *m_doneButton;
    QPushButton *m_cancelButton;

    QSettings m_settings;

public:
    OptionsWidget(QString productName,QWidget *parent = 0);

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
    void onProxyTypeChanged(int index);

signals:
    /* is emitted when "ok" button is pressed */
    void done();
    void cancel();

};

#endif // OPTIONSWIDGET_H
