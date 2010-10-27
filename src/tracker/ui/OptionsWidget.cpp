#include "OptionsWidget.h"

#include <QVBoxLayout>
#include <QLabel>
#include <QFile>
#include <QtXml/QDomDocument>
#include <QtXml/QDomElement>
#include <QDebug>

OptionsWidget::OptionsWidget(QWidget *parent) :
        QWidget(parent)
{
    QVBoxLayout *layout = new QVBoxLayout();
    layout->addWidget(new QLabel("Login:"));
    layout->addWidget(m_name = new QLineEdit());
    layout->addWidget(new QLabel("Password:"));
    layout->addWidget(m_password = new QLineEdit());
    layout->addWidget(new QLabel("Channel name:"));
    layout->addWidget(m_channel = new QLineEdit());
    layout->addStretch();
    layout->addWidget(m_doneButton = new QPushButton("Done"));
    this->setLayout(layout);

    connect(m_doneButton, SIGNAL(clicked()), this, SLOT(onDoneClicked()));

    /*
     * Read settings for /var/tracker/tracker.conf
     */
    QDomDocument docSettings("Settings");
    QFile settingsFile("/var/tracker/tracker.conf");
    if (settingsFile.open(QIODevice::ReadOnly))
    {
        if (docSettings.setContent(&settingsFile))
        {
            QDomElement docElem = docSettings.documentElement();

            QDomNode n = docElem.firstChild();
            while(!n.isNull()) {
                QDomElement e = n.toElement(); // try to convert the node to an element.
                if(!e.isNull()) {
                    if (e.tagName() == "login")
                    {
                        m_name->setText(e.text());
                        qDebug() << "read login: " << e.text();
                    }
                    else if (e.tagName() == "password")
                    {
                        m_password->setText(e.text());
                        qDebug() << "read password: " << e.text();
                    }
                }
                n = n.nextSibling();
            }
        }
        else
        {
            qDebug() << "can't find xml document in config file";
        }
    }
    else
    {
        qDebug() << "can't open settings file /var/tracker/tracker.conf";
        settingsFile.close();
    }


    settingsFile.close();

}

QString OptionsWidget::name()
{
    return m_name->text();
}

QString OptionsWidget::password()
{
    return m_password->text();
}

QString OptionsWidget::channel()
{
    return m_channel->text();
}

void OptionsWidget::onDoneClicked()
{
    emit this->done();
}
