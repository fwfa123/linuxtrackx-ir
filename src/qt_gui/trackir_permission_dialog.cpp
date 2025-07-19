#include "trackir_permission_dialog.h"
#include <QApplication>
#include <QDir>
#include <QStandardPaths>
#include <QTextStream>
#include <QDebug>

const QString TrackIRPermissionDialog::CONFIG_FILE = QString::fromUtf8("trackir_permissions.conf");
const QString TrackIRPermissionDialog::DONT_SHOW_KEY = QString::fromUtf8("dont_show_permission_dialog");

TrackIRPermissionDialog::TrackIRPermissionDialog(QWidget *parent)
    : QDialog(parent)
    , dontShowAgainCheckBox(nullptr)
    , installRulesButton(nullptr)
    , skipButton(nullptr)
    , helpButton(nullptr)
    , statusLabel(nullptr)
{
    setupUI();
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
        "<li>Add your user to the 'plugdev' group</li>"
        "<li>Reload udev rules to apply changes</li>"
        "</ul>"
        "<p><b>Note:</b> You will need to log out and log back in for group changes to take effect.</p>"
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
    bool rulesInstalled = QFile::exists(QString::fromUtf8("/lib/udev/rules.d/99-TIR.rules"));
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
           "<pre>sudo cp /path/to/linuxtrack/src/99-TIR.rules /lib/udev/rules.d/\n"
           "sudo udevadm control --reload-rules</pre>"
           "<p><b>2. Add user to plugdev group:</b></p>"
           "<pre>sudo usermod -a -G plugdev $USER</pre>"
           "<p><b>3. Log out and log back in</b></p>"
           "<p><b>4. Test TrackIR access:</b></p>"
           "<pre>lsusb | grep 131d</pre>"
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
    
    // Copy rules to system location using pkexec or sudo
    QStringList arguments;
    arguments << QString::fromUtf8("cp") << tempRulesFile << QString::fromUtf8("/lib/udev/rules.d/99-TIR.rules");
    
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
    arguments << QString::fromUtf8("chmod") << QString::fromUtf8("644") << QString::fromUtf8("/lib/udev/rules.d/99-TIR.rules");
    
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
    QProcess process;
    
    // Get current user
    QString currentUser = QString::fromUtf8(qgetenv("USER"));
    if (currentUser.isEmpty()) {
        currentUser = QString::fromUtf8(qgetenv("USERNAME"));
    }
    
    if (currentUser.isEmpty()) {
        qDebug() << "Could not determine current user";
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
        "# Copy TrackIR rules to system location\n"
        "cp /tmp/99-TIR.rules /lib/udev/rules.d/99-TIR.rules\n"
        "\n"
        "# Copy Mickey rules to system location\n"
        "cp /tmp/99-Mickey.rules /lib/udev/rules.d/99-Mickey.rules\n"
        "\n"
        "# Set proper permissions\n"
        "chmod 644 /lib/udev/rules.d/99-TIR.rules\n"
        "chmod 644 /lib/udev/rules.d/99-Mickey.rules\n"
        "\n"
        "# Add user to plugdev group\n"
        "usermod -a -G plugdev \"%1\"\n"
        "\n"
        "# Add user to uinput group (create group if it doesn't exist)\n"
        "if ! getent group uinput > /dev/null 2>&1; then\n"
        "    groupadd uinput\n"
        "fi\n"
        "usermod -a -G uinput \"%1\"\n"
        "\n"
        "# Reload udev rules\n"
        "udevadm control --reload-rules\n"
        "\n"
        "# Clean up temp files\n"
        "rm -f /tmp/99-TIR.rules\n"
        "rm -f /tmp/99-Mickey.rules\n"
        "\n"
        "echo \"Installation completed successfully\"\n"
    ).arg(currentUser);
    
    // Write script to a temporary file
    QString tempScriptFile = QStandardPaths::writableLocation(QStandardPaths::TempLocation) + QString::fromUtf8("/install_trackir_rules.sh");
    QFile tempScript(tempScriptFile);
    if (!tempScript.open(QIODevice::WriteOnly | QIODevice::Text)) {
        qDebug() << "Failed to create temporary script file:" << tempScript.errorString();
        return false;
    }
    
    QTextStream out(&tempScript);
    out << scriptContent;
    tempScript.close();
    
    // Make script executable
    QFile::setPermissions(tempScriptFile, QFile::permissions(tempScriptFile) | QFile::ExeOwner);
    
    // Run the script with sudo (single password prompt)
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
            return false;
        }
    }
    
    // Clean up temp script
    QFile::remove(tempScriptFile);
    
    if (process.exitCode() != 0) {
        qDebug() << "Failed to install rules and add user to groups:" << process.errorString();
        qDebug() << "Process output:" << QString::fromUtf8(process.readAllStandardOutput());
        qDebug() << "Process error:" << QString::fromUtf8(process.readAllStandardError());
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
    
    // Add user to plugdev group
    QStringList arguments;
    arguments << QString::fromUtf8("usermod") << QString::fromUtf8("-a") << QString::fromUtf8("-G") << QString::fromUtf8("plugdev") << currentUser;
    
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
        qDebug() << "Failed to add user to plugdev group:" << process.errorString();
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
           "<p>1. Log out and log back in</p>"
           "<p>2. Unplug and replug your TrackIR device</p>"
           "<p>Would you like to log out now?</p>"),
        QMessageBox::Yes | QMessageBox::No,
        QMessageBox::Yes);
    
    if (reply == QMessageBox::Yes) {
        // Log out the user
        QProcess::startDetached(QString::fromUtf8("loginctl"), QStringList() << QString::fromUtf8("terminate-user") << QString::fromUtf8(qgetenv("USER")));
        
        // Fallback methods if loginctl fails
        QProcess::startDetached(QString::fromUtf8("pkill"), QStringList() << QString::fromUtf8("-u") << QString::fromUtf8(qgetenv("USER")));
        QProcess::startDetached(QString::fromUtf8("killall"), QStringList() << QString::fromUtf8("-u") << QString::fromUtf8(qgetenv("USER")));
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

#include "moc_trackir_permission_dialog.cpp" 