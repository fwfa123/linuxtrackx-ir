/********************************************************************************
** Form generated from reading UI file 'wii_setup.ui'
**
** Created by: Qt User Interface Compiler version 5.15.8
**
** WARNING! All changes made in this file will be lost when recompiling UI file!
********************************************************************************/

#ifndef UI_WII_SETUP_H
#define UI_WII_SETUP_H

#include <QtCore/QVariant>
#include <QtWidgets/QApplication>
#include <QtWidgets/QCheckBox>
#include <QtWidgets/QFrame>
#include <QtWidgets/QGridLayout>
#include <QtWidgets/QLabel>
#include <QtWidgets/QSpacerItem>
#include <QtWidgets/QVBoxLayout>
#include <QtWidgets/QWidget>

QT_BEGIN_NAMESPACE

class Ui_WiiSetupForm
{
public:
    QVBoxLayout *verticalLayout_2;
    QFrame *frame;
    QVBoxLayout *verticalLayout;
    QLabel *label;
    QGridLayout *gridLayout;
    QLabel *label_16;
    QLabel *label_17;
    QCheckBox *Wii_r1;
    QCheckBox *Wii_p1;
    QCheckBox *Wii_r2;
    QCheckBox *Wii_p2;
    QCheckBox *Wii_r3;
    QCheckBox *Wii_p3;
    QCheckBox *Wii_r4;
    QCheckBox *Wii_p4;
    QLabel *label_18;
    QLabel *label_19;
    QLabel *label_20;
    QLabel *label_21;
    QSpacerItem *verticalSpacer_4;

    void setupUi(QWidget *WiiSetupForm)
    {
        if (WiiSetupForm->objectName().isEmpty())
            WiiSetupForm->setObjectName(QString::fromUtf8("WiiSetupForm"));
        WiiSetupForm->resize(411, 138);
        verticalLayout_2 = new QVBoxLayout(WiiSetupForm);
        verticalLayout_2->setObjectName(QString::fromUtf8("verticalLayout_2"));
        frame = new QFrame(WiiSetupForm);
        frame->setObjectName(QString::fromUtf8("frame"));
        frame->setFrameShape(QFrame::StyledPanel);
        frame->setFrameShadow(QFrame::Raised);
        verticalLayout = new QVBoxLayout(frame);
        verticalLayout->setObjectName(QString::fromUtf8("verticalLayout"));
        label = new QLabel(frame);
        label->setObjectName(QString::fromUtf8("label"));
        QFont font;
        font.setBold(true);
        font.setWeight(75);
        label->setFont(font);

        verticalLayout->addWidget(label);

        gridLayout = new QGridLayout();
        gridLayout->setObjectName(QString::fromUtf8("gridLayout"));
        label_16 = new QLabel(frame);
        label_16->setObjectName(QString::fromUtf8("label_16"));

        gridLayout->addWidget(label_16, 1, 0, 1, 1);

        label_17 = new QLabel(frame);
        label_17->setObjectName(QString::fromUtf8("label_17"));

        gridLayout->addWidget(label_17, 2, 0, 1, 1);

        Wii_r1 = new QCheckBox(frame);
        Wii_r1->setObjectName(QString::fromUtf8("Wii_r1"));

        gridLayout->addWidget(Wii_r1, 1, 1, 1, 1);

        Wii_p1 = new QCheckBox(frame);
        Wii_p1->setObjectName(QString::fromUtf8("Wii_p1"));

        gridLayout->addWidget(Wii_p1, 2, 1, 1, 1);

        Wii_r2 = new QCheckBox(frame);
        Wii_r2->setObjectName(QString::fromUtf8("Wii_r2"));

        gridLayout->addWidget(Wii_r2, 1, 2, 1, 1);

        Wii_p2 = new QCheckBox(frame);
        Wii_p2->setObjectName(QString::fromUtf8("Wii_p2"));

        gridLayout->addWidget(Wii_p2, 2, 2, 1, 1);

        Wii_r3 = new QCheckBox(frame);
        Wii_r3->setObjectName(QString::fromUtf8("Wii_r3"));

        gridLayout->addWidget(Wii_r3, 1, 3, 1, 1);

        Wii_p3 = new QCheckBox(frame);
        Wii_p3->setObjectName(QString::fromUtf8("Wii_p3"));

        gridLayout->addWidget(Wii_p3, 2, 3, 1, 1);

        Wii_r4 = new QCheckBox(frame);
        Wii_r4->setObjectName(QString::fromUtf8("Wii_r4"));

        gridLayout->addWidget(Wii_r4, 1, 4, 1, 1);

        Wii_p4 = new QCheckBox(frame);
        Wii_p4->setObjectName(QString::fromUtf8("Wii_p4"));

        gridLayout->addWidget(Wii_p4, 2, 4, 1, 1);

        label_18 = new QLabel(frame);
        label_18->setObjectName(QString::fromUtf8("label_18"));
        QSizePolicy sizePolicy(QSizePolicy::Preferred, QSizePolicy::Maximum);
        sizePolicy.setHorizontalStretch(0);
        sizePolicy.setVerticalStretch(0);
        sizePolicy.setHeightForWidth(label_18->sizePolicy().hasHeightForWidth());
        label_18->setSizePolicy(sizePolicy);

        gridLayout->addWidget(label_18, 0, 1, 1, 1);

        label_19 = new QLabel(frame);
        label_19->setObjectName(QString::fromUtf8("label_19"));
        sizePolicy.setHeightForWidth(label_19->sizePolicy().hasHeightForWidth());
        label_19->setSizePolicy(sizePolicy);

        gridLayout->addWidget(label_19, 0, 2, 1, 1);

        label_20 = new QLabel(frame);
        label_20->setObjectName(QString::fromUtf8("label_20"));
        sizePolicy.setHeightForWidth(label_20->sizePolicy().hasHeightForWidth());
        label_20->setSizePolicy(sizePolicy);

        gridLayout->addWidget(label_20, 0, 3, 1, 1);

        label_21 = new QLabel(frame);
        label_21->setObjectName(QString::fromUtf8("label_21"));
        sizePolicy.setHeightForWidth(label_21->sizePolicy().hasHeightForWidth());
        label_21->setSizePolicy(sizePolicy);

        gridLayout->addWidget(label_21, 0, 4, 1, 1);


        verticalLayout->addLayout(gridLayout);


        verticalLayout_2->addWidget(frame);

        verticalSpacer_4 = new QSpacerItem(18, 170, QSizePolicy::Minimum, QSizePolicy::Expanding);

        verticalLayout_2->addItem(verticalSpacer_4);

        QWidget::setTabOrder(Wii_r1, Wii_r2);
        QWidget::setTabOrder(Wii_r2, Wii_r3);
        QWidget::setTabOrder(Wii_r3, Wii_r4);
        QWidget::setTabOrder(Wii_r4, Wii_p1);
        QWidget::setTabOrder(Wii_p1, Wii_p2);
        QWidget::setTabOrder(Wii_p2, Wii_p3);
        QWidget::setTabOrder(Wii_p3, Wii_p4);

        retranslateUi(WiiSetupForm);

        QMetaObject::connectSlotsByName(WiiSetupForm);
    } // setupUi

    void retranslateUi(QWidget *WiiSetupForm)
    {
        WiiSetupForm->setWindowTitle(QCoreApplication::translate("WiiSetupForm", "Form", nullptr));
        label->setText(QCoreApplication::translate("WiiSetupForm", "Wiimote Indication Setup", nullptr));
        label_16->setText(QCoreApplication::translate("WiiSetupForm", "Running", nullptr));
        label_17->setText(QCoreApplication::translate("WiiSetupForm", "Paused", nullptr));
        Wii_r1->setText(QString());
        Wii_p1->setText(QString());
        Wii_r2->setText(QString());
        Wii_p2->setText(QString());
        Wii_r3->setText(QString());
        Wii_p3->setText(QString());
        Wii_r4->setText(QString());
        Wii_p4->setText(QString());
        label_18->setText(QCoreApplication::translate("WiiSetupForm", "LED 1 on", nullptr));
        label_19->setText(QCoreApplication::translate("WiiSetupForm", "LED 2 on", nullptr));
        label_20->setText(QCoreApplication::translate("WiiSetupForm", "LED 3 on", nullptr));
        label_21->setText(QCoreApplication::translate("WiiSetupForm", "LED 4 on", nullptr));
    } // retranslateUi

};

namespace Ui {
    class WiiSetupForm: public Ui_WiiSetupForm {};
} // namespace Ui

QT_END_NAMESPACE

#endif // UI_WII_SETUP_H
