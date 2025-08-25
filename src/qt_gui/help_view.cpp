#ifdef HAVE_CONFIG_H
  #include "../../config.h"
#endif

#include <QSettings>
#include <QRegExp>
#include <QDesktopServices>
#include <QHelpEngine>
#include <QHelpContentWidget>
#include <QSplitter>
#include <QDir>
#include <QApplication>
#include <QStandardPaths>
#include <QPluginLoader>
#include <QSqlDatabase>
#include <QSqlError>
#include <QSqlQuery>
#include <QFileInfo>
#include <QDebug>
#include <QMessageBox>
#include <QProcessEnvironment>

#include "ltr_gui_prefs.h"
#include "help_view.h"
#include "help_viewer.h"
#include <iostream>

HelpViewer *HelpViewer::hlp = NULL;

HelpViewer &HelpViewer::getHlp()
{
  if(hlp == NULL){
    hlp = new HelpViewer();
  }
  return *hlp;
}

void HelpViewer::ShowWindow()
{
  getHlp().show();
}

void HelpViewer::RaiseWindow()
{
  getHlp().raise();
  getHlp().activateWindow();
}

void HelpViewer::ChangePage(QString name)
{
  getHlp().ChangeHelpPage(name);
}

void HelpViewer::ChangeHelpPage(QString name)
{
  QString tmp = QString::fromUtf8("qthelp://uglyDwarf.com.linuxtrack.1.0/doc/help/") + name;
  viewer->setSource(QUrl(tmp));
}

void HelpViewer::CloseWindow()
{
  getHlp().close();
}

void HelpViewer::LoadPrefs(QSettings &settings)
{
  HelpViewer &hv = getHlp();
  settings.beginGroup(QString::fromUtf8("HelpWindow"));
  hv.resize(settings.value(QString::fromUtf8("size"), QSize(800, 600)).toSize());
  hv.move(settings.value(QString::fromUtf8("pos"), QPoint(0, 0)).toPoint());
  settings.endGroup();
}

void HelpViewer::StorePrefs(QSettings &settings)
{
  HelpViewer &hv = getHlp();
  settings.beginGroup(QString::fromUtf8("HelpWindow"));
  settings.setValue(QString::fromUtf8("size"), hv.size());
  settings.setValue(QString::fromUtf8("pos"), hv.pos());
  settings.endGroup();
}

// Robust help system initialization with comprehensive error handling and compatibility validation
bool HelpViewer::initializeHelpSystem(QString &helpFile, QHelpEngine *&helpEngine)
{
    std::cout << "=== LinuxTrack Help System Initialization ===" << std::endl;
    
    // Step 1: Verify Qt environment and version compatibility
    std::cout << "1. Checking Qt environment and version compatibility..." << std::endl;
    std::cout << "   Qt version: " << qVersion() << std::endl;
    std::cout << "   Application path: " << QApplication::applicationDirPath().toStdString() << std::endl;
    
    // Check Qt version compatibility
    QString currentQtVersion = QString::fromUtf8(qVersion());
    if (currentQtVersion.startsWith(QStringLiteral("5."))) {
        std::cout << "   ✅ Qt5 detected - compatible with help system" << std::endl;
    } else if (currentQtVersion.startsWith(QStringLiteral("6."))) {
        std::cout << "   ⚠️  Qt6 detected - may have compatibility issues with Qt5 help files" << std::endl;
    } else {
        std::cout << "   ❓ Unknown Qt version - compatibility uncertain" << std::endl;
    }
    
    // Step 2: Check Qt plugin paths
    std::cout << "2. Checking Qt plugin paths..." << std::endl;
    QStringList pluginPaths = QApplication::libraryPaths();
    for (const QString &path : pluginPaths) {
        std::cout << "   Plugin path: " << path.toStdString() << std::endl;
    }
    
    // Step 3: Check environment variables
    std::cout << "3. Checking environment variables..." << std::endl;
    QProcessEnvironment env = QProcessEnvironment::systemEnvironment();
    QString qtPluginPath = env.value(QStringLiteral("QT_PLUGIN_PATH"));
    QString qtSqlDriverPath = env.value(QStringLiteral("QT_SQL_DRIVER_PATH"));
    QString qtHelpPath = env.value(QStringLiteral("QT_HELP_PATH"));
    
    std::cout << "   QT_PLUGIN_PATH: " << (qtPluginPath.isEmpty() ? "NOT SET" : qtPluginPath.toStdString()) << std::endl;
    std::cout << "   QT_SQL_DRIVER_PATH: " << (qtSqlDriverPath.isEmpty() ? "NOT SET" : qtSqlDriverPath.toStdString()) << std::endl;
    std::cout << "   QT_HELP_PATH: " << (qtHelpPath.isEmpty() ? "NOT SET" : qtHelpPath.toStdString()) << std::endl;
    
    // Step 4: Verify SQLite driver availability
    std::cout << "4. Checking SQLite driver availability..." << std::endl;
    QStringList availableDrivers = QSqlDatabase::drivers();
    std::cout << "   Available SQL drivers: " << availableDrivers.join(QStringLiteral(", ")).toStdString() << std::endl;
    
    if (!availableDrivers.contains(QStringLiteral("QSQLITE"))) {
        std::cout << "   ❌ QSQLITE driver not found in available drivers!" << std::endl;
        
        // Try to manually load the SQLite plugin
        std::cout << "   Attempting to manually load SQLite plugin..." << std::endl;
        
        // Check multiple possible locations for the SQLite plugin
        QStringList sqlitePluginPaths;
        sqlitePluginPaths << qtSqlDriverPath + QStringLiteral("/libqsqlite.so")
                         << qtPluginPath + QStringLiteral("/sqldrivers/libqsqlite.so")
                         << QApplication::applicationDirPath() + QStringLiteral("/../lib/qt5/plugins/sqldrivers/libqsqlite.so")
                         << QApplication::applicationDirPath() + QStringLiteral("/../lib/qt5/plugins/sqldrivers/libqsqlite.so")
                         << QStringLiteral("/usr/lib/x86_64-linux-gnu/qt5/plugins/sqldrivers/libqsqlite.so")
                         << QStringLiteral("/usr/lib/qt5/plugins/sqldrivers/libqsqlite.so");
        
        bool sqliteLoaded = false;
        for (const QString &pluginPath : sqlitePluginPaths) {
            if (QFile::exists(pluginPath)) {
                std::cout << "   Found SQLite plugin at: " << pluginPath.toStdString() << std::endl;
                QPluginLoader loader(pluginPath);
                if (loader.load()) {
                    std::cout << "   ✅ Successfully loaded SQLite plugin from: " << pluginPath.toStdString() << std::endl;
                    sqliteLoaded = true;
                    break;
                } else {
                    std::cout << "   ❌ Failed to load SQLite plugin from: " << pluginPath.toStdString() << std::endl;
                    std::cout << "      Error: " << loader.errorString().toStdString() << std::endl;
                }
            }
        }
        
        if (!sqliteLoaded) {
            std::cout << "   ❌ Could not load SQLite plugin from any location!" << std::endl;
            return false;
        }
        
        // Re-check available drivers after loading
        availableDrivers = QSqlDatabase::drivers();
        std::cout << "   Available SQL drivers after plugin load: " << availableDrivers.join(QStringLiteral(", ")).toStdString() << std::endl;
    } else {
        std::cout << "   ✅ QSQLITE driver found in available drivers" << std::endl;
    }
    
    // Step 5: Verify help file
    std::cout << "5. Checking help file..." << std::endl;
    std::cout << "   Help file path: " << helpFile.toStdString() << std::endl;
    
    if (!QFile::exists(helpFile)) {
        std::cout << "   ❌ Help file does not exist!" << std::endl;
        
        // Try alternative paths
        QStringList alternativePaths;
                alternativePaths << QApplication::applicationDirPath() + QStringLiteral("/../share/linuxtrack/help/ltr_gui/help.qhc")
                         << QApplication::applicationDirPath() + QStringLiteral("/../share/linuxtrack/help/ltr_gui/help.qhc")
                         << QStringLiteral("/usr/share/linuxtrack/help/ltr_gui/help.qhc")
                         << QStringLiteral("/opt/linuxtrack/share/linuxtrack/help/ltr_gui/help.qhc");
        
        for (const QString &altPath : alternativePaths) {
            if (QFile::exists(altPath)) {
                std::cout << "   Found help file at alternative location: " << altPath.toStdString() << std::endl;
                helpFile = altPath;
                break;
            }
        }
        
        if (!QFile::exists(helpFile)) {
            std::cout << "   ❌ Help file not found in any location!" << std::endl;
            return false;
        }
    } else {
        std::cout << "   ✅ Help file exists" << std::endl;
    }
    
    // Step 6: Test SQLite database access
    std::cout << "6. Testing SQLite database access..." << std::endl;
    QSqlDatabase testDb = QSqlDatabase::addDatabase(QStringLiteral("QSQLITE"), QStringLiteral("test_connection"));
    testDb.setDatabaseName(QStringLiteral(":memory:"));
    
    if (!testDb.open()) {
        std::cout << "   ❌ Failed to open test SQLite database!" << std::endl;
        std::cout << "      Error: " << testDb.lastError().text().toStdString() << std::endl;
        return false;
    } else {
        std::cout << "   ✅ SQLite database access working" << std::endl;
        testDb.close();
    }
    
    // Step 7: Validate help file format and compatibility
    std::cout << "7. Validating help file format and compatibility..." << std::endl;
    
    // Check if help file exists and is readable
    QFileInfo helpFileInfo(helpFile);
    if (!helpFileInfo.exists()) {
        std::cout << "   ❌ Help file does not exist: " << helpFile.toStdString() << std::endl;
        return false;
    }
    
    if (!helpFileInfo.isReadable()) {
        std::cout << "   ❌ Help file is not readable: " << helpFile.toStdString() << std::endl;
        return false;
    }
    
    std::cout << "   ✅ Help file exists and is readable" << std::endl;
    
    // Validate help file format using SQLite
    QSqlDatabase helpDb = QSqlDatabase::addDatabase(QStringLiteral("QSQLITE"), QStringLiteral("help_validation"));
    helpDb.setDatabaseName(helpFile);
    
    if (!helpDb.open()) {
        std::cout << "   ❌ Cannot open help file as SQLite database: " << helpDb.lastError().text().toStdString() << std::endl;
        std::cout << "      This indicates the help file is corrupted or in an incompatible format" << std::endl;
        return false;
    }
    
    // Check for required tables to validate format compatibility
    QSqlQuery query(helpDb);
    QStringList requiredTables;
    requiredTables << QStringLiteral("ContentsTable") << QStringLiteral("FileDataTable");
    QStringList missingTables;
    
    for (const QString &table : requiredTables) {
        query.exec(QStringLiteral("SELECT name FROM sqlite_master WHERE type='table' AND name='%1'").arg(table));
        if (!query.next()) {
            missingTables.append(table);
        }
    }
    
    if (!missingTables.isEmpty()) {
        std::cout << "   ❌ Help file missing required tables: " << missingTables.join(QStringLiteral(", ")).toStdString() << std::endl;
        std::cout << "      This indicates format incompatibility - the file may have been generated with a different Qt version" << std::endl;
        helpDb.close();
        return false;
    }
    
    std::cout << "   ✅ Help file format validation passed - all required tables present" << std::endl;
    helpDb.close();
    
    // Step 8: Initialize help engine
    std::cout << "8. Initializing help engine..." << std::endl;
    helpEngine = new QHelpEngine(helpFile);
    
    if (!helpEngine->setupData()) {
        std::cout << "   ❌ Failed to setup help data!" << std::endl;
        std::cout << "      Help engine error: " << helpEngine->error().toStdString() << std::endl;
        
        // Enhanced error analysis for compatibility issues
        QString errorText = helpEngine->error();
        if (errorText.contains(QStringLiteral("Cannot unregister index tables"), Qt::CaseInsensitive)) {
            std::cout << "      🔍 COMPATIBILITY ISSUE DETECTED: 'Cannot unregister index tables'" << std::endl;
            std::cout << "      This error typically indicates Qt version mismatch between help file generation and consumption" << std::endl;
            std::cout << "      Solution: Regenerate help files with the same Qt version that will be used at runtime" << std::endl;
        } else if (errorText.contains(QStringLiteral("database"), Qt::CaseInsensitive)) {
            std::cout << "      🔍 DATABASE ISSUE DETECTED: Database-related error" << std::endl;
            std::cout << "      This may indicate help file corruption or SQLite driver issues" << std::endl;
        } else {
            std::cout << "      🔍 UNKNOWN ERROR: Unrecognized help engine error" << std::endl;
        }
        
        return false;
    } else {
        std::cout << "   ✅ Help engine setup successful" << std::endl;
    }
    
    std::cout << "=== Help System Initialization Complete ===" << std::endl;
    return true;
}

HelpViewer::HelpViewer(QWidget *parent) : QWidget(parent)
{
  ui.setupUi(this);
  setWindowTitle(QString::fromUtf8("Help viewer"));

  // Determine writable runtime help collection path
  QProcessEnvironment env = QProcessEnvironment::systemEnvironment();
  QString runtimeRoot = env.value(QStringLiteral("QT_HELP_PATH"));
  auto chooseUserHelpRoot = [](){
    QString base = QStandardPaths::writableLocation(QStandardPaths::AppDataLocation);
    if (base.isEmpty()) base = QDir::homePath() + QStringLiteral("/.local/share");
    return base + QStringLiteral("/linuxtrack/help");
  };
  if (runtimeRoot.isEmpty()) {
    runtimeRoot = chooseUserHelpRoot();
  } else {
    // If QT_HELP_PATH points to a read-only mount (e.g., AppImage), prefer user dir
    QString probe = runtimeRoot + QStringLiteral("/.wtest");
    QFile tf(probe);
    if (!tf.open(QIODevice::WriteOnly)) {
      runtimeRoot = chooseUserHelpRoot();
    } else {
      tf.close();
      QFile::remove(probe);
    }
  }
  QDir().mkpath(runtimeRoot + QStringLiteral("/") + QString::fromUtf8(HELP_BASE));

  // Packaged (read-only) help location inside AppImage or system install
  QString packagedHelpDir = PREF.getDataPath(QString::fromUtf8("/help/") +
                           QString::fromUtf8(HELP_BASE));

  // Ensure writable copy exists (qhc/qch)
  QString runtimeHelpDir = runtimeRoot + QStringLiteral("/") + QString::fromUtf8(HELP_BASE);
  QString runtimeQhc = runtimeHelpDir + QStringLiteral("/help.qhc");
  QString runtimeQch = runtimeHelpDir + QStringLiteral("/help.qch");
  QString packagedQhc = QDir(packagedHelpDir).filePath(QStringLiteral("help.qhc"));
  QString packagedQch = QDir(packagedHelpDir).filePath(QStringLiteral("help.qch"));

  auto ensureWritableCopy = [&](const QString &src, const QString &dst){
    if (QFile::exists(src)) {
      // Copy if destination missing or older than source
      QFileInfo sfi(src), dfi(dst);
      if (!dfi.exists() || sfi.lastModified() > dfi.lastModified()) {
        QFile::remove(dst);
        QFile::copy(src, dst);
        QFile::setPermissions(dst, QFileDevice::ReadUser | QFileDevice::WriteUser | QFileDevice::ReadGroup | QFileDevice::ReadOther);
      }
    }
  };

  ensureWritableCopy(packagedQhc, runtimeQhc);
  ensureWritableCopy(packagedQch, runtimeQch);

  QString helpFile = runtimeQhc;
  
  // Clean up the path to remove double slashes
  helpFile = QDir::cleanPath(helpFile);
  
  // Initialize help system with robust error handling
  if (!initializeHelpSystem(helpFile, helpEngine)) {
    std::cout << "Help system initialization failed - creating fallback help viewer" << std::endl;
    
    // Create a fallback help viewer that shows error information
        QMessageBox::warning(this, QStringLiteral("Help System Error"),
                        QStringLiteral("The help system could not be initialized.\n\n"
                        "This is likely due to missing SQLite database drivers in the AppImage.\n\n"
                        "The application will continue to work, but help content will not be available.\n\n"
                        "Technical details have been printed to the console."));
    
    // Create a minimal help engine that won't crash
    helpEngine = new QHelpEngine(QStringLiteral(":memory:"));
    helpEngine->setupData();
  }
  
  contents = helpEngine->contentWidget();
  splitter = new QSplitter();
  viewer = new HelpViewWidget(helpEngine, this);
  layout = new QHBoxLayout();
  layout->addWidget(splitter);
  splitter->addWidget(contents);
  splitter->addWidget(viewer);
  ui.verticalLayout->insertLayout(0, layout);
  QObject::connect(contents, SIGNAL(linkActivated(const QUrl &)), viewer, SLOT(setSource(const QUrl &)));
  QObject::connect(contents, SIGNAL(clicked(const QModelIndex &)), this, SLOT(itemClicked(const QModelIndex &)));
  QObject::connect(helpEngine->contentModel(), SIGNAL(contentsCreated()), this, SLOT(helpInitialized()));
  QObject::connect(viewer, SIGNAL(anchorClicked(const QUrl&)), this, SLOT(followLink(const QUrl&)));
  viewer->setOpenLinks(false);
  splitter->setStretchFactor(1, 4);
}

HelpViewer::~HelpViewer()
{
  ui.verticalLayout->removeItem(layout);
  layout->removeWidget(contents);
  layout->removeWidget(viewer);
  delete(layout);
  delete(contents);
  delete(viewer);
}

void HelpViewer::itemClicked(const QModelIndex &index)
{
  const QHelpContentItem *ci = helpEngine->contentModel()->contentItemAt(index);
  if(ci){
    viewer->setSource(ci->url());
  }
}

void HelpViewer::helpInitialized()
{
  contents->expandAll();
}

void HelpViewer::on_CloseButton_pressed()
{
  close();
}


void HelpViewer::followLink(const QUrl &url)
{
  if(QString::fromUtf8("http").compare(url.scheme(), Qt::CaseInsensitive) == 0){
    QDesktopServices::openUrl(url);
  }else{
    viewer->setSource(url);
  }
}

