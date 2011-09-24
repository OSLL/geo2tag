#ifndef OPTIONSWIDGET_H
#define OPTIONSWIDGET_H

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

class OptionsWidget : public QScrollArea
{
  Q_OBJECT

    QWidget * m_widget;

  QLineEdit *m_nameEdit;
  QLineEdit *m_passwordEdit;
  QLineEdit *m_visibleNameEdit;
  QLineEdit *m_channelEdit;
  QLineEdit *m_proxyHostEdit;
  QLineEdit *m_serverUrlEdit;
  QSpinBox *m_serverPortEdit;

  QCheckBox * m_passwordCheckBox;

  QSpinBox * m_proxyPortEdit;
  QComboBox * m_proxyType;

  QComboBox * m_cacheType;
  QLineEdit * m_cachePath;
  QSpinBox * m_cacheMaxSize;

  QString m_name;
  QString m_password;
  QString m_channel;
  QString m_productName;

  QPushButton * m_cachePathButton;
  QPushButton * m_doneButton;
  QPushButton * m_cancelButton;
  QPushButton * m_defaultButton;

  QSettings m_settings;

  private:
    void applyProxySettings();

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
    void setDefaultSettings();
    void onProxyTypeChanged(int index);
    void onCacheTypeChanged(int index);
    void onShowPasswordChecked(bool checked);
    void onCachePathButtonClick();
    void onCachePathSelected(QString path);

    signals:
    /* is emitted when "ok" button is pressed */
    void done();
    void cancel();
};
// OPTIONSWIDGET_H
#endif
