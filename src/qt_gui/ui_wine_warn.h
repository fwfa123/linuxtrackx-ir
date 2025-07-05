/********************************************************************************
** Form generated from reading UI file 'wine_warn.ui'
**
** Created by: Qt User Interface Compiler version 5.15.8
**
** WARNING! All changes made in this file will be lost when recompiling UI file!
********************************************************************************/

#ifndef UI_WINE_WARN_H
#define UI_WINE_WARN_H

#include <QtCore/QVariant>
#include <QtWidgets/QApplication>
#include <QtWidgets/QCheckBox>
#include <QtWidgets/QDialog>
#include <QtWidgets/QHBoxLayout>
#include <QtWidgets/QLabel>
#include <QtWidgets/QPushButton>
#include <QtWidgets/QSpacerItem>
#include <QtWidgets/QVBoxLayout>

QT_BEGIN_NAMESPACE

class Ui_Dialog
{
public:
    QVBoxLayout *verticalLayout;
    QLabel *label;
    QHBoxLayout *horizontalLayout;
    QCheckBox *DontShow;
    QSpacerItem *horizontalSpacer;
    QPushButton *OKButton;

    void setupUi(QDialog *Dialog)
    {
        if (Dialog->objectName().isEmpty())
            Dialog->setObjectName(QString::fromUtf8("Dialog"));
        Dialog->setWindowModality(Qt::ApplicationModal);
        Dialog->resize(400, 103);
        verticalLayout = new QVBoxLayout(Dialog);
        verticalLayout->setObjectName(QString::fromUtf8("verticalLayout"));
        label = new QLabel(Dialog);
        label->setObjectName(QString::fromUtf8("label"));
        QSizePolicy sizePolicy(QSizePolicy::Preferred, QSizePolicy::MinimumExpanding);
        sizePolicy.setHorizontalStretch(0);
        sizePolicy.setVerticalStretch(0);
        sizePolicy.setHeightForWidth(label->sizePolicy().hasHeightForWidth());
        label->setSizePolicy(sizePolicy);

        verticalLayout->addWidget(label);

        horizontalLayout = new QHBoxLayout();
        horizontalLayout->setObjectName(QString::fromUtf8("horizontalLayout"));
        DontShow = new QCheckBox(Dialog);
        DontShow->setObjectName(QString::fromUtf8("DontShow"));

        horizontalLayout->addWidget(DontShow);

        horizontalSpacer = new QSpacerItem(40, 20, QSizePolicy::Expanding, QSizePolicy::Minimum);

        horizontalLayout->addItem(horizontalSpacer);

        OKButton = new QPushButton(Dialog);
        OKButton->setObjectName(QString::fromUtf8("OKButton"));
        OKButton->setAutoDefault(true);

        horizontalLayout->addWidget(OKButton);


        verticalLayout->addLayout(horizontalLayout);

        QWidget::setTabOrder(OKButton, DontShow);

        retranslateUi(Dialog);

        OKButton->setDefault(true);


        QMetaObject::connectSlotsByName(Dialog);
    } // setupUi

    void retranslateUi(QDialog *Dialog)
    {
        Dialog->setWindowTitle(QCoreApplication::translate("Dialog", "Wine not available", nullptr));
        label->setText(QCoreApplication::translate("Dialog", "Wine is not available.\n"
"You'll not be able to install NP firmware and Wine plugin!", nullptr));
        DontShow->setText(QCoreApplication::translate("Dialog", "Don't show this warning again.", nullptr));
        OKButton->setText(QCoreApplication::translate("Dialog", "OK", nullptr));
    } // retranslateUi

};

namespace Ui {
    class Dialog: public Ui_Dialog {};
} // namespace Ui

QT_END_NAMESPACE

#endif // UI_WINE_WARN_H
