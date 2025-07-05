/********************************************************************************
** Form generated from reading UI file 'scurve.ui'
**
** Created by: Qt User Interface Compiler version 5.15.8
**
** WARNING! All changes made in this file will be lost when recompiling UI file!
********************************************************************************/

#ifndef UI_SCURVE_H
#define UI_SCURVE_H

#include <QtCore/QVariant>
#include <QtWidgets/QApplication>
#include <QtWidgets/QCheckBox>
#include <QtWidgets/QDoubleSpinBox>
#include <QtWidgets/QFrame>
#include <QtWidgets/QGridLayout>
#include <QtWidgets/QHBoxLayout>
#include <QtWidgets/QLabel>
#include <QtWidgets/QSlider>
#include <QtWidgets/QVBoxLayout>
#include <QtWidgets/QWidget>

QT_BEGIN_NAMESPACE

class Ui_SCurveForm
{
public:
    QVBoxLayout *verticalLayout_4;
    QLabel *SCTitle;
    QHBoxLayout *horizontalLayout_2;
    QVBoxLayout *SCView;
    QVBoxLayout *verticalLayout_3;
    QFrame *frame_2;
    QVBoxLayout *verticalLayout_2;
    QLabel *SCLeftLabel;
    QGridLayout *gridLayout_2;
    QLabel *label_2;
    QDoubleSpinBox *SCLeftLimit;
    QLabel *SCCurvL;
    QSlider *SCLeftCurv;
    QFrame *frame;
    QVBoxLayout *verticalLayout;
    QHBoxLayout *horizontalLayout_3;
    QLabel *SCRightLabel;
    QCheckBox *SCSymetrical;
    QGridLayout *gridLayout_3;
    QDoubleSpinBox *SCRightLimit;
    QLabel *label_3;
    QLabel *SCCurvR;
    QSlider *SCRightCurv;
    QFrame *frame_3;
    QHBoxLayout *horizontalLayout;
    QGridLayout *gridLayout;
    QLabel *label_4;
    QLabel *label_5;
    QLabel *label_9;
    QLabel *label;
    QSlider *SCFactor;
    QLabel *label_6;
    QSlider *SCDeadZone;
    QSlider *SCFilterSlider;
    QLabel *label_7;
    QLabel *label_8;
    QLabel *label_10;
    QLabel *label_11;

    void setupUi(QWidget *SCurveForm)
    {
        if (SCurveForm->objectName().isEmpty())
            SCurveForm->setObjectName(QString::fromUtf8("SCurveForm"));
        SCurveForm->resize(720, 420);
        verticalLayout_4 = new QVBoxLayout(SCurveForm);
        verticalLayout_4->setObjectName(QString::fromUtf8("verticalLayout_4"));
        SCTitle = new QLabel(SCurveForm);
        SCTitle->setObjectName(QString::fromUtf8("SCTitle"));
        QSizePolicy sizePolicy(QSizePolicy::Preferred, QSizePolicy::Maximum);
        sizePolicy.setHorizontalStretch(0);
        sizePolicy.setVerticalStretch(0);
        sizePolicy.setHeightForWidth(SCTitle->sizePolicy().hasHeightForWidth());
        SCTitle->setSizePolicy(sizePolicy);
        QFont font;
        font.setPointSize(15);
        SCTitle->setFont(font);

        verticalLayout_4->addWidget(SCTitle);

        horizontalLayout_2 = new QHBoxLayout();
        horizontalLayout_2->setObjectName(QString::fromUtf8("horizontalLayout_2"));
        SCView = new QVBoxLayout();
        SCView->setObjectName(QString::fromUtf8("SCView"));

        horizontalLayout_2->addLayout(SCView);

        verticalLayout_3 = new QVBoxLayout();
        verticalLayout_3->setObjectName(QString::fromUtf8("verticalLayout_3"));
        frame_2 = new QFrame(SCurveForm);
        frame_2->setObjectName(QString::fromUtf8("frame_2"));
        sizePolicy.setHeightForWidth(frame_2->sizePolicy().hasHeightForWidth());
        frame_2->setSizePolicy(sizePolicy);
        frame_2->setFrameShape(QFrame::StyledPanel);
        frame_2->setFrameShadow(QFrame::Raised);
        verticalLayout_2 = new QVBoxLayout(frame_2);
        verticalLayout_2->setObjectName(QString::fromUtf8("verticalLayout_2"));
        SCLeftLabel = new QLabel(frame_2);
        SCLeftLabel->setObjectName(QString::fromUtf8("SCLeftLabel"));
        sizePolicy.setHeightForWidth(SCLeftLabel->sizePolicy().hasHeightForWidth());
        SCLeftLabel->setSizePolicy(sizePolicy);

        verticalLayout_2->addWidget(SCLeftLabel);

        gridLayout_2 = new QGridLayout();
        gridLayout_2->setObjectName(QString::fromUtf8("gridLayout_2"));
        label_2 = new QLabel(frame_2);
        label_2->setObjectName(QString::fromUtf8("label_2"));
        sizePolicy.setHeightForWidth(label_2->sizePolicy().hasHeightForWidth());
        label_2->setSizePolicy(sizePolicy);

        gridLayout_2->addWidget(label_2, 0, 0, 1, 1);

        SCLeftLimit = new QDoubleSpinBox(frame_2);
        SCLeftLimit->setObjectName(QString::fromUtf8("SCLeftLimit"));
        QSizePolicy sizePolicy1(QSizePolicy::Minimum, QSizePolicy::Maximum);
        sizePolicy1.setHorizontalStretch(0);
        sizePolicy1.setVerticalStretch(0);
        sizePolicy1.setHeightForWidth(SCLeftLimit->sizePolicy().hasHeightForWidth());
        SCLeftLimit->setSizePolicy(sizePolicy1);
        SCLeftLimit->setDecimals(1);
        SCLeftLimit->setMinimum(1.000000000000000);
        SCLeftLimit->setMaximum(9999.899999999999636);
        SCLeftLimit->setValue(600.000000000000000);

        gridLayout_2->addWidget(SCLeftLimit, 0, 1, 1, 1);

        SCCurvL = new QLabel(frame_2);
        SCCurvL->setObjectName(QString::fromUtf8("SCCurvL"));
        sizePolicy.setHeightForWidth(SCCurvL->sizePolicy().hasHeightForWidth());
        SCCurvL->setSizePolicy(sizePolicy);

        gridLayout_2->addWidget(SCCurvL, 1, 0, 1, 1);

        SCLeftCurv = new QSlider(frame_2);
        SCLeftCurv->setObjectName(QString::fromUtf8("SCLeftCurv"));
        SCLeftCurv->setEnabled(true);
        QSizePolicy sizePolicy2(QSizePolicy::Expanding, QSizePolicy::Maximum);
        sizePolicy2.setHorizontalStretch(0);
        sizePolicy2.setVerticalStretch(0);
        sizePolicy2.setHeightForWidth(SCLeftCurv->sizePolicy().hasHeightForWidth());
        SCLeftCurv->setSizePolicy(sizePolicy2);
        SCLeftCurv->setMaximum(100);
        SCLeftCurv->setValue(50);
        SCLeftCurv->setOrientation(Qt::Horizontal);
        SCLeftCurv->setTickPosition(QSlider::TicksBelow);
        SCLeftCurv->setTickInterval(10);

        gridLayout_2->addWidget(SCLeftCurv, 1, 1, 1, 1);


        verticalLayout_2->addLayout(gridLayout_2);


        verticalLayout_3->addWidget(frame_2);

        frame = new QFrame(SCurveForm);
        frame->setObjectName(QString::fromUtf8("frame"));
        sizePolicy.setHeightForWidth(frame->sizePolicy().hasHeightForWidth());
        frame->setSizePolicy(sizePolicy);
        frame->setFrameShape(QFrame::StyledPanel);
        frame->setFrameShadow(QFrame::Raised);
        verticalLayout = new QVBoxLayout(frame);
        verticalLayout->setObjectName(QString::fromUtf8("verticalLayout"));
        horizontalLayout_3 = new QHBoxLayout();
        horizontalLayout_3->setObjectName(QString::fromUtf8("horizontalLayout_3"));
        SCRightLabel = new QLabel(frame);
        SCRightLabel->setObjectName(QString::fromUtf8("SCRightLabel"));
        QSizePolicy sizePolicy3(QSizePolicy::Maximum, QSizePolicy::Maximum);
        sizePolicy3.setHorizontalStretch(0);
        sizePolicy3.setVerticalStretch(0);
        sizePolicy3.setHeightForWidth(SCRightLabel->sizePolicy().hasHeightForWidth());
        SCRightLabel->setSizePolicy(sizePolicy3);

        horizontalLayout_3->addWidget(SCRightLabel);

        SCSymetrical = new QCheckBox(frame);
        SCSymetrical->setObjectName(QString::fromUtf8("SCSymetrical"));
        sizePolicy1.setHeightForWidth(SCSymetrical->sizePolicy().hasHeightForWidth());
        SCSymetrical->setSizePolicy(sizePolicy1);
        SCSymetrical->setChecked(true);

        horizontalLayout_3->addWidget(SCSymetrical);


        verticalLayout->addLayout(horizontalLayout_3);

        gridLayout_3 = new QGridLayout();
        gridLayout_3->setObjectName(QString::fromUtf8("gridLayout_3"));
        SCRightLimit = new QDoubleSpinBox(frame);
        SCRightLimit->setObjectName(QString::fromUtf8("SCRightLimit"));
        SCRightLimit->setEnabled(false);
        sizePolicy1.setHeightForWidth(SCRightLimit->sizePolicy().hasHeightForWidth());
        SCRightLimit->setSizePolicy(sizePolicy1);
        SCRightLimit->setDecimals(1);
        SCRightLimit->setMinimum(1.000000000000000);
        SCRightLimit->setMaximum(9999.899999999999636);
        SCRightLimit->setValue(601.000000000000000);

        gridLayout_3->addWidget(SCRightLimit, 0, 1, 1, 1);

        label_3 = new QLabel(frame);
        label_3->setObjectName(QString::fromUtf8("label_3"));
        sizePolicy.setHeightForWidth(label_3->sizePolicy().hasHeightForWidth());
        label_3->setSizePolicy(sizePolicy);

        gridLayout_3->addWidget(label_3, 0, 0, 1, 1);

        SCCurvR = new QLabel(frame);
        SCCurvR->setObjectName(QString::fromUtf8("SCCurvR"));
        sizePolicy.setHeightForWidth(SCCurvR->sizePolicy().hasHeightForWidth());
        SCCurvR->setSizePolicy(sizePolicy);

        gridLayout_3->addWidget(SCCurvR, 1, 0, 1, 1);

        SCRightCurv = new QSlider(frame);
        SCRightCurv->setObjectName(QString::fromUtf8("SCRightCurv"));
        SCRightCurv->setEnabled(false);
        sizePolicy2.setHeightForWidth(SCRightCurv->sizePolicy().hasHeightForWidth());
        SCRightCurv->setSizePolicy(sizePolicy2);
        SCRightCurv->setMaximum(100);
        SCRightCurv->setValue(50);
        SCRightCurv->setOrientation(Qt::Horizontal);
        SCRightCurv->setTickPosition(QSlider::TicksBelow);
        SCRightCurv->setTickInterval(10);

        gridLayout_3->addWidget(SCRightCurv, 1, 1, 1, 1);


        verticalLayout->addLayout(gridLayout_3);


        verticalLayout_3->addWidget(frame);


        horizontalLayout_2->addLayout(verticalLayout_3);


        verticalLayout_4->addLayout(horizontalLayout_2);

        frame_3 = new QFrame(SCurveForm);
        frame_3->setObjectName(QString::fromUtf8("frame_3"));
        frame_3->setFrameShape(QFrame::StyledPanel);
        frame_3->setFrameShadow(QFrame::Raised);
        horizontalLayout = new QHBoxLayout(frame_3);
        horizontalLayout->setObjectName(QString::fromUtf8("horizontalLayout"));
        gridLayout = new QGridLayout();
        gridLayout->setObjectName(QString::fromUtf8("gridLayout"));
        label_4 = new QLabel(frame_3);
        label_4->setObjectName(QString::fromUtf8("label_4"));

        gridLayout->addWidget(label_4, 1, 0, 1, 1);

        label_5 = new QLabel(frame_3);
        label_5->setObjectName(QString::fromUtf8("label_5"));

        gridLayout->addWidget(label_5, 2, 0, 1, 1);

        label_9 = new QLabel(frame_3);
        label_9->setObjectName(QString::fromUtf8("label_9"));
        sizePolicy.setHeightForWidth(label_9->sizePolicy().hasHeightForWidth());
        label_9->setSizePolicy(sizePolicy);

        gridLayout->addWidget(label_9, 0, 0, 1, 1);

        label = new QLabel(frame_3);
        label->setObjectName(QString::fromUtf8("label"));
        label->setAlignment(Qt::AlignRight|Qt::AlignTrailing|Qt::AlignVCenter);

        gridLayout->addWidget(label, 0, 1, 1, 1);

        SCFactor = new QSlider(frame_3);
        SCFactor->setObjectName(QString::fromUtf8("SCFactor"));
        SCFactor->setOrientation(Qt::Horizontal);
        SCFactor->setTickPosition(QSlider::TicksBelow);
        SCFactor->setTickInterval(10);

        gridLayout->addWidget(SCFactor, 0, 2, 1, 1);

        label_6 = new QLabel(frame_3);
        label_6->setObjectName(QString::fromUtf8("label_6"));

        gridLayout->addWidget(label_6, 0, 3, 1, 1);

        SCDeadZone = new QSlider(frame_3);
        SCDeadZone->setObjectName(QString::fromUtf8("SCDeadZone"));
        SCDeadZone->setMaximum(100);
        SCDeadZone->setOrientation(Qt::Horizontal);
        SCDeadZone->setTickPosition(QSlider::TicksBelow);
        SCDeadZone->setTickInterval(10);

        gridLayout->addWidget(SCDeadZone, 1, 2, 1, 1);

        SCFilterSlider = new QSlider(frame_3);
        SCFilterSlider->setObjectName(QString::fromUtf8("SCFilterSlider"));
        SCFilterSlider->setMinimum(0);
        SCFilterSlider->setMaximum(100);
        SCFilterSlider->setOrientation(Qt::Horizontal);
        SCFilterSlider->setTickPosition(QSlider::TicksBelow);
        SCFilterSlider->setTickInterval(10);

        gridLayout->addWidget(SCFilterSlider, 2, 2, 1, 1);

        label_7 = new QLabel(frame_3);
        label_7->setObjectName(QString::fromUtf8("label_7"));

        gridLayout->addWidget(label_7, 1, 1, 1, 1);

        label_8 = new QLabel(frame_3);
        label_8->setObjectName(QString::fromUtf8("label_8"));

        gridLayout->addWidget(label_8, 1, 3, 1, 1);

        label_10 = new QLabel(frame_3);
        label_10->setObjectName(QString::fromUtf8("label_10"));

        gridLayout->addWidget(label_10, 2, 1, 1, 1);

        label_11 = new QLabel(frame_3);
        label_11->setObjectName(QString::fromUtf8("label_11"));

        gridLayout->addWidget(label_11, 2, 3, 1, 1);


        horizontalLayout->addLayout(gridLayout);


        verticalLayout_4->addWidget(frame_3);


        retranslateUi(SCurveForm);

        QMetaObject::connectSlotsByName(SCurveForm);
    } // setupUi

    void retranslateUi(QWidget *SCurveForm)
    {
        SCurveForm->setWindowTitle(QCoreApplication::translate("SCurveForm", "Form", nullptr));
        SCTitle->setText(QCoreApplication::translate("SCurveForm", "Title", nullptr));
        SCLeftLabel->setText(QCoreApplication::translate("SCurveForm", "Left", nullptr));
        label_2->setText(QCoreApplication::translate("SCurveForm", "Limit", nullptr));
        SCCurvL->setText(QCoreApplication::translate("SCurveForm", "Curvature", nullptr));
        SCRightLabel->setText(QCoreApplication::translate("SCurveForm", "Right", nullptr));
        SCSymetrical->setText(QCoreApplication::translate("SCurveForm", "Symetrical", nullptr));
        label_3->setText(QCoreApplication::translate("SCurveForm", "Limit", nullptr));
        SCCurvR->setText(QCoreApplication::translate("SCurveForm", "Curvature", nullptr));
        label_4->setText(QCoreApplication::translate("SCurveForm", "Dead Zone:", nullptr));
        label_5->setText(QCoreApplication::translate("SCurveForm", "Smoothing:", nullptr));
        label_9->setText(QCoreApplication::translate("SCurveForm", "Sensitivity:", nullptr));
        label->setText(QCoreApplication::translate("SCurveForm", "Low", nullptr));
        label_6->setText(QCoreApplication::translate("SCurveForm", "High", nullptr));
        label_7->setText(QCoreApplication::translate("SCurveForm", "None", nullptr));
        label_8->setText(QCoreApplication::translate("SCurveForm", "Full", nullptr));
        label_10->setText(QCoreApplication::translate("SCurveForm", "None", nullptr));
        label_11->setText(QCoreApplication::translate("SCurveForm", "Max", nullptr));
    } // retranslateUi

};

namespace Ui {
    class SCurveForm: public Ui_SCurveForm {};
} // namespace Ui

QT_END_NAMESPACE

#endif // UI_SCURVE_H
