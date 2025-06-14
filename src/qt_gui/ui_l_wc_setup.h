/********************************************************************************
** Form generated from reading UI file 'l_wc_setup.ui'
**
** Created by: Qt User Interface Compiler version 5.15.17
**
** WARNING! All changes made in this file will be lost when recompiling UI file!
********************************************************************************/

#ifndef UI_L_WC_SETUP_H
#define UI_L_WC_SETUP_H

#include <QtCore/QVariant>
#include <QtWidgets/QApplication>
#include <QtWidgets/QComboBox>
#include <QtWidgets/QFrame>
#include <QtWidgets/QGridLayout>
#include <QtWidgets/QHBoxLayout>
#include <QtWidgets/QLabel>
#include <QtWidgets/QSlider>
#include <QtWidgets/QSpacerItem>
#include <QtWidgets/QSpinBox>
#include <QtWidgets/QVBoxLayout>
#include <QtWidgets/QWidget>

QT_BEGIN_NAMESPACE

class Ui_WebcamSetupForm
{
public:
    QVBoxLayout *verticalLayout_3;
    QFrame *frame;
    QVBoxLayout *verticalLayout_2;
    QLabel *label_4;
    QGridLayout *gridLayout_2;
    QLabel *label_3;
    QComboBox *WebcamResolutions;
    QLabel *label_15;
    QComboBox *WebcamFormats;
    QHBoxLayout *horizontalLayout;
    QLabel *label_2;
    QLabel *label_32;
    QSlider *WebcamThreshold;
    QLabel *label;
    QFrame *frame_2;
    QVBoxLayout *verticalLayout;
    QLabel *label_5;
    QHBoxLayout *horizontalLayout_2;
    QLabel *label_42;
    QSpinBox *WebcamMinBlob;
    QLabel *label_40;
    QSpinBox *WebcamMaxBlob;
    QLabel *label_41;
    QSpacerItem *horizontalSpacer;
    QSpacerItem *verticalSpacer;

    void setupUi(QWidget *WebcamSetupForm)
    {
        if (WebcamSetupForm->objectName().isEmpty())
            WebcamSetupForm->setObjectName(QString::fromUtf8("WebcamSetupForm"));
        WebcamSetupForm->resize(462, 241);
        verticalLayout_3 = new QVBoxLayout(WebcamSetupForm);
        verticalLayout_3->setObjectName(QString::fromUtf8("verticalLayout_3"));
        frame = new QFrame(WebcamSetupForm);
        frame->setObjectName(QString::fromUtf8("frame"));
        frame->setFrameShape(QFrame::StyledPanel);
        frame->setFrameShadow(QFrame::Raised);
        verticalLayout_2 = new QVBoxLayout(frame);
        verticalLayout_2->setObjectName(QString::fromUtf8("verticalLayout_2"));
        label_4 = new QLabel(frame);
        label_4->setObjectName(QString::fromUtf8("label_4"));
        QFont font;
        font.setBold(true);
        font.setWeight(75);
        label_4->setFont(font);

        verticalLayout_2->addWidget(label_4);

        gridLayout_2 = new QGridLayout();
        gridLayout_2->setObjectName(QString::fromUtf8("gridLayout_2"));
        label_3 = new QLabel(frame);
        label_3->setObjectName(QString::fromUtf8("label_3"));
        QSizePolicy sizePolicy(QSizePolicy::Maximum, QSizePolicy::Preferred);
        sizePolicy.setHorizontalStretch(0);
        sizePolicy.setVerticalStretch(0);
        sizePolicy.setHeightForWidth(label_3->sizePolicy().hasHeightForWidth());
        label_3->setSizePolicy(sizePolicy);

        gridLayout_2->addWidget(label_3, 0, 0, 1, 1);

        WebcamResolutions = new QComboBox(frame);
        WebcamResolutions->setObjectName(QString::fromUtf8("WebcamResolutions"));
        QSizePolicy sizePolicy1(QSizePolicy::Preferred, QSizePolicy::Preferred);
        sizePolicy1.setHorizontalStretch(0);
        sizePolicy1.setVerticalStretch(0);
        sizePolicy1.setHeightForWidth(WebcamResolutions->sizePolicy().hasHeightForWidth());
        WebcamResolutions->setSizePolicy(sizePolicy1);

        gridLayout_2->addWidget(WebcamResolutions, 1, 1, 1, 1);

        label_15 = new QLabel(frame);
        label_15->setObjectName(QString::fromUtf8("label_15"));
        QSizePolicy sizePolicy2(QSizePolicy::Preferred, QSizePolicy::Maximum);
        sizePolicy2.setHorizontalStretch(0);
        sizePolicy2.setVerticalStretch(0);
        sizePolicy2.setHeightForWidth(label_15->sizePolicy().hasHeightForWidth());
        label_15->setSizePolicy(sizePolicy2);

        gridLayout_2->addWidget(label_15, 1, 0, 1, 1);

        WebcamFormats = new QComboBox(frame);
        WebcamFormats->setObjectName(QString::fromUtf8("WebcamFormats"));

        gridLayout_2->addWidget(WebcamFormats, 0, 1, 1, 1);


        verticalLayout_2->addLayout(gridLayout_2);

        horizontalLayout = new QHBoxLayout();
        horizontalLayout->setObjectName(QString::fromUtf8("horizontalLayout"));
        label_2 = new QLabel(frame);
        label_2->setObjectName(QString::fromUtf8("label_2"));

        horizontalLayout->addWidget(label_2);

        label_32 = new QLabel(frame);
        label_32->setObjectName(QString::fromUtf8("label_32"));

        horizontalLayout->addWidget(label_32);

        WebcamThreshold = new QSlider(frame);
        WebcamThreshold->setObjectName(QString::fromUtf8("WebcamThreshold"));
        WebcamThreshold->setMaximum(255);
        WebcamThreshold->setValue(150);
        WebcamThreshold->setOrientation(Qt::Horizontal);
        WebcamThreshold->setTickPosition(QSlider::TicksBelow);
        WebcamThreshold->setTickInterval(32);

        horizontalLayout->addWidget(WebcamThreshold);

        label = new QLabel(frame);
        label->setObjectName(QString::fromUtf8("label"));

        horizontalLayout->addWidget(label);


        verticalLayout_2->addLayout(horizontalLayout);


        verticalLayout_3->addWidget(frame);

        frame_2 = new QFrame(WebcamSetupForm);
        frame_2->setObjectName(QString::fromUtf8("frame_2"));
        frame_2->setFrameShape(QFrame::StyledPanel);
        frame_2->setFrameShadow(QFrame::Raised);
        verticalLayout = new QVBoxLayout(frame_2);
        verticalLayout->setObjectName(QString::fromUtf8("verticalLayout"));
        label_5 = new QLabel(frame_2);
        label_5->setObjectName(QString::fromUtf8("label_5"));
        label_5->setFont(font);

        verticalLayout->addWidget(label_5);

        horizontalLayout_2 = new QHBoxLayout();
        horizontalLayout_2->setObjectName(QString::fromUtf8("horizontalLayout_2"));
        label_42 = new QLabel(frame_2);
        label_42->setObjectName(QString::fromUtf8("label_42"));
        sizePolicy.setHeightForWidth(label_42->sizePolicy().hasHeightForWidth());
        label_42->setSizePolicy(sizePolicy);

        horizontalLayout_2->addWidget(label_42);

        WebcamMinBlob = new QSpinBox(frame_2);
        WebcamMinBlob->setObjectName(QString::fromUtf8("WebcamMinBlob"));
        sizePolicy1.setHeightForWidth(WebcamMinBlob->sizePolicy().hasHeightForWidth());
        WebcamMinBlob->setSizePolicy(sizePolicy1);
        WebcamMinBlob->setAlignment(Qt::AlignRight|Qt::AlignTrailing|Qt::AlignVCenter);
        WebcamMinBlob->setMinimum(1);
        WebcamMinBlob->setMaximum(10000);
        WebcamMinBlob->setValue(4);

        horizontalLayout_2->addWidget(WebcamMinBlob);

        label_40 = new QLabel(frame_2);
        label_40->setObjectName(QString::fromUtf8("label_40"));
        sizePolicy.setHeightForWidth(label_40->sizePolicy().hasHeightForWidth());
        label_40->setSizePolicy(sizePolicy);

        horizontalLayout_2->addWidget(label_40);

        WebcamMaxBlob = new QSpinBox(frame_2);
        WebcamMaxBlob->setObjectName(QString::fromUtf8("WebcamMaxBlob"));
        sizePolicy1.setHeightForWidth(WebcamMaxBlob->sizePolicy().hasHeightForWidth());
        WebcamMaxBlob->setSizePolicy(sizePolicy1);
        WebcamMaxBlob->setAlignment(Qt::AlignRight|Qt::AlignTrailing|Qt::AlignVCenter);
        WebcamMaxBlob->setMinimum(1);
        WebcamMaxBlob->setMaximum(10000);
        WebcamMaxBlob->setValue(2500);

        horizontalLayout_2->addWidget(WebcamMaxBlob);

        label_41 = new QLabel(frame_2);
        label_41->setObjectName(QString::fromUtf8("label_41"));
        sizePolicy.setHeightForWidth(label_41->sizePolicy().hasHeightForWidth());
        label_41->setSizePolicy(sizePolicy);

        horizontalLayout_2->addWidget(label_41);

        horizontalSpacer = new QSpacerItem(40, 20, QSizePolicy::Expanding, QSizePolicy::Minimum);

        horizontalLayout_2->addItem(horizontalSpacer);


        verticalLayout->addLayout(horizontalLayout_2);


        verticalLayout_3->addWidget(frame_2);

        verticalSpacer = new QSpacerItem(20, 40, QSizePolicy::Minimum, QSizePolicy::Expanding);

        verticalLayout_3->addItem(verticalSpacer);

        QWidget::setTabOrder(WebcamFormats, WebcamResolutions);
        QWidget::setTabOrder(WebcamResolutions, WebcamThreshold);
        QWidget::setTabOrder(WebcamThreshold, WebcamMinBlob);
        QWidget::setTabOrder(WebcamMinBlob, WebcamMaxBlob);

        retranslateUi(WebcamSetupForm);

        QMetaObject::connectSlotsByName(WebcamSetupForm);
    } // setupUi

    void retranslateUi(QWidget *WebcamSetupForm)
    {
        WebcamSetupForm->setProperty("WindowTitle", QVariant(QCoreApplication::translate("WebcamSetupForm", "WebcamSetupForm", nullptr)));
        label_4->setText(QCoreApplication::translate("WebcamSetupForm", "Webcam Setup", nullptr));
        label_3->setText(QCoreApplication::translate("WebcamSetupForm", "Pixel Format", nullptr));
        label_15->setText(QCoreApplication::translate("WebcamSetupForm", "Resolution & Framerate", nullptr));
        label_2->setText(QCoreApplication::translate("WebcamSetupForm", "Threshold:", nullptr));
        label_32->setText(QCoreApplication::translate("WebcamSetupForm", "0", nullptr));
        label->setText(QCoreApplication::translate("WebcamSetupForm", "255", nullptr));
        label_5->setText(QCoreApplication::translate("WebcamSetupForm", "Troubleshooting", nullptr));
        label_42->setText(QCoreApplication::translate("WebcamSetupForm", "Valid Blob Size:", nullptr));
        label_40->setText(QCoreApplication::translate("WebcamSetupForm", "to", nullptr));
        label_41->setText(QCoreApplication::translate("WebcamSetupForm", "pixels", nullptr));
    } // retranslateUi

};

namespace Ui {
    class WebcamSetupForm: public Ui_WebcamSetupForm {};
} // namespace Ui

QT_END_NAMESPACE

#endif // UI_L_WC_SETUP_H
