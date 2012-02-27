#ifndef LOGINWIDGET_H
#define LOGINWIDGET_H

#include <QWidget>
#include <QLineEdit>
#include <QCheckBox>
#include <QPushButton>

class LoginWidget : public QWidget
{
    Q_OBJECT

    QLineEdit *m_loginEdit;
    QLineEdit *m_passwordEdit;
    QCheckBox *m_rememberCheck;
    QPushButton *m_signInButton;
    QPushButton *m_createAccountButton;

public:
    explicit LoginWidget(QWidget *parent = 0);

signals:
    void signedIn();
    void settingsRequested();

public slots:
    void onSignInClicked();
    void onCreateAccountClicked();

private:
    void initGUI();

};

#endif // LOGINWIDGET_H
