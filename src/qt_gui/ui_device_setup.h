/********************************************************************************
** Form generated from reading UI file 'device_setup.ui'
**
** Created by: Qt User Interface Compiler version 5.15.8
**
** WARNING! All changes made in this file will be lost when recompiling UI file!
********************************************************************************/

#ifndef UI_DEVICE_SETUP_H
#define UI_DEVICE_SETUP_H

#include <QtCore/QVariant>
#include <QtWidgets/QApplication>
#include <QtWidgets/QComboBox>
#include <QtWidgets/QFrame>
#include <QtWidgets/QGridLayout>
#include <QtWidgets/QHBoxLayout>
#include <QtWidgets/QLabel>
#include <QtWidgets/QPushButton>
#include <QtWidgets/QVBoxLayout>
#include <QtWidgets/QWidget>

QT_BEGIN_NAMESPACE

class Ui_DeviceSetupForm
{
public:
    QVBoxLayout *verticalLayout_2;
    QFrame *frame;
    QVBoxLayout *verticalLayout;
    QGridLayout *gridLayout;
    QLabel *label_2;
    QLabel *label;
    QComboBox *CameraOrientation;
    QHBoxLayout *horizontalLayout_11;
    QComboBox *DeviceSelector;
    QPushButton *RefreshDevices;

    void setupUi(QWidget *DeviceSetupForm)
    {
        if (DeviceSetupForm->objectName().isEmpty())
            DeviceSetupForm->setObjectName(QString::fromUtf8("DeviceSetupForm"));
        DeviceSetupForm->resize(527, 102);
        verticalLayout_2 = new QVBoxLayout(DeviceSetupForm);
        verticalLayout_2->setObjectName(QString::fromUtf8("verticalLayout_2"));
        frame = new QFrame(DeviceSetupForm);
        frame->setObjectName(QString::fromUtf8("frame"));
        frame->setFrameShape(QFrame::StyledPanel);
        frame->setFrameShadow(QFrame::Raised);
        verticalLayout = new QVBoxLayout(frame);
        verticalLayout->setObjectName(QString::fromUtf8("verticalLayout"));
        gridLayout = new QGridLayout();
        gridLayout->setObjectName(QString::fromUtf8("gridLayout"));
        gridLayout->setSizeConstraint(QLayout::SetDefaultConstraint);
        label_2 = new QLabel(frame);
        label_2->setObjectName(QString::fromUtf8("label_2"));
        QSizePolicy sizePolicy(QSizePolicy::Maximum, QSizePolicy::Maximum);
        sizePolicy.setHorizontalStretch(0);
        sizePolicy.setVerticalStretch(0);
        sizePolicy.setHeightForWidth(label_2->sizePolicy().hasHeightForWidth());
        label_2->setSizePolicy(sizePolicy);

        gridLayout->addWidget(label_2, 0, 0, 1, 1);

        label = new QLabel(frame);
        label->setObjectName(QString::fromUtf8("label"));
        sizePolicy.setHeightForWidth(label->sizePolicy().hasHeightForWidth());
        label->setSizePolicy(sizePolicy);

        gridLayout->addWidget(label, 1, 0, 1, 1);

        CameraOrientation = new QComboBox(frame);
        CameraOrientation->setObjectName(QString::fromUtf8("CameraOrientation"));
        QSizePolicy sizePolicy1(QSizePolicy::Preferred, QSizePolicy::Maximum);
        sizePolicy1.setHorizontalStretch(0);
        sizePolicy1.setVerticalStretch(0);
        sizePolicy1.setHeightForWidth(CameraOrientation->sizePolicy().hasHeightForWidth());
        CameraOrientation->setSizePolicy(sizePolicy1);

        gridLayout->addWidget(CameraOrientation, 1, 1, 1, 1);

        horizontalLayout_11 = new QHBoxLayout();
        horizontalLayout_11->setObjectName(QString::fromUtf8("horizontalLayout_11"));
        DeviceSelector = new QComboBox(frame);
        DeviceSelector->setObjectName(QString::fromUtf8("DeviceSelector"));
        sizePolicy1.setHeightForWidth(DeviceSelector->sizePolicy().hasHeightForWidth());
        DeviceSelector->setSizePolicy(sizePolicy1);

        horizontalLayout_11->addWidget(DeviceSelector);

        RefreshDevices = new QPushButton(frame);
        RefreshDevices->setObjectName(QString::fromUtf8("RefreshDevices"));
        sizePolicy.setHeightForWidth(RefreshDevices->sizePolicy().hasHeightForWidth());
        RefreshDevices->setSizePolicy(sizePolicy);

        horizontalLayout_11->addWidget(RefreshDevices);


        gridLayout->addLayout(horizontalLayout_11, 0, 1, 1, 1);


        verticalLayout->addLayout(gridLayout);


        verticalLayout_2->addWidget(frame);

        QWidget::setTabOrder(DeviceSelector, RefreshDevices);
        QWidget::setTabOrder(RefreshDevices, CameraOrientation);

        retranslateUi(DeviceSetupForm);

        QMetaObject::connectSlotsByName(DeviceSetupForm);
    } // setupUi

    void retranslateUi(QWidget *DeviceSetupForm)
    {
        DeviceSetupForm->setWindowTitle(QCoreApplication::translate("DeviceSetupForm", "Form", nullptr));
        label_2->setText(QCoreApplication::translate("DeviceSetupForm", "Tracking Device", nullptr));
        label->setText(QCoreApplication::translate("DeviceSetupForm", "Device Orientation", nullptr));
        RefreshDevices->setText(QCoreApplication::translate("DeviceSetupForm", "Refresh", nullptr));
    } // retranslateUi

};

namespace Ui {
    class DeviceSetupForm: public Ui_DeviceSetupForm {};
} // namespace Ui

QT_END_NAMESPACE

#endif // UI_DEVICE_SETUP_H
