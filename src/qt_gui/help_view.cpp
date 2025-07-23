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
#include <QPluginLoader>
#include <QSqlDatabase>
#include <QSqlError>
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

// Robust help system initialization with comprehensive error handling
bool HelpViewer::initializeHelpSystem(QString &helpFile, QHelpEngine *&helpEngine)
{
    std::cout << "=== LinuxTrack Help System Initialization ===" << std::endl;
    
    // Step 1: Verify Qt environment
    std::cout << "1. Checking Qt environment..." << std::endl;
    std::cout << "   Qt version: " << qVersion() << std::endl;
    std::cout << "   Application path: " << QApplication::applicationDirPath().toStdString() << std::endl;
    
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
    
    // Step 7: Initialize help engine
    std::cout << "7. Initializing help engine..." << std::endl;
    helpEngine = new QHelpEngine(helpFile);
    
    if (!helpEngine->setupData()) {
        std::cout << "   ❌ Failed to setup help data!" << std::endl;
        std::cout << "      Help engine error: " << helpEngine->error().toStdString() << std::endl;
        
        // Try to get more detailed error information
        QSqlDatabase helpDb = QSqlDatabase::addDatabase(QStringLiteral("QSQLITE"), QStringLiteral("help_test"));
        helpDb.setDatabaseName(helpFile);
        
        if (!helpDb.open()) {
            std::cout << "      Database open error: " << helpDb.lastError().text().toStdString() << std::endl;
        } else {
            std::cout << "      Database opens successfully, but help engine fails" << std::endl;
            helpDb.close();
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

  QString helpFile = PREF.getDataPath(QString::fromUtf8("/help/") +
                     QString::fromUtf8(HELP_BASE) + QString::fromUtf8("/help.qhc"));
  
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

