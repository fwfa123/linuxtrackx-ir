/********************************************************************************
** Form generated from reading UI file 'xplugin.ui'
**
** Created by: Qt User Interface Compiler version 5.15.17
**
** WARNING! All changes made in this file will be lost when recompiling UI file!
********************************************************************************/

#ifndef UI_XPLUGIN_H
#define UI_XPLUGIN_H

#include <QtCore/QVariant>
#include <QtWidgets/QApplication>
#include <QtWidgets/QDialog>
#include <QtWidgets/QHBoxLayout>
#include <QtWidgets/QLabel>
#include <QtWidgets/QPushButton>
#include <QtWidgets/QVBoxLayout>

QT_BEGIN_NAMESPACE

class Ui_XPluginInstall
{
public:
    QVBoxLayout *verticalLayout;
    QLabel *label_2;
    QLabel *label;
    QHBoxLayout *horizontalLayout;
    QPushButton *BrowseXPlane;
    QPushButton *CancelButton;

    void setupUi(QDialog *XPluginInstall)
    {
        if (XPluginInstall->objectName().isEmpty())
            XPluginInstall->setObjectName(QString::fromUtf8("XPluginInstall"));
        XPluginInstall->resize(415, 133);
        verticalLayout = new QVBoxLayout(XPluginInstall);
        verticalLayout->setObjectName(QString::fromUtf8("verticalLayout"));
        label_2 = new QLabel(XPluginInstall);
        label_2->setObjectName(QString::fromUtf8("label_2"));
        QSizePolicy sizePolicy(QSizePolicy::Preferred, QSizePolicy::Maximum);
        sizePolicy.setHorizontalStretch(0);
        sizePolicy.setVerticalStretch(0);
        sizePolicy.setHeightForWidth(label_2->sizePolicy().hasHeightForWidth());
        label_2->setSizePolicy(sizePolicy);
        QFont font;
        font.setPointSize(15);
        font.setBold(true);
        font.setWeight(75);
        label_2->setFont(font);
        label_2->setAlignment(Qt::AlignCenter);

        verticalLayout->addWidget(label_2);

        label = new QLabel(XPluginInstall);
        label->setObjectName(QString::fromUtf8("label"));
        label->setAlignment(Qt::AlignCenter);

        verticalLayout->addWidget(label);

        horizontalLayout = new QHBoxLayout();
        horizontalLayout->setObjectName(QString::fromUtf8("horizontalLayout"));
        BrowseXPlane = new QPushButton(XPluginInstall);
        BrowseXPlane->setObjectName(QString::fromUtf8("BrowseXPlane"));
        BrowseXPlane->setAutoDefault(false);

        horizontalLayout->addWidget(BrowseXPlane);

        CancelButton = new QPushButton(XPluginInstall);
        CancelButton->setObjectName(QString::fromUtf8("CancelButton"));
        CancelButton->setAutoDefault(false);

        horizontalLayout->addWidget(CancelButton);


        verticalLayout->addLayout(horizontalLayout);


        retranslateUi(XPluginInstall);
        QObject::connect(CancelButton, SIGNAL(clicked()), XPluginInstall, SLOT(reject()));

        BrowseXPlane->setDefault(true);


        QMetaObject::connectSlotsByName(XPluginInstall);
    } // setupUi

    void retranslateUi(QDialog *XPluginInstall)
    {
        XPluginInstall->setWindowTitle(QCoreApplication::translate("XPluginInstall", "Dialog", nullptr));
        label_2->setText(QCoreApplication::translate("XPluginInstall", "XPlane Plugin Installation", nullptr));
        label->setText(QCoreApplication::translate("XPluginInstall", "<html><head/><body><p>Press the <span style=\" font-weight:600;\">Browse</span> button and navigate to the XPlane executable.</p><p>Select it and press <span style=\" font-weight:600;\">Open</span> button to install the plugin.</p></body></html>", nullptr));
        BrowseXPlane->setText(QCoreApplication::translate("XPluginInstall", "Browse", nullptr));
        CancelButton->setText(QCoreApplication::translate("XPluginInstall", "Cancel", nullptr));
    } // retranslateUi

};

namespace Ui {
    class XPluginInstall: public Ui_XPluginInstall {};
} // namespace Ui

QT_END_NAMESPACE

#endif // UI_XPLUGIN_H
