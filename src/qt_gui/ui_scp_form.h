/********************************************************************************
** Form generated from reading UI file 'scp_form.ui'
**
** Created by: Qt User Interface Compiler version 5.15.8
**
** WARNING! All changes made in this file will be lost when recompiling UI file!
********************************************************************************/

#ifndef UI_SCP_FORM_H
#define UI_SCP_FORM_H

#include <QtCore/QVariant>
#include <QtGui/QIcon>
#include <QtWidgets/QApplication>
#include <QtWidgets/QHBoxLayout>
#include <QtWidgets/QPushButton>
#include <QtWidgets/QSpacerItem>
#include <QtWidgets/QTabWidget>
#include <QtWidgets/QVBoxLayout>
#include <QtWidgets/QWidget>

QT_BEGIN_NAMESPACE

class Ui_SCPForm
{
public:
    QVBoxLayout *verticalLayout_2;
    QVBoxLayout *verticalLayout;
    QTabWidget *SCPTab;
    QWidget *SCPPitch1;
    QVBoxLayout *verticalLayout_3;
    QVBoxLayout *SCPPitch;
    QWidget *SCPYaw1;
    QVBoxLayout *verticalLayout_4;
    QVBoxLayout *SCPYaw;
    QWidget *SCPRoll1;
    QVBoxLayout *verticalLayout_5;
    QVBoxLayout *SCPRoll;
    QWidget *SCPX1;
    QVBoxLayout *verticalLayout_6;
    QVBoxLayout *SCPX;
    QWidget *SCPY1;
    QVBoxLayout *verticalLayout_7;
    QVBoxLayout *SCPY;
    QWidget *SCPZ1;
    QVBoxLayout *verticalLayout_8;
    QVBoxLayout *SCPZ;
    QHBoxLayout *horizontalLayout;
    QSpacerItem *horizontalSpacer;
    QPushButton *SCPCloseButton;

    void setupUi(QWidget *SCPForm)
    {
        if (SCPForm->objectName().isEmpty())
            SCPForm->setObjectName(QString::fromUtf8("SCPForm"));
        SCPForm->resize(639, 520);
        QIcon icon;
        icon.addFile(QString::fromUtf8(":/ltr/linuxtrack.svg"), QSize(), QIcon::Normal, QIcon::Off);
        SCPForm->setWindowIcon(icon);
        verticalLayout_2 = new QVBoxLayout(SCPForm);
        verticalLayout_2->setObjectName(QString::fromUtf8("verticalLayout_2"));
        verticalLayout = new QVBoxLayout();
        verticalLayout->setObjectName(QString::fromUtf8("verticalLayout"));
        SCPTab = new QTabWidget(SCPForm);
        SCPTab->setObjectName(QString::fromUtf8("SCPTab"));
        SCPTab->setEnabled(true);
        SCPPitch1 = new QWidget();
        SCPPitch1->setObjectName(QString::fromUtf8("SCPPitch1"));
        verticalLayout_3 = new QVBoxLayout(SCPPitch1);
        verticalLayout_3->setObjectName(QString::fromUtf8("verticalLayout_3"));
        SCPPitch = new QVBoxLayout();
        SCPPitch->setObjectName(QString::fromUtf8("SCPPitch"));

        verticalLayout_3->addLayout(SCPPitch);

        SCPTab->addTab(SCPPitch1, QString());
        SCPYaw1 = new QWidget();
        SCPYaw1->setObjectName(QString::fromUtf8("SCPYaw1"));
        verticalLayout_4 = new QVBoxLayout(SCPYaw1);
        verticalLayout_4->setObjectName(QString::fromUtf8("verticalLayout_4"));
        SCPYaw = new QVBoxLayout();
        SCPYaw->setObjectName(QString::fromUtf8("SCPYaw"));

        verticalLayout_4->addLayout(SCPYaw);

        SCPTab->addTab(SCPYaw1, QString());
        SCPRoll1 = new QWidget();
        SCPRoll1->setObjectName(QString::fromUtf8("SCPRoll1"));
        verticalLayout_5 = new QVBoxLayout(SCPRoll1);
        verticalLayout_5->setObjectName(QString::fromUtf8("verticalLayout_5"));
        SCPRoll = new QVBoxLayout();
        SCPRoll->setObjectName(QString::fromUtf8("SCPRoll"));

        verticalLayout_5->addLayout(SCPRoll);

        SCPTab->addTab(SCPRoll1, QString());
        SCPX1 = new QWidget();
        SCPX1->setObjectName(QString::fromUtf8("SCPX1"));
        verticalLayout_6 = new QVBoxLayout(SCPX1);
        verticalLayout_6->setObjectName(QString::fromUtf8("verticalLayout_6"));
        SCPX = new QVBoxLayout();
        SCPX->setObjectName(QString::fromUtf8("SCPX"));

        verticalLayout_6->addLayout(SCPX);

        SCPTab->addTab(SCPX1, QString());
        SCPY1 = new QWidget();
        SCPY1->setObjectName(QString::fromUtf8("SCPY1"));
        verticalLayout_7 = new QVBoxLayout(SCPY1);
        verticalLayout_7->setObjectName(QString::fromUtf8("verticalLayout_7"));
        SCPY = new QVBoxLayout();
        SCPY->setObjectName(QString::fromUtf8("SCPY"));

        verticalLayout_7->addLayout(SCPY);

        SCPTab->addTab(SCPY1, QString());
        SCPZ1 = new QWidget();
        SCPZ1->setObjectName(QString::fromUtf8("SCPZ1"));
        verticalLayout_8 = new QVBoxLayout(SCPZ1);
        verticalLayout_8->setObjectName(QString::fromUtf8("verticalLayout_8"));
        SCPZ = new QVBoxLayout();
        SCPZ->setObjectName(QString::fromUtf8("SCPZ"));

        verticalLayout_8->addLayout(SCPZ);

        SCPTab->addTab(SCPZ1, QString());

        verticalLayout->addWidget(SCPTab);


        verticalLayout_2->addLayout(verticalLayout);

        horizontalLayout = new QHBoxLayout();
        horizontalLayout->setObjectName(QString::fromUtf8("horizontalLayout"));
        horizontalSpacer = new QSpacerItem(40, 20, QSizePolicy::Expanding, QSizePolicy::Minimum);

        horizontalLayout->addItem(horizontalSpacer);

        SCPCloseButton = new QPushButton(SCPForm);
        SCPCloseButton->setObjectName(QString::fromUtf8("SCPCloseButton"));

        horizontalLayout->addWidget(SCPCloseButton);


        verticalLayout_2->addLayout(horizontalLayout);


        retranslateUi(SCPForm);

        SCPTab->setCurrentIndex(0);


        QMetaObject::connectSlotsByName(SCPForm);
    } // setupUi

    void retranslateUi(QWidget *SCPForm)
    {
        SCPForm->setWindowTitle(QCoreApplication::translate("SCPForm", "Sensitivity curves setup", nullptr));
        SCPTab->setTabText(SCPTab->indexOf(SCPPitch1), QCoreApplication::translate("SCPForm", "Pitch", nullptr));
        SCPTab->setTabText(SCPTab->indexOf(SCPYaw1), QCoreApplication::translate("SCPForm", "Yaw", nullptr));
        SCPTab->setTabText(SCPTab->indexOf(SCPRoll1), QCoreApplication::translate("SCPForm", "Roll", nullptr));
        SCPTab->setTabText(SCPTab->indexOf(SCPX1), QCoreApplication::translate("SCPForm", "X Translation", nullptr));
        SCPTab->setTabText(SCPTab->indexOf(SCPY1), QCoreApplication::translate("SCPForm", "Y Translation", nullptr));
        SCPTab->setTabText(SCPTab->indexOf(SCPZ1), QCoreApplication::translate("SCPForm", "Z Translation", nullptr));
        SCPCloseButton->setText(QCoreApplication::translate("SCPForm", "Close", nullptr));
    } // retranslateUi

};

namespace Ui {
    class SCPForm: public Ui_SCPForm {};
} // namespace Ui

QT_END_NAMESPACE

#endif // UI_SCP_FORM_H
