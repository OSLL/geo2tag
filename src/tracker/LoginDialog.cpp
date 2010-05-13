/*
 * LoginDialog.cpp
 *
 *  Created on: 13.05.2010
 *      Author: Администратор
 */

#include "LoginDialog.h"

#include <QPushButton>
#include <QLabel>
#include <QHBoxLayout>
#include <QVBoxLayout>

LoginDialog::LoginDialog(QWidget *parent) : QDialog(parent)
{
    QPushButton *loginButton = new QPushButton("Create new", this);
    QPushButton *attachButton = new QPushButton("Attach existing", this);
    QLabel *label1 = new QLabel("Create new channel", this);
    QLabel *label2 = new QLabel("or attach existing?", this);

    QHBoxLayout *butLayout = new QHBoxLayout();
    butLayout->addWidget(loginButton);
    butLayout->addWidget(attachButton);

    QVBoxLayout *mainLayout = new QVBoxLayout(this);
    mainLayout->addStretch();
    mainLayout->addWidget(label1);
    mainLayout->addWidget(label2);
    mainLayout->addLayout(butLayout);

    this->showMaximized();

    connect(loginButton, SIGNAL(clicked()), this, SLOT(accept()));
    connect(attachButton, SIGNAL(clicked()), this, SLOT(reject()));
}

