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

// Helper function to copy a directory recursively
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
  
  QDirIterator it(src, QDirIterator::Subdirectories);
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
  int count = 0;
  while(1){
    if(fs.atEnd()){
      break;
    }
    fs>>url;
    if(!url.isNull()){
      count++;
    }
  }
  progress(sources + QString::fromUtf8(" found and read."));
  return (count != 0);
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
  setWindowIcon(QIcon(QStringLiteral(":/ltr/linuxtrack.svg")));
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
  
  // Hide download UI elements for TIR firmware extractor since we don't provide download links
  ui.downloadLabel->setVisible(false);
  ui.FWCombo->setVisible(false);
  ui.DownloadButton->setVisible(false);
  
  // Update title and instructions for TIR firmware extraction
  ui.label_2->setText(QString::fromUtf8("TrackIR Firmware Extraction"));
  ui.downloadInstructions->setText(QString::fromUtf8(
    "<div style='margin: 0; padding: 0;'>"
    "<p>To extract TrackIR firmware, you need to download the latest TrackIR software from NaturalPoint's website.</p>"
    "<p><b>Steps:</b></p>"
    "<div style='margin-left: 20px;'>"
    "<p style='margin: 5px 0;'>1. Visit <a href='https://www.naturalpoint.com/trackir/'>https://www.naturalpoint.com/trackir/</a></p>"
    "<p style='margin: 5px 0;'>2. Download the latest TrackIR software installer</p>"
    "<p style='margin: 5px 0;'>3. Use the 'Extract from installer' button below to select and process the downloaded file</p>"
    "</div>"
    "<p>The firmware will be extracted from the installer and made available for LinuxTrack.</p>"
    "</div>"
  ));
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
  
  winePrefix = QDir::tempPath() + QString::fromUtf8("/wineXXXXXX");
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
    QMessageBox::warning(this, QString::fromUtf8("Firmware extraction unsuccessfull"),
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
  ui.QuitButton->setEnabled(enable);
}

void TirFwExtractor::enableButtons(bool enable)
{
  ui.BrowseInstaller->setEnabled(haveSpec && enable);
  ui.BrowseDir->setEnabled(haveSpec && enable);
  ui.QuitButton->setEnabled(enable);
}


// MFC140 Extractor Implementation
Mfc140uExtractor::Mfc140uExtractor(QWidget *parent) : Extractor(parent), cabextract(NULL), stage(0)
{
  cabextract = new QProcess(this);
  QObject::connect(cabextract, SIGNAL(finished(int, QProcess::ExitStatus)),
                   this, SLOT(cabextractFinished(int, QProcess::ExitStatus)));
  QObject::connect(wine, SIGNAL(finished(bool)), this, SLOT(wineFinished(bool)));
  QObject::connect(ui.DownloadButton, SIGNAL(pressed()),
                   this, SLOT(on_DownloadButton_pressed()));
  
  // Read sources from the correct location (same as TIR firmware)
  QString sources = QString::fromUtf8("sources_mfc140.txt");
  readSources(sources);
  
  // Set up UI like TIR firmware extractor
  ui.AnalyzeSourceButton->setVisible(false);
  ui.BrowseDir->setEnabled(true);
  ui.BrowseInstaller->setEnabled(true);
  
  // Update button text for MFC140
  ui.BrowseInstaller->setText(QString::fromUtf8("Browse for\nInstaller"));
  ui.BrowseDir->setText(QString::fromUtf8("Browse\nDirectory"));
  
  // Update the UI text for MFC140 installation
  ui.label_2->setText(QString::fromUtf8("Install Visual C++ 2015-2022 MFC Libraries"));
  ui.downloadInstructions->setText(QString::fromUtf8(
    "<div style='margin: 0; padding: 0;'>"
    "<p>To install the required MFC140 libraries, you can:</p>"
    "<div style='margin-left: 20px;'>"
    "<p style='margin: 5px 0;'>1. <b>Download and install</b>: Select a source from the dropdown and click 'Download'</p>"
    "<p style='margin: 5px 0;'>2. <b>Browse for installer</b>: If you already have the Visual C++ 2015-2022 Redistributable</p>"
    "<p style='margin: 5px 0;'>3. <b>Browse directory</b>: If you have extracted the installer to a directory</p>"
    "</div>"
    "<p>The Visual C++ 2015-2022 Redistributable contains the required mfc140u.dll file.</p>"
    "</div>"
  ));
  
  // Set up download combo box and button for MFC140
  ui.downloadLabel->setText(QString::fromUtf8("Select Visual C++ 2015-2022 Redistributable to Download"));
  ui.DownloadButton->setText(QString::fromUtf8("Download and Install"));
  
  // Populate the combo box with sources from sources_mfc140.txt
  populateDownloadCombo();
  
  // Set up cached download path
  cachedDownloadPath = QDir::tempPath() + QString::fromUtf8("/linuxtrack_mfc140_cache");
  QDir().mkpath(cachedDownloadPath);
}

Mfc140uExtractor::~Mfc140uExtractor()
{
}

void Mfc140uExtractor::wineFinished(bool result)
{
  if(!result){
    // Wine extraction failed, try alternative methods
    progress(QString::fromUtf8("Wine extraction failed, trying alternative methods..."));
    
    // Check if mfc140u.dll already exists in common locations
    QStringList searchPaths;
    searchPaths << winePrefix + QString::fromUtf8("/drive_c/windows/system32/mfc140u.dll");
    searchPaths << winePrefix + QString::fromUtf8("/drive_c/windows/syswow64/mfc140u.dll");
    searchPaths << winePrefix + QString::fromUtf8("/drive_c/mfc140u.dll");
    
    bool found = false;
    for(const QString& searchPath : searchPaths) {
      if(QFile::exists(searchPath)) {
        progress(QString::fromUtf8("Found mfc140u.dll at: %1").arg(searchPath));
        destPath = PrefProxy::getRsrcDirPath() + QString::fromUtf8("/tir_firmware/mfc140u.dll");
        if(QFile::copy(searchPath, destPath)) {
          progress(QString::fromUtf8("Mfc140u.dll copied successfully"));
          found = true;
          break;
        }
      }
    }
    
    if(!found) {
      // Try to find cached download
      QString cachedFile = findCachedDownload();
      if(!cachedFile.isEmpty()) {
        progress(QString::fromUtf8("Found cached download: %1").arg(cachedFile));
        if(extractMfc140FromInstaller(cachedFile)) {
          found = true;
        }
      }
      
      if(!found) {
        // Show download instructions
        showDownloadInstructions();
        enableButtons(true);
        emit finished(false);
        return;
      }
    }
    
    if(found) {
      QMessageBox::information(this, QString::fromUtf8("Installation Successful"),
        QString::fromUtf8("Visual C++ 2015-2022 MFC libraries were successfully installed."));
      emit finished(true);
      hide();
    } else {
      QMessageBox::warning(this, QString::fromUtf8("Installation Failed"),
        QString::fromUtf8("MFC140 installation failed. Please try downloading the installer manually."));
      emit finished(false);
    }
    return;
  }
  
  // Handle successful Wine extraction
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
        destPath = PrefProxy::getRsrcDirPath() + QString::fromUtf8("/tir_firmware/mfc140u.dll");
        QString srcPath = winePrefix + QString::fromUtf8("/drive_c/mfc140u.dll");
        if(!QFile::copy(srcPath, destPath)){
          QMessageBox::warning(this, QString::fromUtf8("Error extracting mfc140u.dll"),
            QString::fromUtf8("There was an error extracting mfc140u.dll.\n"
            "Please see the help to learn other ways\n"
  	  "ways of obtaining this file.\n\n")
          );
          enableButtons(true);
          emit finished(false);
        }else{
          progress(QString::fromUtf8("Mfc140u.dll extracted successfully"));
          QMessageBox::information(this, QString::fromUtf8("Installation Successful"),
            QString::fromUtf8("Visual C++ 2015-2022 MFC libraries were successfully installed."));
          enableButtons(true);
          emit finished(true);
          hide();
        }
      }
      break;
    default:
      break;
  }
}





void Mfc140uExtractor::startAutomaticInstallation()
{
  // This method is called when the user clicks "Download" or similar
  // It should follow the same pattern as TIR firmware - show UI first, then proceed
  progress(QString::fromUtf8("Starting MFC140 installation..."));
  
  // Check for cached download first
  QString cachedFile = findCachedDownload();
  if(!cachedFile.isEmpty()) {
    progress(QString::fromUtf8("Found cached download: %1").arg(cachedFile));
    if(extractMfc140FromInstaller(cachedFile)) {
      progress(QString::fromUtf8("MFC140 installation completed successfully"));
      QMessageBox::information(this, QString::fromUtf8("Installation Successful"),
        QString::fromUtf8("Visual C++ 2015-2022 MFC libraries were successfully installed from cached download."));
      enableButtons(true);
      emit finished(true);
      hide();
      return;
    }
  }
  
  // Try to download if no cached file found
  if(downloadVCRedist()) {
    progress(QString::fromUtf8("MFC140 installation completed successfully"));
    QMessageBox::information(this, QString::fromUtf8("Installation Successful"),
      QString::fromUtf8("Visual C++ 2015-2022 MFC libraries were successfully downloaded and installed."));
    enableButtons(true);
    emit finished(true);
    hide();
    return;
  }
  
  // If all automatic methods fail, show the manual installation dialog
  progress(QString::fromUtf8("Automatic installation failed, showing manual options"));
  enableButtons(true);
  // Keep dialog open for manual installation
}

QString Mfc140uExtractor::findCachedDownload()
{
  QDir cacheDir(cachedDownloadPath);
  QStringList filters;
  filters << QString::fromUtf8("vc_redist*.exe");
  QStringList files = cacheDir.entryList(filters, QDir::Files);
  
  if(!files.isEmpty()) {
    // Return the most recent file
    QString mostRecent = files.first();
    QDateTime mostRecentTime = QFileInfo(cacheDir.filePath(mostRecent)).lastModified();
    
    for(const QString& file : files) {
      QDateTime fileTime = QFileInfo(cacheDir.filePath(file)).lastModified();
      if(fileTime > mostRecentTime) {
        mostRecent = file;
        mostRecentTime = fileTime;
      }
    }
    
    return cacheDir.filePath(mostRecent);
  }
  
  return QString();
}

bool Mfc140uExtractor::downloadVCRedist()
{
  progress(QString::fromUtf8("Downloading Visual C++ 2015-2022 Redistributable..."));
  
  // Read sources from sources_mfc140.txt (same location as other sources files)
  QString sourcesFile = PREF.getDataPath(QString::fromUtf8("sources_mfc140.txt"));
  if(!QFile::exists(sourcesFile)) {
    progress(QString::fromUtf8("Sources file not found: %1").arg(sourcesFile));
    progress(QString::fromUtf8("Please ensure sources_mfc140.txt is installed in the data directory"));
    return false;
  }
  
  QFile file(sourcesFile);
  if(!file.open(QIODevice::ReadOnly)) {
    progress(QString::fromUtf8("Cannot open sources file"));
    return false;
  }
  
  QStringList urls;
  while(!file.atEnd()) {
    QString line = QString::fromUtf8(file.readLine()).trimmed();
    if(!line.startsWith(QString::fromUtf8("#")) && !line.isEmpty()) {
      urls << line;
    }
  }
  file.close();
  
  if(urls.isEmpty()) {
    progress(QString::fromUtf8("No download URLs found in sources file"));
    return false;
  }
  
  // Try each URL until one works
  for(const QString& url : urls) {
    progress(QString::fromUtf8("Trying to download from: %1").arg(url));
    
    // Extract filename from URL
    QUrl qurl(url);
    QString filename = qurl.fileName();
    if(filename.isEmpty()) {
      filename = QString::fromUtf8("vc_redist.x64.exe");
    }
    
    QString downloadPath = cachedDownloadPath + QString::fromUtf8("/") + filename;
    
    // Use wget or curl to download
    QProcess downloader;
    QStringList args;
    
    // Try wget first
    downloader.start(QString::fromUtf8("wget"), QStringList() << QString::fromUtf8("-O") << downloadPath << url);
    if(downloader.waitForFinished(300000)) { // 5 minutes timeout
      if(downloader.exitCode() == 0) {
        progress(QString::fromUtf8("Download completed: %1").arg(downloadPath));
        if(extractMfc140FromInstaller(downloadPath)) {
          return true;
        }
      }
    }
    
    // Try curl if wget failed
    downloader.start(QString::fromUtf8("curl"), QStringList() << QString::fromUtf8("-L") << QString::fromUtf8("-o") << downloadPath << url);
    if(downloader.waitForFinished(300000)) { // 5 minutes timeout
      if(downloader.exitCode() == 0) {
        progress(QString::fromUtf8("Download completed: %1").arg(downloadPath));
        if(extractMfc140FromInstaller(downloadPath)) {
          return true;
        }
      }
    }
    
    progress(QString::fromUtf8("Download failed from: %1").arg(url));
  }
  
  progress(QString::fromUtf8("All download attempts failed"));
  return false;
}

bool Mfc140uExtractor::extractMfc140FromInstaller(const QString &installerPath)
{
  progress(QString::fromUtf8("Extracting MFC140 from installer: %1").arg(installerPath));
  
  // Use Wine to run the installer
  stage = 0;
  QStringList args;
  args << QStringLiteral("/C") << QStringLiteral("/Q") << QStringLiteral("/T:c:\\");
  wine->setEnv(QString::fromUtf8("WINEPREFIX"), winePrefix);
  wine->run(installerPath, args);
  
  // The actual extraction will be handled in wineFinished()
  return true;
}

void Mfc140uExtractor::showDownloadInstructions()
{
  QMessageBox::information(this, QString::fromUtf8("Manual Download Required"),
    QString::fromUtf8("Please download the Visual C++ 2015-2022 Redistributable manually:\n\n"
    "1. Go to: https://aka.ms/vs/17/release/vc_redist.x64.exe\n"
    "2. Download the file\n"
    "3. Use the 'Browse Directory' button to select the folder containing the downloaded file\n"
    "4. The installer will automatically extract the required MFC140 library.\n\n"
    "Alternatively, you can use the 'Browse Installer' button to select the installer file directly."));
}

void Mfc140uExtractor::commenceExtraction(QString file)
{
  stage = 0;
  
  // Create wine prefix if it doesn't exist
  if(winePrefix.isEmpty()) {
    winePrefix = QDir::tempPath() + QString::fromUtf8("/wineXXXXXX");
    QByteArray charData = winePrefix.toUtf8();
    char *prefix = mkdtemp(charData.data());
    if(prefix == NULL){
      progress(QString::fromUtf8("Failed to create wine prefix"));
      enableButtons(true);
      return;
    }
    winePrefix = QString::fromUtf8(prefix);
  }
  
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

void Mfc140uExtractor::cabextractFinished(int exitCode, QProcess::ExitStatus status)
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
        destPath = PrefProxy::getRsrcDirPath() + QString::fromUtf8("/tir_firmware/mfc140u.dll");
        QString srcPath = winePrefix + QString::fromUtf8("/mfc140u.dll");
        if(!QFile::copy(srcPath, destPath)){
          QMessageBox::warning(this, QString::fromUtf8("Error extracting mfc140u.dll"),
            QString::fromUtf8("There was an error extracting mfc140u.dll.\n"
            "Please see the help to learn other ways\n"
          "ways of obtaining this file.\n\n")
          );
        }else{
          progress(QString::fromUtf8("Mfc140u.dll extracted successfully"));
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

void Mfc140uExtractor::browseDirPressed()
{
  QString dir = QFileDialog::getExistingDirectory(this, QString::fromUtf8("Select directory with Visual C++ Redistributable..."));
  if(!dir.isEmpty()){
    QDirIterator it(dir, QStringList() << QString::fromUtf8("*.exe"), QDir::Files);
    while(it.hasNext()){
      QString file = it.next();
      if(file.contains(QString::fromUtf8("vc_redist"), Qt::CaseInsensitive)){
        installerFile = file;
        progress(QString::fromUtf8("Found installer: %1").arg(file));
        commenceExtraction(file);
        return;
      }
    }
    
    // Also check for extracted mfc140u.dll directly
    QString mfc140Path = dir + QString::fromUtf8("/mfc140u.dll");
    if(QFile::exists(mfc140Path)) {
      progress(QString::fromUtf8("Found mfc140u.dll directly: %1").arg(mfc140Path));
      destPath = PrefProxy::getRsrcDirPath() + QString::fromUtf8("/tir_firmware/mfc140u.dll");
      if(QFile::copy(mfc140Path, destPath)) {
        progress(QString::fromUtf8("Mfc140u.dll copied successfully"));
        QMessageBox::information(this, QString::fromUtf8("Installation Successful"),
          QString::fromUtf8("Visual C++ 2015-2022 MFC libraries were successfully installed."));
        emit finished(true);
        hide();
        return;
      }
    }
    
    QMessageBox::warning(this, QString::fromUtf8("No installer found"),
      QString::fromUtf8("No Visual C++ Redistributable installer or mfc140u.dll found in the selected directory.\n"
      "Please select a directory containing vc_redist.x86.exe, vc_redist.x64.exe, or mfc140u.dll"));
  }
}

void Mfc140uExtractor::on_BrowseInstaller_pressed()
{
  enableButtons(false);
  ui.BrowseInstaller->setEnabled(false);
  
  // Allow user to browse for installer
  QString file = QFileDialog::getOpenFileName(this, QString::fromUtf8("Select Visual C++ 2015-2022 Redistributable installer:"));
  if(file.isEmpty()){
    enableButtons(true);
    return;
  }
  
  // Check if it's a valid VC++ redistributable
  if(!file.contains(QString::fromUtf8("vc_redist"), Qt::CaseInsensitive)) {
    QMessageBox::warning(this, QString::fromUtf8("Invalid Installer"),
      QString::fromUtf8("Please select a Visual C++ 2015-2022 Redistributable installer (vc_redist.x64.exe or vc_redist.x86.exe)"));
    enableButtons(true);
    return;
  }
  
  if(tryBlob(file)){
    progress(QStringLiteral("Blob extraction finished."));
    enableButtons(true);
    linkResult(destPath);
    return;
  }
  
  winePrefix = QDir::tempPath() + QString::fromUtf8("/wineXXXXXX");
  QByteArray charData = winePrefix.toUtf8();
  char *prefix = mkdtemp(charData.data());
  if(prefix == NULL){
    enableButtons(true);
    return;
  }
  winePrefix = QString::fromUtf8(prefix);
  commenceExtraction(file);
}

void Mfc140uExtractor::populateDownloadCombo()
{
  ui.FWCombo->clear();
  
  // Read sources from sources_mfc140.txt
  QString sourcesFile = PREF.getDataPath(QString::fromUtf8("sources_mfc140.txt"));
  if(!QFile::exists(sourcesFile)) {
    progress(QString::fromUtf8("Sources file not found: %1").arg(sourcesFile));
    return;
  }
  
  QFile file(sourcesFile);
  if(!file.open(QIODevice::ReadOnly)) {
    progress(QString::fromUtf8("Cannot open sources file"));
    return;
  }
  
  while(!file.atEnd()) {
    QString line = QString::fromUtf8(file.readLine()).trimmed();
    if(!line.startsWith(QString::fromUtf8("#")) && !line.isEmpty()) {
      ui.FWCombo->addItem(line);
    }
  }
  file.close();
  
  if(ui.FWCombo->count() > 0) {
    progress(QString::fromUtf8("Found %1 download sources").arg(ui.FWCombo->count()));
  } else {
    progress(QString::fromUtf8("No download sources found"));
  }
}

void Mfc140uExtractor::on_DownloadButton_pressed()
{
  if(ui.FWCombo->count() == 0) {
    QMessageBox::warning(this, QString::fromUtf8("No Sources Available"),
      QString::fromUtf8("No download sources are available. Please check that sources_mfc140.txt is properly installed."));
    return;
  }
  
  QString selectedUrl = ui.FWCombo->currentText();
  if(selectedUrl.isEmpty()) {
    QMessageBox::warning(this, QString::fromUtf8("No Source Selected"),
      QString::fromUtf8("Please select a download source from the dropdown list."));
    return;
  }
  
  enableButtons(false);
  ui.DownloadButton->setEnabled(false);
  
  progress(QString::fromUtf8("Starting download from: %1").arg(selectedUrl));
  
  // Extract filename from URL
  QUrl qurl(selectedUrl);
  QString filename = qurl.fileName();
  if(filename.isEmpty()) {
    filename = QString::fromUtf8("vc_redist.x64.exe");
  }
  
  QString downloadPath = cachedDownloadPath + QString::fromUtf8("/") + filename;
  
  // Use wget or curl to download
  QProcess downloader;
  QStringList args;
  
  // Try wget first
  downloader.start(QString::fromUtf8("wget"), QStringList() << QString::fromUtf8("-O") << downloadPath << selectedUrl);
  if(downloader.waitForFinished(300000)) { // 5 minutes timeout
    if(downloader.exitCode() == 0) {
      progress(QString::fromUtf8("Download completed: %1").arg(downloadPath));
      // Start the extraction process
      commenceExtraction(downloadPath);
      return;
    }
  }
  
  // Try curl if wget failed
  downloader.start(QString::fromUtf8("curl"), QStringList() << QString::fromUtf8("-L") << QString::fromUtf8("-o") << downloadPath << selectedUrl);
  if(downloader.waitForFinished(300000)) { // 5 minutes timeout
    if(downloader.exitCode() == 0) {
      progress(QString::fromUtf8("Download completed: %1").arg(downloadPath));
      // Start the extraction process
      commenceExtraction(downloadPath);
      return;
    }
  }
  
  progress(QString::fromUtf8("Download failed from: %1").arg(selectedUrl));
  QMessageBox::warning(this, QString::fromUtf8("Download Failed"),
    QString::fromUtf8("Failed to download from the selected source. Please try another source or use the browse options."));
  enableButtons(true);
}

void Mfc140uExtractor::enableButtons(bool enable)
{
  ui.BrowseInstaller->setEnabled(enable);
  ui.BrowseDir->setEnabled(enable);
  ui.QuitButton->setEnabled(enable);
  ui.DownloadButton->setEnabled(enable);
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


