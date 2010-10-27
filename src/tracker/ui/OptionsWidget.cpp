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
    layout->addWidget(m_nameEdit = new QLineEdit());
    layout->addWidget(new QLabel("Password:"));
    layout->addWidget(m_passwordEdit = new QLineEdit());
    layout->addWidget(new QLabel("Channel name:"));
    layout->addWidget(m_channelEdit = new QLineEdit());
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
                        m_nameEdit->setText(e.text());
                        m_name = e.text();
                        qDebug() << "read login: " << e.text();
                    }
                    else if (e.tagName() == "password")
                    {
                        m_passwordEdit->setText(e.text());
                        m_password = e.text();
                        qDebug() << "read password: " << e.text();
                    }
                    else if (e.tagName() == "channel")
                    {
                        m_channelEdit->setText(e.text());
                        m_channel = e.text();
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
    return m_name;
}

QString OptionsWidget::password()
{
    return m_password;
}

QString OptionsWidget::channel()
{
    return m_channel;
}

void OptionsWidget::onDoneClicked()
{
    QDomDocument docSettings("Settings");
    QFile settingsFile("/var/tracker/tracker.conf");
    if (settingsFile.open(QIODevice::ReadWrite))
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
                        if (m_nameEdit->text() != m_name)
                        {
                            e.setNodeValue(m_nameEdit->text());
                            docSettings.documentElement().childNodes().at(1).toElement().setNodeValue(m_nameEdit->text());
                            m_name = m_nameEdit->text();
                            qDebug() << "write login: " << m_name;
                        }
                    }
                    else if (e.tagName() == "password")
                    {
                        if (m_passwordEdit->text() != m_password)
                        {
                            e.setNodeValue(m_passwordEdit->text());
                            docSettings.documentElement().childNodes().at(2).setNodeValue(m_passwordEdit->text());
                            m_password = m_passwordEdit->text();
                            qDebug() << "write password: " << m_password;// << m_passwordEdit->text();
                        }
                    }
                    else if (e.tagName() == "channel")
                    {
                        if (m_channelEdit->text() != m_channel)
                        {
                            e.setNodeValue(m_channelEdit->text());
                            m_channel = m_channelEdit->text();
                            qDebug() << "write channel: " << m_channel;
                        }
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

    QString savedXML = docSettings.toString();
    qDebug() << savedXML;
    //QTextStream tempTS(&savedXML);
    //do

    emit this->done();
}
