#include "lutris_path_config_dialog.h"
#include <QApplication>
#include <QDir>
#include <QStandardPaths>
#include <QTextStream>
#include <QDebug>
#include <QIcon>

const QString LutrisPathConfigDialog::SETTINGS_GROUP = QString::fromUtf8("LutrisIntegration");
const QString LutrisPathConfigDialog::DATABASE_PATH_KEY = QString::fromUtf8("customDatabasePath");
const QString LutrisPathConfigDialog::CONFIG_PATH_KEY = QString::fromUtf8("customConfigPath");

LutrisPathConfigDialog::LutrisPathConfigDialog(QWidget *parent)
    : QDialog(parent)
    , statusLabel(nullptr)
    , databasePathEdit(nullptr)
    , configPathEdit(nullptr)
    , databaseBrowseButton(nullptr)
    , configBrowseButton(nullptr)
    , resetButton(nullptr)
    , okButton(nullptr)
    , cancelButton(nullptr)
{
    setupUI();
    loadCurrentSettings();
    setWindowIcon(QIcon(QStringLiteral(":/ltr/linuxtrack.svg")));
    setWindowTitle(QStringLiteral("Configure Lutris Paths"));
    setModal(true);
    setMinimumSize(600, 350);
}

LutrisPathConfigDialog::~LutrisPathConfigDialog()
{
    // Destructor implementation - cleanup is handled by Qt parent-child system
}

void LutrisPathConfigDialog::setupUI()
{
    QVBoxLayout *mainLayout = new QVBoxLayout(this);

    // Main message
    QLabel *messageLabel = new QLabel(tr(
        "<h3>Lutris Configuration Files Not Found</h3>"
        "<p>Lutris was detected, but its configuration files were not found at the default locations. "
        "This can happen with non-standard Lutris installations.</p>"
        "<p>Please specify the locations of your Lutris database file and configuration directory.</p>"
    ));
    messageLabel->setWordWrap(true);
    mainLayout->addWidget(messageLabel);

    // Status label
    statusLabel = new QLabel();
    statusLabel->setWordWrap(true);
    mainLayout->addWidget(statusLabel);

    // Database path group
    QGroupBox *dbGroup = new QGroupBox(tr("Database File (pga.db)"));
    QVBoxLayout *dbLayout = new QVBoxLayout(dbGroup);

    QHBoxLayout *dbPathLayout = new QHBoxLayout();
    databasePathEdit = new QLineEdit();
    databasePathEdit->setPlaceholderText(tr("Path to pga.db file"));
    dbPathLayout->addWidget(databasePathEdit);

    databaseBrowseButton = new QPushButton(tr("Browse..."));
    connect(databaseBrowseButton, &QPushButton::clicked, this, &LutrisPathConfigDialog::onDatabaseBrowseClicked);
    dbPathLayout->addWidget(databaseBrowseButton);

    dbLayout->addLayout(dbPathLayout);
    mainLayout->addWidget(dbGroup);

    // Config path group
    QGroupBox *cfgGroup = new QGroupBox(tr("Configuration Directory (games/)"));
    QVBoxLayout *cfgLayout = new QVBoxLayout(cfgGroup);

    QHBoxLayout *cfgPathLayout = new QHBoxLayout();
    configPathEdit = new QLineEdit();
    configPathEdit->setPlaceholderText(tr("Path to games directory"));
    cfgPathLayout->addWidget(configPathEdit);

    configBrowseButton = new QPushButton(tr("Browse..."));
    connect(configBrowseButton, &QPushButton::clicked, this, &LutrisPathConfigDialog::onConfigBrowseClicked);
    cfgPathLayout->addWidget(configBrowseButton);

    cfgLayout->addLayout(cfgPathLayout);
    mainLayout->addWidget(cfgGroup);

    // Button layout
    QHBoxLayout *buttonLayout = new QHBoxLayout();

    resetButton = new QPushButton(tr("Reset to Defaults"));
    connect(resetButton, &QPushButton::clicked, this, &LutrisPathConfigDialog::onResetClicked);
    buttonLayout->addWidget(resetButton);

    buttonLayout->addStretch();

    okButton = new QPushButton(tr("OK"));
    okButton->setDefault(true);
    connect(okButton, &QPushButton::clicked, this, &LutrisPathConfigDialog::onAcceptClicked);
    buttonLayout->addWidget(okButton);

    cancelButton = new QPushButton(tr("Cancel"));
    connect(cancelButton, &QPushButton::clicked, this, &QDialog::reject);
    buttonLayout->addWidget(cancelButton);

    mainLayout->addLayout(buttonLayout);

    validatePaths();
}

void LutrisPathConfigDialog::loadCurrentSettings()
{
    QSettings settings(QStringLiteral("linuxtrack"), QStringLiteral("ltr_gui"));
    settings.beginGroup(SETTINGS_GROUP);

    QString dbPath = settings.value(DATABASE_PATH_KEY).toString();
    QString cfgPath = settings.value(CONFIG_PATH_KEY).toString();

    settings.endGroup();

    if (!dbPath.isEmpty()) {
        databasePathEdit->setText(dbPath);
    }
    if (!cfgPath.isEmpty()) {
        configPathEdit->setText(cfgPath);
    }
}

void LutrisPathConfigDialog::onDatabaseBrowseClicked()
{
    QString currentPath = databasePathEdit->text();
    if (currentPath.isEmpty()) {
        // Default to home directory
        currentPath = QStandardPaths::writableLocation(QStandardPaths::HomeLocation);
    }

    QString filePath = QFileDialog::getOpenFileName(
        this,
        tr("Select Lutris Database File"),
        currentPath,
        tr("SQLite Database (*.db);;All Files (*)")
    );

    if (!filePath.isEmpty()) {
        databasePathEdit->setText(filePath);
        validatePaths();
    }
}

void LutrisPathConfigDialog::onConfigBrowseClicked()
{
    QString currentPath = configPathEdit->text();
    if (currentPath.isEmpty()) {
        // Default to home directory
        currentPath = QStandardPaths::writableLocation(QStandardPaths::HomeLocation);
    }

    QString dirPath = QFileDialog::getExistingDirectory(
        this,
        tr("Select Lutris Configuration Directory"),
        currentPath
    );

    if (!dirPath.isEmpty()) {
        configPathEdit->setText(dirPath);
        validatePaths();
    }
}

void LutrisPathConfigDialog::onResetClicked()
{
    databasePathEdit->clear();
    configPathEdit->clear();
    validatePaths();

    // Clear settings
    QSettings settings(QStringLiteral("linuxtrack"), QStringLiteral("ltr_gui"));
    settings.beginGroup(SETTINGS_GROUP);
    settings.remove(DATABASE_PATH_KEY);
    settings.remove(CONFIG_PATH_KEY);
    settings.endGroup();
}

void LutrisPathConfigDialog::onAcceptClicked()
{
    // Validate paths before accepting
    QString dbPath = databasePathEdit->text().trimmed();
    QString cfgPath = configPathEdit->text().trimmed();

    // Allow empty paths (means use defaults) or validate non-empty paths
    if (!dbPath.isEmpty() && !cfgPath.isEmpty()) {
        // Both paths provided - validate they exist
        QFileInfo dbFile(dbPath);
        if (!dbFile.exists() || !dbFile.isFile()) {
            QMessageBox::warning(this, tr("Invalid Database Path"),
                tr("The specified database file does not exist or is not a valid file."));
            return;
        }

        QDir cfgDir(cfgPath);
        if (!cfgDir.exists()) {
            QMessageBox::warning(this, tr("Invalid Config Path"),
                tr("The specified configuration directory does not exist."));
            return;
        }
    } else if (dbPath.isEmpty() != cfgPath.isEmpty()) {
        // One path provided but not the other
        QMessageBox::warning(this, tr("Invalid Paths"),
            tr("Either specify both database file and configuration directory, or leave both empty to use defaults."));
        return;
    }
    // Empty paths are allowed (means use defaults)

    // Save settings (empty strings will be saved, which the integration class handles)
    QSettings settings(QStringLiteral("linuxtrack"), QStringLiteral("ltr_gui"));
    settings.beginGroup(SETTINGS_GROUP);
    settings.setValue(DATABASE_PATH_KEY, dbPath);
    settings.setValue(CONFIG_PATH_KEY, cfgPath);
    settings.endGroup();

    accept();
}

void LutrisPathConfigDialog::validatePaths()
{
    QString dbPath = databasePathEdit->text().trimmed();
    QString cfgPath = configPathEdit->text().trimmed();

    QString statusText;
    bool isValid = false;

    if (dbPath.isEmpty() && cfgPath.isEmpty()) {
        // Both empty - use defaults
        statusText = tr("<p style='color: blue;'><b>Using default Lutris paths.</b></p>");
        isValid = true;
    } else if (!dbPath.isEmpty() && !cfgPath.isEmpty()) {
        // Both provided - validate they exist
        bool dbValid = QFileInfo(dbPath).exists() && QFileInfo(dbPath).isFile();
        bool cfgValid = QDir(cfgPath).exists();

        if (dbValid && cfgValid) {
            statusText = tr("<p style='color: green;'><b>✓ All paths are valid!</b></p>");
            isValid = true;
        } else {
            statusText = tr("<p style='color: red;'><b>⚠ Invalid paths detected:</b></p>");
            if (!dbValid) {
                statusText += tr("<p>• Database file path is invalid</p>");
            }
            if (!cfgValid) {
                statusText += tr("<p>• Configuration directory path is invalid</p>");
            }
            isValid = false;
        }
    } else {
        // Mixed state - one empty, one not
        statusText = tr("<p style='color: orange;'><b>⚠ Incomplete configuration:</b></p>");
        statusText += tr("<p>Either specify both paths or leave both empty to use defaults.</p>");
        isValid = false;
    }

    statusLabel->setText(statusText);
    okButton->setEnabled(isValid);
}
