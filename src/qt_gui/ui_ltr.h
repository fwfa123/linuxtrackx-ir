/********************************************************************************
** Form generated from reading UI file 'ltr.ui'
**
** Created by: Qt User Interface Compiler version 5.15.17
**
** WARNING! All changes made in this file will be lost when recompiling UI file!
********************************************************************************/

#ifndef UI_LTR_H
#define UI_LTR_H

#include <QtCore/QVariant>
#include <QtGui/QIcon>
#include <QtWidgets/QApplication>
#include <QtWidgets/QCheckBox>
#include <QtWidgets/QDoubleSpinBox>
#include <QtWidgets/QFrame>
#include <QtWidgets/QGridLayout>
#include <QtWidgets/QHBoxLayout>
#include <QtWidgets/QLabel>
#include <QtWidgets/QPushButton>
#include <QtWidgets/QSpacerItem>
#include <QtWidgets/QTabWidget>
#include <QtWidgets/QVBoxLayout>
#include <QtWidgets/QWidget>

QT_BEGIN_NAMESPACE

class Ui_LinuxtrackMainForm
{
public:
    QVBoxLayout *verticalLayout_2;
    QTabWidget *LtrTab;
    QWidget *tab;
    QHBoxLayout *horizontalLayout;
    QFrame *DSS;
    QVBoxLayout *verticalLayout_4;
    QVBoxLayout *DeviceSetupSite;
    QSpacerItem *verticalSpacer;
    QWidget *tab_2;
    QVBoxLayout *verticalLayout_5;
    QFrame *frame;
    QVBoxLayout *verticalLayout_6;
    QHBoxLayout *ModelEditSite;
    QWidget *tab_3;
    QHBoxLayout *horizontalLayout_5;
    QFrame *ProfileSetupFrame;
    QVBoxLayout *verticalLayout;
    QVBoxLayout *ProfileSetupSite;
    QWidget *tab_4;
    QHBoxLayout *horizontalLayout_7;
    QFrame *frame_2;
    QVBoxLayout *verticalLayout_3;
    QGridLayout *gridLayout;
    QPushButton *XplanePluginButton;
    QPushButton *ViewLogButton;
    QPushButton *LinuxtrackWineButton;
    QPushButton *PackageLogsButton;
    QPushButton *TIRFWButton;
    QFrame *frame_7;
    QVBoxLayout *verticalLayout_23;
    QLabel *label;
    QGridLayout *gridLayout_2;
    QCheckBox *DisableCamView;
    QCheckBox *LegacyRotation;
    QCheckBox *LegacyPose;
    QCheckBox *Disable3DView;
    QCheckBox *TransRotDisable;
    QHBoxLayout *horizontalLayout_2;
    QLabel *label_2;
    QDoubleSpinBox *FocalLength;
    QSpacerItem *verticalSpacer_2;
    QHBoxLayout *horizontalLayout_6;
    QPushButton *HelpButton;
    QSpacerItem *horizontalSpacer_2;
    QPushButton *DefaultsButton;
    QPushButton *DiscardChangesButton;
    QPushButton *SaveButton;
    QPushButton *QuitButton;

    void setupUi(QWidget *LinuxtrackMainForm)
    {
        if (LinuxtrackMainForm->objectName().isEmpty())
            LinuxtrackMainForm->setObjectName(QString::fromUtf8("LinuxtrackMainForm"));
        LinuxtrackMainForm->resize(678, 425);
        QSizePolicy sizePolicy(QSizePolicy::Preferred, QSizePolicy::Preferred);
        sizePolicy.setHorizontalStretch(0);
        sizePolicy.setVerticalStretch(0);
        sizePolicy.setHeightForWidth(LinuxtrackMainForm->sizePolicy().hasHeightForWidth());
        LinuxtrackMainForm->setSizePolicy(sizePolicy);
        QIcon icon;
        icon.addFile(QString::fromUtf8(":/ltr/linuxtrack.svg"), QSize(), QIcon::Normal, QIcon::Off);
        LinuxtrackMainForm->setWindowIcon(icon);
        verticalLayout_2 = new QVBoxLayout(LinuxtrackMainForm);
        verticalLayout_2->setObjectName(QString::fromUtf8("verticalLayout_2"));
        LtrTab = new QTabWidget(LinuxtrackMainForm);
        LtrTab->setObjectName(QString::fromUtf8("LtrTab"));
        QSizePolicy sizePolicy1(QSizePolicy::Minimum, QSizePolicy::Expanding);
        sizePolicy1.setHorizontalStretch(0);
        sizePolicy1.setVerticalStretch(0);
        sizePolicy1.setHeightForWidth(LtrTab->sizePolicy().hasHeightForWidth());
        LtrTab->setSizePolicy(sizePolicy1);
        tab = new QWidget();
        tab->setObjectName(QString::fromUtf8("tab"));
        horizontalLayout = new QHBoxLayout(tab);
        horizontalLayout->setObjectName(QString::fromUtf8("horizontalLayout"));
        DSS = new QFrame(tab);
        DSS->setObjectName(QString::fromUtf8("DSS"));
        DSS->setFrameShape(QFrame::StyledPanel);
        DSS->setFrameShadow(QFrame::Raised);
        verticalLayout_4 = new QVBoxLayout(DSS);
        verticalLayout_4->setObjectName(QString::fromUtf8("verticalLayout_4"));
        DeviceSetupSite = new QVBoxLayout();
        DeviceSetupSite->setObjectName(QString::fromUtf8("DeviceSetupSite"));

        verticalLayout_4->addLayout(DeviceSetupSite);

        verticalSpacer = new QSpacerItem(20, 290, QSizePolicy::Minimum, QSizePolicy::Expanding);

        verticalLayout_4->addItem(verticalSpacer);


        horizontalLayout->addWidget(DSS);

        LtrTab->addTab(tab, QString());
        tab_2 = new QWidget();
        tab_2->setObjectName(QString::fromUtf8("tab_2"));
        verticalLayout_5 = new QVBoxLayout(tab_2);
        verticalLayout_5->setObjectName(QString::fromUtf8("verticalLayout_5"));
        frame = new QFrame(tab_2);
        frame->setObjectName(QString::fromUtf8("frame"));
        frame->setFrameShape(QFrame::StyledPanel);
        frame->setFrameShadow(QFrame::Raised);
        verticalLayout_6 = new QVBoxLayout(frame);
        verticalLayout_6->setObjectName(QString::fromUtf8("verticalLayout_6"));
        ModelEditSite = new QHBoxLayout();
        ModelEditSite->setObjectName(QString::fromUtf8("ModelEditSite"));

        verticalLayout_6->addLayout(ModelEditSite);


        verticalLayout_5->addWidget(frame);

        LtrTab->addTab(tab_2, QString());
        tab_3 = new QWidget();
        tab_3->setObjectName(QString::fromUtf8("tab_3"));
        horizontalLayout_5 = new QHBoxLayout(tab_3);
        horizontalLayout_5->setObjectName(QString::fromUtf8("horizontalLayout_5"));
        ProfileSetupFrame = new QFrame(tab_3);
        ProfileSetupFrame->setObjectName(QString::fromUtf8("ProfileSetupFrame"));
        ProfileSetupFrame->setFrameShape(QFrame::StyledPanel);
        ProfileSetupFrame->setFrameShadow(QFrame::Raised);
        verticalLayout = new QVBoxLayout(ProfileSetupFrame);
        verticalLayout->setObjectName(QString::fromUtf8("verticalLayout"));
        ProfileSetupSite = new QVBoxLayout();
        ProfileSetupSite->setObjectName(QString::fromUtf8("ProfileSetupSite"));

        verticalLayout->addLayout(ProfileSetupSite);


        horizontalLayout_5->addWidget(ProfileSetupFrame);

        LtrTab->addTab(tab_3, QString());
        tab_4 = new QWidget();
        tab_4->setObjectName(QString::fromUtf8("tab_4"));
        horizontalLayout_7 = new QHBoxLayout(tab_4);
        horizontalLayout_7->setObjectName(QString::fromUtf8("horizontalLayout_7"));
        frame_2 = new QFrame(tab_4);
        frame_2->setObjectName(QString::fromUtf8("frame_2"));
        frame_2->setFrameShape(QFrame::StyledPanel);
        frame_2->setFrameShadow(QFrame::Raised);
        verticalLayout_3 = new QVBoxLayout(frame_2);
        verticalLayout_3->setObjectName(QString::fromUtf8("verticalLayout_3"));
        gridLayout = new QGridLayout();
        gridLayout->setObjectName(QString::fromUtf8("gridLayout"));
        XplanePluginButton = new QPushButton(frame_2);
        XplanePluginButton->setObjectName(QString::fromUtf8("XplanePluginButton"));

        gridLayout->addWidget(XplanePluginButton, 0, 0, 1, 1);

        ViewLogButton = new QPushButton(frame_2);
        ViewLogButton->setObjectName(QString::fromUtf8("ViewLogButton"));

        gridLayout->addWidget(ViewLogButton, 3, 0, 1, 1);

        LinuxtrackWineButton = new QPushButton(frame_2);
        LinuxtrackWineButton->setObjectName(QString::fromUtf8("LinuxtrackWineButton"));
        LinuxtrackWineButton->setEnabled(false);

        gridLayout->addWidget(LinuxtrackWineButton, 0, 1, 1, 1);

        PackageLogsButton = new QPushButton(frame_2);
        PackageLogsButton->setObjectName(QString::fromUtf8("PackageLogsButton"));

        gridLayout->addWidget(PackageLogsButton, 3, 1, 1, 1);

        TIRFWButton = new QPushButton(frame_2);
        TIRFWButton->setObjectName(QString::fromUtf8("TIRFWButton"));

        gridLayout->addWidget(TIRFWButton, 1, 0, 1, 1);


        verticalLayout_3->addLayout(gridLayout);

        frame_7 = new QFrame(frame_2);
        frame_7->setObjectName(QString::fromUtf8("frame_7"));
        frame_7->setFrameShape(QFrame::StyledPanel);
        frame_7->setFrameShadow(QFrame::Raised);
        verticalLayout_23 = new QVBoxLayout(frame_7);
        verticalLayout_23->setObjectName(QString::fromUtf8("verticalLayout_23"));
        label = new QLabel(frame_7);
        label->setObjectName(QString::fromUtf8("label"));
        QSizePolicy sizePolicy2(QSizePolicy::Preferred, QSizePolicy::Maximum);
        sizePolicy2.setHorizontalStretch(0);
        sizePolicy2.setVerticalStretch(0);
        sizePolicy2.setHeightForWidth(label->sizePolicy().hasHeightForWidth());
        label->setSizePolicy(sizePolicy2);
        QFont font;
        font.setBold(true);
        font.setWeight(75);
        label->setFont(font);
        label->setAlignment(Qt::AlignCenter);

        verticalLayout_23->addWidget(label);

        gridLayout_2 = new QGridLayout();
        gridLayout_2->setObjectName(QString::fromUtf8("gridLayout_2"));
        DisableCamView = new QCheckBox(frame_7);
        DisableCamView->setObjectName(QString::fromUtf8("DisableCamView"));

        gridLayout_2->addWidget(DisableCamView, 0, 0, 1, 1);

        LegacyRotation = new QCheckBox(frame_7);
        LegacyRotation->setObjectName(QString::fromUtf8("LegacyRotation"));

        gridLayout_2->addWidget(LegacyRotation, 1, 1, 1, 1);

        LegacyPose = new QCheckBox(frame_7);
        LegacyPose->setObjectName(QString::fromUtf8("LegacyPose"));

        gridLayout_2->addWidget(LegacyPose, 1, 0, 1, 1);

        Disable3DView = new QCheckBox(frame_7);
        Disable3DView->setObjectName(QString::fromUtf8("Disable3DView"));

        gridLayout_2->addWidget(Disable3DView, 0, 1, 1, 1);

        TransRotDisable = new QCheckBox(frame_7);
        TransRotDisable->setObjectName(QString::fromUtf8("TransRotDisable"));

        gridLayout_2->addWidget(TransRotDisable, 2, 0, 1, 1);

        horizontalLayout_2 = new QHBoxLayout();
        horizontalLayout_2->setObjectName(QString::fromUtf8("horizontalLayout_2"));
        label_2 = new QLabel(frame_7);
        label_2->setObjectName(QString::fromUtf8("label_2"));
        QSizePolicy sizePolicy3(QSizePolicy::Maximum, QSizePolicy::Preferred);
        sizePolicy3.setHorizontalStretch(0);
        sizePolicy3.setVerticalStretch(0);
        sizePolicy3.setHeightForWidth(label_2->sizePolicy().hasHeightForWidth());
        label_2->setSizePolicy(sizePolicy3);

        horizontalLayout_2->addWidget(label_2);

        FocalLength = new QDoubleSpinBox(frame_7);
        FocalLength->setObjectName(QString::fromUtf8("FocalLength"));
        FocalLength->setAlignment(Qt::AlignRight|Qt::AlignTrailing|Qt::AlignVCenter);
        FocalLength->setButtonSymbols(QAbstractSpinBox::UpDownArrows);
        FocalLength->setDecimals(1);
        FocalLength->setMinimum(1.000000000000000);
        FocalLength->setMaximum(10000.000000000000000);
        FocalLength->setValue(660.000000000000000);

        horizontalLayout_2->addWidget(FocalLength);


        gridLayout_2->addLayout(horizontalLayout_2, 2, 1, 1, 1);


        verticalLayout_23->addLayout(gridLayout_2);


        verticalLayout_3->addWidget(frame_7);

        verticalSpacer_2 = new QSpacerItem(20, 40, QSizePolicy::Minimum, QSizePolicy::Expanding);

        verticalLayout_3->addItem(verticalSpacer_2);


        horizontalLayout_7->addWidget(frame_2);

        LtrTab->addTab(tab_4, QString());

        verticalLayout_2->addWidget(LtrTab);

        horizontalLayout_6 = new QHBoxLayout();
        horizontalLayout_6->setObjectName(QString::fromUtf8("horizontalLayout_6"));
        horizontalLayout_6->setSizeConstraint(QLayout::SetMinimumSize);
        HelpButton = new QPushButton(LinuxtrackMainForm);
        HelpButton->setObjectName(QString::fromUtf8("HelpButton"));

        horizontalLayout_6->addWidget(HelpButton);

        horizontalSpacer_2 = new QSpacerItem(40, 10, QSizePolicy::Expanding, QSizePolicy::Minimum);

        horizontalLayout_6->addItem(horizontalSpacer_2);

        DefaultsButton = new QPushButton(LinuxtrackMainForm);
        DefaultsButton->setObjectName(QString::fromUtf8("DefaultsButton"));

        horizontalLayout_6->addWidget(DefaultsButton);

        DiscardChangesButton = new QPushButton(LinuxtrackMainForm);
        DiscardChangesButton->setObjectName(QString::fromUtf8("DiscardChangesButton"));

        horizontalLayout_6->addWidget(DiscardChangesButton);

        SaveButton = new QPushButton(LinuxtrackMainForm);
        SaveButton->setObjectName(QString::fromUtf8("SaveButton"));

        horizontalLayout_6->addWidget(SaveButton);

        QuitButton = new QPushButton(LinuxtrackMainForm);
        QuitButton->setObjectName(QString::fromUtf8("QuitButton"));

        horizontalLayout_6->addWidget(QuitButton);


        verticalLayout_2->addLayout(horizontalLayout_6);

        QWidget::setTabOrder(LtrTab, HelpButton);
        QWidget::setTabOrder(HelpButton, DefaultsButton);
        QWidget::setTabOrder(DefaultsButton, DiscardChangesButton);
        QWidget::setTabOrder(DiscardChangesButton, SaveButton);
        QWidget::setTabOrder(SaveButton, QuitButton);
        QWidget::setTabOrder(QuitButton, XplanePluginButton);
        QWidget::setTabOrder(XplanePluginButton, ViewLogButton);

        retranslateUi(LinuxtrackMainForm);

        LtrTab->setCurrentIndex(0);


        QMetaObject::connectSlotsByName(LinuxtrackMainForm);
    } // setupUi

    void retranslateUi(QWidget *LinuxtrackMainForm)
    {
        LinuxtrackMainForm->setWindowTitle(QCoreApplication::translate("LinuxtrackMainForm", "Linuxtrack", nullptr));
        LtrTab->setTabText(LtrTab->indexOf(tab), QCoreApplication::translate("LinuxtrackMainForm", "Device Setup", nullptr));
        LtrTab->setTabText(LtrTab->indexOf(tab_2), QCoreApplication::translate("LinuxtrackMainForm", "Model Setup", nullptr));
        LtrTab->setTabText(LtrTab->indexOf(tab_3), QCoreApplication::translate("LinuxtrackMainForm", "Tracking setup", nullptr));
        XplanePluginButton->setText(QCoreApplication::translate("LinuxtrackMainForm", "Install Xplane plugin...", nullptr));
        ViewLogButton->setText(QCoreApplication::translate("LinuxtrackMainForm", "View logfile", nullptr));
        LinuxtrackWineButton->setText(QCoreApplication::translate("LinuxtrackMainForm", "Install Linuxtrack-Wine support...", nullptr));
        PackageLogsButton->setText(QCoreApplication::translate("LinuxtrackMainForm", "Package logs...", nullptr));
        TIRFWButton->setText(QCoreApplication::translate("LinuxtrackMainForm", "Reinstall TrackIR firmware", nullptr));
        label->setText(QCoreApplication::translate("LinuxtrackMainForm", "Troubleshooting options", nullptr));
        DisableCamView->setText(QCoreApplication::translate("LinuxtrackMainForm", "Disable camera view", nullptr));
        LegacyRotation->setText(QCoreApplication::translate("LinuxtrackMainForm", "Use legacy rotation computation method", nullptr));
        LegacyPose->setText(QCoreApplication::translate("LinuxtrackMainForm", "Use legacy pose computation method\n"
"  (weak perspective/Alter)", nullptr));
        Disable3DView->setText(QCoreApplication::translate("LinuxtrackMainForm", "Disable 3D tracking preview", nullptr));
        TransRotDisable->setText(QCoreApplication::translate("LinuxtrackMainForm", "Disable translation/rotation alignment ", nullptr));
        label_2->setText(QCoreApplication::translate("LinuxtrackMainForm", "Focal length (pixels)", nullptr));
        LtrTab->setTabText(LtrTab->indexOf(tab_4), QCoreApplication::translate("LinuxtrackMainForm", "Misc.", nullptr));
        HelpButton->setText(QCoreApplication::translate("LinuxtrackMainForm", "Help", nullptr));
        DefaultsButton->setText(QCoreApplication::translate("LinuxtrackMainForm", "Load Defaults", nullptr));
        DiscardChangesButton->setText(QCoreApplication::translate("LinuxtrackMainForm", "Discard changes", nullptr));
        SaveButton->setText(QCoreApplication::translate("LinuxtrackMainForm", "Save", nullptr));
        QuitButton->setText(QCoreApplication::translate("LinuxtrackMainForm", "Quit", nullptr));
    } // retranslateUi

};

namespace Ui {
    class LinuxtrackMainForm: public Ui_LinuxtrackMainForm {};
} // namespace Ui

QT_END_NAMESPACE

#endif // UI_LTR_H
