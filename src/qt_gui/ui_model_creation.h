/********************************************************************************
** Form generated from reading UI file 'model_creation.ui'
**
** Created by: Qt User Interface Compiler version 5.15.17
**
** WARNING! All changes made in this file will be lost when recompiling UI file!
********************************************************************************/

#ifndef UI_MODEL_CREATION_H
#define UI_MODEL_CREATION_H

#include <QtCore/QVariant>
#include <QtWidgets/QApplication>
#include <QtWidgets/QComboBox>
#include <QtWidgets/QDialog>
#include <QtWidgets/QFrame>
#include <QtWidgets/QHBoxLayout>
#include <QtWidgets/QLabel>
#include <QtWidgets/QLineEdit>
#include <QtWidgets/QPushButton>
#include <QtWidgets/QSpacerItem>
#include <QtWidgets/QVBoxLayout>

QT_BEGIN_NAMESPACE

class Ui_ModelCreation
{
public:
    QVBoxLayout *verticalLayout_5;
    QVBoxLayout *verticalLayout_3;
    QFrame *frame_2;
    QVBoxLayout *verticalLayout_2;
    QLabel *label;
    QLineEdit *ModelName;
    QFrame *frame;
    QVBoxLayout *verticalLayout;
    QHBoxLayout *horizontalLayout_2;
    QLabel *label_2;
    QComboBox *ModelTypeCombo;
    QFrame *frame_3;
    QVBoxLayout *verticalLayout_4;
    QVBoxLayout *MdlLayout;
    QSpacerItem *verticalSpacer;
    QHBoxLayout *horizontalLayout;
    QSpacerItem *horizontalSpacer;
    QPushButton *CancelButton;
    QPushButton *CreateButton;

    void setupUi(QDialog *ModelCreation)
    {
        if (ModelCreation->objectName().isEmpty())
            ModelCreation->setObjectName(QString::fromUtf8("ModelCreation"));
        ModelCreation->resize(530, 300);
        verticalLayout_5 = new QVBoxLayout(ModelCreation);
        verticalLayout_5->setObjectName(QString::fromUtf8("verticalLayout_5"));
        verticalLayout_3 = new QVBoxLayout();
        verticalLayout_3->setObjectName(QString::fromUtf8("verticalLayout_3"));
        frame_2 = new QFrame(ModelCreation);
        frame_2->setObjectName(QString::fromUtf8("frame_2"));
        frame_2->setFrameShape(QFrame::StyledPanel);
        frame_2->setFrameShadow(QFrame::Raised);
        verticalLayout_2 = new QVBoxLayout(frame_2);
        verticalLayout_2->setObjectName(QString::fromUtf8("verticalLayout_2"));
        label = new QLabel(frame_2);
        label->setObjectName(QString::fromUtf8("label"));
        QFont font;
        font.setBold(true);
        font.setWeight(75);
        label->setFont(font);

        verticalLayout_2->addWidget(label);

        ModelName = new QLineEdit(frame_2);
        ModelName->setObjectName(QString::fromUtf8("ModelName"));

        verticalLayout_2->addWidget(ModelName);


        verticalLayout_3->addWidget(frame_2);

        frame = new QFrame(ModelCreation);
        frame->setObjectName(QString::fromUtf8("frame"));
        frame->setFrameShape(QFrame::StyledPanel);
        frame->setFrameShadow(QFrame::Raised);
        verticalLayout = new QVBoxLayout(frame);
        verticalLayout->setObjectName(QString::fromUtf8("verticalLayout"));
        horizontalLayout_2 = new QHBoxLayout();
        horizontalLayout_2->setObjectName(QString::fromUtf8("horizontalLayout_2"));
        label_2 = new QLabel(frame);
        label_2->setObjectName(QString::fromUtf8("label_2"));
        QSizePolicy sizePolicy(QSizePolicy::Maximum, QSizePolicy::Preferred);
        sizePolicy.setHorizontalStretch(0);
        sizePolicy.setVerticalStretch(0);
        sizePolicy.setHeightForWidth(label_2->sizePolicy().hasHeightForWidth());
        label_2->setSizePolicy(sizePolicy);
        label_2->setFont(font);

        horizontalLayout_2->addWidget(label_2);

        ModelTypeCombo = new QComboBox(frame);
        ModelTypeCombo->addItem(QString());
        ModelTypeCombo->addItem(QString());
        ModelTypeCombo->addItem(QString());
        ModelTypeCombo->addItem(QString());
        ModelTypeCombo->addItem(QString());
        ModelTypeCombo->setObjectName(QString::fromUtf8("ModelTypeCombo"));

        horizontalLayout_2->addWidget(ModelTypeCombo);


        verticalLayout->addLayout(horizontalLayout_2);


        verticalLayout_3->addWidget(frame);

        frame_3 = new QFrame(ModelCreation);
        frame_3->setObjectName(QString::fromUtf8("frame_3"));
        frame_3->setFrameShape(QFrame::StyledPanel);
        frame_3->setFrameShadow(QFrame::Raised);
        verticalLayout_4 = new QVBoxLayout(frame_3);
        verticalLayout_4->setObjectName(QString::fromUtf8("verticalLayout_4"));
        MdlLayout = new QVBoxLayout();
        MdlLayout->setObjectName(QString::fromUtf8("MdlLayout"));

        verticalLayout_4->addLayout(MdlLayout);


        verticalLayout_3->addWidget(frame_3);

        verticalSpacer = new QSpacerItem(148, 38, QSizePolicy::Minimum, QSizePolicy::Expanding);

        verticalLayout_3->addItem(verticalSpacer);


        verticalLayout_5->addLayout(verticalLayout_3);

        horizontalLayout = new QHBoxLayout();
        horizontalLayout->setObjectName(QString::fromUtf8("horizontalLayout"));
        horizontalSpacer = new QSpacerItem(40, 20, QSizePolicy::Expanding, QSizePolicy::Minimum);

        horizontalLayout->addItem(horizontalSpacer);

        CancelButton = new QPushButton(ModelCreation);
        CancelButton->setObjectName(QString::fromUtf8("CancelButton"));
        CancelButton->setAutoDefault(false);

        horizontalLayout->addWidget(CancelButton);

        CreateButton = new QPushButton(ModelCreation);
        CreateButton->setObjectName(QString::fromUtf8("CreateButton"));
        CreateButton->setAutoDefault(true);

        horizontalLayout->addWidget(CreateButton);


        verticalLayout_5->addLayout(horizontalLayout);


        retranslateUi(ModelCreation);

        CreateButton->setDefault(true);


        QMetaObject::connectSlotsByName(ModelCreation);
    } // setupUi

    void retranslateUi(QDialog *ModelCreation)
    {
        ModelCreation->setWindowTitle(QCoreApplication::translate("ModelCreation", "Dialog", nullptr));
        label->setText(QCoreApplication::translate("ModelCreation", "Model Name", nullptr));
        label_2->setText(QCoreApplication::translate("ModelCreation", "Model Type", nullptr));
        ModelTypeCombo->setItemText(0, QCoreApplication::translate("ModelCreation", "3 Point Cap", nullptr));
        ModelTypeCombo->setItemText(1, QCoreApplication::translate("ModelCreation", "3 Point Clip", nullptr));
        ModelTypeCombo->setItemText(2, QCoreApplication::translate("ModelCreation", "1 Point", nullptr));
        ModelTypeCombo->setItemText(3, QCoreApplication::translate("ModelCreation", "Face", nullptr));
        ModelTypeCombo->setItemText(4, QCoreApplication::translate("ModelCreation", "Absolute", nullptr));

        CancelButton->setText(QCoreApplication::translate("ModelCreation", "Cancel", nullptr));
        CreateButton->setText(QCoreApplication::translate("ModelCreation", "Create", nullptr));
    } // retranslateUi

};

namespace Ui {
    class ModelCreation: public Ui_ModelCreation {};
} // namespace Ui

QT_END_NAMESPACE

#endif // UI_MODEL_CREATION_H
