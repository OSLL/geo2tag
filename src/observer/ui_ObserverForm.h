/********************************************************************************
** Form generated from reading ui file 'ObserverForm.ui'
**
** Created: Wed May 12 03:11:07 2010
**      by: Qt User Interface Compiler version 4.5.1
**
** WARNING! All changes made in this file will be lost when recompiling ui file!
********************************************************************************/

#ifndef UI_OBSERVERFORM_H
#define UI_OBSERVERFORM_H

#include <QtCore/QVariant>
#include <QtGui/QAction>
#include <QtGui/QApplication>
#include <QtGui/QButtonGroup>
#include <QtGui/QHBoxLayout>
#include <QtGui/QHeaderView>
#include <QtGui/QLabel>
#include <QtGui/QLineEdit>
#include <QtGui/QPushButton>
#include <QtGui/QSlider>
#include <QtGui/QVBoxLayout>
#include <QtGui/QWidget>

QT_BEGIN_NAMESPACE

class Ui_Form
{
public:
    QVBoxLayout *verticalLayout_3;
    QHBoxLayout *horizontalLayout_5;
    QVBoxLayout *verticalLayout;
    QHBoxLayout *horizontalLayout;
    QLabel *label;
    QLineEdit *m_user;
    QHBoxLayout *horizontalLayout_2;
    QLabel *label_2;
    QLineEdit *m_password;
    QVBoxLayout *verticalLayout_2;
    QHBoxLayout *horizontalLayout_3;
    QLabel *label_3;
    QLineEdit *m_channel;
    QHBoxLayout *horizontalLayout_4;
    QLabel *label_4;
    QLineEdit *m_channelKey;
    QPushButton *m_updateButton;
    QHBoxLayout *horizontalLayout_6;
    QWidget *m_mapArea;
    QSlider *m_scale;

    void setupUi(QWidget *Form)
    {
        if (Form->objectName().isEmpty())
            Form->setObjectName(QString::fromUtf8("Form"));
        Form->resize(436, 359);
        verticalLayout_3 = new QVBoxLayout(Form);
        verticalLayout_3->setObjectName(QString::fromUtf8("verticalLayout_3"));
        horizontalLayout_5 = new QHBoxLayout();
        horizontalLayout_5->setObjectName(QString::fromUtf8("horizontalLayout_5"));
        verticalLayout = new QVBoxLayout();
        verticalLayout->setObjectName(QString::fromUtf8("verticalLayout"));
        horizontalLayout = new QHBoxLayout();
        horizontalLayout->setObjectName(QString::fromUtf8("horizontalLayout"));
        label = new QLabel(Form);
        label->setObjectName(QString::fromUtf8("label"));

        horizontalLayout->addWidget(label);

        m_user = new QLineEdit(Form);
        m_user->setObjectName(QString::fromUtf8("m_user"));

        horizontalLayout->addWidget(m_user);


        verticalLayout->addLayout(horizontalLayout);

        horizontalLayout_2 = new QHBoxLayout();
        horizontalLayout_2->setObjectName(QString::fromUtf8("horizontalLayout_2"));
        label_2 = new QLabel(Form);
        label_2->setObjectName(QString::fromUtf8("label_2"));

        horizontalLayout_2->addWidget(label_2);

        m_password = new QLineEdit(Form);
        m_password->setObjectName(QString::fromUtf8("m_password"));

        horizontalLayout_2->addWidget(m_password);


        verticalLayout->addLayout(horizontalLayout_2);


        horizontalLayout_5->addLayout(verticalLayout);

        verticalLayout_2 = new QVBoxLayout();
        verticalLayout_2->setObjectName(QString::fromUtf8("verticalLayout_2"));
        horizontalLayout_3 = new QHBoxLayout();
        horizontalLayout_3->setObjectName(QString::fromUtf8("horizontalLayout_3"));
        label_3 = new QLabel(Form);
        label_3->setObjectName(QString::fromUtf8("label_3"));

        horizontalLayout_3->addWidget(label_3);

        m_channel = new QLineEdit(Form);
        m_channel->setObjectName(QString::fromUtf8("m_channel"));

        horizontalLayout_3->addWidget(m_channel);


        verticalLayout_2->addLayout(horizontalLayout_3);

        horizontalLayout_4 = new QHBoxLayout();
        horizontalLayout_4->setObjectName(QString::fromUtf8("horizontalLayout_4"));
        label_4 = new QLabel(Form);
        label_4->setObjectName(QString::fromUtf8("label_4"));

        horizontalLayout_4->addWidget(label_4);

        m_channelKey = new QLineEdit(Form);
        m_channelKey->setObjectName(QString::fromUtf8("m_channelKey"));

        horizontalLayout_4->addWidget(m_channelKey);


        verticalLayout_2->addLayout(horizontalLayout_4);


        horizontalLayout_5->addLayout(verticalLayout_2);

        m_updateButton = new QPushButton(Form);
        m_updateButton->setObjectName(QString::fromUtf8("m_updateButton"));

        horizontalLayout_5->addWidget(m_updateButton);


        verticalLayout_3->addLayout(horizontalLayout_5);

        horizontalLayout_6 = new QHBoxLayout();
        horizontalLayout_6->setObjectName(QString::fromUtf8("horizontalLayout_6"));
        m_mapArea = new QWidget(Form);
        m_mapArea->setObjectName(QString::fromUtf8("m_mapArea"));

        horizontalLayout_6->addWidget(m_mapArea);

        m_scale = new QSlider(Form);
        m_scale->setObjectName(QString::fromUtf8("m_scale"));
        m_scale->setMinimum(1);
        m_scale->setMaximum(10);
        m_scale->setValue(5);
        m_scale->setOrientation(Qt::Vertical);

        horizontalLayout_6->addWidget(m_scale);


        verticalLayout_3->addLayout(horizontalLayout_6);


        retranslateUi(Form);

        QMetaObject::connectSlotsByName(Form);
    } // setupUi

    void retranslateUi(QWidget *Form)
    {
        Form->setWindowTitle(QApplication::translate("Form", "Form", 0, QApplication::UnicodeUTF8));
        label->setText(QApplication::translate("Form", "User", 0, QApplication::UnicodeUTF8));
        label_2->setText(QApplication::translate("Form", "Password", 0, QApplication::UnicodeUTF8));
        label_3->setText(QApplication::translate("Form", "Channel", 0, QApplication::UnicodeUTF8));
        label_4->setText(QApplication::translate("Form", "Key", 0, QApplication::UnicodeUTF8));
        m_updateButton->setText(QApplication::translate("Form", "Update", 0, QApplication::UnicodeUTF8));
        Q_UNUSED(Form);
    } // retranslateUi

};

namespace Ui {
    class Form: public Ui_Form {};
} // namespace Ui

QT_END_NAMESPACE

#endif // UI_OBSERVERFORM_H
