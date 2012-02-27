#include "inc/LoginWidget.h"
#include <QVBoxLayout>
#include <QLabel>

LoginWidget::LoginWidget(QWidget *parent) :
    QWidget(parent)
{
    m_loginEdit = new QLineEdit(this);
    m_passwordEdit = new QLineEdit(this);
    m_rememberCheck = new QCheckBox("Remember me", this);
    m_signInButton = new QPushButton("Sign in", this);
    m_createAccountButton = new QPushButton("Create account", this);

    initGUI();

    connect(m_signInButton, SIGNAL(clicked()),
             this, SLOT(onSignInClicked()));
    connect(m_createAccountButton, SIGNAL(clicked()),
            this, SLOT(onCreateAccountClicked()));
}

void LoginWidget::initGUI()
{
    QVBoxLayout *mainLayout = new QVBoxLayout();
    mainLayout->addWidget(new QLabel("Login", this));
    mainLayout->addWidget(m_loginEdit);
    mainLayout->addWidget(new QLabel("Password"));
    mainLayout->addWidget(m_passwordEdit);
    mainLayout->addWidget(m_rememberCheck);
    mainLayout->addWidget(m_signInButton);
    mainLayout->addWidget(m_createAccountButton);
    mainLayout->addStretch();
    this->setLayout(mainLayout);
}

void LoginWidget::onSignInClicked()
{
}

void LoginWidget::onCreateAccountClicked()
{
}


