/********************************************************************************
** Form generated from reading UI file 'joy_setup.ui'
**
** Created by: Qt User Interface Compiler version 5.15.8
**
** WARNING! All changes made in this file will be lost when recompiling UI file!
********************************************************************************/

#ifndef UI_JOY_SETUP_H
#define UI_JOY_SETUP_H

#include <QtCore/QVariant>
#include <QtWidgets/QApplication>
#include <QtWidgets/QComboBox>
#include <QtWidgets/QFormLayout>
#include <QtWidgets/QGroupBox>
#include <QtWidgets/QHBoxLayout>
#include <QtWidgets/QLabel>
#include <QtWidgets/QRadioButton>
#include <QtWidgets/QSpacerItem>
#include <QtWidgets/QSpinBox>
#include <QtWidgets/QVBoxLayout>
#include <QtWidgets/QWidget>

QT_BEGIN_NAMESPACE

class Ui_JoySetup
{
public:
    QVBoxLayout *verticalLayout;
    QGroupBox *groupBox_2;
    QHBoxLayout *horizontalLayout_2;
    QFormLayout *formLayout_3;
    QLabel *label;
    QComboBox *PitchCombo;
    QLabel *label_2;
    QComboBox *YawCombo;
    QLabel *label_3;
    QComboBox *RollCombo;
    QFormLayout *formLayout_2;
    QLabel *label_4;
    QComboBox *TXCombo;
    QLabel *label_5;
    QComboBox *TYCombo;
    QLabel *label_6;
    QComboBox *TZCombo;
    QGroupBox *groupBox;
    QHBoxLayout *horizontalLayout;
    QRadioButton *EvdevButton;
    QRadioButton *JsButton;
    QGroupBox *groupBox_3;
    QHBoxLayout *horizontalLayout_3;
    QHBoxLayout *horizontalLayout_4;
    QLabel *label_7;
    QSpinBox *PPSFreq;
    QSpacerItem *horizontalSpacer;
    QSpacerItem *verticalSpacer;

    void setupUi(QWidget *JoySetup)
    {
        if (JoySetup->objectName().isEmpty())
            JoySetup->setObjectName(QString::fromUtf8("JoySetup"));
        JoySetup->resize(361, 285);
        verticalLayout = new QVBoxLayout(JoySetup);
        verticalLayout->setObjectName(QString::fromUtf8("verticalLayout"));
        groupBox_2 = new QGroupBox(JoySetup);
        groupBox_2->setObjectName(QString::fromUtf8("groupBox_2"));
        horizontalLayout_2 = new QHBoxLayout(groupBox_2);
        horizontalLayout_2->setObjectName(QString::fromUtf8("horizontalLayout_2"));
        formLayout_3 = new QFormLayout();
        formLayout_3->setObjectName(QString::fromUtf8("formLayout_3"));
        label = new QLabel(groupBox_2);
        label->setObjectName(QString::fromUtf8("label"));

        formLayout_3->setWidget(0, QFormLayout::LabelRole, label);

        PitchCombo = new QComboBox(groupBox_2);
        PitchCombo->addItem(QString());
        PitchCombo->setObjectName(QString::fromUtf8("PitchCombo"));

        formLayout_3->setWidget(0, QFormLayout::FieldRole, PitchCombo);

        label_2 = new QLabel(groupBox_2);
        label_2->setObjectName(QString::fromUtf8("label_2"));

        formLayout_3->setWidget(1, QFormLayout::LabelRole, label_2);

        YawCombo = new QComboBox(groupBox_2);
        YawCombo->addItem(QString());
        YawCombo->setObjectName(QString::fromUtf8("YawCombo"));

        formLayout_3->setWidget(1, QFormLayout::FieldRole, YawCombo);

        label_3 = new QLabel(groupBox_2);
        label_3->setObjectName(QString::fromUtf8("label_3"));

        formLayout_3->setWidget(2, QFormLayout::LabelRole, label_3);

        RollCombo = new QComboBox(groupBox_2);
        RollCombo->addItem(QString());
        RollCombo->setObjectName(QString::fromUtf8("RollCombo"));

        formLayout_3->setWidget(2, QFormLayout::FieldRole, RollCombo);


        horizontalLayout_2->addLayout(formLayout_3);

        formLayout_2 = new QFormLayout();
        formLayout_2->setObjectName(QString::fromUtf8("formLayout_2"));
        label_4 = new QLabel(groupBox_2);
        label_4->setObjectName(QString::fromUtf8("label_4"));

        formLayout_2->setWidget(0, QFormLayout::LabelRole, label_4);

        TXCombo = new QComboBox(groupBox_2);
        TXCombo->addItem(QString());
        TXCombo->setObjectName(QString::fromUtf8("TXCombo"));

        formLayout_2->setWidget(0, QFormLayout::FieldRole, TXCombo);

        label_5 = new QLabel(groupBox_2);
        label_5->setObjectName(QString::fromUtf8("label_5"));

        formLayout_2->setWidget(1, QFormLayout::LabelRole, label_5);

        TYCombo = new QComboBox(groupBox_2);
        TYCombo->addItem(QString());
        TYCombo->setObjectName(QString::fromUtf8("TYCombo"));

        formLayout_2->setWidget(1, QFormLayout::FieldRole, TYCombo);

        label_6 = new QLabel(groupBox_2);
        label_6->setObjectName(QString::fromUtf8("label_6"));

        formLayout_2->setWidget(2, QFormLayout::LabelRole, label_6);

        TZCombo = new QComboBox(groupBox_2);
        TZCombo->addItem(QString());
        TZCombo->setObjectName(QString::fromUtf8("TZCombo"));

        formLayout_2->setWidget(2, QFormLayout::FieldRole, TZCombo);


        horizontalLayout_2->addLayout(formLayout_2);


        verticalLayout->addWidget(groupBox_2);

        groupBox = new QGroupBox(JoySetup);
        groupBox->setObjectName(QString::fromUtf8("groupBox"));
        horizontalLayout = new QHBoxLayout(groupBox);
        horizontalLayout->setObjectName(QString::fromUtf8("horizontalLayout"));
        EvdevButton = new QRadioButton(groupBox);
        EvdevButton->setObjectName(QString::fromUtf8("EvdevButton"));
        EvdevButton->setChecked(true);

        horizontalLayout->addWidget(EvdevButton);

        JsButton = new QRadioButton(groupBox);
        JsButton->setObjectName(QString::fromUtf8("JsButton"));

        horizontalLayout->addWidget(JsButton);


        verticalLayout->addWidget(groupBox);

        groupBox_3 = new QGroupBox(JoySetup);
        groupBox_3->setObjectName(QString::fromUtf8("groupBox_3"));
        horizontalLayout_3 = new QHBoxLayout(groupBox_3);
        horizontalLayout_3->setObjectName(QString::fromUtf8("horizontalLayout_3"));
        horizontalLayout_4 = new QHBoxLayout();
        horizontalLayout_4->setObjectName(QString::fromUtf8("horizontalLayout_4"));
        label_7 = new QLabel(groupBox_3);
        label_7->setObjectName(QString::fromUtf8("label_7"));

        horizontalLayout_4->addWidget(label_7);

        PPSFreq = new QSpinBox(groupBox_3);
        PPSFreq->setObjectName(QString::fromUtf8("PPSFreq"));
        PPSFreq->setAlignment(Qt::AlignRight|Qt::AlignTrailing|Qt::AlignVCenter);
        PPSFreq->setMinimum(1);
        PPSFreq->setMaximum(120);
        PPSFreq->setValue(60);

        horizontalLayout_4->addWidget(PPSFreq);

        horizontalSpacer = new QSpacerItem(40, 20, QSizePolicy::Expanding, QSizePolicy::Minimum);

        horizontalLayout_4->addItem(horizontalSpacer);


        horizontalLayout_3->addLayout(horizontalLayout_4);


        verticalLayout->addWidget(groupBox_3);

        verticalSpacer = new QSpacerItem(20, 40, QSizePolicy::Minimum, QSizePolicy::Expanding);

        verticalLayout->addItem(verticalSpacer);

        QWidget::setTabOrder(PitchCombo, YawCombo);
        QWidget::setTabOrder(YawCombo, RollCombo);
        QWidget::setTabOrder(RollCombo, TXCombo);
        QWidget::setTabOrder(TXCombo, TYCombo);
        QWidget::setTabOrder(TYCombo, TZCombo);
        QWidget::setTabOrder(TZCombo, EvdevButton);
        QWidget::setTabOrder(EvdevButton, JsButton);

        retranslateUi(JoySetup);

        QMetaObject::connectSlotsByName(JoySetup);
    } // setupUi

    void retranslateUi(QWidget *JoySetup)
    {
        JoySetup->setWindowTitle(QCoreApplication::translate("JoySetup", "Form", nullptr));
        groupBox_2->setTitle(QCoreApplication::translate("JoySetup", "Axes mapping", nullptr));
        label->setText(QCoreApplication::translate("JoySetup", "Pitch", nullptr));
        PitchCombo->setItemText(0, QCoreApplication::translate("JoySetup", "-", nullptr));

        label_2->setText(QCoreApplication::translate("JoySetup", "Yaw", nullptr));
        YawCombo->setItemText(0, QCoreApplication::translate("JoySetup", "-", nullptr));

        label_3->setText(QCoreApplication::translate("JoySetup", "Roll", nullptr));
        RollCombo->setItemText(0, QCoreApplication::translate("JoySetup", "-", nullptr));

        label_4->setText(QCoreApplication::translate("JoySetup", "X translation", nullptr));
        TXCombo->setItemText(0, QCoreApplication::translate("JoySetup", "-", nullptr));

        label_5->setText(QCoreApplication::translate("JoySetup", "Y translation", nullptr));
        TYCombo->setItemText(0, QCoreApplication::translate("JoySetup", "-", nullptr));

        label_6->setText(QCoreApplication::translate("JoySetup", "Z translation", nullptr));
        TZCombo->setItemText(0, QCoreApplication::translate("JoySetup", "-", nullptr));

        groupBox->setTitle(QCoreApplication::translate("JoySetup", "Select interface", nullptr));
        EvdevButton->setText(QCoreApplication::translate("JoySetup", "Evdev", nullptr));
        JsButton->setText(QCoreApplication::translate("JoySetup", "Joystick", nullptr));
        groupBox_3->setTitle(QCoreApplication::translate("JoySetup", "Misc", nullptr));
        label_7->setText(QCoreApplication::translate("JoySetup", "Polling frequency", nullptr));
    } // retranslateUi

};

namespace Ui {
    class JoySetup: public Ui_JoySetup {};
} // namespace Ui

QT_END_NAMESPACE

#endif // UI_JOY_SETUP_H
