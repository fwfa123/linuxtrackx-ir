/********************************************************************************
** Form generated from reading UI file 'progress.ui'
**
** Created by: Qt User Interface Compiler version 5.15.17
**
** WARNING! All changes made in this file will be lost when recompiling UI file!
********************************************************************************/

#ifndef UI_PROGRESS_H
#define UI_PROGRESS_H

#include <QtCore/QVariant>
#include <QtWidgets/QApplication>
#include <QtWidgets/QLabel>
#include <QtWidgets/QProgressBar>
#include <QtWidgets/QVBoxLayout>
#include <QtWidgets/QWidget>

QT_BEGIN_NAMESPACE

class Ui_DLProgress
{
public:
    QVBoxLayout *verticalLayout;
    QLabel *InfoLabel;
    QProgressBar *ProgressBar;

    void setupUi(QWidget *DLProgress)
    {
        if (DLProgress->objectName().isEmpty())
            DLProgress->setObjectName(QString::fromUtf8("DLProgress"));
        DLProgress->resize(400, 66);
        verticalLayout = new QVBoxLayout(DLProgress);
        verticalLayout->setObjectName(QString::fromUtf8("verticalLayout"));
        InfoLabel = new QLabel(DLProgress);
        InfoLabel->setObjectName(QString::fromUtf8("InfoLabel"));

        verticalLayout->addWidget(InfoLabel);

        ProgressBar = new QProgressBar(DLProgress);
        ProgressBar->setObjectName(QString::fromUtf8("ProgressBar"));
        ProgressBar->setValue(0);

        verticalLayout->addWidget(ProgressBar);


        retranslateUi(DLProgress);

        QMetaObject::connectSlotsByName(DLProgress);
    } // setupUi

    void retranslateUi(QWidget *DLProgress)
    {
        DLProgress->setWindowTitle(QCoreApplication::translate("DLProgress", "Download Progress", nullptr));
        InfoLabel->setText(QCoreApplication::translate("DLProgress", "TextLabel", nullptr));
    } // retranslateUi

};

namespace Ui {
    class DLProgress: public Ui_DLProgress {};
} // namespace Ui

QT_END_NAMESPACE

#endif // UI_PROGRESS_H
