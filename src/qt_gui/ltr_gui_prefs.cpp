#ifdef HAVE_CONFIG_H
  #include "../../config.h"
#endif

#include "pref.h"
#include "pref_global.h"
#include "ltr_gui_prefs.h"
#include <map>
#include "utils.h"

#include <QStringList>
#include <QApplication>
#include <QMessageBox>
#include <QDir>
#include <iostream>

PrefProxy *PrefProxy::prf = NULL;

static int warnMessage(const QString &message){
 return QMessageBox::warning(NULL, QStringLiteral("Linuxtrack"),
                                message, QMessageBox::Ok, QMessageBox::Ok);
}

PrefProxy::PrefProxy()
{
  if(ltr_int_read_prefs(NULL, false)){
    // Ensure the user config file actually exists; if missing, copy defaults now
    QString targetDir = PrefProxy::getRsrcDirPath();
    if(targetDir.endsWith(QStringLiteral("/"))){
      targetDir.chop(1);
    }
    QString targetCfg = targetDir + QStringLiteral("/linuxtrack1.conf");
    if(!QFileInfo::exists(targetCfg)){
      if(makeRsrcDir() && copyDefaultPrefs()){
        ltr_int_new_prefs();
        ltr_int_read_prefs(NULL, true);
      }
    }
    checkPrefix(true);
    return;
  }
  ltr_int_log_message("Pref file not found, trying linuxtrack.conf\n");
  if(ltr_int_read_prefs("linuxtrack.conf", false)){
    // Ensure the user config file exists even if legacy name was read
    QString targetDir = PrefProxy::getRsrcDirPath();
    if(targetDir.endsWith(QStringLiteral("/"))){
      targetDir.chop(1);
    }
    QString targetCfg = targetDir + QStringLiteral("/linuxtrack1.conf");
    if(!QFileInfo::exists(targetCfg)){
      if(makeRsrcDir() && copyDefaultPrefs()){
        ltr_int_new_prefs();
        ltr_int_read_prefs(NULL, true);
      }
    }
    ltr_int_prefs_changed();
    checkPrefix(true);
    return;
  }

  ltr_int_log_message("Couldn't load preferences (GUI), copying default!\n");
  if(!makeRsrcDir()){
    ltr_int_log_message("Failed to create resource directory - using default preferences\n");
    ltr_int_new_prefs();
    ltr_int_read_prefs(NULL, true);
    checkPrefix(true);
    return;
  }
  if(!copyDefaultPrefs()){
    ltr_int_log_message("Failed to copy default preferences - using new preferences\n");
    ltr_int_new_prefs();
    ltr_int_read_prefs(NULL, true);
    checkPrefix(true);
    return;
  }
  ltr_int_new_prefs();
  ltr_int_read_prefs(NULL, true);
  checkPrefix(true);
}

bool PrefProxy::checkPrefix(bool save)
{
  QString appPath = QApplication::applicationDirPath();
  appPath.prepend(QStringLiteral("\"")).append(QStringLiteral("\""));
  char *tmp_prefix = ltr_int_get_key("Global", "Prefix");
  bool found = false;
  if(tmp_prefix != NULL){
    found = true;
    prefix = QString::fromStdString(tmp_prefix);
  }else{
    prefix = QStringLiteral("");
  }
  if(found && (prefix == appPath)){
    // Intentionally left empty
  }else{
    prefix = appPath;
    bool changed = ltr_int_change_key("Global", "Prefix", appPath.toUtf8().constData());
    if(save){
      changed &= savePrefs();
    }
    return changed;
  }
  return true;
}

bool PrefProxy::makeRsrcDir()
{
  QString targetDir = PrefProxy::getRsrcDirPath();
  if(targetDir.endsWith(QStringLiteral("/"))){
    targetDir.chop(1);
  }
  QFileInfo rsrcDir(targetDir);
  if(rsrcDir.isDir()){
    return true;
  }
  if(rsrcDir.exists() || rsrcDir.isSymLink()){
    QString bck = targetDir + QStringLiteral(".pre");
    QFileInfo bckInfo(bck);
    if(bckInfo.exists() || bckInfo.isSymLink()){
      if(!QFile::remove(bck)){
        QString msg = QStringLiteral("Can't remove '") + bck + QStringLiteral("'!");
        ltr_int_log_message(QString(msg + QStringLiteral("\n")).toUtf8().data());
        warnMessage(msg);
        return false;
      }
    }
    if(!QFile::rename(targetDir, bck)){
      QString msg = QStringLiteral("Can't rename '") + targetDir + QStringLiteral("' to '") + bck + QStringLiteral("'!");
      ltr_int_log_message(QString(msg + QStringLiteral("\n")).toUtf8().data());
      warnMessage(msg);
      return false;
    }
  }
  if(!QDir::home().mkpath(targetDir)){
    QString msg = QStringLiteral("Can't create '") + targetDir + QStringLiteral("'!");
    ltr_int_log_message(QString(msg + QStringLiteral("\n")).toUtf8().data());
    warnMessage(msg);
    return false;
  }
  return true;
}

bool PrefProxy::copyDefaultPrefs()
{
  // we can assume the rsrc dir exists now...
  QString targetDir = PrefProxy::getRsrcDirPath();
  if(targetDir.endsWith(QStringLiteral("/"))){
    targetDir.chop(1);
  }
  QString target = targetDir + QStringLiteral("/linuxtrack1.conf");
  QString source = PrefProxy::getDataPath(QStringLiteral("linuxtrack1.conf"));
  QFileInfo target_info(target);
  if(target_info.exists() || target_info.isSymLink()){
    QString bck = target + QStringLiteral(".backup");
    QFileInfo bckInfo(bck);
    if(bckInfo.exists() || bckInfo.isSymLink()){
      if(!QFile::remove(bck)){
        QString msg = QStringLiteral("Can't remove '") + bck + QStringLiteral("'!");
        ltr_int_log_message(QString(msg + QStringLiteral("\n")).toUtf8().constData());
        warnMessage(msg);
        return false;
      }
    }
    if(!QFile::rename(target, bck)){
      QString msg = QStringLiteral("Can't rename '") + target + QStringLiteral("' to '") + bck + QStringLiteral("'!");
      ltr_int_log_message(QString(msg + QStringLiteral("\n")).toUtf8().constData());
      warnMessage(msg);
      return false;
    }
  }
  if(!QFile::copy(source, target)){
    QString msg = QStringLiteral("Can't copy '") + source + QStringLiteral("' to '") + target + QStringLiteral("'!");
    ltr_int_log_message(QString(msg + QStringLiteral("\n")).toUtf8().constData());
    warnMessage(msg);
    return false;
  }
  return true;
}

PrefProxy::~PrefProxy()
{
}

PrefProxy& PrefProxy::Pref()
{
  if(prf == NULL){
    prf = new PrefProxy();
  }
  return *prf;
}

void PrefProxy::ClosePrefs()
{
  if(prf != NULL){
    delete prf;
    prf = NULL;
    ltr_int_free_prefs();
  }
}

void PrefProxy::SavePrefsOnExit()
{
  if(ltr_int_need_saving()){
    QMessageBox::StandardButton res;
    res = QMessageBox::warning(NULL, QStringLiteral("Linuxtrack"),
       QStringLiteral("Preferences were modified,Do you want to save them?"),
       QMessageBox::Save | QMessageBox::Close, QMessageBox::Save);
    if(res == QMessageBox::Save){
      savePrefs();
    }
  }
}

bool PrefProxy::activateDevice(const QString &sectionName)
{
  ltr_int_change_key("Global", "Input", sectionName.toUtf8().constData());
  return true;
}

bool PrefProxy::activateModel(const QString &sectionName)
{
  ltr_int_change_key("Global", "Model", sectionName.toUtf8().constData());
  return true;
}

bool PrefProxy::createSection(QString
&sectionName)
{
  char *tmp = ltr_int_add_unique_section(sectionName.toUtf8().constData());
  if(tmp != NULL){
    sectionName = QString::fromUtf8(tmp);
    return true;
  }else{
    sectionName = QString::fromUtf8("");
    return false;
  }
}

bool PrefProxy::getKeyVal(const QString &sectionName, const QString &keyName,
			  QString &result)
{
  char *val = ltr_int_get_key(sectionName.toUtf8().constData(), keyName.toUtf8().constData());
  if(val != NULL){
    result = QString::fromUtf8(val);
    return true;
  }else{
    return false;
  }
}

/*
bool PrefProxy::getKeyVal(const QString &keyName, QString &result)
{
  const char *val = ltr_int_get_key(NULL, keyName.toUtf8().constData());
  if(val != NULL){
    result = val;
    return true;
  }else{
    return false;
  }
}
*/

bool PrefProxy::addKeyVal(const QString &sectionName, const QString &keyName,
			  const QString &value)
{
  return ltr_int_change_key(sectionName.toUtf8().constData(), keyName.toUtf8().constData(),
		 value.toUtf8().constData());
}

bool PrefProxy::setKeyVal(const QString &sectionName, const QString &keyName,
			  const QString &value)
{
  return ltr_int_change_key(sectionName.toUtf8().constData(), keyName.toUtf8().constData(),
		 value.toUtf8().constData());
}

bool PrefProxy::setKeyVal(const QString &sectionName, const QString &keyName,
                          const int &value)
{
  return ltr_int_change_key_int(sectionName.toUtf8().constData(), keyName.toUtf8().constData(), value);
}

bool PrefProxy::setKeyVal(const QString &sectionName, const QString &keyName,
                          const float &value)
{
  return ltr_int_change_key_flt(sectionName.toUtf8().constData(), keyName.toUtf8().constData(), value);
}

bool PrefProxy::setKeyVal(const QString &sectionName, const QString &keyName,
                          const double &value)
{
  return ltr_int_change_key_flt(sectionName.toUtf8().constData(), keyName.toUtf8().constData(), value);
}

bool PrefProxy::getFirstDeviceSection(const QString &devType, QString &result)
{
  char *devName = ltr_int_find_section("Capture-device", devType.toUtf8().constData());
  if(devName != NULL){
    result = QString::fromUtf8(devName);
    return true;
  }else{
    return false;
  }
}

bool PrefProxy::getFirstDeviceSection(const QString &devType,
				      const QString &devId, QString &result)
{
  QStringList sections;
  getSectionList(sections);
  char *devName, *devIdStr;
  for(ssize_t i = 0; i < sections.size(); ++i){
    devName = ltr_int_get_key(sections[i].toUtf8().constData(), "Capture-device");
    devIdStr = ltr_int_get_key(sections[i].toUtf8().constData(), "Capture-device-id");
    if((devName != NULL) && (devIdStr != NULL)){
      if((devType.compare(QString::fromUtf8(devName), Qt::CaseInsensitive) == 0)
         && (devId.compare(QString::fromUtf8(devIdStr), Qt::CaseInsensitive) == 0)){
	result = QString(sections[i]);
	return true;
      }
    }
  }
  return false;
}

bool PrefProxy::getActiveDevice(deviceType_t &devType, QString &id, QString &secName)
{
  char *devSection = ltr_int_get_key("Global", "Input");
  if(devSection == NULL){
    return false;
  }
  char *devName = ltr_int_get_key(devSection, "Capture-device");
  char *devId = ltr_int_get_key(devSection, "Capture-device-id");
  if((devName == NULL) || (devId == NULL)){
    return false;
  }

  QString dn = QString::fromUtf8(devName);
  if(dn.compare(QString::fromUtf8("Webcam"), Qt::CaseInsensitive) == 0){
    devType = WEBCAM;
  }else if(dn.compare(QString::fromUtf8("Webcam-face"), Qt::CaseInsensitive) == 0){
	  devType = WEBCAM_FT;
  }else if(dn.compare(QString::fromUtf8("MacWebcam"), Qt::CaseInsensitive) == 0){
	  devType = MACWEBCAM;
  }else if(dn.compare(QString::fromUtf8("MacWebcam-face"), Qt::CaseInsensitive) == 0){
	  devType = MACWEBCAM_FT;
  }else if(dn.compare(QString::fromUtf8("Wiimote"), Qt::CaseInsensitive) == 0){
    devType = WIIMOTE;
  }else if(dn.compare(QString::fromUtf8("Tir"), Qt::CaseInsensitive) == 0){
    devType = TIR;
  }else if(dn.compare(QString::fromUtf8("Tir_openusb"), Qt::CaseInsensitive) == 0){
    devType = TIR;
  }else if(dn.compare(QString::fromUtf8("PS3Eye"), Qt::CaseInsensitive) == 0){
    devType = MACPS3EYE;
  }else if(dn.compare(QString::fromUtf8("PS3Eye-face"), Qt::CaseInsensitive) == 0){
    devType = MACPS3EYE_FT;
  }else if(dn.compare(QString::fromUtf8("Joystick"), Qt::CaseInsensitive) == 0){
    devType = JOYSTICK;
  }else{
    devType = NONE;
  }
  id = QString::fromUtf8(devId);
  secName = QString::fromUtf8(devSection);
  return true;
}

bool PrefProxy::getActiveDevice(deviceType_t &devType, QString &id)
{
  QString tmp;
  return getActiveDevice(devType, id, tmp);
}

bool PrefProxy::getActiveModel(QString &model)
{
  char *modelSection = ltr_int_get_key("Global", "Model");
  if(modelSection == NULL){
    return false;
  }
  model = QString::fromUtf8(modelSection);
  return true;
}

bool PrefProxy::getModelList(QStringList &list)
{
  std::vector<std::string> sections;
  ltr_int_find_sections("Model-type", (void*)&sections);
  list.clear();
  for(size_t i = 0; i < sections.size(); ++i){
    list.append(QString::fromStdString(sections[i]));
  }
  return (list.size() != 0);
}

bool PrefProxy::getProfiles(QStringList &list)
{
  std::vector<std::string> profiles;
  char *title;
  ltr_int_find_sections("Title", (void*)&profiles);
  list.clear();
  for(size_t i = 0; i < profiles.size(); ++i){
    title = ltr_int_get_key(profiles[i].c_str(), "Title");
    if(title != NULL){
      list.append(QString::fromUtf8(title));
    }
  }
  return (list.size() != 0);
}

bool PrefProxy::getProfileSection(const QString &name, QString &section)
{
  char *secName = ltr_int_find_section("Title", name.toUtf8().constData());
  if(secName != NULL){
    section = QString::fromUtf8(secName);
    return true;
  }
  return false;
}

bool PrefProxy::savePrefs()
{
  bool res = ltr_int_save_prefs(NULL);
  return res;
}

QString PrefProxy::getDataPath(QString file)
{
  char *path = ltr_int_get_data_path_prefix(file.toUtf8().constData(),
                                            QApplication::applicationDirPath().toUtf8().constData());
  QString res;
  if (path != nullptr) {
    res = QString::fromUtf8(path);
    free(path);
  } else {
    // Fallback to hardcoded path if ltr_int_get_data_path_prefix fails
    QString appPath = QApplication::applicationDirPath();
#ifndef DARWIN
    res = appPath + QStringLiteral("/../share/linuxtrack/") + file;
#else
    res = appPath + QStringLiteral("/../Resources/linuxtrack/") + file;
#endif
  }
  return res;
/*
  QString appPath = QApplication::applicationDirPath();
#ifndef DARWIN
  return appPath + QString::fromUtf8("/../share/linuxtrack/") + file;
#else
  return appPath + QString::fromUtf8("/../Resources/linuxtrack/") + file;
#endif
*/
}

QString PrefProxy::getLibPath(QString file)
{
  char *path = ltr_int_get_lib_path(file.toUtf8().constData());
  QString res;
  if (path != nullptr) {
    res = QString::fromUtf8(path);
    free(path);
  } else {
    // Fallback to hardcoded path if ltr_int_get_lib_path fails
    QString appPath = QApplication::applicationDirPath();
#ifndef DARWIN
    res = appPath + QStringLiteral("/../lib/") + file + QStringLiteral(".so.0");
#else
    res = appPath + QStringLiteral("/../Frameworks/") + file + QStringLiteral(".0.dylib");
#endif
  }
  return res;
/*
  QString appPath = QApplication::applicationDirPath();
#ifndef DARWIN
  return appPath + QString::fromUtf8("/../lib/") + file + QString::fromUtf8(".so.0");
#else
  return appPath + QString::fromUtf8("/../Frameworks/") + file + QString::fromUtf8(".0.dylib");
#endif
*/
}

QString PrefProxy::getRsrcDirPath()
{
  return QDir::homePath() + QStringLiteral("/.config/linuxtrack/");
}

bool PrefProxy::rereadPrefs()
{
  ltr_int_free_prefs();
  ltr_int_read_prefs(NULL, true);
  checkPrefix(true);
  return true;
}

void PrefProxy::announceModelChange()
{
  ltr_int_announce_model_change();
}

void PrefProxy::getSectionList(QStringList &list)
{
  std::vector<std::string> tmpList;
  ltr_int_get_section_list((void*)&tmpList);
  list.clear();
  for(size_t i = 0; i < tmpList.size(); ++i){
    list.append(QString::fromStdString(tmpList[i]));
  }
}



