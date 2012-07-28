#ifndef SETTINGSWIDGET_H
#define SETTINGSWIDGET_H

#include <QWidget>
#include <QLineEdit>
#include <QPushButton>
#include <QTextEdit>
#include "Settings.h"

class SettingsWidget : public QWidget
{
  Q_OBJECT

    Settings m_settings;

  // GUI
  QTextEdit *m_description;
  QLineEdit *m_serverUrlEdit;
  QLineEdit *m_radiusEdit;
  QLineEdit *m_trackingPeriodEdit;
  QPushButton *m_okButton;
  QPushButton *m_cancelButton;

  public:
    explicit SettingsWidget(QWidget *parent = 0);

    void fill();

    signals:
    void saved();
    void cancelled();

  public slots:
    void onOkClicked();
    void onCancelClicked();

  private:
    void initGUI();

};
#endif                                  // SETTINGSWIDGET_H
