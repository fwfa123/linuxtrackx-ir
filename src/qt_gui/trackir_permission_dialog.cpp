#include "trackir_permission_dialog.h"
#include <QApplication>
#include <QDir>
#include <QStandardPaths>
#include <QTextStream>
#include <QDebug>

const QString TrackIRPermissionDialog::CONFIG_FILE = QString::fromUtf8("trackir_permissions.conf");
const QString TrackIRPermissionDialog::DONT_SHOW_KEY = QString::fromUtf8("dont_show_permission_dialog");

// Use /etc/udev/rules.d for local admin rules (highest priority)
static QString udevRulesDir()
{
    return QString::fromUtf8("/etc/udev/rules.d");
}

static bool isUdevRulesInstalled()
{
    // Check /etc first (primary location), then legacy locations for backward compatibility
    return QFile::exists(QString::fromUtf8("/etc/udev/rules.d/99-TIR.rules"))
        || QFile::exists(QString::fromUtf8("/usr/lib/udev/rules.d/99-TIR.rules"))
        || QFile::exists(QString::fromUtf8("/lib/udev/rules.d/99-TIR.rules"));
}

TrackIRPermissionDialog::TrackIRPermissionDialog(QWidget *parent)
    : QDialog(parent)
    , dontShowAgainCheckBox(nullptr)
    , installRulesButton(nullptr)
    , skipButton(nullptr)
    , helpButton(nullptr)
    , statusLabel(nullptr)
{
    setupUI();
    setWindowIcon(QIcon(QStringLiteral(":/ltr/linuxtrack.svg")));
    setWindowTitle(tr("TrackIR Permission Setup"));
    setModal(true);
    setFixedSize(500, 300);
}

TrackIRPermissionDialog::~TrackIRPermissionDialog()
{
}

void TrackIRPermissionDialog::setupUI()
{
    QVBoxLayout *mainLayout = new QVBoxLayout(this);
    
    // Main message
    QLabel *messageLabel = new QLabel(tr(
        "<h3>TrackIR Device Detected</h3>"
        "<p>LinuxTrack has detected your TrackIR device, but you don't have "
        "permissions to access it. This can be fixed by installing udev rules "
        "and adding your user to the required groups.</p>"
        "<p><b>What this will do:</b></p>"
        "<ul>"
        "<li>Install udev rules to allow access to TrackIR devices</li>"
        "<li>Add your user to the required groups (plugdev, input, uinput)</li>"
        "<li>Reload udev rules to apply changes</li>"
        "</ul>"
        "<p><b>Note:</b> You will need to <b>reboot your system</b> for group changes to take effect.</p>"
    ));
    messageLabel->setWordWrap(true);
    messageLabel->setOpenExternalLinks(true);
    mainLayout->addWidget(messageLabel);
    
    // Status label
    statusLabel = new QLabel();
    statusLabel->setWordWrap(true);
    mainLayout->addWidget(statusLabel);
    
    // Checkbox for "don't show again"
    dontShowAgainCheckBox = new QCheckBox(tr("Don't show this warning again"));
    mainLayout->addWidget(dontShowAgainCheckBox);
    
    // Button layout
    QHBoxLayout *buttonLayout = new QHBoxLayout();
    
    // Install rules button
    installRulesButton = new QPushButton(tr("Install Rules & Add to Groups"));
    installRulesButton->setDefault(true);
    connect(installRulesButton, &QPushButton::clicked, this, &TrackIRPermissionDialog::onInstallRulesClicked);
    buttonLayout->addWidget(installRulesButton);
    
    // Skip button
    skipButton = new QPushButton(tr("Skip for Now"));
    connect(skipButton, &QPushButton::clicked, this, &TrackIRPermissionDialog::onSkipClicked);
    buttonLayout->addWidget(skipButton);
    
    // Help button
    helpButton = new QPushButton(tr("Help"));
    connect(helpButton, &QPushButton::clicked, this, &TrackIRPermissionDialog::onHelpClicked);
    buttonLayout->addWidget(helpButton);
    
    mainLayout->addLayout(buttonLayout);
    
    // Check current status
    updateStatus();
}

void TrackIRPermissionDialog::updateStatus()
{
    QString status;
    bool rulesInstalled = isUdevRulesInstalled();
    bool userInGroup = checkIfUserInGroup(QString::fromUtf8("plugdev"));
    
    if (rulesInstalled && userInGroup) {
        status = tr("<p style='color: green;'><b>✓ All permissions are properly configured!</b></p>");
        installRulesButton->setEnabled(false);
        installRulesButton->setText(tr("Already Configured"));
    } else {
        status = tr("<p style='color: orange;'><b>⚠ Configuration needed:</b></p>");
        if (!rulesInstalled) {
            status += tr("<p>• udev rules not installed</p>");
        }
        if (!userInGroup) {
            status += tr("<p>• User not in plugdev group</p>");
        }
    }
    
    statusLabel->setText(status);
}

void TrackIRPermissionDialog::onInstallRulesClicked()
{
    statusLabel->setText(tr("<p style='color: blue;'>Installing udev rules and configuring groups...</p>"));
    QApplication::processEvents();
    
    bool success = installUdevRulesAndGroups();
    
    if (success) {
        updateStatus();
        showLogoutDialog();
    } else {
        QString message = tr("Installation failed. Please try manual installation or contact support.");
        showInstallationResult(false, message);
    }
}

void TrackIRPermissionDialog::onSkipClicked()
{
    if (dontShowAgainCheckBox->isChecked()) {
        setDialogShown();
    }
    accept();
}

void TrackIRPermissionDialog::onHelpClicked()
{
    QMessageBox::information(this, tr("TrackIR Permission Help"),
        tr("<h3>Manual Setup Instructions</h3>"
           "<p>If the automatic setup doesn't work, you can configure permissions manually:</p>"
           "<p><b>1. Install udev rules:</b></p>"
           "<pre>sudo cp /path/to/linuxtrack/src/99-TIR.rules /etc/udev/rules.d/\n"
           "sudo udevadm control --reload-rules</pre>"
           "<p><b>2. Add user to required groups:</b></p>"
           "<pre>sudo usermod -a -G plugdev,input,uinput $USER</pre>"
           "<p><b>3. Reboot your system</b></p>"
           "<p><b>4. Test TrackIR access:</b></p>"
           "<pre>lsusb | grep 131d</pre>"
           "<p><b>Note:</b> You may need to create a 99-TIR.rules file with the content:</p>"
           "<pre>SUBSYSTEM==\"usb\", ATTRS{idVendor}==\"131d\", MODE=\"0666\"</pre>"
           "<p>For more detailed help, see the LinuxTrack documentation."));
}

bool TrackIRPermissionDialog::installUdevRules()
{
    QProcess process;
    
    // Create the udev rules content
    QString rulesContent = QString::fromUtf8("SUBSYSTEM==\"usb\", ATTRS{idVendor}==\"131d\", MODE=\"0666\"\n");
    
    // Write rules to a temporary file
    QString tempRulesFile = QStandardPaths::writableLocation(QStandardPaths::TempLocation) + QString::fromUtf8("/99-TIR.rules");
    QFile tempFile(tempRulesFile);
    if (!tempFile.open(QIODevice::WriteOnly | QIODevice::Text)) {
        qDebug() << "Failed to create temporary rules file:" << tempFile.errorString();
        return false;
    }
    
    QTextStream out(&tempFile);
    out << rulesContent;
    tempFile.close();
    
    // Copy rules to system location using pkexec or sudo (prefer FHS path if it exists)
    QString rulesDest = udevRulesDir() + QString::fromUtf8("/99-TIR.rules");
    QStringList arguments;
    arguments << QString::fromUtf8("cp") << tempRulesFile << rulesDest;
    
    process.start(QString::fromUtf8("pkexec"), arguments);
    if (!process.waitForFinished(30000)) { // 30 second timeout
        qDebug() << "pkexec failed, trying sudo...";
        process.start(QString::fromUtf8("sudo"), arguments);
        if (!process.waitForFinished(30000)) {
            qDebug() << "sudo also failed";
            return false;
        }
    }
    
    if (process.exitCode() != 0) {
        qDebug() << "Failed to install udev rules:" << process.errorString();
        return false;
    }
    
    // Set proper permissions
    arguments.clear();
    arguments << QString::fromUtf8("chmod") << QString::fromUtf8("644") << rulesDest;
    
    process.start(QString::fromUtf8("pkexec"), arguments);
    if (!process.waitForFinished(10000)) {
        process.start(QString::fromUtf8("sudo"), arguments);
        process.waitForFinished(10000);
    }
    
    // Reload udev rules
    arguments.clear();
    arguments << QString::fromUtf8("udevadm") << QString::fromUtf8("control") << QString::fromUtf8("--reload-rules");
    
    process.start(QString::fromUtf8("pkexec"), arguments);
    if (!process.waitForFinished(10000)) {
        process.start(QString::fromUtf8("sudo"), arguments);
        process.waitForFinished(10000);
    }
    
    // Clean up temp file
    QFile::remove(tempRulesFile);
    
    return true;
}

bool TrackIRPermissionDialog::installUdevRulesAndGroups()
{
    // Show custom sudo dialog with manual instructions
    QString instructions = tr(
        "TrackIR Setup Instructions:\n\n"
        "The automatic installation will:\n"
        "• Install udev rules for TrackIR devices\n"
        "• Add your user to required groups (plugdev, input, uinput)\n"
        "• Reload udev rules to apply changes\n\n"
        "If you prefer to do this manually, you can:\n\n"
        "1. Install udev rules:\n"
        "   sudo cp /path/to/linuxtrack/src/99-TIR.rules /etc/udev/rules.d/\n"
        "   sudo udevadm control --reload-rules\n\n"
        "2. Add user to groups:\n"
        "   sudo usermod -a -G plugdev,input,uinput $USER\n\n"
        "3. <b>Reboot your system</b> for changes to take effect\n\n"
        "4. Test TrackIR access:\n"
        "   lsusb | grep 131d\n\n"
        "Note: You may need to create a 99-TIR.rules file with the content:\n"
        "   SUBSYSTEM==\"usb\", ATTRS{idVendor}==\"131d\", MODE=\"0666\"\n\n"
        "For more help, see the LinuxTrack documentation."
    );

    SudoPasswordDialog dialog(tr("TrackIR Setup"), instructions, this);

    if (dialog.exec() != QDialog::Accepted) {
        // User chose to cancel and do it manually
        showInstallationResult(false, tr("Installation cancelled. Please follow the manual instructions shown in the dialog above."));
        return false;
    }

    if (!dialog.shouldInstallAutomatically()) {
        // User wants to do it manually but confirmed they saw instructions
        showInstallationResult(false, tr("Please follow the manual instructions to complete the setup."));
        return false;
    }

    // Get current user
    QString currentUser = QString::fromUtf8(qgetenv("USER"));
    if (currentUser.isEmpty()) {
        currentUser = QString::fromUtf8(qgetenv("USERNAME"));
    }

    if (currentUser.isEmpty()) {
        qDebug() << "Could not determine current user";
        showInstallationResult(false, tr("Could not determine current user. Please try manual installation."));
        return false;
    }

    // Create a temporary script that does everything in one sudo session
    QString scriptContent = QString::fromUtf8(
        "#!/bin/bash\n"
        "set -e\n"
        "\n"
        "# Create TrackIR udev rules content\n"
        "cat > /tmp/99-TIR.rules << 'EOF'\n"
        "SUBSYSTEM==\"usb\", ATTRS{idVendor}==\"131d\", MODE=\"0666\"\n"
        "EOF\n"
        "\n"
        "# Create Mickey udev rules content\n"
        "cat > /tmp/99-Mickey.rules << 'EOF'\n"
        "KERNEL==\"uinput\", GROUP=\"uinput\", MODE=\"0660\"\n"
        "EOF\n"
        "\n"
        "# Copy TrackIR rules to local admin location (/etc/udev/rules.d)\n"
        "cp /tmp/99-TIR.rules %1/99-TIR.rules\n"
        "\n"
        "# Copy Mickey rules to system location\n"
        "cp /tmp/99-Mickey.rules %1/99-Mickey.rules\n"
        "\n"
        "# Set proper permissions\n"
        "chmod 644 %1/99-TIR.rules\n"
        "chmod 644 %1/99-Mickey.rules\n"
        "\n"
        "# Create plugdev group if it doesn't exist (common on Arch Linux)\n"
        "if ! getent group plugdev > /dev/null 2>&1; then\n"
        "    groupadd plugdev\n"
        "    echo \"Created plugdev group\"\n"
        "fi\n"
        "\n"
        "# Add user to plugdev group\n"
        "usermod -a -G plugdev,input \"%2\"\n"
        "\n"
        "# Create uinput group if it doesn't exist (as system group for udev)\n"
        "if ! getent group uinput > /dev/null 2>&1; then\n"
        "    groupadd -r uinput\n"
        "    echo \"Created uinput system group\"\n"
        "elif [ $(getent group uinput | cut -d: -f3) -ge 1000 ]; then\n"
        "    # Group exists but is not a system group - recreate it\n"
        "    groupdel uinput 2>/dev/null || true\n"
        "    groupadd -r uinput\n"
        "    echo \"Recreated uinput as system group\"\n"
        "fi\n"
        "\n"
        "# Add user to uinput group\n"
        "usermod -a -G uinput \"%2\"\n"
        "\n"
        "# On Arch Linux, also try adding to video group for OpenGL access\n"
        "if getent group video > /dev/null 2>&1; then\n"
        "    usermod -a -G video \"%2\"\n"
        "    echo \"Added user to video group for OpenGL access\"\n"
        "fi\n"
        "\n"
        "# Reload udev rules\n"
        "udevadm control --reload-rules\n"
        "\n"
        "# Clean up temp files\n"
        "rm -f /tmp/99-TIR.rules\n"
        "rm -f /tmp/99-Mickey.rules\n"
        "\n"
        "echo \"Installation completed successfully\"\n"
    ).arg(udevRulesDir()).arg(currentUser);

    // Write script to a temporary file
    QString tempScriptFile = QStandardPaths::writableLocation(QStandardPaths::TempLocation) + QString::fromUtf8("/install_trackir_rules.sh");
    QFile tempScript(tempScriptFile);
    if (!tempScript.open(QIODevice::WriteOnly | QIODevice::Text)) {
        qDebug() << "Failed to create temporary script file:" << tempScript.errorString();
        showInstallationResult(false, tr("Failed to create installation script. Please try manual installation."));
        return false;
    }

    QTextStream out(&tempScript);
    out << scriptContent;
    tempScript.close();

    // Make script executable
    QFile::setPermissions(tempScriptFile, QFile::permissions(tempScriptFile) | QFile::ExeOwner);

    // Run the script with sudo using the provided password
    QProcess process;
    QStringList arguments;
    arguments << tempScriptFile;

    qDebug() << "Running installation script with pkexec:" << tempScriptFile;
    process.start(QString::fromUtf8("pkexec"), arguments);
    if (!process.waitForFinished(60000)) { // 60 second timeout
        qDebug() << "pkexec failed, trying sudo...";
        process.start(QString::fromUtf8("sudo"), arguments);
        if (!process.waitForFinished(60000)) {
            qDebug() << "sudo also failed";
            QFile::remove(tempScriptFile);
            showInstallationResult(false, tr("Both pkexec and sudo failed. Please try manual installation."));
            return false;
        }
    }

    // Clean up temp script
    QFile::remove(tempScriptFile);

    if (process.exitCode() != 0) {
        qDebug() << "Failed to install rules and add user to groups:" << process.errorString();
        qDebug() << "Process output:" << QString::fromUtf8(process.readAllStandardOutput());
        qDebug() << "Process error:" << QString::fromUtf8(process.readAllStandardError());
        showInstallationResult(false, tr("Installation failed. Please try manual installation."));
        return false;
    }

    return true;
}

bool TrackIRPermissionDialog::addUserToGroups()
{
    QProcess process;
    QString currentUser = QString::fromUtf8(qgetenv("USER"));
    if (currentUser.isEmpty()) {
        currentUser = QString::fromUtf8(qgetenv("USERNAME"));
    }
    
    if (currentUser.isEmpty()) {
        qDebug() << "Could not determine current user";
        return false;
    }
    
    // Add user to plugdev and input groups
    QStringList arguments;
    arguments << QString::fromUtf8("usermod") << QString::fromUtf8("-a") << QString::fromUtf8("-G") << QString::fromUtf8("plugdev,input") << currentUser;
    
    process.start(QString::fromUtf8("pkexec"), arguments);
    if (!process.waitForFinished(30000)) {
        qDebug() << "pkexec failed, trying sudo...";
        process.start(QString::fromUtf8("sudo"), arguments);
        if (!process.waitForFinished(30000)) {
            qDebug() << "sudo also failed";
            return false;
        }
    }
    
    if (process.exitCode() != 0) {
        qDebug() << "Failed to add user to plugdev and input groups:" << process.errorString();
        return false;
    }
    
    return true;
}

bool TrackIRPermissionDialog::checkIfUserInGroup(const QString &group)
{
    QProcess process;
    QString currentUser = QString::fromUtf8(qgetenv("USER"));
    if (currentUser.isEmpty()) {
        currentUser = QString::fromUtf8(qgetenv("USERNAME"));
    }
    
    if (currentUser.isEmpty()) {
        return false;
    }
    
    process.start(QString::fromUtf8("groups"), QStringList() << currentUser);
    if (!process.waitForFinished(5000)) {
        return false;
    }
    
    QString output = QString::fromUtf8(process.readAllStandardOutput());
    return output.contains(group);
}

void TrackIRPermissionDialog::showInstallationResult(bool success, const QString &message)
{
    if (success) {
        QMessageBox::information(this, tr("Installation Complete"), message);
    } else {
        QMessageBox::warning(this, tr("Installation Failed"), 
            tr("Some operations failed:\n\n%1\n\nPlease try manual installation or contact support.").arg(message));
    }
}

void TrackIRPermissionDialog::showLogoutDialog()
{
    QMessageBox::StandardButton reply = QMessageBox::question(this, tr("Installation Complete"),
        tr("<h3>TrackIR permissions have been successfully configured!</h3>"
           "<p>The TrackIR and Mickey udev rules have been installed and your user has been added to the required groups.</p>"
           "<p><b>For these changes to take effect, you need to:</b></p>"
           "<p>1. <b>Reboot your system</b></p>"
           "<p>2. Unplug and replug your TrackIR device</p>"
           "<p>Would you like to reboot now?</p>"),
        QMessageBox::Yes | QMessageBox::No,
        QMessageBox::Yes);

    if (reply == QMessageBox::Yes) {
        // Reboot the system
        QProcess::startDetached(QString::fromUtf8("systemctl"), QStringList() << QString::fromUtf8("reboot"));

        // Fallback methods if systemctl reboot fails
        QProcess::startDetached(QString::fromUtf8("reboot"), QStringList());
        QProcess::startDetached(QString::fromUtf8("shutdown"), QStringList() << QString::fromUtf8("-r") << QString::fromUtf8("now"));
    }
}

bool TrackIRPermissionDialog::shouldShowDialog()
{
    QSettings settings(QStandardPaths::writableLocation(QStandardPaths::ConfigLocation) + QString::fromUtf8("/linuxtrack/") + CONFIG_FILE, QSettings::IniFormat);
    return !settings.value(DONT_SHOW_KEY, false).toBool();
}

void TrackIRPermissionDialog::setDialogShown()
{
    QSettings settings(QStandardPaths::writableLocation(QStandardPaths::ConfigLocation) + QString::fromUtf8("/linuxtrack/") + CONFIG_FILE, QSettings::IniFormat);
    settings.setValue(DONT_SHOW_KEY, true);
    settings.sync();
}

// SudoPasswordDialog implementation
SudoPasswordDialog::SudoPasswordDialog(const QString &title, const QString &instructions, QWidget *parent)
    : QDialog(parent)
    , instructionsText(nullptr)
    , installCheckBox(nullptr)
    , okButton(nullptr)
    , cancelButton(nullptr)
    , installAutomatically(false)
{
    setupUI(title, instructions);
    setWindowIcon(QIcon(QStringLiteral(":/ltr/linuxtrack.svg")));
    setWindowTitle(title);
    setModal(true);
    setFixedSize(600, 500);
}

SudoPasswordDialog::~SudoPasswordDialog()
{
}

void SudoPasswordDialog::setupUI(const QString &title, const QString &instructions)
{
    QVBoxLayout *mainLayout = new QVBoxLayout(this);

    // Title
    QLabel *titleLabel = new QLabel(QStringLiteral("<h3>%1</h3>").arg(title));
    titleLabel->setAlignment(Qt::AlignCenter);
    mainLayout->addWidget(titleLabel);

    // Instructions text (read-only)
    instructionsText = new QTextEdit();
    instructionsText->setPlainText(instructions);
    instructionsText->setReadOnly(true);
    instructionsText->setMaximumHeight(300);
    mainLayout->addWidget(instructionsText);

    // Checkbox for automatic installation
    installCheckBox = new QCheckBox(tr("I understand and want to proceed with automatic installation"));
    installCheckBox->setChecked(false);
    mainLayout->addWidget(installCheckBox);

    // Buttons
    QHBoxLayout *buttonLayout = new QHBoxLayout();

    okButton = new QPushButton(tr("Proceed with Automatic Installation"));
    okButton->setDefault(true);
    okButton->setEnabled(false); // Disabled until checkbox is checked
    connect(okButton, &QPushButton::clicked, this, &SudoPasswordDialog::onOkClicked);
    buttonLayout->addWidget(okButton);

    cancelButton = new QPushButton(tr("Cancel - I'll do it manually"));
    connect(cancelButton, &QPushButton::clicked, this, &SudoPasswordDialog::onCancelClicked);
    buttonLayout->addWidget(cancelButton);

    mainLayout->addLayout(buttonLayout);

    // Connect checkbox to enable/disable OK button
    connect(installCheckBox, &QCheckBox::stateChanged, this, [this](int state) {
        okButton->setEnabled(state == Qt::Checked);
    });
}

void SudoPasswordDialog::onOkClicked()
{
    installAutomatically = installCheckBox->isChecked();
    accept();
}

void SudoPasswordDialog::onCancelClicked()
{
    installAutomatically = false;
    reject();
}

