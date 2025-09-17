#include "about_dialog.h"
#include <QVBoxLayout>
#include <QHBoxLayout>
#include <QLabel>
#include <QPushButton>
#include <QTextEdit>
#include <QScrollArea>
#include <QApplication>
#include <QDateTime>
#include <QDesktopServices>
#include <QUrl>

#ifdef HAVE_CONFIG_H
  #include "../../config.h"
#endif

AboutDialog::AboutDialog(QWidget *parent) : QDialog(parent)
{
    setupUI();
    setWindowTitle(tr("About Linuxtrack X-IR"));
    setModal(true);
    setFixedSize(500, 400);
}

void AboutDialog::setupUI()
{
    setupLayout();
    setupVersionInfo();
    setupCredits();
}

void AboutDialog::setupLayout()
{
    QVBoxLayout *mainLayout = new QVBoxLayout(this);
    
    // Application icon and title
    QHBoxLayout *headerLayout = new QHBoxLayout();
    QLabel *iconLabel = new QLabel();
    iconLabel->setPixmap(QIcon(QStringLiteral(":/ltr/linuxtrack.svg")).pixmap(64, 64));
    iconLabel->setAlignment(Qt::AlignCenter);
    
    QVBoxLayout *titleLayout = new QVBoxLayout();
    QLabel *titleLabel = new QLabel(tr("Linuxtrack X-IR"));
    titleLabel->setStyleSheet(QStringLiteral("font-size: 18px; font-weight: bold;"));
    titleLabel->setAlignment(Qt::AlignCenter);
    
    QLabel *versionLabel = new QLabel(QStringLiteral("Version %1").arg(QStringLiteral(PACKAGE_VERSION)));
    versionLabel->setStyleSheet(QStringLiteral("font-size: 14px; color: #666;"));
    versionLabel->setAlignment(Qt::AlignCenter);
    
    titleLayout->addWidget(titleLabel);
    titleLayout->addWidget(versionLabel);
    
    headerLayout->addWidget(iconLabel);
    headerLayout->addLayout(titleLayout);
    headerLayout->addStretch();
    
    mainLayout->addLayout(headerLayout);
    
    // Separator
    QFrame *separator = new QFrame();
    separator->setFrameShape(QFrame::HLine);
    separator->setFrameShadow(QFrame::Sunken);
    mainLayout->addWidget(separator);
    
    // Description
    QLabel *descriptionLabel = new QLabel(
        tr("A head-tracking software specifically tailored for Linux and supporting TrackIR devices.\n\n"
           "Linuxtrack X-IR allows you to use your TrackIR device as a head-tracking device, "
           "facilitating view control in simulators (flight, racing) or any other type of software.")
    );
    descriptionLabel->setWordWrap(true);
    descriptionLabel->setAlignment(Qt::AlignJustify);
    mainLayout->addWidget(descriptionLabel);
    
    // Credits section
    QLabel *creditsLabel = new QLabel(tr("Credits:"));
    creditsLabel->setStyleSheet(QStringLiteral("font-weight: bold; margin-top: 10px;"));
    mainLayout->addWidget(creditsLabel);
    
    QTextEdit *creditsText = new QTextEdit();
    creditsText->setReadOnly(true);
    creditsText->setMaximumHeight(120);
    creditsText->setHtml(
        tr("<p><b>Original Author:</b> UglyDwarf - Created Linuxtrack</p>"
           "<p><b>Maintainer:</b> fwfa123 - Maintaining LinuxtrackX-IR</p>"
           "<p><b>Build Date:</b> %1</p>"
           "<p><b>Qt Version:</b> %2</p>")
        .arg(QDateTime::currentDateTime().toString(QStringLiteral("yyyy-MM-dd")))
        .arg(QStringLiteral(QT_VERSION_STR))
    );
    mainLayout->addWidget(creditsText);
    
    // Links
    QHBoxLayout *linksLayout = new QHBoxLayout();
    
    QPushButton *projectButton = new QPushButton(tr("Project Page"));
    connect(projectButton, &QPushButton::clicked, []() {
        QDesktopServices::openUrl(QUrl(QStringLiteral("https://gitlab.com/fwfa123/linuxtrackx-ir")));
    });
    
    QPushButton *issuesButton = new QPushButton(tr("Report Issues"));
    connect(issuesButton, &QPushButton::clicked, []() {
        QDesktopServices::openUrl(QUrl(QStringLiteral("https://gitlab.com/fwfa123/linuxtrackx-ir/-/issues")));
    });
    
    linksLayout->addWidget(projectButton);
    linksLayout->addWidget(issuesButton);
    linksLayout->addStretch();
    
    mainLayout->addLayout(linksLayout);
    
    // OK button
    QHBoxLayout *buttonLayout = new QHBoxLayout();
    buttonLayout->addStretch();
    QPushButton *okButton = new QPushButton(tr("OK"));
    okButton->setDefault(true);
    connect(okButton, &QPushButton::clicked, this, &QDialog::accept);
    buttonLayout->addWidget(okButton);
    buttonLayout->addStretch();
    
    mainLayout->addLayout(buttonLayout);
}

void AboutDialog::setupVersionInfo()
{
    // Version info is set up in setupLayout()
}

void AboutDialog::setupCredits()
{
    // Credits are set up in setupLayout()
}
