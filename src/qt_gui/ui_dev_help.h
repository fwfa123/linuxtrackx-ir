/********************************************************************************
** Form generated from reading UI file 'dev_help.ui'
**
** Created by: Qt User Interface Compiler version 5.15.17
**
** WARNING! All changes made in this file will be lost when recompiling UI file!
********************************************************************************/

#ifndef UI_DEV_HELP_H
#define UI_DEV_HELP_H

#include <QtCore/QVariant>
#include <QtWidgets/QApplication>
#include <QtWidgets/QPushButton>
#include <QtWidgets/QSpacerItem>
#include <QtWidgets/QVBoxLayout>
#include <QtWidgets/QWidget>

QT_BEGIN_NAMESPACE

class Ui_DevHelp
{
public:
    QVBoxLayout *verticalLayout;
    QPushButton *DumpPrefsButton;
    QSpacerItem *verticalSpacer;

    void setupUi(QWidget *DevHelp)
    {
        if (DevHelp->objectName().isEmpty())
            DevHelp->setObjectName(QString::fromUtf8("DevHelp"));
        DevHelp->resize(300, 80);
        verticalLayout = new QVBoxLayout(DevHelp);
        verticalLayout->setObjectName(QString::fromUtf8("verticalLayout"));
        DumpPrefsButton = new QPushButton(DevHelp);
        DumpPrefsButton->setObjectName(QString::fromUtf8("DumpPrefsButton"));

        verticalLayout->addWidget(DumpPrefsButton);

        verticalSpacer = new QSpacerItem(20, 40, QSizePolicy::Minimum, QSizePolicy::Expanding);

        verticalLayout->addItem(verticalSpacer);


        retranslateUi(DevHelp);

        QMetaObject::connectSlotsByName(DevHelp);
    } // setupUi

    void retranslateUi(QWidget *DevHelp)
    {
        DevHelp->setWindowTitle(QCoreApplication::translate("DevHelp", "DevHelp", nullptr));
        DumpPrefsButton->setText(QCoreApplication::translate("DevHelp", "Dump preferences", nullptr));
    } // retranslateUi

};

namespace Ui {
    class DevHelp: public Ui_DevHelp {};
} // namespace Ui

QT_END_NAMESPACE

#endif // UI_DEV_HELP_H
