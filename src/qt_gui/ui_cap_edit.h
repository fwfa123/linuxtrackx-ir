/********************************************************************************
** Form generated from reading UI file 'cap_edit.ui'
**
** Created by: Qt User Interface Compiler version 5.15.8
**
** WARNING! All changes made in this file will be lost when recompiling UI file!
********************************************************************************/

#ifndef UI_CAP_EDIT_H
#define UI_CAP_EDIT_H

#include <QtCore/QVariant>
#include <QtWidgets/QApplication>
#include <QtWidgets/QDoubleSpinBox>
#include <QtWidgets/QFrame>
#include <QtWidgets/QGridLayout>
#include <QtWidgets/QHBoxLayout>
#include <QtWidgets/QLabel>
#include <QtWidgets/QRadioButton>
#include <QtWidgets/QSpacerItem>
#include <QtWidgets/QVBoxLayout>
#include <QtWidgets/QWidget>

QT_BEGIN_NAMESPACE

class Ui_CapEditForm
{
public:
    QVBoxLayout *verticalLayout_4;
    QHBoxLayout *horizontalLayout;
    QLabel *label_24;
    QVBoxLayout *verticalLayout;
    QFrame *frame;
    QVBoxLayout *verticalLayout_3;
    QLabel *label;
    QGridLayout *gridLayout_2;
    QLabel *label_25;
    QDoubleSpinBox *CapA;
    QDoubleSpinBox *CapB;
    QLabel *label_26;
    QLabel *label_27;
    QDoubleSpinBox *CapC;
    QSpacerItem *horizontalSpacer;
    QFrame *frame_2;
    QVBoxLayout *verticalLayout_2;
    QLabel *label_2;
    QHBoxLayout *horizontalLayout_3;
    QRadioButton *CapLeds;
    QRadioButton *ClipReflective;
    QSpacerItem *verticalSpacer_5;
    QSpacerItem *verticalSpacer;

    void setupUi(QWidget *CapEditForm)
    {
        if (CapEditForm->objectName().isEmpty())
            CapEditForm->setObjectName(QString::fromUtf8("CapEditForm"));
        CapEditForm->resize(506, 326);
        verticalLayout_4 = new QVBoxLayout(CapEditForm);
        verticalLayout_4->setObjectName(QString::fromUtf8("verticalLayout_4"));
        horizontalLayout = new QHBoxLayout();
        horizontalLayout->setObjectName(QString::fromUtf8("horizontalLayout"));
        label_24 = new QLabel(CapEditForm);
        label_24->setObjectName(QString::fromUtf8("label_24"));
        QSizePolicy sizePolicy(QSizePolicy::Maximum, QSizePolicy::MinimumExpanding);
        sizePolicy.setHorizontalStretch(0);
        sizePolicy.setVerticalStretch(0);
        sizePolicy.setHeightForWidth(label_24->sizePolicy().hasHeightForWidth());
        label_24->setSizePolicy(sizePolicy);
        label_24->setPixmap(QPixmap(QString::fromUtf8(":/ltr/cap_small.png")));
        label_24->setScaledContents(false);

        horizontalLayout->addWidget(label_24);

        verticalLayout = new QVBoxLayout();
        verticalLayout->setObjectName(QString::fromUtf8("verticalLayout"));
        frame = new QFrame(CapEditForm);
        frame->setObjectName(QString::fromUtf8("frame"));
        frame->setFrameShape(QFrame::StyledPanel);
        frame->setFrameShadow(QFrame::Raised);
        verticalLayout_3 = new QVBoxLayout(frame);
        verticalLayout_3->setObjectName(QString::fromUtf8("verticalLayout_3"));
        label = new QLabel(frame);
        label->setObjectName(QString::fromUtf8("label"));

        verticalLayout_3->addWidget(label);

        gridLayout_2 = new QGridLayout();
        gridLayout_2->setObjectName(QString::fromUtf8("gridLayout_2"));
        label_25 = new QLabel(frame);
        label_25->setObjectName(QString::fromUtf8("label_25"));
        QSizePolicy sizePolicy1(QSizePolicy::Maximum, QSizePolicy::Preferred);
        sizePolicy1.setHorizontalStretch(0);
        sizePolicy1.setVerticalStretch(0);
        sizePolicy1.setHeightForWidth(label_25->sizePolicy().hasHeightForWidth());
        label_25->setSizePolicy(sizePolicy1);

        gridLayout_2->addWidget(label_25, 0, 0, 1, 1);

        CapA = new QDoubleSpinBox(frame);
        CapA->setObjectName(QString::fromUtf8("CapA"));
        QSizePolicy sizePolicy2(QSizePolicy::Minimum, QSizePolicy::Fixed);
        sizePolicy2.setHorizontalStretch(0);
        sizePolicy2.setVerticalStretch(0);
        sizePolicy2.setHeightForWidth(CapA->sizePolicy().hasHeightForWidth());
        CapA->setSizePolicy(sizePolicy2);
        CapA->setMinimum(1.000000000000000);
        CapA->setMaximum(999.990000000000009);
        CapA->setValue(50.000000000000000);

        gridLayout_2->addWidget(CapA, 0, 1, 1, 1);

        CapB = new QDoubleSpinBox(frame);
        CapB->setObjectName(QString::fromUtf8("CapB"));
        sizePolicy2.setHeightForWidth(CapB->sizePolicy().hasHeightForWidth());
        CapB->setSizePolicy(sizePolicy2);
        CapB->setMinimum(1.000000000000000);
        CapB->setMaximum(999.990000000000009);
        CapB->setValue(70.000000000000000);

        gridLayout_2->addWidget(CapB, 1, 1, 1, 1);

        label_26 = new QLabel(frame);
        label_26->setObjectName(QString::fromUtf8("label_26"));

        gridLayout_2->addWidget(label_26, 1, 0, 1, 1);

        label_27 = new QLabel(frame);
        label_27->setObjectName(QString::fromUtf8("label_27"));

        gridLayout_2->addWidget(label_27, 2, 0, 1, 1);

        CapC = new QDoubleSpinBox(frame);
        CapC->setObjectName(QString::fromUtf8("CapC"));
        sizePolicy2.setHeightForWidth(CapC->sizePolicy().hasHeightForWidth());
        CapC->setSizePolicy(sizePolicy2);
        CapC->setMinimum(1.000000000000000);
        CapC->setMaximum(999.990000000000009);
        CapC->setValue(92.500000000000000);

        gridLayout_2->addWidget(CapC, 2, 1, 1, 1);

        horizontalSpacer = new QSpacerItem(40, 20, QSizePolicy::Expanding, QSizePolicy::Minimum);

        gridLayout_2->addItem(horizontalSpacer, 0, 2, 1, 1);


        verticalLayout_3->addLayout(gridLayout_2);


        verticalLayout->addWidget(frame);

        frame_2 = new QFrame(CapEditForm);
        frame_2->setObjectName(QString::fromUtf8("frame_2"));
        frame_2->setFrameShape(QFrame::StyledPanel);
        frame_2->setFrameShadow(QFrame::Raised);
        verticalLayout_2 = new QVBoxLayout(frame_2);
        verticalLayout_2->setObjectName(QString::fromUtf8("verticalLayout_2"));
        label_2 = new QLabel(frame_2);
        label_2->setObjectName(QString::fromUtf8("label_2"));
        QFont font;
        font.setBold(true);
        font.setWeight(75);
        label_2->setFont(font);

        verticalLayout_2->addWidget(label_2);

        horizontalLayout_3 = new QHBoxLayout();
        horizontalLayout_3->setObjectName(QString::fromUtf8("horizontalLayout_3"));
        CapLeds = new QRadioButton(frame_2);
        CapLeds->setObjectName(QString::fromUtf8("CapLeds"));
        QSizePolicy sizePolicy3(QSizePolicy::Preferred, QSizePolicy::Fixed);
        sizePolicy3.setHorizontalStretch(0);
        sizePolicy3.setVerticalStretch(0);
        sizePolicy3.setHeightForWidth(CapLeds->sizePolicy().hasHeightForWidth());
        CapLeds->setSizePolicy(sizePolicy3);
        CapLeds->setChecked(true);

        horizontalLayout_3->addWidget(CapLeds);

        ClipReflective = new QRadioButton(frame_2);
        ClipReflective->setObjectName(QString::fromUtf8("ClipReflective"));

        horizontalLayout_3->addWidget(ClipReflective);


        verticalLayout_2->addLayout(horizontalLayout_3);


        verticalLayout->addWidget(frame_2);

        verticalSpacer_5 = new QSpacerItem(0, 0, QSizePolicy::Minimum, QSizePolicy::Expanding);

        verticalLayout->addItem(verticalSpacer_5);


        horizontalLayout->addLayout(verticalLayout);


        verticalLayout_4->addLayout(horizontalLayout);

        verticalSpacer = new QSpacerItem(20, 8, QSizePolicy::Minimum, QSizePolicy::Expanding);

        verticalLayout_4->addItem(verticalSpacer);


        retranslateUi(CapEditForm);

        QMetaObject::connectSlotsByName(CapEditForm);
    } // setupUi

    void retranslateUi(QWidget *CapEditForm)
    {
        CapEditForm->setWindowTitle(QCoreApplication::translate("CapEditForm", "Form", nullptr));
        label_24->setText(QString());
        label->setText(QCoreApplication::translate("CapEditForm", "<!DOCTYPE HTML PUBLIC \"-//W3C//DTD HTML 4.0//EN\" \"http://www.w3.org/TR/REC-html40/strict.dtd\">\n"
"<html><head><meta name=\"qrichtext\" content=\"1\" /><style type=\"text/css\">\n"
"p, li { white-space: pre-wrap; }\n"
"</style></head><body style=\" font-family:'Sans'; font-size:10pt; font-weight:400; font-style:normal;\">\n"
"<p style=\" margin-top:0px; margin-bottom:0px; margin-left:0px; margin-right:0px; -qt-block-indent:0; text-indent:0px;\"><span style=\" font-weight:600;\">Model Dimmensions</span></p></body></html>", nullptr));
        label_25->setText(QCoreApplication::translate("CapEditForm", "A [mm]", nullptr));
        label_26->setText(QCoreApplication::translate("CapEditForm", "B [mm]", nullptr));
        label_27->setText(QCoreApplication::translate("CapEditForm", "C [mm]", nullptr));
        label_2->setText(QCoreApplication::translate("CapEditForm", "Model Type", nullptr));
        CapLeds->setText(QCoreApplication::translate("CapEditForm", "LEDs", nullptr));
        ClipReflective->setText(QCoreApplication::translate("CapEditForm", "Reflective", nullptr));
    } // retranslateUi

};

namespace Ui {
    class CapEditForm: public Ui_CapEditForm {};
} // namespace Ui

QT_END_NAMESPACE

#endif // UI_CAP_EDIT_H
