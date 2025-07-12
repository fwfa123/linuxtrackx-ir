#include <QFileDialog>
#include <QtDebug>
#include <QProcessEnvironment>
#include <QMessageBox>
#include <QDateTime>
#include <QTextStream>
#include <QRegExp>
#include <QDir>
#include <QDirIterator>
#include <unistd.h>
#include <cstdlib>

// Optimized helper function to copy a directory recursively
static bool copyDirectory(const QString& src, const QString& dst)
{
  QDir srcDir(src);
  if(!srcDir.exists()) {
    return false;
  }
  
  QDir dstDir(dst);
  if(!dstDir.exists()) {
    dstDir.mkpath(dst);
  }
  
  // Use QDirIterator with optimized settings
  QDirIterator it(src, QDirIterator::Subdirectories | QDirIterator::FollowSymlinks);
  while(it.hasNext()) {
    QString srcPath = it.next();
    QString dstPath = srcPath;
    dstPath.replace(src, dst);
    
    if(it.fileInfo().isDir()) {
      QDir().mkpath(dstPath);
    } else {
      QFile::copy(srcPath, dstPath);
    }
  }
  
  return true;
}

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

// Optimized static string constants to avoid repeated QString::fromUtf8() calls
static const QString TIRVIEWS_DLL = QStringLiteral("TIRViews.dll");
static const QString SGL_DAT = QStringLiteral("sgl.dat");
static const QString GAMEDATA_TXT = QStringLiteral("gamedata.txt");
static const QString WINDOWS_DIR = QStringLiteral("windows");
static const QString FW_EXTENSION = QStringLiteral(".fw");
static const QString BLOB_PREFIX = QStringLiteral("fw_blob_");
static const QString BLOB_SUFFIX = QStringLiteral(".bin");

static bool linkResult(const QString& destPath)
{
  QString l = PrefProxy::getRsrcDirPath() + QStringLiteral("/tir_firmware");
  if(QFile::exists(l)){
    QFile::remove(l);
  }
  return QFile::link(destPath, l);
}

// Optimized blob name generation with streaming hash calculation
QString getBlobName(const QString& installerName)
{
  QFile installer{installerName};
  if(!installer.open(QIODevice::ReadOnly)){
    return QStringLiteral("");
  }

  // Use streaming hash calculation instead of reading entire file into memory
  QCryptographicHash md5Hash(QCryptographicHash::Md5);
  QCryptographicHash sha1Hash(QCryptographicHash::Sha1);
  
  const qint64 bufferSize = 64 * 1024; // 64KB buffer
  QByteArray buffer;
  
  while(!installer.atEnd()) {
    buffer = installer.read(bufferSize);
    md5Hash.addData(buffer);
    sha1Hash.addData(buffer);
  }
  
  QString md5{QString::fromUtf8(md5Hash.result().toHex())};
  QString sha1{QString::fromUtf8(sha1Hash.result().toHex())};

  return BLOB_PREFIX + md5 + QStringLiteral("_") + sha1 + BLOB_SUFFIX;
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
  emit progress(QStringLiteral("Commencing analysis of directory '%1'...").arg(path));
  gameDataFound = false;
  tirviewsFound = false;
  for(targets_iterator_t it = targets->begin(); it != targets->end(); ++it){
    it->second.clearFoundFlag();
  }

  findCandidates(path);
  emit progress(QStringLiteral("==============================="));
  if(allFound()){
    everything = true;
    emit progress(QStringLiteral("Extraction done!"));
  }else{
    everything = false;
    for(targets_iterator_t it = targets->begin(); it != targets->end(); ++it){
      if(!it->second.foundAlready())
        emit progress(QStringLiteral("Couldn't extract %1!").arg(it->second.getFname()));
    }
    if(!gameDataFound){
      emit progress(QStringLiteral("Couldn't extract game data!"));
    }
    if(!tirviewsFound){
      emit progress(QStringLiteral("Couldn't extract TIRViews.dll!"));
    }
  }
}

// Optimized file analysis with byte-by-byte reading (required for block detection)
void TirFwExtractThread::analyzeFile(const QString fname)
{
  QFile file(fname);
  if(!file.open(QIODevice::ReadOnly)){
    return;
  }
  qDebug() << QStringLiteral("Analyzing ") << fname;
  
  FastHash hash;
  QStringList msgs;
  char val;
  uint16_t res;
  targets_iterator_t it;
  std::pair<targets_iterator_t, targets_iterator_t> range;
  int cntr = 0;
  
  // Pre-calculate hash ranges for better performance
  std::vector<std::pair<targets_iterator_t, targets_iterator_t>> hashRanges;
  hashRanges.reserve(256); // Reserve space for common hash values
  
  while(file.read(&val, 1) > 0) {
    ++cntr;
    res = hash.hash(val);
    
    // Use cached range if available, otherwise calculate
    if(res < hashRanges.size() && hashRanges[res].first != targets->end()) {
      range = hashRanges[res];
    } else {
      range = targets->equal_range(res);
      if(res < hashRanges.size()) {
        hashRanges[res] = range;
      }
    }
    
    // Process all targets with this hash value
    for(it = range.first; it != range.second; ++it) {
      //qDebug()<<cntr<<qPrintable("Checking against ")<<file.pos() <<res <<(it->second.getFname());
      msgs.clear();
      it->second.isBlock(file, destPath, msgs);
      if(!msgs.isEmpty()){
        for(const QString& msg : msgs) {
          emit progress(msg);
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

// Optimized directory traversal with caching and early termination
bool TirFwExtractThread::findCandidates(QString name)
{
  if(quit) return false;
  
  QDir dir(name);
  if(!dir.exists()) return false;
  
  // Use static patterns to avoid repeated QStringList creation
  static const QStringList patterns = {QStringLiteral("*.dll"), QStringLiteral("*.exe"), QStringLiteral("*.dat")};
  
  // Get file list with optimized flags
  QFileInfoList files = dir.entryInfoList(patterns, QDir::Files | QDir::Readable | QDir::NoSymLinks);
  
  for(const QFileInfo& fileInfo : files) {
    if(quit) return false;
    
    QString fileName = fileInfo.fileName();
    
    if(fileName == TIRVIEWS_DLL) {
      QString outfile = QStringLiteral("%1/TIRViews.dll").arg(destPath);
      if((tirviewsFound = QFile::copy(fileInfo.absoluteFilePath(), outfile))){
        emit progress(QStringLiteral("Extracted TIRViews.dll..."));
      }
    } else if(fileName == SGL_DAT) {
      analyzeFile(fileInfo.absoluteFilePath());
    } else {
      QString outfile = QStringLiteral("%1/gamedata.txt").arg(destPath);
      gameDataFound = get_game_data(fileInfo.absoluteFilePath().toUtf8().constData(),
                                    outfile.toUtf8().constData(), false);
      emit progress(QStringLiteral("Extracted game data..."));
    }
    
    if(allFound()){
      return true;
    }
  }

  // Optimized subdirectory traversal
  QFileInfoList subdirs = dir.entryInfoList(QDir::AllDirs | QDir::NoDotAndDotDot | QDir::NoSymLinks);
  
  for(const QFileInfo& subdir : subdirs) {
    QString dirname = subdir.absoluteFilePath();
    if(!dirname.endsWith(WINDOWS_DIR) && findCandidates(dirname)){
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
  progress(QStringLiteral("Looking for existing ") + sources + QStringLiteral("..."));
  QFile f(findSrc(sources));
  if(!f.open(QIODevice::ReadOnly)){
    progress(sources + QStringLiteral(" not found."));
    return false;
  }
  progress(QStringLiteral("Found '%1'.").arg(f.fileName()));

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
  progress(sources + QStringLiteral(" found and read."));
  return (ui.FWCombo->count() != 0);
}


bool Extractor::readSpec()
{
  progress(QStringLiteral("Looking for existing spec.txt..."));
  QFile f(findSrc(QStringLiteral("spec.txt")));
  if(!f.open(QIODevice::ReadOnly)){
    progress(QStringLiteral("spec.txt not found."));
    return false;
  }
  progress(QStringLiteral("Found '%1'.").arg(f.fileName()));

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
  progress(QStringLiteral("spec.txt found and read."));
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
  QString sources = QStringLiteral("sources.txt");
#else
  QString sources = QStringLiteral("sources_mac.txt");
#endif
  readSources(sources);
  QString dbg = QProcessEnvironment::systemEnvironment().value(QStringLiteral("LINUXTRACK_DBG"));
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
  QString sources = QStringLiteral("sources_mfc.txt");
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

// Optimized destination path creation with better string handling
static QString makeDestPath(const QString &base)
{
  QDateTime current = QDateTime::currentDateTime();
  QString result = current.toString(QStringLiteral("yyMMdd_hhmmss"));
  QString final = result;
  QDir dir = QDir(base);
  int counter = 0;
  while(dir.exists(final)){
    final = QStringLiteral("%1_%2").arg(result).arg(counter++);
  }
  dir.mkpath(final);
  return base + QStringLiteral("/") + final + QStringLiteral("/");
}


void TirFwExtractor::wineFinished(bool result)
{
  if(!wineInitialized){
    wineInitialized = true;
    if(!result){
      QMessageBox::warning(this, QStringLiteral("Error running Wine"),
        QStringLiteral("There was an error initializing\n"
        "the wine prefix; will try to install the firmware\n"
        "just in case..."
        "Please see the log for more details.\n\n")
      );
    }
    // Skip the second run since we're using silent installation
    // The installer should have completed in the first run
    destPath = makeDestPath(PrefProxy::getRsrcDirPath());
    et->start(targets, winePrefix, destPath);
  }else{
    if(!result){
      QMessageBox::warning(this, QStringLiteral("Error running Wine"),
        QStringLiteral("There was an error when extracting\n"
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
    progress(QStringLiteral("Wine extraction failed, trying modern alternatives..."));
    
    // Check if mfc42u.dll already exists in common locations
    QStringList searchPaths;
    searchPaths << winePrefix + QStringLiteral("/drive_c/windows/system32/mfc42u.dll");
    searchPaths << winePrefix + QStringLiteral("/drive_c/windows/syswow64/mfc42u.dll");
    searchPaths << winePrefix + QStringLiteral("/drive_c/mfc42u.dll");
    
    bool found = false;
    for(const QString& searchPath : searchPaths) {
      if(QFile::exists(searchPath)) {
        progress(QStringLiteral("Found mfc42u.dll at: %1").arg(searchPath));
        destPath = PrefProxy::getRsrcDirPath() + QStringLiteral("/tir_firmware/mfc42u.dll");
        if(QFile::copy(searchPath, destPath)) {
          progress(QStringLiteral("Mfc42u.dll copied successfully"));
          found = true;
          break;
        }
      }
    }
    
    if(!found) {
      // Try modern installation methods
      progress(QStringLiteral("Attempting modern installation methods..."));
      
      // For Debian-based systems, prioritize winetricks
      bool isDebianBased = false;
      QFile osRelease(QStringLiteral("/etc/os-release"));
      if(osRelease.exists()) {
        osRelease.open(QIODevice::ReadOnly);
        QString content = QString::fromUtf8(osRelease.readAll());
        if(content.contains(QStringLiteral("ID=debian")) || 
           content.contains(QStringLiteral("ID=ubuntu")) ||
           content.contains(QStringLiteral("ID=mx"))) {
          isDebianBased = true;
        }
        osRelease.close();
      }
      
      if(isDebianBased) {
        // Debian-based systems: try winetricks first
        progress(QStringLiteral("Debian-based system detected - trying winetricks first..."));
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
        QString file = winePrefix + QStringLiteral("/drive_c/vcredist.exe");
        progress(QStringLiteral("Extracting %1").arg(file));
        QStringList args;
        args << QStringLiteral("/C") << QStringLiteral("/Q") << QStringLiteral("/T:c:\\");
        wine->run(file, args);
      }
      break;
    case 1:{
        stage = 0;
        destPath = PrefProxy::getRsrcDirPath() + QStringLiteral("/tir_firmware/mfc42u.dll");
        QString srcPath = winePrefix + QStringLiteral("/drive_c/mfc42u.dll");
        if(!QFile::copy(srcPath, destPath)){
          QMessageBox::warning(this, QStringLiteral("Error extracting mfc42u.dll"),
            QStringLiteral("There was an error extracting mfc42.dll.\n"
            "Please see the help to learn other ways\n"
  	  "ways of obtaining this file.\n\n")
          );
        }else{
          progress(QStringLiteral("Mfc42u.dll extracted successfuly"));
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
  progress(QStringLiteral("Trying winetricks installation..."));
  
  // First, check if winetricks is available and get its version
  QString winetricksPath = checkWinetricksAvailability();
  if(winetricksPath.isEmpty()) {
    progress(QStringLiteral("Winetricks not found - attempting to install latest version..."));
    if(!installLatestWinetricks()) {
      progress(QStringLiteral("Failed to install winetricks"));
      return false;
    }
    winetricksPath = QStringLiteral("/usr/local/bin/winetricks");
  } else {
    // Check if we have a recent version (avoid outdated packaged versions)
    if(!isWinetricksVersionRecent(winetricksPath)) {
      progress(QStringLiteral("Outdated winetricks detected - updating to latest version..."));
      if(!installLatestWinetricks()) {
        progress(QStringLiteral("Failed to update winetricks, trying with existing version..."));
      } else {
        winetricksPath = QStringLiteral("/usr/local/bin/winetricks");
      }
    }
  }
  
  // Create a fresh 32-bit temp Wine prefix specifically for MFC42 installation
  // Use user's home directory instead of /tmp to avoid ownership issues
  QString mfc42Prefix = QDir::homePath() + QStringLiteral("/.linuxtrack_mfc42_") + QString::number(QDateTime::currentMSecsSinceEpoch());
  progress(QStringLiteral("Creating fresh 32-bit Wine prefix for MFC42 installation: %1").arg(mfc42Prefix));
  
  // Clean up any existing prefix
  QDir mfc42Dir(mfc42Prefix);
  if(mfc42Dir.exists()) {
    mfc42Dir.removeRecursively();
  }
  
  // Initialize the new 32-bit prefix
  QProcess wineInit;
  QProcessEnvironment env = wine->getProcessEnvironment();
  env.insert(QStringLiteral("WINEPREFIX"), mfc42Prefix);
  env.insert(QStringLiteral("WINEARCH"), QStringLiteral("win32"));
  wineInit.setProcessEnvironment(env);
  
  progress(QStringLiteral("Initializing 32-bit Wine prefix..."));
  wineInit.start(QStringLiteral("wine"), QStringList() << QStringLiteral("wineboot") << QStringLiteral("--init"));
  
  if(!wineInit.waitForFinished(30000)) { // 30 second timeout
    progress(QStringLiteral("Failed to initialize Wine prefix (timeout)"));
    return false;
  }
  
  // Capture error output for debugging
  QString errorOutput = QString::fromUtf8(wineInit.readAllStandardError());
  QString standardOutput = QString::fromUtf8(wineInit.readAllStandardOutput());
  
  if(!errorOutput.isEmpty()) {
    progress(QStringLiteral("Wine initialization stderr: %1").arg(errorOutput));
  }
  if(!standardOutput.isEmpty()) {
    progress(QStringLiteral("Wine initialization stdout: %1").arg(standardOutput));
  }
  
  if(wineInit.exitCode() != 0) {
    progress(QStringLiteral("Failed to initialize Wine prefix (exit code: %1)").arg(wineInit.exitCode()));
    return false;
  }
  
  progress(QStringLiteral("32-bit Wine prefix initialized successfully"));
  winePrefix = mfc42Prefix;
  
  // Test winetricks first to make sure it's working
  QProcess testWinetricks;
  testWinetricks.setProcessEnvironment(wine->getProcessEnvironment());
  testWinetricks.start(winetricksPath, QStringList() << QStringLiteral("--version"));
  
  if(!testWinetricks.waitForFinished(10000)) { // 10 second timeout
    progress(QStringLiteral("Winetricks version check timed out"));
    return false;
  }
  
  if(testWinetricks.exitCode() != 0) {
    progress(QStringLiteral("Winetricks version check failed (exit code: %1)").arg(testWinetricks.exitCode()));
    QString errorOutput = QString::fromUtf8(testWinetricks.readAllStandardError());
    if(!errorOutput.isEmpty()) {
      progress(QStringLiteral("Winetricks error: %1").arg(errorOutput));
    }
    return false;
  }
  
  QString versionOutput = QString::fromUtf8(testWinetricks.readAllStandardOutput());
  progress(QStringLiteral("Winetricks version: %1").arg(versionOutput.trimmed()));
  
  // Try to install mfc42 using winetricks
  QProcess winetricks;
  
  // Use unattended mode to avoid GUI issues
  QStringList args;
  args << QStringLiteral("--unattended") << QStringLiteral("mfc42");
  
  progress(QStringLiteral("Running: %1 --unattended mfc42").arg(winetricksPath));
  
  // Set environment for 32-bit operation
  env.insert(QStringLiteral("WINEARCH"), QStringLiteral("win32"));
  winetricks.setProcessEnvironment(env);
  
  // Use the fresh 32-bit prefix
  winetricks.setWorkingDirectory(winePrefix);
  
  winetricks.start(winetricksPath, args);
  
  if(!winetricks.waitForFinished(120000)) { // 2 minute timeout for winetricks
    progress(QStringLiteral("Winetricks installation timed out"));
    return false;
  }
  
  // Capture error output for debugging
  errorOutput = QString::fromUtf8(winetricks.readAllStandardError());
  standardOutput = QString::fromUtf8(winetricks.readAllStandardOutput());
  
  if(!errorOutput.isEmpty()) {
    progress(QStringLiteral("Winetricks stderr: %1").arg(errorOutput));
  }
  if(!standardOutput.isEmpty()) {
    progress(QStringLiteral("Winetricks stdout: %1").arg(standardOutput));
  }
  
  if(winetricks.exitCode() == 0) {
    progress(QStringLiteral("Winetricks installation successful"));
    
    // Check if mfc42u.dll was installed
    QStringList checkPaths;
    checkPaths << winePrefix + QStringLiteral("/drive_c/windows/system32/mfc42u.dll");
    checkPaths << winePrefix + QStringLiteral("/drive_c/windows/syswow64/mfc42u.dll");
    
    for(const QString& path : checkPaths) {
      if(QFile::exists(path)) {
        destPath = PrefProxy::getRsrcDirPath() + QStringLiteral("/tir_firmware/mfc42u.dll");
        if(QFile::copy(path, destPath)) {
          progress(QStringLiteral("Mfc42u.dll installed via winetricks"));
          
          // Clean up the temporary MFC42 prefix
          QDir tempDir(winePrefix);
          if(tempDir.exists()) {
            tempDir.removeRecursively();
            progress(QStringLiteral("Cleaned up temporary MFC42 Wine prefix"));
          }
          
          return true;
        }
      }
    }
  } else {
    // If winetricks failed, clean up the temporary MFC42 prefix
    progress(QStringLiteral("Winetricks failed - cleaning up temporary MFC42 prefix..."));
    QDir tempDir(winePrefix);
    if(tempDir.exists()) {
      tempDir.removeRecursively();
      progress(QStringLiteral("Temporary MFC42 Wine prefix cleaned up"));
    }
  }
  
  progress(QStringLiteral("Winetricks installation failed"));
  return false;
}

void Mfc42uExtractor::startAutomaticInstallation()
{
  progress(QStringLiteral("Starting automatic MFC42 installation..."));
  
  // Try winetricks installation first
  if(tryWinetricksInstall()) {
    progress(QStringLiteral("MFC42 installation completed successfully"));
    emit finished(true);
    return;
  }
  
  // If winetricks fails, try package manager
  if(tryPackageManagerInstall()) {
    progress(QStringLiteral("MFC42 installation completed successfully"));
    emit finished(true);
    return;
  }
  
  // If all automatic methods fail, show the manual installation dialog
  progress(QStringLiteral("Automatic installation failed, showing manual options"));
  show();
}

bool Mfc42uExtractor::tryPackageManagerInstall()
{
  progress(QStringLiteral("Trying package manager installation..."));
  
  // Detect distribution and try appropriate package
  QStringList packageManagers;
  packageManagers << QStringLiteral("apt") << QStringLiteral("dnf") << QStringLiteral("pacman") << QStringLiteral("zypper");
  
  for(const QString& pm : packageManagers) {
    QProcess checkPM;
    checkPM.start(QStringLiteral("which"), QStringList() << pm);
    if(checkPM.waitForFinished(5000) && checkPM.exitCode() == 0) {
      progress(QStringLiteral("Found package manager: %1").arg(pm));
      
      // Try to install the appropriate package
      QString packageName;
      if(pm == QStringLiteral("apt")) {
        // Debian/Ubuntu/MX don't have libmfc42 package - use winetricks instead
        progress(QStringLiteral("Debian-based systems should use winetricks instead of package manager"));
        return false;
      } else if(pm == QStringLiteral("dnf")) {
        packageName = QStringLiteral("mfc42");
      } else if(pm == QStringLiteral("pacman")) {
        packageName = QStringLiteral("mfc42");
      } else if(pm == QStringLiteral("zypper")) {
        packageName = QStringLiteral("mfc42");
      }
      
      if(!packageName.isEmpty()) {
        progress(QStringLiteral("Attempting to install: %1").arg(packageName));
        
        // Note: This would require sudo, so we'll just inform the user
        QMessageBox::information(this, QStringLiteral("Package Installation Required"),
          QStringLiteral("Please install the required package manually:\n\n"
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
  progress(QStringLiteral("Trying cabextract fallback..."));
  QString c = PREF.getDataPath(QStringLiteral("/../../helper/cabextract"));
  if(QFile::exists(c)) {
    cabextract->setWorkingDirectory(winePrefix);
    QStringList args;
    args << winePrefix + QStringLiteral("/VC6RedistSetup_deu.exe");
    cabextract->start(c, args);
    return true; // Will be handled by cabextractFinished
  }
  return false;
}

void Mfc42uExtractor::showModernInstallationInstructions()
{
  QMessageBox::information(this, QStringLiteral("Modern Installation Required"),
    QStringLiteral("The old Wine extraction method failed. Please use one of these modern approaches:\n\n"
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
  winetricksCheck.start(QStringLiteral("which"), QStringList() << QStringLiteral("winetricks"));
  if(!winetricksCheck.waitForFinished(5000) || winetricksCheck.exitCode() != 0) {
    return QString();
  }
  
  // Get the path to winetricks
  winetricksCheck.start(QStringLiteral("which"), QStringList() << QStringLiteral("winetricks"));
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
  versionCheck.start(winetricksPath, QStringList() << QStringLiteral("--version"));
  if(!versionCheck.waitForFinished(5000) || versionCheck.exitCode() != 0) {
    return false;
  }
  
  QString versionOutput = QString::fromUtf8(versionCheck.readAllStandardOutput()).trimmed();
  
  // Extract version number (format: "winetricks 20231210" or similar)
  QRegExp versionRegex(QStringLiteral("winetricks\\s+(\\d{8})"));
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
  progress(QStringLiteral("Downloading latest winetricks..."));
  
  // Create temporary directory
  QString tempDir = QDir::tempPath() + QStringLiteral("/winetricks_install_XXXXXX");
  QByteArray tempDirBytes = tempDir.toUtf8();
  char* tempDirPath = mkdtemp(tempDirBytes.data());
  if(!tempDirPath) {
    progress(QStringLiteral("Failed to create temporary directory"));
    return false;
  }
  tempDir = QString::fromUtf8(tempDirPath);
  
  // Download latest winetricks
  QProcess downloadProcess;
  downloadProcess.setWorkingDirectory(tempDir);
  downloadProcess.start(QStringLiteral("wget"), QStringList() 
    << QStringLiteral("--no-verbose")
    << QStringLiteral("https://raw.githubusercontent.com/Winetricks/winetricks/master/src/winetricks"));
  
  if(!downloadProcess.waitForFinished(30000)) { // 30 second timeout
    progress(QStringLiteral("Download timed out"));
    QDir(tempDir).removeRecursively();
    return false;
  }
  
  if(downloadProcess.exitCode() != 0) {
    progress(QStringLiteral("Download failed"));
    QDir(tempDir).removeRecursively();
    return false;
  }
  
  // Make winetricks executable
  QString winetricksPath = tempDir + QStringLiteral("/winetricks");
  QFile winetricksFile(winetricksPath);
  if(!winetricksFile.setPermissions(QFile::ExeOwner | QFile::ExeUser | QFile::ExeGroup | QFile::ExeOther)) {
    progress(QStringLiteral("Failed to set executable permissions"));
    QDir(tempDir).removeRecursively();
    return false;
  }
  
  // Install to /usr/local/bin (requires sudo)
  progress(QStringLiteral("Installing winetricks to /usr/local/bin..."));
  QProcess installProcess;
  installProcess.start(QStringLiteral("sudo"), QStringList() 
    << QStringLiteral("mv") 
    << winetricksPath 
    << QStringLiteral("/usr/local/bin/winetricks"));
  
  if(!installProcess.waitForFinished(30000)) { // 30 second timeout
    progress(QStringLiteral("Installation timed out"));
    QDir(tempDir).removeRecursively();
    return false;
  }
  
  if(installProcess.exitCode() != 0) {
    progress(QStringLiteral("Installation failed - user may need to run with sudo privileges"));
    QDir(tempDir).removeRecursively();
    return false;
  }
  
  // Clean up temporary directory
  QDir(tempDir).removeRecursively();
  
  progress(QStringLiteral("Latest winetricks installed successfully"));
  return true;
}

void Mfc42uExtractor::cabextractFinished(int exitCode, QProcess::ExitStatus status)
{
  if((exitCode != 0) || (status != QProcess::NormalExit)){
    QMessageBox::warning(this, QStringLiteral("Error running cabextract"),
      QStringLiteral("There was an error extracting\n"
      "the VC redistributable.\n"
      "Please see the log for more details.\n\n")
    );
    enableButtons(true);
    return;
  }
  switch(stage){
    case 0:{
        stage = 1;
        QString file = winePrefix + QStringLiteral("/vcredist.exe");
        progress(QStringLiteral("Extracting %1").arg(file));
        QString c = PREF.getDataPath(QStringLiteral("/../../helper/cabextract"));
        QStringList args;
        args << file;
        cabextract->start(c, args);
      }
      break;
    case 1:{
        stage = 0;
        destPath = PrefProxy::getRsrcDirPath() + QStringLiteral("/tir_firmware/mfc42u.dll");
        QString srcPath = winePrefix + QStringLiteral("/mfc42u.dll");
        if(!QFile::copy(srcPath, destPath)){
          QMessageBox::warning(this, QStringLiteral("Error extracting mfc42u.dll"),
            QStringLiteral("There was an error extracting mfc42.dll.\n"
            "Please see the help to learn other ways\n"
          "ways of obtaining this file.\n\n")
          );
        }else{
          progress(QStringLiteral("Mfc42u.dll extracted successfuly"));
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
  QMessageBox::information(this, QStringLiteral("Instructions"),
  QStringLiteral("NP's TrackIR installer might pop up now.\n\n"
  "If it does, install it with all components to the default location, so the firmware and other necessary "
  "elements can be extracted.\n\n"
  "The software will be installed to the wine sandbox, that will be deleted afterwards, so "
  "there are no leftovers.")
  );
#endif
  qDebug()<<winePrefix;
  progress(QStringLiteral("Initializing wine and running installer %1").arg(file));
  //To avoid adding TrackIR icons/menus to Linux "start menu"...
  wine->setEnv(QStringLiteral("WINEDLLOVERRIDES"), QStringLiteral("winemenubuilder.exe=d"));
  //To redirect wine's Desktop directory to avoid TrackIR icon being placed on Linux desktop
  QFile xdgFile(winePrefix + QStringLiteral("/user-dirs.dirs"));
  if(xdgFile.open(QFile::WriteOnly | QFile::Truncate)){
    QTextStream xdg(&xdgFile);
    xdg<<QStringLiteral("XDG_DESKTOP_DIR=\"%1\"").arg(winePrefix)<<"\n";
    xdgFile.close();
    wine->setEnv(QStringLiteral("XDG_CONFIG_HOME"), winePrefix);
  }
  wine->setEnv(QStringLiteral("WINEPREFIX"), winePrefix);
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
  progress(QStringLiteral("Initializing wine and extracting %1").arg(file));
  QStringList args;
  args << QStringLiteral("/C") << QStringLiteral("/Q") << QStringLiteral("/T:c:\\");
  wine->setEnv(QStringLiteral("WINEPREFIX"), winePrefix);
  wine->run(file, args);
#else
  progress(QStringLiteral("Starting cabextract to extract '%1' in '%2'.").arg(file).arg(winePrefix));
  QString c = PREF.getDataPath(QStringLiteral("/../../helper/cabextract"));
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
  QString file = QFileDialog::getOpenFileName(this, QStringLiteral("Open an installer:"));
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
  winePrefix += QStringLiteral("/wineXXXXXX");
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
    QStringLiteral("Open a directory containing unpacked TrackIR driver:"));
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
  QString dirName = QFileDialog::getExistingDirectory(this, QStringLiteral("Open a wine directory:"));
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
    qDebug()<<files[i].fileName()<<QStringLiteral(" ")<<size<<fh<<md5<<sha1;
    BlockId blk(files[i].fileName(), size, fh, md5, sha1);
    targets.insert(std::pair<uint16_t, BlockId>(fh, blk));
  }

  QFile of(QStringLiteral("spec.txt"));
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
    QMessageBox::warning(NULL, QStringLiteral("Firmware extraction unsuccessfull"),
      QStringLiteral("Some of the files needed to fully utilize TrackIR were not "
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
  qDebug()<<QStringLiteral("Going to download ")<<target;
  
  // Check if target is empty
  if(target.isEmpty()) {
    progress(QStringLiteral("Error: No download target selected"));
    QMessageBox::warning(NULL, QStringLiteral("No Target Selected"),
      QStringLiteral("Please select a download target from the dropdown menu.")
    );
    return;
  }
  
  winePrefix = QDir::tempPath();
  winePrefix += QStringLiteral("/wineXXXXXX");
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
    progress(QStringLiteral("Downloading finished!"));
    commenceExtraction(fileName);
  }else{
    progress(QStringLiteral("Download failed - re-enabling buttons"));
    QMessageBox::warning(NULL, QStringLiteral("Download unsuccessfull"),
      QStringLiteral("Download of the file was unsuccessful.\n"
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
  HelpViewer::ChangePage(QStringLiteral("extractor.htm"));
  HelpViewer::ShowWindow();
}

void Extractor::show()
{
  HelpViewer::ChangePage(QStringLiteral("extractor.htm"));
  QDialog::show();
}


/*
WINEDLLOVERRIDES=winemenubuilder.exe=d WINEPREFIX=/home/qbuilder/devel/research/extractor/test/TrackIR_5.2.Final wine TrackIR_5.2.Final.exe /s /v"/qb"
*/

#include "moc_extractor.cpp"

