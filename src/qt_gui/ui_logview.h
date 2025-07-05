/********************************************************************************
** Form generated from reading UI file 'logview.ui'
**
** Created by: Qt User Interface Compiler version 5.15.8
**
** WARNING! All changes made in this file will be lost when recompiling UI file!
********************************************************************************/

#ifndef UI_LOGVIEW_H
#define UI_LOGVIEW_H

#include <QtCore/QVariant>
#include <QtGui/QIcon>
#include <QtWidgets/QApplication>
#include <QtWidgets/QHBoxLayout>
#include <QtWidgets/QPushButton>
#include <QtWidgets/QSpacerItem>
#include <QtWidgets/QVBoxLayout>
#include <QtWidgets/QWidget>

QT_BEGIN_NAMESPACE

class Ui_LogViewerForm
{
public:
    QVBoxLayout *verticalLayout_2;
    QVBoxLayout *verticalLayout;
    QHBoxLayout *horizontalLayout;
    QSpacerItem *horizontalSpacer;
    QPushButton *CloseButton;

    void setupUi(QWidget *LogViewerForm)
    {
        if (LogViewerForm->objectName().isEmpty())
            LogViewerForm->setObjectName(QString::fromUtf8("LogViewerForm"));
        LogViewerForm->resize(581, 510);
        QIcon icon;
        icon.addFile(QString::fromUtf8(":/ltr/linuxtrack.svg"), QSize(), QIcon::Normal, QIcon::Off);
        LogViewerForm->setWindowIcon(icon);
        verticalLayout_2 = new QVBoxLayout(LogViewerForm);
        verticalLayout_2->setObjectName(QString::fromUtf8("verticalLayout_2"));
        verticalLayout = new QVBoxLayout();
        verticalLayout->setObjectName(QString::fromUtf8("verticalLayout"));
        horizontalLayout = new QHBoxLayout();
        horizontalLayout->setObjectName(QString::fromUtf8("horizontalLayout"));
        horizontalSpacer = new QSpacerItem(40, 20, QSizePolicy::Expanding, QSizePolicy::Minimum);

        horizontalLayout->addItem(horizontalSpacer);

        CloseButton = new QPushButton(LogViewerForm);
        CloseButton->setObjectName(QString::fromUtf8("CloseButton"));

        horizontalLayout->addWidget(CloseButton);


        verticalLayout->addLayout(horizontalLayout);


        verticalLayout_2->addLayout(verticalLayout);


        retranslateUi(LogViewerForm);

        QMetaObject::connectSlotsByName(LogViewerForm);
    } // setupUi

    void retranslateUi(QWidget *LogViewerForm)
    {
        LogViewerForm->setWindowTitle(QCoreApplication::translate("LogViewerForm", "Form", nullptr));
        CloseButton->setText(QCoreApplication::translate("LogViewerForm", "Close", nullptr));
    } // retranslateUi

};

namespace Ui {
    class LogViewerForm: public Ui_LogViewerForm {};
} // namespace Ui

QT_END_NAMESPACE

#endif // UI_LOGVIEW_H
