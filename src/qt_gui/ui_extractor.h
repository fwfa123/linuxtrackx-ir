/********************************************************************************
** Form generated from reading UI file 'extractor.ui'
**
** Created by: Qt User Interface Compiler version 5.15.17
**
** WARNING! All changes made in this file will be lost when recompiling UI file!
********************************************************************************/

#ifndef UI_EXTRACTOR_H
#define UI_EXTRACTOR_H

#include <QtCore/QVariant>
#include <QtWidgets/QApplication>
#include <QtWidgets/QComboBox>
#include <QtWidgets/QFrame>
#include <QtWidgets/QHBoxLayout>
#include <QtWidgets/QLabel>
#include <QtWidgets/QPlainTextEdit>
#include <QtWidgets/QPushButton>
#include <QtWidgets/QVBoxLayout>
#include <QtWidgets/QWidget>

QT_BEGIN_NAMESPACE

class Ui_Form
{
public:
    QVBoxLayout *verticalLayout;
    QFrame *frame;
    QVBoxLayout *verticalLayout_2;
    QLabel *label_2;
    QHBoxLayout *horizontalLayout_2;
    QComboBox *FWCombo;
    QPushButton *DownloadButton;
    QLabel *label;
    QPlainTextEdit *LogView;
    QHBoxLayout *horizontalLayout_3;
    QPushButton *HelpButton;
    QPushButton *AnalyzeSourceButton;
    QPushButton *BrowseInstaller;
    QPushButton *BrowseDir;
    QPushButton *QuitButton;

    void setupUi(QWidget *Form)
    {
        if (Form->objectName().isEmpty())
            Form->setObjectName(QString::fromUtf8("Form"));
        Form->resize(588, 337);
        verticalLayout = new QVBoxLayout(Form);
        verticalLayout->setObjectName(QString::fromUtf8("verticalLayout"));
        frame = new QFrame(Form);
        frame->setObjectName(QString::fromUtf8("frame"));
        frame->setFrameShape(QFrame::StyledPanel);
        frame->setFrameShadow(QFrame::Raised);
        verticalLayout_2 = new QVBoxLayout(frame);
        verticalLayout_2->setObjectName(QString::fromUtf8("verticalLayout_2"));
        label_2 = new QLabel(frame);
        label_2->setObjectName(QString::fromUtf8("label_2"));
        QFont font;
        font.setBold(true);
        font.setWeight(75);
        label_2->setFont(font);

        verticalLayout_2->addWidget(label_2);

        horizontalLayout_2 = new QHBoxLayout();
        horizontalLayout_2->setObjectName(QString::fromUtf8("horizontalLayout_2"));
        FWCombo = new QComboBox(frame);
        FWCombo->setObjectName(QString::fromUtf8("FWCombo"));
        FWCombo->setEditable(true);

        horizontalLayout_2->addWidget(FWCombo);

        DownloadButton = new QPushButton(frame);
        DownloadButton->setObjectName(QString::fromUtf8("DownloadButton"));
        QSizePolicy sizePolicy(QSizePolicy::Maximum, QSizePolicy::Fixed);
        sizePolicy.setHorizontalStretch(0);
        sizePolicy.setVerticalStretch(0);
        sizePolicy.setHeightForWidth(DownloadButton->sizePolicy().hasHeightForWidth());
        DownloadButton->setSizePolicy(sizePolicy);

        horizontalLayout_2->addWidget(DownloadButton);


        verticalLayout_2->addLayout(horizontalLayout_2);

        label = new QLabel(frame);
        label->setObjectName(QString::fromUtf8("label"));
        label->setFont(font);

        verticalLayout_2->addWidget(label);

        LogView = new QPlainTextEdit(frame);
        LogView->setObjectName(QString::fromUtf8("LogView"));
        LogView->setLineWrapMode(QPlainTextEdit::NoWrap);
        LogView->setReadOnly(true);

        verticalLayout_2->addWidget(LogView);


        verticalLayout->addWidget(frame);

        horizontalLayout_3 = new QHBoxLayout();
        horizontalLayout_3->setObjectName(QString::fromUtf8("horizontalLayout_3"));
        HelpButton = new QPushButton(Form);
        HelpButton->setObjectName(QString::fromUtf8("HelpButton"));
        QSizePolicy sizePolicy1(QSizePolicy::Minimum, QSizePolicy::Minimum);
        sizePolicy1.setHorizontalStretch(0);
        sizePolicy1.setVerticalStretch(0);
        sizePolicy1.setHeightForWidth(HelpButton->sizePolicy().hasHeightForWidth());
        HelpButton->setSizePolicy(sizePolicy1);

        horizontalLayout_3->addWidget(HelpButton);

        AnalyzeSourceButton = new QPushButton(Form);
        AnalyzeSourceButton->setObjectName(QString::fromUtf8("AnalyzeSourceButton"));

        horizontalLayout_3->addWidget(AnalyzeSourceButton);

        BrowseInstaller = new QPushButton(Form);
        BrowseInstaller->setObjectName(QString::fromUtf8("BrowseInstaller"));

        horizontalLayout_3->addWidget(BrowseInstaller);

        BrowseDir = new QPushButton(Form);
        BrowseDir->setObjectName(QString::fromUtf8("BrowseDir"));

        horizontalLayout_3->addWidget(BrowseDir);

        QuitButton = new QPushButton(Form);
        QuitButton->setObjectName(QString::fromUtf8("QuitButton"));
        sizePolicy1.setHeightForWidth(QuitButton->sizePolicy().hasHeightForWidth());
        QuitButton->setSizePolicy(sizePolicy1);

        horizontalLayout_3->addWidget(QuitButton);


        verticalLayout->addLayout(horizontalLayout_3);

        QWidget::setTabOrder(FWCombo, DownloadButton);
        QWidget::setTabOrder(DownloadButton, LogView);
        QWidget::setTabOrder(LogView, HelpButton);
        QWidget::setTabOrder(HelpButton, AnalyzeSourceButton);
        QWidget::setTabOrder(AnalyzeSourceButton, BrowseInstaller);
        QWidget::setTabOrder(BrowseInstaller, QuitButton);

        retranslateUi(Form);

        QMetaObject::connectSlotsByName(Form);
    } // setupUi

    void retranslateUi(QWidget *Form)
    {
        Form->setWindowTitle(QCoreApplication::translate("Form", "Extractor", nullptr));
        label_2->setText(QCoreApplication::translate("Form", "Select Package to Download (any of them should do)", nullptr));
        DownloadButton->setText(QCoreApplication::translate("Form", "Download", nullptr));
        label->setText(QCoreApplication::translate("Form", "Progress information", nullptr));
        HelpButton->setText(QCoreApplication::translate("Form", "Help", nullptr));
        AnalyzeSourceButton->setText(QCoreApplication::translate("Form", "Extract Firmware\n"
"(developer)", nullptr));
        BrowseInstaller->setText(QCoreApplication::translate("Form", "Extract from\n"
"installer", nullptr));
        BrowseDir->setText(QCoreApplication::translate("Form", "Extract from\n"
"unpacked", nullptr));
        QuitButton->setText(QCoreApplication::translate("Form", "Close", nullptr));
    } // retranslateUi

};

namespace Ui {
    class Form: public Ui_Form {};
} // namespace Ui

QT_END_NAMESPACE

#endif // UI_EXTRACTOR_H
