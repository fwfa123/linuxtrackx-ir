/********************************************************************************
** Form generated from reading UI file 'clip_tweaking.ui'
**
** Created by: Qt User Interface Compiler version 5.15.17
**
** WARNING! All changes made in this file will be lost when recompiling UI file!
********************************************************************************/

#ifndef UI_CLIP_TWEAKING_H
#define UI_CLIP_TWEAKING_H

#include <QtCore/QVariant>
#include <QtWidgets/QApplication>
#include <QtWidgets/QFrame>
#include <QtWidgets/QGridLayout>
#include <QtWidgets/QHBoxLayout>
#include <QtWidgets/QLabel>
#include <QtWidgets/QRadioButton>
#include <QtWidgets/QSlider>
#include <QtWidgets/QSpacerItem>
#include <QtWidgets/QVBoxLayout>
#include <QtWidgets/QWidget>

QT_BEGIN_NAMESPACE

class Ui_ClipTweakingForm
{
public:
    QVBoxLayout *verticalLayout_2;
    QFrame *frame_2;
    QVBoxLayout *verticalLayout_3;
    QLabel *label_11;
    QHBoxLayout *horizontalLayout;
    QRadioButton *ClipLeft;
    QRadioButton *ClipRight;
    QFrame *frame;
    QVBoxLayout *verticalLayout;
    QLabel *label_10;
    QGridLayout *gridLayout_2;
    QSlider *ClipHx;
    QSlider *ClipHy;
    QLabel *label_5;
    QLabel *label_3;
    QLabel *label_4;
    QSlider *ClipHz;
    QSpacerItem *verticalSpacer;

    void setupUi(QWidget *ClipTweakingForm)
    {
        if (ClipTweakingForm->objectName().isEmpty())
            ClipTweakingForm->setObjectName(QString::fromUtf8("ClipTweakingForm"));
        ClipTweakingForm->resize(424, 286);
        verticalLayout_2 = new QVBoxLayout(ClipTweakingForm);
        verticalLayout_2->setObjectName(QString::fromUtf8("verticalLayout_2"));
        frame_2 = new QFrame(ClipTweakingForm);
        frame_2->setObjectName(QString::fromUtf8("frame_2"));
        frame_2->setFrameShape(QFrame::StyledPanel);
        frame_2->setFrameShadow(QFrame::Raised);
        verticalLayout_3 = new QVBoxLayout(frame_2);
        verticalLayout_3->setObjectName(QString::fromUtf8("verticalLayout_3"));
        label_11 = new QLabel(frame_2);
        label_11->setObjectName(QString::fromUtf8("label_11"));
        QFont font;
        font.setBold(true);
        font.setWeight(75);
        label_11->setFont(font);

        verticalLayout_3->addWidget(label_11);

        horizontalLayout = new QHBoxLayout();
        horizontalLayout->setObjectName(QString::fromUtf8("horizontalLayout"));
        ClipLeft = new QRadioButton(frame_2);
        ClipLeft->setObjectName(QString::fromUtf8("ClipLeft"));
        QSizePolicy sizePolicy(QSizePolicy::Preferred, QSizePolicy::Fixed);
        sizePolicy.setHorizontalStretch(0);
        sizePolicy.setVerticalStretch(0);
        sizePolicy.setHeightForWidth(ClipLeft->sizePolicy().hasHeightForWidth());
        ClipLeft->setSizePolicy(sizePolicy);
        ClipLeft->setChecked(true);

        horizontalLayout->addWidget(ClipLeft);

        ClipRight = new QRadioButton(frame_2);
        ClipRight->setObjectName(QString::fromUtf8("ClipRight"));

        horizontalLayout->addWidget(ClipRight);


        verticalLayout_3->addLayout(horizontalLayout);


        verticalLayout_2->addWidget(frame_2);

        frame = new QFrame(ClipTweakingForm);
        frame->setObjectName(QString::fromUtf8("frame"));
        frame->setFrameShape(QFrame::StyledPanel);
        frame->setFrameShadow(QFrame::Raised);
        verticalLayout = new QVBoxLayout(frame);
        verticalLayout->setObjectName(QString::fromUtf8("verticalLayout"));
        label_10 = new QLabel(frame);
        label_10->setObjectName(QString::fromUtf8("label_10"));
        label_10->setFont(font);

        verticalLayout->addWidget(label_10);

        gridLayout_2 = new QGridLayout();
        gridLayout_2->setObjectName(QString::fromUtf8("gridLayout_2"));
        ClipHx = new QSlider(frame);
        ClipHx->setObjectName(QString::fromUtf8("ClipHx"));
        ClipHx->setMinimum(1);
        ClipHx->setMaximum(299);
        ClipHx->setOrientation(Qt::Horizontal);

        gridLayout_2->addWidget(ClipHx, 1, 0, 1, 1);

        ClipHy = new QSlider(frame);
        ClipHy->setObjectName(QString::fromUtf8("ClipHy"));
        ClipHy->setMinimum(1);
        ClipHy->setMaximum(299);
        ClipHy->setOrientation(Qt::Horizontal);

        gridLayout_2->addWidget(ClipHy, 3, 0, 1, 1);

        label_5 = new QLabel(frame);
        label_5->setObjectName(QString::fromUtf8("label_5"));
        label_5->setAlignment(Qt::AlignCenter);

        gridLayout_2->addWidget(label_5, 4, 0, 1, 1);

        label_3 = new QLabel(frame);
        label_3->setObjectName(QString::fromUtf8("label_3"));
        label_3->setAlignment(Qt::AlignCenter);

        gridLayout_2->addWidget(label_3, 0, 0, 1, 1);

        label_4 = new QLabel(frame);
        label_4->setObjectName(QString::fromUtf8("label_4"));
        label_4->setAlignment(Qt::AlignCenter);

        gridLayout_2->addWidget(label_4, 2, 0, 1, 1);

        ClipHz = new QSlider(frame);
        ClipHz->setObjectName(QString::fromUtf8("ClipHz"));
        ClipHz->setMinimum(1);
        ClipHz->setMaximum(299);
        ClipHz->setOrientation(Qt::Horizontal);

        gridLayout_2->addWidget(ClipHz, 5, 0, 1, 1);


        verticalLayout->addLayout(gridLayout_2);


        verticalLayout_2->addWidget(frame);

        verticalSpacer = new QSpacerItem(20, 1, QSizePolicy::Minimum, QSizePolicy::Expanding);

        verticalLayout_2->addItem(verticalSpacer);


        retranslateUi(ClipTweakingForm);

        QMetaObject::connectSlotsByName(ClipTweakingForm);
    } // setupUi

    void retranslateUi(QWidget *ClipTweakingForm)
    {
        ClipTweakingForm->setWindowTitle(QCoreApplication::translate("ClipTweakingForm", "Form", nullptr));
        label_11->setText(QCoreApplication::translate("ClipTweakingForm", "On which side of your head the clip is?", nullptr));
        ClipLeft->setText(QCoreApplication::translate("ClipTweakingForm", "Left", nullptr));
        ClipRight->setText(QCoreApplication::translate("ClipTweakingForm", "Right", nullptr));
        label_10->setText(QCoreApplication::translate("ClipTweakingForm", "Three point Clip finetuning", nullptr));
        label_5->setText(QCoreApplication::translate("ClipTweakingForm", "Minimize left/right monvement while panning left/right", nullptr));
        label_3->setText(QCoreApplication::translate("ClipTweakingForm", "Minimize back/forth movement while panning left/right", nullptr));
        label_4->setText(QCoreApplication::translate("ClipTweakingForm", "Minimize back/forth movement while panning up/down", nullptr));
    } // retranslateUi

};

namespace Ui {
    class ClipTweakingForm: public Ui_ClipTweakingForm {};
} // namespace Ui

QT_END_NAMESPACE

#endif // UI_CLIP_TWEAKING_H
