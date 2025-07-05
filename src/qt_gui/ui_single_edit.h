/********************************************************************************
** Form generated from reading UI file 'single_edit.ui'
**
** Created by: Qt User Interface Compiler version 5.15.8
**
** WARNING! All changes made in this file will be lost when recompiling UI file!
********************************************************************************/

#ifndef UI_SINGLE_EDIT_H
#define UI_SINGLE_EDIT_H

#include <QtCore/QVariant>
#include <QtWidgets/QApplication>
#include <QtWidgets/QGroupBox>
#include <QtWidgets/QHBoxLayout>
#include <QtWidgets/QRadioButton>
#include <QtWidgets/QSpacerItem>
#include <QtWidgets/QVBoxLayout>
#include <QtWidgets/QWidget>

QT_BEGIN_NAMESPACE

class Ui_SingleEditForm
{
public:
    QVBoxLayout *verticalLayout_2;
    QVBoxLayout *verticalLayout;
    QHBoxLayout *horizontalLayout;
    QGroupBox *groupBox;
    QHBoxLayout *horizontalLayout_2;
    QRadioButton *SinglePtLeds;
    QRadioButton *ClipReflective;
    QSpacerItem *horizontalSpacer;
    QSpacerItem *verticalSpacer;

    void setupUi(QWidget *SingleEditForm)
    {
        if (SingleEditForm->objectName().isEmpty())
            SingleEditForm->setObjectName(QString::fromUtf8("SingleEditForm"));
        SingleEditForm->resize(353, 86);
        verticalLayout_2 = new QVBoxLayout(SingleEditForm);
        verticalLayout_2->setObjectName(QString::fromUtf8("verticalLayout_2"));
        verticalLayout = new QVBoxLayout();
        verticalLayout->setObjectName(QString::fromUtf8("verticalLayout"));
        horizontalLayout = new QHBoxLayout();
        horizontalLayout->setObjectName(QString::fromUtf8("horizontalLayout"));
        groupBox = new QGroupBox(SingleEditForm);
        groupBox->setObjectName(QString::fromUtf8("groupBox"));
        QSizePolicy sizePolicy(QSizePolicy::Preferred, QSizePolicy::Preferred);
        sizePolicy.setHorizontalStretch(0);
        sizePolicy.setVerticalStretch(0);
        sizePolicy.setHeightForWidth(groupBox->sizePolicy().hasHeightForWidth());
        groupBox->setSizePolicy(sizePolicy);
        groupBox->setMinimumSize(QSize(180, 0));
        horizontalLayout_2 = new QHBoxLayout(groupBox);
        horizontalLayout_2->setObjectName(QString::fromUtf8("horizontalLayout_2"));
        SinglePtLeds = new QRadioButton(groupBox);
        SinglePtLeds->setObjectName(QString::fromUtf8("SinglePtLeds"));
        QSizePolicy sizePolicy1(QSizePolicy::Preferred, QSizePolicy::Fixed);
        sizePolicy1.setHorizontalStretch(0);
        sizePolicy1.setVerticalStretch(0);
        sizePolicy1.setHeightForWidth(SinglePtLeds->sizePolicy().hasHeightForWidth());
        SinglePtLeds->setSizePolicy(sizePolicy1);
        SinglePtLeds->setChecked(true);

        horizontalLayout_2->addWidget(SinglePtLeds);

        ClipReflective = new QRadioButton(groupBox);
        ClipReflective->setObjectName(QString::fromUtf8("ClipReflective"));

        horizontalLayout_2->addWidget(ClipReflective);


        horizontalLayout->addWidget(groupBox);

        horizontalSpacer = new QSpacerItem(40, 20, QSizePolicy::Expanding, QSizePolicy::Minimum);

        horizontalLayout->addItem(horizontalSpacer);


        verticalLayout->addLayout(horizontalLayout);

        verticalSpacer = new QSpacerItem(20, 40, QSizePolicy::Minimum, QSizePolicy::Expanding);

        verticalLayout->addItem(verticalSpacer);


        verticalLayout_2->addLayout(verticalLayout);


        retranslateUi(SingleEditForm);

        QMetaObject::connectSlotsByName(SingleEditForm);
    } // setupUi

    void retranslateUi(QWidget *SingleEditForm)
    {
        SingleEditForm->setWindowTitle(QCoreApplication::translate("SingleEditForm", "Form", nullptr));
        groupBox->setTitle(QCoreApplication::translate("SingleEditForm", "Model Type", nullptr));
        SinglePtLeds->setText(QCoreApplication::translate("SingleEditForm", "LEDs", nullptr));
        ClipReflective->setText(QCoreApplication::translate("SingleEditForm", "Reflective", nullptr));
    } // retranslateUi

};

namespace Ui {
    class SingleEditForm: public Ui_SingleEditForm {};
} // namespace Ui

QT_END_NAMESPACE

#endif // UI_SINGLE_EDIT_H
