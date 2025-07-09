#include <QFileDialog>
#include <QtDebug>
#include <QProcessEnvironment>
#include <QMessageBox>
#include <QDateTime>
#include <QTextStream>
#include <QRegExp>
#include <QDir>
#include <unistd.h>
#include <cstdlib>

#include "extractor.h"
#include "hashing.h"
#include "extract.h"
#include "game_data.h"
#include "ltr_gui_prefs.h"
#include "help_view.h"
#include "utils.h"


#ifdef HAVE_CONFIG_H
  #include "../../config.h"
#endif

static bool linkResult(const QString& destPath)
{
  QString l = PrefProxy::getRsrcDirPath() + QString::fromUtf8("/tir_firmware");
  if(QFile::exists(l)){
    QFile::remove(l);
  }
  return QFile::link(destPath, l);
}

QString getBlobName(const QString& installerName)
{
  QFile installer{installerName};
  if(!installer.open(QIODevice::ReadOnly)){
    return QStringLiteral("");
  }

  QByteArray buf = installer.read(installer.size());
  QString md5{QString::fromUtf8(QCryptographicHash::hash(buf, QCryptographicHash::Md5).toHex())};
  QString sha1{QString::fromUtf8(QCryptographicHash::hash(buf, QCryptographicHash::Sha1).toHex())};

  return QStringLiteral("fw_blob_") + md5 + QStringLiteral("_") + sha1 + QStringLiteral(".bin");
}

void TirFwExtractThread::start(targets_t &t, const QString &p, const QString &d)
{
  if(!isRunning()){
    targets = &t;
    path = p;
    destPath = d;
    quit = false;
    QThread::start();
  }
}

void TirFwExtractThread::run()
{
  emit progress(QString::fromUtf8("Commencing analysis of directory '%1'...").arg(path));
  gameDataFound = false;
  tirviewsFound = false;
  for(targets_iterator_t it = targets->begin(); it != targets->end(); ++it){
    it->second.clearFoundFlag();
  }

  findCandidates(path);
  emit progress(QString::fromUtf8("==============================="));
  if(allFound()){
    everything = true;
    emit progress(QString::fromUtf8("Extraction done!"));
  }else{
    everything = false;
    for(targets_iterator_t it = targets->begin(); it != targets->end(); ++it){
      if(!it->second.foundAlready())
        emit progress(QString::fromUtf8("Couldn't extract %1!").arg(it->second.getFname()));
    }
    if(!gameDataFound){
      emit progress(QString::fromUtf8("Couldn't extract game data!"));
    }
    if(!tirviewsFound){
      emit progress(QString::fromUtf8("Couldn't extract TIRViews.dll!"));
    }
  }
}

void TirFwExtractThread::analyzeFile(const QString fname)
{
  QFile file(fname);
  if(!file.open(QIODevice::ReadOnly)){
    return;
  }
  qDebug()<<QString::fromUtf8("Analyzing ")<<fname;
  FastHash hash;
  QStringList msgs;
  char val;
  uint16_t res;
  targets_iterator_t it;
  std::pair<targets_iterator_t,targets_iterator_t> range;
  int cntr = 0;
  while(file.read(&val, 1) > 0){
    ++cntr;
    res = hash.hash(val);
    range = targets->equal_range(res);
    for(it = range.first; it != range.second; ++it){
      //qDebug()<<cntr<<qPrintable("Checking against ")<<file.pos() <<res <<(it->second.getFname());
      msgs.clear();
      it->second.isBlock(file, destPath, msgs);
      if(!msgs.isEmpty()){
        for(int i = 0; i < msgs.size(); ++i){
          emit progress(msgs[i]);
        }
      }
    }
  }
  file.close();
}

bool TirFwExtractThread::allFound()
{
  for(targets_iterator_t it = targets->begin(); it != targets->end(); ++it){
    if(!it->second.foundAlready()) return false;
  }
  return gameDataFound && tirviewsFound;
}

bool TirFwExtractThread::findCandidates(QString name)
{
  if(quit) return false;
  int i;
  QDir dir(name);
  QStringList patt;
  patt<<QString::fromUtf8("*.dll")<<QString::fromUtf8("*.exe")<<QString::fromUtf8("*.dat");
  QFileInfoList files = dir.entryInfoList(patt, QDir::Files | QDir::Readable);
  for(i = 0; i < files.size(); ++i){
    if(quit) return false;
    if(files[i].fileName().compare(QString::fromUtf8("TIRViews.dll")) == 0){
      QString outfile = QString::fromUtf8("%1/TIRViews.dll").arg(destPath);
      if((tirviewsFound = QFile::copy(files[i].canonicalFilePath(), outfile))){
        emit progress(QString::fromUtf8("Extracted TIRViews.dll..."));
      }
    }else if(files[i].fileName().compare(QString::fromUtf8("sgl.dat"))){
      analyzeFile(files[i].canonicalFilePath());
    }else{
      QString outfile = QString::fromUtf8("%1/gamedata.txt").arg(destPath);
      gameDataFound = get_game_data(files[i].canonicalFilePath().toUtf8().constData(),
                                    outfile.toUtf8().constData(), false);
      emit progress(QString::fromUtf8("Extracted game data..."));
    }
    if(allFound()){
      return true;
    }
  }

  QFileInfoList subdirs =
    dir.entryInfoList(QDir::AllDirs | QDir::NoDotAndDotDot | QDir::NoSymLinks);
  QString dirname;
  for(i = 0; i < subdirs.size(); ++i){
    dirname = subdirs[i].canonicalFilePath();
    if((!dirname.endsWith(QString::fromUtf8("windows"))) && findCandidates(dirname)){
      return true;
    }
  }
  return false;
}

QString Extractor::findSrc(const QString &name)
{
  //First look at ~/.config/linuxtrack, then /usr/share/...
  QString path1 = PrefProxy::getRsrcDirPath();
  QString path2 = PrefProxy::getDataPath(name);
  path1 += name;
  QFileInfo fi(path1);
  if(fi.isReadable()) return path1;
  fi.setFile(path2);
  if(fi.isReadable()) return path2;
  return QString();
}

bool Extractor::readSources(const QString &sources)
{
  progress(QString::fromUtf8("Looking for existing ") + sources + QString::fromUtf8("..."));
  QFile f(findSrc(sources));
  if(!f.open(QIODevice::ReadOnly)){
    progress(sources + QString::fromUtf8(" not found."));
    return false;
  }
  progress(QString::fromUtf8("Found '%1'.").arg(f.fileName()));

  QTextStream fs(&f);
  QString url;
  while(1){
    if(fs.atEnd()){
      break;
    }
    fs>>url;
    if(!url.isNull()){
      ui.FWCombo->addItem(url);
    }
  }
  progress(sources + QString::fromUtf8(" found and read."));
  return (ui.FWCombo->count() != 0);
}


bool Extractor::readSpec()
{
  progress(QString::fromUtf8("Looking for existing spec.txt..."));
  QFile f(findSrc(QString::fromUtf8("spec.txt")));
  if(!f.open(QIODevice::ReadOnly)){
    progress(QString::fromUtf8("spec.txt not found."));
    return false;
  }
  progress(QString::fromUtf8("Found '%1'.").arg(f.fileName()));

  QTextStream fs(&f);
  QString name;
    uint16_t fh;
    qint64 size;
    QByteArray sha1, md5;
  while(1){
    if(fs.atEnd()){
      break;
    }
    fs>>name>>size>>fh>>md5>>sha1;
    if(!name.isNull()){
      BlockId blk(name, size, fh, md5, sha1);
      targets.insert(std::pair<uint16_t, BlockId>(fh, blk));
    }
  }
  progress(QString::fromUtf8("spec.txt found and read."));
  return (targets.size() != 0);
}

Extractor::Extractor(QWidget *parent) : QDialog(parent), dl(NULL), progressDlg(NULL)
{
  ui.setupUi(this);
  wine = new WineLauncher();
  dl = new Downloading();
  progressDlg = new Progress();
  enableButtons(true);
}

TirFwExtractor::TirFwExtractor(QWidget *parent) : Extractor(parent), et(NULL)
{
  et = new TirFwExtractThread();
  QObject::connect(et, SIGNAL(progress(const QString &)), this, SLOT(progress(const QString &)));
  QObject::connect(et, SIGNAL(finished()), this, SLOT(threadFinished()));
  QObject::connect(wine, SIGNAL(finished(bool)), this, SLOT(wineFinished(bool)));
  QObject::connect(dl, SIGNAL(done(bool, QString)), this, SLOT(downloadDone(bool, QString)));
  QObject::connect(dl, SIGNAL(msg(const QString &)), this, SLOT(progress(const QString &)));
  QObject::connect(dl, SIGNAL(msg(qint64, qint64)), progressDlg, SLOT(message(qint64, qint64)));
  haveSpec = readSpec();
#ifndef DARWIN
  QString sources = QString::fromUtf8("sources.txt");
#else
  QString sources = QString::fromUtf8("sources_mac.txt");
#endif
  readSources(sources);
  QString dbg = QProcessEnvironment::systemEnvironment().value(QString::fromUtf8("LINUXTRACK_DBG"));
  if(!dbg.contains(QChar::fromLatin1('d'))){
    ui.AnalyzeSourceButton->setVisible(false);
  }
}

Mfc42uExtractor::Mfc42uExtractor(QWidget *parent) : Extractor(parent), cabextract(NULL)
{
  cabextract = new QProcess(this);
  QObject::connect(cabextract, SIGNAL(finished(int, QProcess::ExitStatus)),
                   this, SLOT(cabextractFinished(int, QProcess::ExitStatus)));
  QObject::connect(wine, SIGNAL(finished(bool)), this, SLOT(wineFinished(bool)));
  QObject::connect(dl, SIGNAL(done(bool, QString)), this, SLOT(downloadDone(bool, QString)));
  QObject::connect(dl, SIGNAL(msg(const QString &)), this, SLOT(progress(const QString &)));
  QObject::connect(dl, SIGNAL(msg(qint64, qint64)), progressDlg, SLOT(message(qint64, qint64)));
  QString sources = QString::fromUtf8("sources_mfc.txt");
  readSources(sources);
  ui.AnalyzeSourceButton->setVisible(false);
  ui.BrowseDir->setEnabled(false);
}

Extractor::~Extractor()
{
  delete wine;
  delete dl;
  delete progressDlg;
}

TirFwExtractor::~TirFwExtractor()
{
  if(et->isRunning()){
    et->stop();
    et->wait(5000);
    if(et->isRunning()){
      et->terminate();
      et->wait(5000);
    }
  }
  delete et;
  et = NULL;
}

Mfc42uExtractor::~Mfc42uExtractor()
{
}

static QString makeDestPath(const QString &base)
{
  QDateTime current = QDateTime::currentDateTime();
  QString result = QString::fromUtf8("%2").arg(current.toString(QString::fromUtf8("yyMMdd_hhmmss")));
  QString final = result;
  QDir dir = QDir(base);
  int counter = 0;
  while(dir.exists(final)){
    final = QString(QString::fromUtf8("%1_%2")).arg(result).arg(counter++);
  }
  dir.mkpath(final);
  return base + QString::fromUtf8("/") + final + QString::fromUtf8("/");
}


void TirFwExtractor::wineFinished(bool result)
{
  if(!wineInitialized){
    wineInitialized = true;
    if(!result){
      QMessageBox::warning(this, QString::fromUtf8("Error running Wine"),
        QString::fromUtf8("There was an error initializing\n"
        "the wine prefix; wil try to install the firmware\n"
        "just in case..."
        "Please see the log for more details.\n\n")
      );
    }
    QStringList args;
    // Run installer normally without silent mode to allow proper installation
    wine->run(installerFile, args);
  }else{
    if(!result){
      QMessageBox::warning(this, QString::fromUtf8("Error running Wine"),
        QString::fromUtf8("There was an error when extracting\n"
        "the firmware, will try the analysis\n"
        "just in case..."
        "Please see the log for more details.\n\n")
      );
    }
    destPath = makeDestPath(PrefProxy::getRsrcDirPath());
    et->start(targets, winePrefix, destPath);
  }
}

void Mfc42uExtractor::wineFinished(bool result)
{
  if(!result){
    // Modern approach: Try package managers and winetricks instead of old Wine extraction
    progress(QString::fromUtf8("Wine extraction failed, trying modern alternatives..."));
    
    // Check if mfc42u.dll already exists in common locations
    QStringList searchPaths;
    searchPaths << winePrefix + QString::fromUtf8("/drive_c/windows/system32/mfc42u.dll");
    searchPaths << winePrefix + QString::fromUtf8("/drive_c/windows/syswow64/mfc42u.dll");
    searchPaths << winePrefix + QString::fromUtf8("/drive_c/mfc42u.dll");
    
    bool found = false;
    for(const QString& searchPath : searchPaths) {
      if(QFile::exists(searchPath)) {
        progress(QString::fromUtf8("Found mfc42u.dll at: %1").arg(searchPath));
        destPath = PrefProxy::getRsrcDirPath() + QString::fromUtf8("/tir_firmware/mfc42u.dll");
        if(QFile::copy(searchPath, destPath)) {
          progress(QString::fromUtf8("Mfc42u.dll copied successfully"));
          found = true;
          break;
        }
      }
    }
    
    if(!found) {
      // Try modern installation methods
      progress(QString::fromUtf8("Attempting modern installation methods..."));
      
      // For Debian-based systems, prioritize winetricks
      bool isDebianBased = false;
      QFile osRelease(QString::fromUtf8("/etc/os-release"));
      if(osRelease.exists()) {
        osRelease.open(QIODevice::ReadOnly);
        QString content = QString::fromUtf8(osRelease.readAll());
        if(content.contains(QString::fromUtf8("ID=debian")) || 
           content.contains(QString::fromUtf8("ID=ubuntu")) ||
           content.contains(QString::fromUtf8("ID=mx"))) {
          isDebianBased = true;
        }
        osRelease.close();
      }
      
      if(isDebianBased) {
        // Debian-based systems: try winetricks first
        progress(QString::fromUtf8("Debian-based system detected - trying winetricks first..."));
        if(tryWinetricksInstall()) {
          found = true;
        } else if(tryCabextractFallback()) {
          return; // Don't enable buttons yet, wait for cabextract to finish
        } else {
          showModernInstallationInstructions();
        }
      } else {
        // Other systems: try package manager first, then winetricks
        if(tryPackageManagerInstall()) {
          found = true;
        } else if(tryWinetricksInstall()) {
          found = true;
        } else if(tryCabextractFallback()) {
          return; // Don't enable buttons yet, wait for cabextract to finish
        } else {
          showModernInstallationInstructions();
        }
      }
    }
    
    // Always enable buttons and emit finished signal, regardless of success/failure
    enableButtons(true);
    emit finished(found);
    hide();
    return;
  }
  
  switch(stage){
    case 0:{
        stage = 1;
        QString file = winePrefix + QString::fromUtf8("/drive_c/vcredist.exe");
        progress(QString::fromUtf8("Extracting %1").arg(file));
        QStringList args;
        args << QStringLiteral("/C") << QStringLiteral("/Q") << QStringLiteral("/T:c:\\");
        wine->run(file, args);
      }
      break;
    case 1:{
        stage = 0;
        destPath = PrefProxy::getRsrcDirPath() + QString::fromUtf8("/tir_firmware/mfc42u.dll");
        QString srcPath = winePrefix + QString::fromUtf8("/drive_c/mfc42u.dll");
        if(!QFile::copy(srcPath, destPath)){
          QMessageBox::warning(this, QString::fromUtf8("Error extracting mfc42u.dll"),
            QString::fromUtf8("There was an error extracting mfc42.dll.\n"
            "Please see the help to learn other ways\n"
  	  "ways of obtaining this file.\n\n")
          );
        }else{
          progress(QString::fromUtf8("Mfc42u.dll extracted successfuly"));
        }
        enableButtons(true);
        emit finished(true);
        hide();
      }
      break;
    default:
      break;
  }
}

bool Mfc42uExtractor::tryWinetricksInstall()
{
  progress(QString::fromUtf8("Trying winetricks installation..."));
  
  // First, check if winetricks is available and get its version
  QString winetricksPath = checkWinetricksAvailability();
  if(winetricksPath.isEmpty()) {
    progress(QString::fromUtf8("Winetricks not found - attempting to install latest version..."));
    if(!installLatestWinetricks()) {
      progress(QString::fromUtf8("Failed to install winetricks"));
      return false;
    }
    winetricksPath = QString::fromUtf8("/usr/local/bin/winetricks");
  } else {
    // Check if we have a recent version (avoid outdated packaged versions)
    if(!isWinetricksVersionRecent(winetricksPath)) {
      progress(QString::fromUtf8("Outdated winetricks detected - updating to latest version..."));
      if(!installLatestWinetricks()) {
        progress(QString::fromUtf8("Failed to update winetricks, trying with existing version..."));
      } else {
        winetricksPath = QString::fromUtf8("/usr/local/bin/winetricks");
      }
    }
  }
  
  // Check if we're using a 64-bit Wine prefix (which can cause issues with MFC42)
  bool is64BitPrefix = false;
  QFile archFile(winePrefix + QString::fromUtf8("/system.reg"));
  if(archFile.exists()) {
    archFile.open(QIODevice::ReadOnly);
    QString content = QString::fromUtf8(archFile.readAll());
    if(content.contains(QString::fromUtf8("#arch=win64"))) {
      is64BitPrefix = true;
      progress(QString::fromUtf8("Detected 64-bit Wine prefix - MFC42 requires 32-bit Wine prefix"));
    }
    archFile.close();
  }
  
  // Test winetricks first to make sure it's working
  QProcess testWinetricks;
  testWinetricks.setProcessEnvironment(wine->getProcessEnvironment());
  testWinetricks.start(winetricksPath, QStringList() << QString::fromUtf8("--version"));
  
  if(!testWinetricks.waitForFinished(10000)) { // 10 second timeout
    progress(QString::fromUtf8("Winetricks version check timed out"));
    return false;
  }
  
  if(testWinetricks.exitCode() != 0) {
    progress(QString::fromUtf8("Winetricks version check failed (exit code: %1)").arg(testWinetricks.exitCode()));
    QString errorOutput = QString::fromUtf8(testWinetricks.readAllStandardError());
    if(!errorOutput.isEmpty()) {
      progress(QString::fromUtf8("Winetricks error: %1").arg(errorOutput));
    }
    return false;
  }
  
  QString versionOutput = QString::fromUtf8(testWinetricks.readAllStandardOutput());
  progress(QString::fromUtf8("Winetricks version: %1").arg(versionOutput.trimmed()));
  
  // Try to install mfc42 using winetricks
  QProcess winetricks;
  winetricks.setProcessEnvironment(wine->getProcessEnvironment());
  
  // Use unattended mode to avoid GUI issues
  QStringList args;
  args << QString::fromUtf8("--unattended") << QString::fromUtf8("mfc42");
  
  progress(QString::fromUtf8("Running: %1 --unattended mfc42").arg(winetricksPath));
  
  // If we have a 64-bit prefix, try creating a 32-bit prefix for this installation
  if(is64BitPrefix) {
    progress(QString::fromUtf8("Creating 32-bit Wine prefix for MFC42 installation..."));
    
    // Create a temporary 32-bit Wine prefix
    QString tempPrefix = winePrefix + QString::fromUtf8("_32bit");
    
    // Clean up any existing temporary prefix
    QDir tempDir(tempPrefix);
    if(tempDir.exists()) {
      tempDir.removeRecursively();
      progress(QString::fromUtf8("Cleaned up existing temporary 32-bit prefix"));
    }
    
    QProcess wineInit;
    wineInit.setProcessEnvironment(wine->getProcessEnvironment());
    
    // Set environment variables for 32-bit prefix
    QProcessEnvironment env = wine->getProcessEnvironment();
    env.insert(QString::fromUtf8("WINEPREFIX"), tempPrefix);
    env.insert(QString::fromUtf8("WINEARCH"), QString::fromUtf8("win32"));
    wineInit.setProcessEnvironment(env);
    
    // Initialize the 32-bit prefix
    progress(QString::fromUtf8("Initializing 32-bit Wine prefix..."));
    wineInit.start(QString::fromUtf8("wine"), QStringList() << QString::fromUtf8("wineboot") << QString::fromUtf8("--init"));
    
    if(!wineInit.waitForFinished(30000)) { // 30 second timeout
      progress(QString::fromUtf8("Failed to initialize 32-bit Wine prefix (timeout) - trying in 64-bit prefix"));
      // Clean up failed prefix
      QDir tempDir(tempPrefix);
      if(tempDir.exists()) {
        tempDir.removeRecursively();
      }
      // Fall back to trying in the 64-bit prefix
      is64BitPrefix = false;
      winetricks.setWorkingDirectory(winePrefix);
    } else if(wineInit.exitCode() != 0) {
      progress(QString::fromUtf8("32-bit Wine prefix initialization failed (exit code: %1) - trying in 64-bit prefix").arg(wineInit.exitCode()));
      // Clean up failed prefix
      QDir tempDir(tempPrefix);
      if(tempDir.exists()) {
        tempDir.removeRecursively();
      }
      // Fall back to trying in the 64-bit prefix
      is64BitPrefix = false;
      winetricks.setWorkingDirectory(winePrefix);
    } else {
      progress(QString::fromUtf8("32-bit Wine prefix initialized successfully"));
      // Now try winetricks in the 32-bit prefix
      winetricks.setProcessEnvironment(env);
      winetricks.setWorkingDirectory(tempPrefix);
    }
  } else {
    winetricks.setWorkingDirectory(winePrefix);
  }
  
  winetricks.start(winetricksPath, args);
  
  if(!winetricks.waitForFinished(120000)) { // 2 minute timeout for winetricks
    progress(QString::fromUtf8("Winetricks installation timed out"));
    return false;
  }
  
  // Capture error output for debugging
  QString errorOutput = QString::fromUtf8(winetricks.readAllStandardError());
  QString standardOutput = QString::fromUtf8(winetricks.readAllStandardOutput());
  
  if(!errorOutput.isEmpty()) {
    progress(QString::fromUtf8("Winetricks stderr: %1").arg(errorOutput));
  }
  if(!standardOutput.isEmpty()) {
    progress(QString::fromUtf8("Winetricks stdout: %1").arg(standardOutput));
  }
  
  if(winetricks.exitCode() == 0) {
    progress(QString::fromUtf8("Winetricks installation successful"));
    
    // Check if mfc42u.dll was installed
    QStringList checkPaths;
    if(is64BitPrefix) {
      QString tempPrefix = winePrefix + QString::fromUtf8("_32bit");
      checkPaths << tempPrefix + QString::fromUtf8("/drive_c/windows/system32/mfc42u.dll");
      checkPaths << tempPrefix + QString::fromUtf8("/drive_c/windows/syswow64/mfc42u.dll");
    } else {
      checkPaths << winePrefix + QString::fromUtf8("/drive_c/windows/system32/mfc42u.dll");
      checkPaths << winePrefix + QString::fromUtf8("/drive_c/windows/syswow64/mfc42u.dll");
    }
    
    for(const QString& path : checkPaths) {
      if(QFile::exists(path)) {
        destPath = PrefProxy::getRsrcDirPath() + QString::fromUtf8("/tir_firmware/mfc42u.dll");
        if(QFile::copy(path, destPath)) {
          progress(QString::fromUtf8("Mfc42u.dll installed via winetricks"));
          
          // Clean up temporary 32-bit prefix if we created one
          if(is64BitPrefix) {
            QString tempPrefix = winePrefix + QString::fromUtf8("_32bit");
            QDir tempDir(tempPrefix);
            if(tempDir.exists()) {
              tempDir.removeRecursively();
              progress(QString::fromUtf8("Cleaned up temporary 32-bit Wine prefix"));
            }
          }
          
          return true;
        }
      }
    }
  } else {
    // If winetricks failed and we were trying a 32-bit prefix, try again in the 64-bit prefix
    if(is64BitPrefix) {
      progress(QString::fromUtf8("Winetricks failed in 32-bit prefix, trying in 64-bit prefix..."));
      
      // Clean up the failed 32-bit prefix
      QString tempPrefix = winePrefix + QString::fromUtf8("_32bit");
      QDir tempDir(tempPrefix);
      if(tempDir.exists()) {
        tempDir.removeRecursively();
        progress(QString::fromUtf8("Cleaned up failed 32-bit Wine prefix"));
      }
      
      // Try again in the 64-bit prefix
      QProcess winetricks64;
      winetricks64.setProcessEnvironment(wine->getProcessEnvironment());
      winetricks64.setWorkingDirectory(winePrefix);
      winetricks64.start(winetricksPath, args);
      
      if(winetricks64.waitForFinished(120000)) { // 2 minute timeout
        // Capture error output for debugging
        QString errorOutput64 = QString::fromUtf8(winetricks64.readAllStandardError());
        QString standardOutput64 = QString::fromUtf8(winetricks64.readAllStandardOutput());
        
        if(!errorOutput64.isEmpty()) {
          progress(QString::fromUtf8("Winetricks 64-bit stderr: %1").arg(errorOutput64));
        }
        if(!standardOutput64.isEmpty()) {
          progress(QString::fromUtf8("Winetricks 64-bit stdout: %1").arg(standardOutput64));
        }
        
        if(winetricks64.exitCode() == 0) {
          progress(QString::fromUtf8("Winetricks installation successful in 64-bit prefix"));
          
          // Check if mfc42u.dll was installed in 64-bit prefix
          QStringList checkPaths64;
          checkPaths64 << winePrefix + QString::fromUtf8("/drive_c/windows/system32/mfc42u.dll");
          checkPaths64 << winePrefix + QString::fromUtf8("/drive_c/windows/syswow64/mfc42u.dll");
          
          for(const QString& path : checkPaths64) {
            if(QFile::exists(path)) {
              destPath = PrefProxy::getRsrcDirPath() + QString::fromUtf8("/tir_firmware/mfc42u.dll");
              if(QFile::copy(path, destPath)) {
                progress(QString::fromUtf8("Mfc42u.dll installed via winetricks in 64-bit prefix"));
                return true;
              }
            }
          }
        }
      }
    }
  }
  
  progress(QString::fromUtf8("Winetricks installation failed"));
  return false;
}

bool Mfc42uExtractor::tryPackageManagerInstall()
{
  progress(QString::fromUtf8("Trying package manager installation..."));
  
  // Detect distribution and try appropriate package
  QStringList packageManagers;
  packageManagers << QString::fromUtf8("apt") << QString::fromUtf8("dnf") << QString::fromUtf8("pacman") << QString::fromUtf8("zypper");
  
  for(const QString& pm : packageManagers) {
    QProcess checkPM;
    checkPM.start(QString::fromUtf8("which"), QStringList() << pm);
    if(checkPM.waitForFinished(5000) && checkPM.exitCode() == 0) {
      progress(QString::fromUtf8("Found package manager: %1").arg(pm));
      
      // Try to install the appropriate package
      QString packageName;
      if(pm == QString::fromUtf8("apt")) {
        // Debian/Ubuntu/MX don't have libmfc42 package - use winetricks instead
        progress(QString::fromUtf8("Debian-based systems should use winetricks instead of package manager"));
        return false;
      } else if(pm == QString::fromUtf8("dnf")) {
        packageName = QString::fromUtf8("mfc42");
      } else if(pm == QString::fromUtf8("pacman")) {
        packageName = QString::fromUtf8("mfc42");
      } else if(pm == QString::fromUtf8("zypper")) {
        packageName = QString::fromUtf8("mfc42");
      }
      
      if(!packageName.isEmpty()) {
        progress(QString::fromUtf8("Attempting to install: %1").arg(packageName));
        
        // Note: This would require sudo, so we'll just inform the user
        QMessageBox::information(this, QString::fromUtf8("Package Installation Required"),
          QString::fromUtf8("Please install the required package manually:\n\n"
          "For %1: sudo %1 install %2\n\n"
          "After installation, try the Wine support installation again.")
          .arg(pm).arg(packageName)
        );
        return false; // Let user handle the installation
      }
    }
  }
  
  return false;
}

bool Mfc42uExtractor::tryCabextractFallback()
{
  progress(QString::fromUtf8("Trying cabextract fallback..."));
  QString c = PREF.getDataPath(QString::fromUtf8("/../../helper/cabextract"));
  if(QFile::exists(c)) {
    cabextract->setWorkingDirectory(winePrefix);
    QStringList args;
    args << winePrefix + QString::fromUtf8("/VC6RedistSetup_deu.exe");
    cabextract->start(c, args);
    return true; // Will be handled by cabextractFinished
  }
  return false;
}

void Mfc42uExtractor::showModernInstallationInstructions()
{
  QMessageBox::information(this, QString::fromUtf8("Modern Installation Required"),
    QString::fromUtf8("The old Wine extraction method failed. Please use one of these modern approaches:\n\n"
    "1. Install via winetricks (Recommended for Debian/Ubuntu/MX):\n"
    "   sudo apt install winetricks\n"
    "   winetricks mfc42\n"
    "   # Then copy the DLL to LinuxTrack:\n"
    "   sudo cp ~/.wine/drive_c/windows/system32/mfc42u.dll /usr/share/linuxtrack/tir_firmware/\n\n"
    "2. Install via package manager (Fedora/RHEL/Arch only):\n"
    "   Fedora: sudo dnf install mfc42\n"
    "   Arch: sudo pacman -S mfc42\n"
    "   # Then copy the DLL to LinuxTrack:\n"
    "   sudo cp /usr/lib/mfc42u.dll /usr/share/linuxtrack/tir_firmware/\n\n"
    "3. Manual installation:\n"
    "   Copy mfc42u.dll from Windows system to:\n"
    "   sudo cp mfc42u.dll /usr/share/linuxtrack/tir_firmware/\n\n"
    "Note: Debian/Ubuntu/MX systems should use winetricks as the package is not available in repositories.\n"
    "After copying the DLL, try the Wine support installation again.")
  );
}

QString Mfc42uExtractor::checkWinetricksAvailability()
{
  QProcess winetricksCheck;
  winetricksCheck.start(QString::fromUtf8("which"), QStringList() << QString::fromUtf8("winetricks"));
  if(!winetricksCheck.waitForFinished(5000) || winetricksCheck.exitCode() != 0) {
    return QString();
  }
  
  // Get the path to winetricks
  winetricksCheck.start(QString::fromUtf8("which"), QStringList() << QString::fromUtf8("winetricks"));
  if(winetricksCheck.waitForFinished(5000) && winetricksCheck.exitCode() == 0) {
    QString output = QString::fromUtf8(winetricksCheck.readAllStandardOutput()).trimmed();
    if(!output.isEmpty()) {
      return output;
    }
  }
  
  return QString();
}

bool Mfc42uExtractor::isWinetricksVersionRecent(const QString& winetricksPath)
{
  if(winetricksPath.isEmpty()) {
    return false;
  }
  
  // Check winetricks version
  QProcess versionCheck;
  versionCheck.start(winetricksPath, QStringList() << QString::fromUtf8("--version"));
  if(!versionCheck.waitForFinished(5000) || versionCheck.exitCode() != 0) {
    return false;
  }
  
  QString versionOutput = QString::fromUtf8(versionCheck.readAllStandardOutput()).trimmed();
  
  // Extract version number (format: "winetricks 20231210" or similar)
  QRegExp versionRegex(QString::fromUtf8("winetricks\\s+(\\d{8})"));
  if(versionRegex.indexIn(versionOutput) != -1) {
    QString versionStr = versionRegex.cap(1);
    bool ok;
    int version = versionStr.toInt(&ok);
    if(ok) {
      // Consider versions from 2023 onwards as recent
      // This is a reasonable cutoff for avoiding very old packaged versions
      return version >= 20230101;
    }
  }
  
  // If we can't parse the version, assume it's recent enough
  return true;
}

bool Mfc42uExtractor::installLatestWinetricks()
{
  progress(QString::fromUtf8("Downloading latest winetricks..."));
  
  // Create temporary directory
  QString tempDir = QDir::tempPath() + QString::fromUtf8("/winetricks_install_XXXXXX");
  QByteArray tempDirBytes = tempDir.toUtf8();
  char* tempDirPath = mkdtemp(tempDirBytes.data());
  if(!tempDirPath) {
    progress(QString::fromUtf8("Failed to create temporary directory"));
    return false;
  }
  tempDir = QString::fromUtf8(tempDirPath);
  
  // Download latest winetricks
  QProcess downloadProcess;
  downloadProcess.setWorkingDirectory(tempDir);
  downloadProcess.start(QString::fromUtf8("wget"), QStringList() 
    << QString::fromUtf8("--no-verbose")
    << QString::fromUtf8("https://raw.githubusercontent.com/Winetricks/winetricks/master/src/winetricks"));
  
  if(!downloadProcess.waitForFinished(30000)) { // 30 second timeout
    progress(QString::fromUtf8("Download timed out"));
    QDir(tempDir).removeRecursively();
    return false;
  }
  
  if(downloadProcess.exitCode() != 0) {
    progress(QString::fromUtf8("Download failed"));
    QDir(tempDir).removeRecursively();
    return false;
  }
  
  // Make winetricks executable
  QString winetricksPath = tempDir + QString::fromUtf8("/winetricks");
  QFile winetricksFile(winetricksPath);
  if(!winetricksFile.setPermissions(QFile::ExeOwner | QFile::ExeUser | QFile::ExeGroup | QFile::ExeOther)) {
    progress(QString::fromUtf8("Failed to set executable permissions"));
    QDir(tempDir).removeRecursively();
    return false;
  }
  
  // Install to /usr/local/bin (requires sudo)
  progress(QString::fromUtf8("Installing winetricks to /usr/local/bin..."));
  QProcess installProcess;
  installProcess.start(QString::fromUtf8("sudo"), QStringList() 
    << QString::fromUtf8("mv") 
    << winetricksPath 
    << QString::fromUtf8("/usr/local/bin/winetricks"));
  
  if(!installProcess.waitForFinished(30000)) { // 30 second timeout
    progress(QString::fromUtf8("Installation timed out"));
    QDir(tempDir).removeRecursively();
    return false;
  }
  
  if(installProcess.exitCode() != 0) {
    progress(QString::fromUtf8("Installation failed - user may need to run with sudo privileges"));
    QDir(tempDir).removeRecursively();
    return false;
  }
  
  // Clean up temporary directory
  QDir(tempDir).removeRecursively();
  
  progress(QString::fromUtf8("Latest winetricks installed successfully"));
  return true;
}

void Mfc42uExtractor::cabextractFinished(int exitCode, QProcess::ExitStatus status)
{
  if((exitCode != 0) || (status != QProcess::NormalExit)){
    QMessageBox::warning(this, QString::fromUtf8("Error running cabextract"),
      QString::fromUtf8("There was an error extracting\n"
      "the VC redistributable.\n"
      "Please see the log for more details.\n\n")
    );
    enableButtons(true);
    return;
  }
  switch(stage){
    case 0:{
        stage = 1;
        QString file = winePrefix + QString::fromUtf8("/vcredist.exe");
        progress(QString::fromUtf8("Extracting %1").arg(file));
        QString c = PREF.getDataPath(QString::fromUtf8("/../../helper/cabextract"));
        QStringList args;
        args << file;
        cabextract->start(c, args);
      }
      break;
    case 1:{
        stage = 0;
        destPath = PrefProxy::getRsrcDirPath() + QString::fromUtf8("/tir_firmware/mfc42u.dll");
        QString srcPath = winePrefix + QString::fromUtf8("/mfc42u.dll");
        if(!QFile::copy(srcPath, destPath)){
          QMessageBox::warning(this, QString::fromUtf8("Error extracting mfc42u.dll"),
            QString::fromUtf8("There was an error extracting mfc42.dll.\n"
            "Please see the help to learn other ways\n"
          "ways of obtaining this file.\n\n")
          );
        }else{
          progress(QString::fromUtf8("Mfc42u.dll extracted successfuly"));
        }
        enableButtons(true);
        emit finished(true);
        hide();
      }
      break;
    default:
      break;
  }
}


void TirFwExtractor::commenceExtraction(QString file)
{
#ifndef DARWIN
  QMessageBox::information(this, QString::fromUtf8("Instructions"),
  QString::fromUtf8("NP's TrackIR installer might pop up now.\n\n"
  "If it does, install it with all components to the default location, so the firmware and other necessary "
  "elements can be extracted.\n\n"
  "The software will be installed to the wine sandbox, that will be deleted afterwards, so "
  "there are no leftovers.")
  );
#endif
  qDebug()<<winePrefix;
  progress(QString::fromUtf8("Initializing wine and running installer %1").arg(file));
  //To avoid adding TrackIR icons/menus to Linux "start menu"...
  wine->setEnv(QString::fromUtf8("WINEDLLOVERRIDES"), QString::fromUtf8("winemenubuilder.exe=d"));
  //To redirect wine's Desktop directory to avoid TrackIR icon being placed on Linux desktop
  QFile xdgFile(winePrefix + QString::fromUtf8("/user-dirs.dirs"));
  if(xdgFile.open(QFile::WriteOnly | QFile::Truncate)){
    QTextStream xdg(&xdgFile);
    xdg<<"XDG_DESKTOP_DIR=\""<<winePrefix<<"\"\n";
    xdgFile.close();
    wine->setEnv(QString::fromUtf8("XDG_CONFIG_HOME"), winePrefix);
  }
  wine->setEnv(QString::fromUtf8("WINEPREFIX"), winePrefix);
  installerFile = file;
  // Try different silent installation parameters
  QStringList args;
  args << QStringLiteral("/VERYSILENT") << QStringLiteral("/NORESTART") << QStringLiteral("/SUPPRESSMSGBOXES");
  wine->run(installerFile, args);
}


void Mfc42uExtractor::commenceExtraction(QString file)
{
  stage = 0;
#ifndef DARWIN
  progress(QString::fromUtf8("Initializing wine and extracting %1").arg(file));
  QStringList args;
  args << QStringLiteral("/C") << QStringLiteral("/Q") << QStringLiteral("/T:c:\\");
  wine->setEnv(QString::fromUtf8("WINEPREFIX"), winePrefix);
  wine->run(file, args);
#else
  progress(QString::fromUtf8("Starting cabextract to extract '%1' in '%2'.").arg(file).arg(winePrefix));
  QString c = PREF.getDataPath(QString::fromUtf8("/../../helper/cabextract"));
  cabextract->setWorkingDirectory(winePrefix);
  QStringList args;
  args << file;
  cabextract->start(c, args);
#endif
}

bool Extractor::tryBlob(const QString& installerName)
{
  QString blob_name{getBlobName(installerName)};
  if(blob_name.isEmpty()){
    return false;
  }
  progress(QStringLiteral("Found blob. Commencing extraction."));
  destPath = makeDestPath(PrefProxy::getRsrcDirPath());
  QString blob_w_path = PrefProxy::getDataPath(blob_name);
  return 0 == extract_blob(installerName.toUtf8().data(),
                      destPath.toUtf8().data());
}

void Extractor::on_BrowseInstaller_pressed()
{
  enableButtons(false);
  ui.BrowseInstaller->setEnabled(false);
  QString file = QFileDialog::getOpenFileName(this, QString::fromUtf8("Open an installer:"));
  if(file.isEmpty()){
    enableButtons(true);
    return;
  }
  if(tryBlob(file)){
    progress(QStringLiteral("Blob extraction finished."));
    enableButtons(true);
    linkResult(destPath);
    return;
  }
  
  winePrefix = QDir::tempPath();
  winePrefix += QString::fromUtf8("/wineXXXXXX");
  QByteArray charData = winePrefix.toUtf8();
  char *prefix = mkdtemp(charData.data());
  if(prefix == NULL){
    enableButtons(true);
    return;
  }
  winePrefix = QString::fromUtf8(prefix);
  commenceExtraction(file);
}


void TirFwExtractor::browseDirPressed()
{
  enableButtons(false);
  QString dirName = QFileDialog::getExistingDirectory(this,
    QString::fromUtf8("Open a directory containing unpacked TrackIR driver:"));
  if(dirName.isEmpty()){
    enableButtons(true);
    return;
  }
  destPath = makeDestPath(PrefProxy::getRsrcDirPath());
  et->start(targets, dirName, destPath);
}

void TirFwExtractor::on_AnalyzeSourceButton_pressed()
{
  enableButtons(false);
  targets.clear();
  QString dirName = QFileDialog::getExistingDirectory(this, QString::fromUtf8("Open a wine directory:"));
  if(dirName.isEmpty()){
    enableButtons(true);
    return;
  }
  QDir dir(dirName);
  QFileInfoList files = dir.entryInfoList(QDir::Files | QDir::Readable);
  FastHash hash;
  for(int i = 0; i < files.size(); ++i){
    uint16_t fh;
    qint64 size;
    QByteArray sha1, md5;
    hashFile(files[i].canonicalFilePath(), size, fh, md5, sha1);
    qDebug()<<files[i].fileName()<<QString::fromUtf8(" ")<<size<<fh<<md5<<sha1;
    BlockId blk(files[i].fileName(), size, fh, md5, sha1);
    targets.insert(std::pair<uint16_t, BlockId>(fh, blk));
  }

  QFile of(QString::fromUtf8("spec.txt"));
  if(!of.open(QFile::WriteOnly | QFile::Truncate)){
    enableButtons(true);
    return;
  }
  QTextStream out(&of);

  for(targets_iterator_t it = targets.begin(); it != targets.end(); ++it){
    it->second.save(out);
  }
  of.close();
  enableButtons(true);
}

void Extractor::progress(const QString &msg)
{
  ui.LogView->appendPlainText(msg);
  ltr_int_log_message(msg.toUtf8().constData());
}

void TirFwExtractor::threadFinished()
{
  enableButtons(true);
  bool everything = et->haveEverything();
  if(everything){
    linkResult(destPath);
  }else{
    QMessageBox::warning(NULL, QString::fromUtf8("Firmware extraction unsuccessfull"),
      QString::fromUtf8("Some of the files needed to fully utilize TrackIR were not "
      "found! Please see the log for more details.")
    );
  }
  emit finished(everything);
  hide();
}


void Extractor::enableButtons(bool enable)
{
  ui.BrowseInstaller->setEnabled(enable);
  ui.BrowseDir->setEnabled(enable);
  ui.DownloadButton->setEnabled(enable);
  ui.QuitButton->setEnabled(enable);
}

void TirFwExtractor::enableButtons(bool enable)
{
  ui.BrowseInstaller->setEnabled(haveSpec && enable);
  ui.BrowseDir->setEnabled(haveSpec && enable);
  ui.DownloadButton->setEnabled(haveSpec && enable);
  ui.QuitButton->setEnabled(enable);
}

void Mfc42uExtractor::enableButtons(bool enable)
{
  ui.BrowseInstaller->setEnabled(enable);
  ui.DownloadButton->setEnabled(enable);
  ui.QuitButton->setEnabled(enable);
}


void Extractor::on_QuitButton_pressed()
{
  hide();
  //emit finished(et->haveEverything());
}

void TirFwExtractor::on_QuitButton_pressed()
{
  if(et->isRunning()){
    et->stop();
    et->wait();
  }
  hide();
  //emit finished(et->haveEverything());
}

void Extractor::on_DownloadButton_pressed()
{
  QString target(ui.FWCombo->currentText());
  qDebug()<<QString::fromUtf8("Going to download ")<<target;
  
  // Check if target is empty
  if(target.isEmpty()) {
    progress(QString::fromUtf8("Error: No download target selected"));
    QMessageBox::warning(NULL, QString::fromUtf8("No Target Selected"),
      QString::fromUtf8("Please select a download target from the dropdown menu.")
    );
    return;
  }
  
  winePrefix = QDir::tempPath();
  winePrefix += QString::fromUtf8("/wineXXXXXX");
  QByteArray charData = winePrefix.toUtf8();
  char *prefix = mkdtemp(charData.data());
  if(prefix == NULL){
    return;
  }
  enableButtons(false);
  winePrefix = QString::fromUtf8(prefix);
  progressDlg->show();
  progressDlg->raise();
  progressDlg->activateWindow();
  dl->download(target, winePrefix);
}


void Extractor::downloadDone(bool ok, QString fileName)
{
  progressDlg->hide();
  if(ok){
    progress(QString::fromUtf8("Downloading finished!"));
    commenceExtraction(fileName);
  }else{
    progress(QString::fromUtf8("Download failed - re-enabling buttons"));
    QMessageBox::warning(NULL, QString::fromUtf8("Download unsuccessfull"),
      QString::fromUtf8("Download of the file was unsuccessful.\n"
      "Please check your network connection and try again;\n"
      "you can also download the file yourself and\n"
      "use the \"Extract from installer\" button to extract it.")
    );
    // Force re-enable all buttons
    enableButtons(true);
    // Also ensure the quit button is enabled
    ui.QuitButton->setEnabled(true);
  }
}

void Extractor::on_HelpButton_pressed()
{
  HelpViewer::ChangePage(QString::fromUtf8("extractor.htm"));
  HelpViewer::ShowWindow();
}

void Extractor::show()
{
  HelpViewer::ChangePage(QString::fromUtf8("extractor.htm"));
  QDialog::show();
}


/*
WINEDLLOVERRIDES=winemenubuilder.exe=d WINEPREFIX=/home/qbuilder/devel/research/extractor/test/TrackIR_5.2.Final wine TrackIR_5.2.Final.exe /s /v"/qb"
*/

#include "moc_extractor.cpp"

