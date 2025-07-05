/********************************************************************************
** Form generated from reading UI file 'model_edit.ui'
**
** Created by: Qt User Interface Compiler version 5.15.8
**
** WARNING! All changes made in this file will be lost when recompiling UI file!
********************************************************************************/

#ifndef UI_MODEL_EDIT_H
#define UI_MODEL_EDIT_H

#include <QtCore/QVariant>
#include <QtWidgets/QApplication>
#include <QtWidgets/QComboBox>
#include <QtWidgets/QHBoxLayout>
#include <QtWidgets/QLabel>
#include <QtWidgets/QPushButton>
#include <QtWidgets/QSpacerItem>
#include <QtWidgets/QVBoxLayout>
#include <QtWidgets/QWidget>

QT_BEGIN_NAMESPACE

class Ui_ModelEditForm
{
public:
    QVBoxLayout *verticalLayout_2;
    QHBoxLayout *horizontalLayout_2;
    QLabel *label_23;
    QComboBox *ModelSelector;
    QPushButton *CreateModelButton;
    QHBoxLayout *horizontalLayout;
    QVBoxLayout *ModelEditorSite;
    QSpacerItem *horizontalSpacer;
    QVBoxLayout *verticalLayout;
    QLabel *ModelPreview;
    QSpacerItem *verticalSpacer_2;
    QSpacerItem *verticalSpacer;

    void setupUi(QWidget *ModelEditForm)
    {
        if (ModelEditForm->objectName().isEmpty())
            ModelEditForm->setObjectName(QString::fromUtf8("ModelEditForm"));
        ModelEditForm->resize(617, 322);
        verticalLayout_2 = new QVBoxLayout(ModelEditForm);
        verticalLayout_2->setObjectName(QString::fromUtf8("verticalLayout_2"));
        horizontalLayout_2 = new QHBoxLayout();
        horizontalLayout_2->setObjectName(QString::fromUtf8("horizontalLayout_2"));
        label_23 = new QLabel(ModelEditForm);
        label_23->setObjectName(QString::fromUtf8("label_23"));
        QSizePolicy sizePolicy(QSizePolicy::Maximum, QSizePolicy::Preferred);
        sizePolicy.setHorizontalStretch(0);
        sizePolicy.setVerticalStretch(0);
        sizePolicy.setHeightForWidth(label_23->sizePolicy().hasHeightForWidth());
        label_23->setSizePolicy(sizePolicy);
        QFont font;
        font.setBold(true);
        font.setWeight(75);
        label_23->setFont(font);

        horizontalLayout_2->addWidget(label_23);

        ModelSelector = new QComboBox(ModelEditForm);
        ModelSelector->setObjectName(QString::fromUtf8("ModelSelector"));

        horizontalLayout_2->addWidget(ModelSelector);

        CreateModelButton = new QPushButton(ModelEditForm);
        CreateModelButton->setObjectName(QString::fromUtf8("CreateModelButton"));
        QSizePolicy sizePolicy1(QSizePolicy::Maximum, QSizePolicy::Fixed);
        sizePolicy1.setHorizontalStretch(0);
        sizePolicy1.setVerticalStretch(0);
        sizePolicy1.setHeightForWidth(CreateModelButton->sizePolicy().hasHeightForWidth());
        CreateModelButton->setSizePolicy(sizePolicy1);

        horizontalLayout_2->addWidget(CreateModelButton);


        verticalLayout_2->addLayout(horizontalLayout_2);

        horizontalLayout = new QHBoxLayout();
        horizontalLayout->setObjectName(QString::fromUtf8("horizontalLayout"));
        ModelEditorSite = new QVBoxLayout();
        ModelEditorSite->setObjectName(QString::fromUtf8("ModelEditorSite"));
        horizontalSpacer = new QSpacerItem(40, 20, QSizePolicy::Expanding, QSizePolicy::Minimum);

        ModelEditorSite->addItem(horizontalSpacer);


        horizontalLayout->addLayout(ModelEditorSite);

        verticalLayout = new QVBoxLayout();
        verticalLayout->setObjectName(QString::fromUtf8("verticalLayout"));
        ModelPreview = new QLabel(ModelEditForm);
        ModelPreview->setObjectName(QString::fromUtf8("ModelPreview"));
        QSizePolicy sizePolicy2(QSizePolicy::Maximum, QSizePolicy::Maximum);
        sizePolicy2.setHorizontalStretch(0);
        sizePolicy2.setVerticalStretch(0);
        sizePolicy2.setHeightForWidth(ModelPreview->sizePolicy().hasHeightForWidth());
        ModelPreview->setSizePolicy(sizePolicy2);
        ModelPreview->setMinimumSize(QSize(150, 150));
        ModelPreview->setMaximumSize(QSize(150, 150));
        ModelPreview->setPixmap(QPixmap(QString::fromUtf8(":/ltr/clip_1.png")));
        ModelPreview->setScaledContents(true);

        verticalLayout->addWidget(ModelPreview);

        verticalSpacer_2 = new QSpacerItem(20, 10, QSizePolicy::Minimum, QSizePolicy::Preferred);

        verticalLayout->addItem(verticalSpacer_2);


        horizontalLayout->addLayout(verticalLayout);


        verticalLayout_2->addLayout(horizontalLayout);

        verticalSpacer = new QSpacerItem(20, 40, QSizePolicy::Minimum, QSizePolicy::Expanding);

        verticalLayout_2->addItem(verticalSpacer);


        retranslateUi(ModelEditForm);

        QMetaObject::connectSlotsByName(ModelEditForm);
    } // setupUi

    void retranslateUi(QWidget *ModelEditForm)
    {
        ModelEditForm->setWindowTitle(QCoreApplication::translate("ModelEditForm", "Form", nullptr));
        label_23->setText(QCoreApplication::translate("ModelEditForm", "Model Name:", nullptr));
        CreateModelButton->setText(QCoreApplication::translate("ModelEditForm", "Create New", nullptr));
        ModelPreview->setText(QString());
    } // retranslateUi

};

namespace Ui {
    class ModelEditForm: public Ui_ModelEditForm {};
} // namespace Ui

QT_END_NAMESPACE

#endif // UI_MODEL_EDIT_H
