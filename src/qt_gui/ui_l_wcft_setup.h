/********************************************************************************
** Form generated from reading UI file 'l_wcft_setup.ui'
**
** Created by: Qt User Interface Compiler version 5.15.17
**
** WARNING! All changes made in this file will be lost when recompiling UI file!
********************************************************************************/

#ifndef UI_L_WCFT_SETUP_H
#define UI_L_WCFT_SETUP_H

#include <QtCore/QVariant>
#include <QtWidgets/QApplication>
#include <QtWidgets/QComboBox>
#include <QtWidgets/QFrame>
#include <QtWidgets/QGridLayout>
#include <QtWidgets/QHBoxLayout>
#include <QtWidgets/QLabel>
#include <QtWidgets/QLineEdit>
#include <QtWidgets/QPushButton>
#include <QtWidgets/QSlider>
#include <QtWidgets/QSpacerItem>
#include <QtWidgets/QVBoxLayout>
#include <QtWidgets/QWidget>

QT_BEGIN_NAMESPACE

class Ui_WebcamFtSetupForm
{
public:
    QVBoxLayout *verticalLayout_2;
    QFrame *frame;
    QVBoxLayout *verticalLayout_6;
    QLabel *label_3;
    QGridLayout *gridLayout_2;
    QLabel *label_61;
    QComboBox *WebcamFtFormats;
    QLabel *label_31;
    QComboBox *WebcamFtResolutions;
    QFrame *frame_2;
    QVBoxLayout *verticalLayout_5;
    QLabel *label_6;
    QVBoxLayout *verticalLayout;
    QLabel *label_4;
    QHBoxLayout *horizontalLayout;
    QLineEdit *CascadePath;
    QPushButton *FindCascade;
    QGridLayout *gridLayout;
    QLabel *label_7;
    QSlider *OptimLevel;
    QLabel *label_10;
    QSlider *ExpFilterFactor;
    QLabel *label_5;
    QLabel *label_2;
    QLabel *label_9;
    QLabel *label;
    QSpacerItem *verticalSpacer;

    void setupUi(QWidget *WebcamFtSetupForm)
    {
        if (WebcamFtSetupForm->objectName().isEmpty())
            WebcamFtSetupForm->setObjectName(QString::fromUtf8("WebcamFtSetupForm"));
        WebcamFtSetupForm->resize(483, 298);
        verticalLayout_2 = new QVBoxLayout(WebcamFtSetupForm);
        verticalLayout_2->setObjectName(QString::fromUtf8("verticalLayout_2"));
        frame = new QFrame(WebcamFtSetupForm);
        frame->setObjectName(QString::fromUtf8("frame"));
        frame->setFrameShape(QFrame::StyledPanel);
        frame->setFrameShadow(QFrame::Raised);
        verticalLayout_6 = new QVBoxLayout(frame);
        verticalLayout_6->setObjectName(QString::fromUtf8("verticalLayout_6"));
        label_3 = new QLabel(frame);
        label_3->setObjectName(QString::fromUtf8("label_3"));
        QFont font;
        font.setBold(true);
        font.setWeight(75);
        label_3->setFont(font);
        label_3->setAlignment(Qt::AlignLeading|Qt::AlignLeft|Qt::AlignVCenter);

        verticalLayout_6->addWidget(label_3);

        gridLayout_2 = new QGridLayout();
        gridLayout_2->setObjectName(QString::fromUtf8("gridLayout_2"));
        label_61 = new QLabel(frame);
        label_61->setObjectName(QString::fromUtf8("label_61"));
        QSizePolicy sizePolicy(QSizePolicy::Maximum, QSizePolicy::Preferred);
        sizePolicy.setHorizontalStretch(0);
        sizePolicy.setVerticalStretch(0);
        sizePolicy.setHeightForWidth(label_61->sizePolicy().hasHeightForWidth());
        label_61->setSizePolicy(sizePolicy);

        gridLayout_2->addWidget(label_61, 0, 0, 1, 1);

        WebcamFtFormats = new QComboBox(frame);
        WebcamFtFormats->setObjectName(QString::fromUtf8("WebcamFtFormats"));
        QSizePolicy sizePolicy1(QSizePolicy::Preferred, QSizePolicy::Preferred);
        sizePolicy1.setHorizontalStretch(0);
        sizePolicy1.setVerticalStretch(0);
        sizePolicy1.setHeightForWidth(WebcamFtFormats->sizePolicy().hasHeightForWidth());
        WebcamFtFormats->setSizePolicy(sizePolicy1);

        gridLayout_2->addWidget(WebcamFtFormats, 0, 1, 1, 1);

        label_31 = new QLabel(frame);
        label_31->setObjectName(QString::fromUtf8("label_31"));
        sizePolicy1.setHeightForWidth(label_31->sizePolicy().hasHeightForWidth());
        label_31->setSizePolicy(sizePolicy1);

        gridLayout_2->addWidget(label_31, 1, 0, 1, 1);

        WebcamFtResolutions = new QComboBox(frame);
        WebcamFtResolutions->setObjectName(QString::fromUtf8("WebcamFtResolutions"));
        sizePolicy1.setHeightForWidth(WebcamFtResolutions->sizePolicy().hasHeightForWidth());
        WebcamFtResolutions->setSizePolicy(sizePolicy1);

        gridLayout_2->addWidget(WebcamFtResolutions, 1, 1, 1, 1);


        verticalLayout_6->addLayout(gridLayout_2);


        verticalLayout_2->addWidget(frame);

        frame_2 = new QFrame(WebcamFtSetupForm);
        frame_2->setObjectName(QString::fromUtf8("frame_2"));
        frame_2->setFrameShape(QFrame::StyledPanel);
        frame_2->setFrameShadow(QFrame::Raised);
        verticalLayout_5 = new QVBoxLayout(frame_2);
        verticalLayout_5->setObjectName(QString::fromUtf8("verticalLayout_5"));
        label_6 = new QLabel(frame_2);
        label_6->setObjectName(QString::fromUtf8("label_6"));
        QSizePolicy sizePolicy2(QSizePolicy::Preferred, QSizePolicy::Maximum);
        sizePolicy2.setHorizontalStretch(0);
        sizePolicy2.setVerticalStretch(0);
        sizePolicy2.setHeightForWidth(label_6->sizePolicy().hasHeightForWidth());
        label_6->setSizePolicy(sizePolicy2);
        label_6->setFont(font);
        label_6->setAlignment(Qt::AlignLeading|Qt::AlignLeft|Qt::AlignVCenter);

        verticalLayout_5->addWidget(label_6);

        verticalLayout = new QVBoxLayout();
        verticalLayout->setObjectName(QString::fromUtf8("verticalLayout"));
        label_4 = new QLabel(frame_2);
        label_4->setObjectName(QString::fromUtf8("label_4"));
        QFont font1;
        font1.setItalic(false);
        font1.setUnderline(false);
        font1.setStrikeOut(false);
        label_4->setFont(font1);

        verticalLayout->addWidget(label_4);

        horizontalLayout = new QHBoxLayout();
        horizontalLayout->setObjectName(QString::fromUtf8("horizontalLayout"));
        CascadePath = new QLineEdit(frame_2);
        CascadePath->setObjectName(QString::fromUtf8("CascadePath"));

        horizontalLayout->addWidget(CascadePath);

        FindCascade = new QPushButton(frame_2);
        FindCascade->setObjectName(QString::fromUtf8("FindCascade"));

        horizontalLayout->addWidget(FindCascade);


        verticalLayout->addLayout(horizontalLayout);


        verticalLayout_5->addLayout(verticalLayout);

        gridLayout = new QGridLayout();
        gridLayout->setObjectName(QString::fromUtf8("gridLayout"));
        label_7 = new QLabel(frame_2);
        label_7->setObjectName(QString::fromUtf8("label_7"));

        gridLayout->addWidget(label_7, 0, 1, 1, 1);

        OptimLevel = new QSlider(frame_2);
        OptimLevel->setObjectName(QString::fromUtf8("OptimLevel"));
        OptimLevel->setMaximum(3);
        OptimLevel->setPageStep(1);
        OptimLevel->setValue(0);
        OptimLevel->setOrientation(Qt::Horizontal);
        OptimLevel->setTickPosition(QSlider::TicksBelow);

        gridLayout->addWidget(OptimLevel, 0, 2, 1, 1);

        label_10 = new QLabel(frame_2);
        label_10->setObjectName(QString::fromUtf8("label_10"));

        gridLayout->addWidget(label_10, 0, 0, 1, 1);

        ExpFilterFactor = new QSlider(frame_2);
        ExpFilterFactor->setObjectName(QString::fromUtf8("ExpFilterFactor"));
        ExpFilterFactor->setOrientation(Qt::Horizontal);
        ExpFilterFactor->setTickPosition(QSlider::TicksBelow);
        ExpFilterFactor->setTickInterval(20);

        gridLayout->addWidget(ExpFilterFactor, 1, 2, 1, 1);

        label_5 = new QLabel(frame_2);
        label_5->setObjectName(QString::fromUtf8("label_5"));

        gridLayout->addWidget(label_5, 1, 0, 1, 1);

        label_2 = new QLabel(frame_2);
        label_2->setObjectName(QString::fromUtf8("label_2"));
        label_2->setAlignment(Qt::AlignRight|Qt::AlignTrailing|Qt::AlignVCenter);

        gridLayout->addWidget(label_2, 1, 1, 1, 1);

        label_9 = new QLabel(frame_2);
        label_9->setObjectName(QString::fromUtf8("label_9"));

        gridLayout->addWidget(label_9, 0, 3, 1, 1);

        label = new QLabel(frame_2);
        label->setObjectName(QString::fromUtf8("label"));

        gridLayout->addWidget(label, 1, 3, 1, 1);


        verticalLayout_5->addLayout(gridLayout);


        verticalLayout_2->addWidget(frame_2);

        verticalSpacer = new QSpacerItem(20, 40, QSizePolicy::Minimum, QSizePolicy::Expanding);

        verticalLayout_2->addItem(verticalSpacer);


        retranslateUi(WebcamFtSetupForm);

        QMetaObject::connectSlotsByName(WebcamFtSetupForm);
    } // setupUi

    void retranslateUi(QWidget *WebcamFtSetupForm)
    {
        WebcamFtSetupForm->setWindowTitle(QCoreApplication::translate("WebcamFtSetupForm", "Form", nullptr));
        label_3->setText(QCoreApplication::translate("WebcamFtSetupForm", "Webcam Setup", nullptr));
        label_61->setText(QCoreApplication::translate("WebcamFtSetupForm", "Pixel Format", nullptr));
        label_31->setText(QCoreApplication::translate("WebcamFtSetupForm", "Resolution & Framerate", nullptr));
        label_6->setText(QCoreApplication::translate("WebcamFtSetupForm", "Facetracker Setup", nullptr));
        label_4->setText(QCoreApplication::translate("WebcamFtSetupForm", "Cascade:", nullptr));
        FindCascade->setText(QCoreApplication::translate("WebcamFtSetupForm", "Browse", nullptr));
        label_7->setText(QCoreApplication::translate("WebcamFtSetupForm", "Precision", nullptr));
        label_10->setText(QCoreApplication::translate("WebcamFtSetupForm", "Optimize for:", nullptr));
        label_5->setText(QCoreApplication::translate("WebcamFtSetupForm", "Smoothing:", nullptr));
        label_2->setText(QCoreApplication::translate("WebcamFtSetupForm", "None", nullptr));
        label_9->setText(QCoreApplication::translate("WebcamFtSetupForm", "Speed", nullptr));
        label->setText(QCoreApplication::translate("WebcamFtSetupForm", "Max", nullptr));
    } // retranslateUi

};

namespace Ui {
    class WebcamFtSetupForm: public Ui_WebcamFtSetupForm {};
} // namespace Ui

QT_END_NAMESPACE

#endif // UI_L_WCFT_SETUP_H
