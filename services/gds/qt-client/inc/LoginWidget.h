#ifndef LOGINWIDGET_H
#define LOGINWIDGET_H

#include <QWidget>
#include <QLineEdit>
#include <QCheckBox>
#include <QPushButton>
#include "LoginQuery.h"

class LoginWidget : public QWidget
{
    Q_OBJECT

    LoginQuery *m_loginQuery;

    // GUI
    QLineEdit *m_loginEdit;
    QLineEdit *m_passwordEdit;
    QCheckBox *m_rememberCheck;
    QPushButton *m_signInButton;
    QPushButton *m_createAccountButton;

public:
    explicit LoginWidget(QWidget *parent = 0);

signals:
    void signedIn(const QString& authToken);
    void settingsRequested();

public slots:
    void onSignInClicked();
    void onCreateAccountClicked();
    void onLoginConnected();
    void onLoginError(int errno);
    void onLoginNetworkError(QString);

private:
    void initGUI();

};

#endif // LOGINWIDGET_H
