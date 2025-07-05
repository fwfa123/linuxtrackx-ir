/********************************************************************************
** Form generated from reading UI file 'tir_setup.ui'
**
** Created by: Qt User Interface Compiler version 5.15.8
**
** WARNING! All changes made in this file will be lost when recompiling UI file!
********************************************************************************/

#ifndef UI_TIR_SETUP_H
#define UI_TIR_SETUP_H

#include <QtCore/QVariant>
#include <QtWidgets/QApplication>
#include <QtWidgets/QCheckBox>
#include <QtWidgets/QFrame>
#include <QtWidgets/QGridLayout>
#include <QtWidgets/QHBoxLayout>
#include <QtWidgets/QLabel>
#include <QtWidgets/QPushButton>
#include <QtWidgets/QSlider>
#include <QtWidgets/QSpacerItem>
#include <QtWidgets/QSpinBox>
#include <QtWidgets/QVBoxLayout>
#include <QtWidgets/QWidget>

QT_BEGIN_NAMESPACE

class Ui_TirSetupForm
{
public:
    QVBoxLayout *verticalLayout_4;
    QFrame *frame;
    QVBoxLayout *verticalLayout;
    QLabel *label_3;
    QGridLayout *gridLayout;
    QSlider *TirStatusBright;
    QLabel *IRBrightLabel;
    QLabel *label_47;
    QLabel *IRBrightLabelLow;
    QLabel *StatusBrightLabel;
    QLabel *IRBrightLabelHigh;
    QLabel *StatusBrightLabelBright;
    QSlider *TirIrBright;
    QLabel *StatusBrightLabelOff;
    QLabel *TirFwLabel;
    QPushButton *TirInstallFirmware;
    QCheckBox *TirSignalizeStatus;
    QFrame *frame_2;
    QVBoxLayout *verticalLayout_2;
    QLabel *label_4;
    QHBoxLayout *horizontalLayout_3;
    QVBoxLayout *verticalLayout_3;
    QLabel *Threshold_label;
    QLabel *TirUseGrayscaleLabel;
    QLabel *label_7;
    QGridLayout *gridLayout_3;
    QCheckBox *TirUseGrayscale;
    QHBoxLayout *horizontalLayout;
    QSpinBox *TirMinBlob;
    QLabel *label_43;
    QSpinBox *TirMaxBlob;
    QLabel *label_44;
    QSpacerItem *horizontalSpacer_3;
    QHBoxLayout *horizontalLayout_2;
    QLabel *TirThresholdMin;
    QSlider *TirThreshold;
    QLabel *TirThresholdMax;
    QSpacerItem *verticalSpacer;

    void setupUi(QWidget *TirSetupForm)
    {
        if (TirSetupForm->objectName().isEmpty())
            TirSetupForm->setObjectName(QString::fromUtf8("TirSetupForm"));
        TirSetupForm->resize(478, 358);
        verticalLayout_4 = new QVBoxLayout(TirSetupForm);
        verticalLayout_4->setObjectName(QString::fromUtf8("verticalLayout_4"));
        frame = new QFrame(TirSetupForm);
        frame->setObjectName(QString::fromUtf8("frame"));
        frame->setFrameShape(QFrame::StyledPanel);
        frame->setFrameShadow(QFrame::Raised);
        verticalLayout = new QVBoxLayout(frame);
        verticalLayout->setObjectName(QString::fromUtf8("verticalLayout"));
        label_3 = new QLabel(frame);
        label_3->setObjectName(QString::fromUtf8("label_3"));
        QFont font;
        font.setBold(true);
        font.setWeight(75);
        label_3->setFont(font);

        verticalLayout->addWidget(label_3);

        gridLayout = new QGridLayout();
        gridLayout->setObjectName(QString::fromUtf8("gridLayout"));
        TirStatusBright = new QSlider(frame);
        TirStatusBright->setObjectName(QString::fromUtf8("TirStatusBright"));
        TirStatusBright->setMaximum(3);
        TirStatusBright->setPageStep(1);
        TirStatusBright->setOrientation(Qt::Horizontal);
        TirStatusBright->setInvertedAppearance(true);
        TirStatusBright->setInvertedControls(false);
        TirStatusBright->setTickPosition(QSlider::TicksBelow);

        gridLayout->addWidget(TirStatusBright, 2, 2, 1, 1);

        IRBrightLabel = new QLabel(frame);
        IRBrightLabel->setObjectName(QString::fromUtf8("IRBrightLabel"));

        gridLayout->addWidget(IRBrightLabel, 3, 0, 1, 1);

        label_47 = new QLabel(frame);
        label_47->setObjectName(QString::fromUtf8("label_47"));
        QSizePolicy sizePolicy(QSizePolicy::Maximum, QSizePolicy::Preferred);
        sizePolicy.setHorizontalStretch(0);
        sizePolicy.setVerticalStretch(0);
        sizePolicy.setHeightForWidth(label_47->sizePolicy().hasHeightForWidth());
        label_47->setSizePolicy(sizePolicy);

        gridLayout->addWidget(label_47, 1, 0, 1, 1);

        IRBrightLabelLow = new QLabel(frame);
        IRBrightLabelLow->setObjectName(QString::fromUtf8("IRBrightLabelLow"));
        IRBrightLabelLow->setLayoutDirection(Qt::RightToLeft);

        gridLayout->addWidget(IRBrightLabelLow, 3, 1, 1, 1);

        StatusBrightLabel = new QLabel(frame);
        StatusBrightLabel->setObjectName(QString::fromUtf8("StatusBrightLabel"));

        gridLayout->addWidget(StatusBrightLabel, 2, 0, 1, 1);

        IRBrightLabelHigh = new QLabel(frame);
        IRBrightLabelHigh->setObjectName(QString::fromUtf8("IRBrightLabelHigh"));

        gridLayout->addWidget(IRBrightLabelHigh, 3, 3, 1, 1);

        StatusBrightLabelBright = new QLabel(frame);
        StatusBrightLabelBright->setObjectName(QString::fromUtf8("StatusBrightLabelBright"));

        gridLayout->addWidget(StatusBrightLabelBright, 2, 3, 1, 1);

        TirIrBright = new QSlider(frame);
        TirIrBright->setObjectName(QString::fromUtf8("TirIrBright"));
        TirIrBright->setMinimum(5);
        TirIrBright->setMaximum(7);
        TirIrBright->setPageStep(1);
        TirIrBright->setValue(7);
        TirIrBright->setOrientation(Qt::Horizontal);
        TirIrBright->setInvertedAppearance(false);
        TirIrBright->setTickPosition(QSlider::TicksBelow);

        gridLayout->addWidget(TirIrBright, 3, 2, 1, 1);

        StatusBrightLabelOff = new QLabel(frame);
        StatusBrightLabelOff->setObjectName(QString::fromUtf8("StatusBrightLabelOff"));
        StatusBrightLabelOff->setAlignment(Qt::AlignRight|Qt::AlignTrailing|Qt::AlignVCenter);

        gridLayout->addWidget(StatusBrightLabelOff, 2, 1, 1, 1);

        TirFwLabel = new QLabel(frame);
        TirFwLabel->setObjectName(QString::fromUtf8("TirFwLabel"));

        gridLayout->addWidget(TirFwLabel, 0, 0, 1, 1);

        TirInstallFirmware = new QPushButton(frame);
        TirInstallFirmware->setObjectName(QString::fromUtf8("TirInstallFirmware"));
        QSizePolicy sizePolicy1(QSizePolicy::Preferred, QSizePolicy::Fixed);
        sizePolicy1.setHorizontalStretch(0);
        sizePolicy1.setVerticalStretch(0);
        sizePolicy1.setHeightForWidth(TirInstallFirmware->sizePolicy().hasHeightForWidth());
        TirInstallFirmware->setSizePolicy(sizePolicy1);

        gridLayout->addWidget(TirInstallFirmware, 0, 2, 1, 1);

        TirSignalizeStatus = new QCheckBox(frame);
        TirSignalizeStatus->setObjectName(QString::fromUtf8("TirSignalizeStatus"));

        gridLayout->addWidget(TirSignalizeStatus, 1, 2, 1, 1);


        verticalLayout->addLayout(gridLayout);


        verticalLayout_4->addWidget(frame);

        frame_2 = new QFrame(TirSetupForm);
        frame_2->setObjectName(QString::fromUtf8("frame_2"));
        frame_2->setFrameShape(QFrame::StyledPanel);
        frame_2->setFrameShadow(QFrame::Raised);
        verticalLayout_2 = new QVBoxLayout(frame_2);
        verticalLayout_2->setObjectName(QString::fromUtf8("verticalLayout_2"));
        label_4 = new QLabel(frame_2);
        label_4->setObjectName(QString::fromUtf8("label_4"));
        label_4->setFont(font);

        verticalLayout_2->addWidget(label_4);

        horizontalLayout_3 = new QHBoxLayout();
        horizontalLayout_3->setObjectName(QString::fromUtf8("horizontalLayout_3"));
        verticalLayout_3 = new QVBoxLayout();
        verticalLayout_3->setObjectName(QString::fromUtf8("verticalLayout_3"));
        Threshold_label = new QLabel(frame_2);
        Threshold_label->setObjectName(QString::fromUtf8("Threshold_label"));
        sizePolicy.setHeightForWidth(Threshold_label->sizePolicy().hasHeightForWidth());
        Threshold_label->setSizePolicy(sizePolicy);

        verticalLayout_3->addWidget(Threshold_label);

        TirUseGrayscaleLabel = new QLabel(frame_2);
        TirUseGrayscaleLabel->setObjectName(QString::fromUtf8("TirUseGrayscaleLabel"));

        verticalLayout_3->addWidget(TirUseGrayscaleLabel);

        label_7 = new QLabel(frame_2);
        label_7->setObjectName(QString::fromUtf8("label_7"));

        verticalLayout_3->addWidget(label_7);


        horizontalLayout_3->addLayout(verticalLayout_3);

        gridLayout_3 = new QGridLayout();
        gridLayout_3->setObjectName(QString::fromUtf8("gridLayout_3"));
        TirUseGrayscale = new QCheckBox(frame_2);
        TirUseGrayscale->setObjectName(QString::fromUtf8("TirUseGrayscale"));

        gridLayout_3->addWidget(TirUseGrayscale, 3, 2, 1, 1);

        horizontalLayout = new QHBoxLayout();
        horizontalLayout->setObjectName(QString::fromUtf8("horizontalLayout"));
        TirMinBlob = new QSpinBox(frame_2);
        TirMinBlob->setObjectName(QString::fromUtf8("TirMinBlob"));
        QSizePolicy sizePolicy2(QSizePolicy::Minimum, QSizePolicy::Preferred);
        sizePolicy2.setHorizontalStretch(0);
        sizePolicy2.setVerticalStretch(0);
        sizePolicy2.setHeightForWidth(TirMinBlob->sizePolicy().hasHeightForWidth());
        TirMinBlob->setSizePolicy(sizePolicy2);
        TirMinBlob->setAlignment(Qt::AlignRight|Qt::AlignTrailing|Qt::AlignVCenter);
        TirMinBlob->setMinimum(1);
        TirMinBlob->setMaximum(10000);
        TirMinBlob->setValue(15);

        horizontalLayout->addWidget(TirMinBlob);

        label_43 = new QLabel(frame_2);
        label_43->setObjectName(QString::fromUtf8("label_43"));

        horizontalLayout->addWidget(label_43);

        TirMaxBlob = new QSpinBox(frame_2);
        TirMaxBlob->setObjectName(QString::fromUtf8("TirMaxBlob"));
        sizePolicy2.setHeightForWidth(TirMaxBlob->sizePolicy().hasHeightForWidth());
        TirMaxBlob->setSizePolicy(sizePolicy2);
        TirMaxBlob->setAlignment(Qt::AlignRight|Qt::AlignTrailing|Qt::AlignVCenter);
        TirMaxBlob->setMinimum(1);
        TirMaxBlob->setMaximum(10000);
        TirMaxBlob->setValue(1024);

        horizontalLayout->addWidget(TirMaxBlob);

        label_44 = new QLabel(frame_2);
        label_44->setObjectName(QString::fromUtf8("label_44"));

        horizontalLayout->addWidget(label_44);

        horizontalSpacer_3 = new QSpacerItem(40, 20, QSizePolicy::Expanding, QSizePolicy::Minimum);

        horizontalLayout->addItem(horizontalSpacer_3);


        gridLayout_3->addLayout(horizontalLayout, 4, 2, 1, 1);

        horizontalLayout_2 = new QHBoxLayout();
        horizontalLayout_2->setObjectName(QString::fromUtf8("horizontalLayout_2"));
        TirThresholdMin = new QLabel(frame_2);
        TirThresholdMin->setObjectName(QString::fromUtf8("TirThresholdMin"));
        sizePolicy.setHeightForWidth(TirThresholdMin->sizePolicy().hasHeightForWidth());
        TirThresholdMin->setSizePolicy(sizePolicy);

        horizontalLayout_2->addWidget(TirThresholdMin);

        TirThreshold = new QSlider(frame_2);
        TirThreshold->setObjectName(QString::fromUtf8("TirThreshold"));
        QSizePolicy sizePolicy3(QSizePolicy::Preferred, QSizePolicy::Preferred);
        sizePolicy3.setHorizontalStretch(0);
        sizePolicy3.setVerticalStretch(0);
        sizePolicy3.setHeightForWidth(TirThreshold->sizePolicy().hasHeightForWidth());
        TirThreshold->setSizePolicy(sizePolicy3);
        TirThreshold->setMinimum(30);
        TirThreshold->setMaximum(253);
        TirThreshold->setValue(200);
        TirThreshold->setOrientation(Qt::Horizontal);
        TirThreshold->setTickPosition(QSlider::TicksBelow);
        TirThreshold->setTickInterval(64);

        horizontalLayout_2->addWidget(TirThreshold);

        TirThresholdMax = new QLabel(frame_2);
        TirThresholdMax->setObjectName(QString::fromUtf8("TirThresholdMax"));
        sizePolicy.setHeightForWidth(TirThresholdMax->sizePolicy().hasHeightForWidth());
        TirThresholdMax->setSizePolicy(sizePolicy);

        horizontalLayout_2->addWidget(TirThresholdMax);


        gridLayout_3->addLayout(horizontalLayout_2, 2, 2, 1, 1);


        horizontalLayout_3->addLayout(gridLayout_3);


        verticalLayout_2->addLayout(horizontalLayout_3);


        verticalLayout_4->addWidget(frame_2);

        verticalSpacer = new QSpacerItem(20, 40, QSizePolicy::Minimum, QSizePolicy::Expanding);

        verticalLayout_4->addItem(verticalSpacer);

        QWidget::setTabOrder(TirMinBlob, TirMaxBlob);

        retranslateUi(TirSetupForm);

        QMetaObject::connectSlotsByName(TirSetupForm);
    } // setupUi

    void retranslateUi(QWidget *TirSetupForm)
    {
        TirSetupForm->setWindowTitle(QCoreApplication::translate("TirSetupForm", "Form", nullptr));
        label_3->setText(QCoreApplication::translate("TirSetupForm", "TrackIR Setup", nullptr));
        IRBrightLabel->setText(QCoreApplication::translate("TirSetupForm", "IR LEDs brightness:", nullptr));
        label_47->setText(QCoreApplication::translate("TirSetupForm", "Status Signalization:", nullptr));
        IRBrightLabelLow->setText(QCoreApplication::translate("TirSetupForm", "low", nullptr));
        StatusBrightLabel->setText(QCoreApplication::translate("TirSetupForm", "Status LED brightness:", nullptr));
        IRBrightLabelHigh->setText(QCoreApplication::translate("TirSetupForm", "high", nullptr));
        StatusBrightLabelBright->setText(QCoreApplication::translate("TirSetupForm", "bright", nullptr));
        StatusBrightLabelOff->setText(QCoreApplication::translate("TirSetupForm", "off", nullptr));
        TirFwLabel->setText(QCoreApplication::translate("TirSetupForm", "TrackIr firmware path", nullptr));
        TirInstallFirmware->setText(QCoreApplication::translate("TirSetupForm", "Install Firmware", nullptr));
        TirSignalizeStatus->setText(QString());
        label_4->setText(QCoreApplication::translate("TirSetupForm", "Troubleshooting", nullptr));
        Threshold_label->setText(QCoreApplication::translate("TirSetupForm", "Threshold:", nullptr));
        TirUseGrayscaleLabel->setText(QCoreApplication::translate("TirSetupForm", "Grayscale mode:", nullptr));
        label_7->setText(QCoreApplication::translate("TirSetupForm", "Valid Blob Size:", nullptr));
        TirUseGrayscale->setText(QString());
        label_43->setText(QCoreApplication::translate("TirSetupForm", "to", nullptr));
        label_44->setText(QCoreApplication::translate("TirSetupForm", "pixels", nullptr));
        TirThresholdMin->setText(QCoreApplication::translate("TirSetupForm", "30", nullptr));
        TirThresholdMax->setText(QCoreApplication::translate("TirSetupForm", "253", nullptr));
    } // retranslateUi

};

namespace Ui {
    class TirSetupForm: public Ui_TirSetupForm {};
} // namespace Ui

QT_END_NAMESPACE

#endif // UI_TIR_SETUP_H
