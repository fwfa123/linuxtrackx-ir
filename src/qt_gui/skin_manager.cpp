#include "skin_manager.h"
#include "ltr_gui_prefs.h"

#include <QApplication>
#include <QDir>
#include <QFile>
#include <QFileInfo>
#include <QSettings>
#include <QMap>
#include <QTextStream>
#include <algorithm>

namespace {
constexpr auto kOrg = "linuxtrack";
constexpr auto kApp = "gui";
constexpr auto kSkinKey = "appearance/skin";
constexpr auto kDefaultSkin = "default";
}

SkinManager &SkinManager::instance()
{
  static SkinManager mgr;
  return mgr;
}

QString SkinManager::userSkinsDir() const
{
  QString dir = PrefProxy::getRsrcDirPath() + QStringLiteral("skins");
  QDir().mkpath(dir);
  return dir;
}

QString SkinManager::currentSkin() const
{
  return current_.isEmpty() ? QString::fromUtf8(kDefaultSkin) : current_;
}

QString SkinManager::savedSkin() const
{
  QSettings settings(QString::fromUtf8(kOrg), QString::fromUtf8(kApp));
  const QString name = settings.value(QString::fromUtf8(kSkinKey),
                                      QString::fromUtf8(kDefaultSkin)).toString();
  return name.isEmpty() ? QString::fromUtf8(kDefaultSkin) : name;
}

QStringList SkinManager::availableSkins() const
{
  QStringList names;

  // Bundled skins under :/ltr/skins/
  const QDir bundled(QStringLiteral(":/ltr/skins"));
  const QStringList bundledEntries =
      bundled.entryList(QDir::Dirs | QDir::NoDotAndDotDot);
  for (const QString &entry : bundledEntries) {
    if (QFile::exists(bundled.filePath(entry) + QStringLiteral("/skin.qss"))) {
      names.append(entry);
    }
  }

  // User skins
  const QDir userDir(userSkinsDir());
  const QStringList userEntries =
      userDir.entryList(QDir::Dirs | QDir::NoDotAndDotDot);
  for (const QString &entry : userEntries) {
    if (QFile::exists(userDir.filePath(entry) + QStringLiteral("/skin.qss"))) {
      if (!names.contains(entry)) {
        names.append(entry);
      }
    }
  }

  names.removeDuplicates();
  std::sort(names.begin(), names.end(), [](const QString &a, const QString &b) {
    if (a == QLatin1String(kDefaultSkin)) {
      return true;
    }
    if (b == QLatin1String(kDefaultSkin)) {
      return false;
    }
    return a.localeAwareCompare(b) < 0;
  });

  if (names.isEmpty()) {
    names.append(QString::fromUtf8(kDefaultSkin));
  }
  return names;
}

QString SkinManager::resolveSkinDir(const QString &name) const
{
  const QString safe = name.isEmpty() ? QString::fromUtf8(kDefaultSkin) : name;

  const QString userPath =
      userSkinsDir() + QLatin1Char('/') + safe;
  if (QFile::exists(userPath + QStringLiteral("/skin.qss"))) {
    return userPath;
  }

  const QString bundledPath = QStringLiteral(":/ltr/skins/") + safe;
  if (QFile::exists(bundledPath + QStringLiteral("/skin.qss"))) {
    return bundledPath;
  }

  return QString();
}

QString SkinManager::resolveSkinQssPath(const QString &name) const
{
  const QString dir = resolveSkinDir(name);
  if (dir.isEmpty()) {
    return QString();
  }
  return dir + QStringLiteral("/skin.qss");
}

QString SkinManager::readAndSubstitute(const QString &qssPath,
                                       const QString &skinDir) const
{
  QFile qssFile(qssPath);
  if (!qssFile.open(QIODevice::ReadOnly | QIODevice::Text)) {
    return QString();
  }
  QString qss = QString::fromUtf8(qssFile.readAll());
  qssFile.close();

  // Optional companion skin.ini with [colors] tokens (@token → value)
  const QString iniPath = skinDir + QStringLiteral("/skin.ini");
  QFile iniFile(iniPath);
  if (iniFile.open(QIODevice::ReadOnly | QIODevice::Text)) {
    QMap<QString, QString> tokens;
    QTextStream in(&iniFile);
    QString section;
    while (!in.atEnd()) {
      QString line = in.readLine().trimmed();
      if (line.isEmpty() || line.startsWith(QLatin1Char('#')) ||
          line.startsWith(QLatin1Char(';'))) {
        continue;
      }
      if (line.startsWith(QLatin1Char('[')) && line.endsWith(QLatin1Char(']'))) {
        section = line.mid(1, line.size() - 2).trimmed().toLower();
        continue;
      }
      if (section != QLatin1String("colors")) {
        continue;
      }
      const int eq = line.indexOf(QLatin1Char('='));
      if (eq <= 0) {
        continue;
      }
      const QString key = line.left(eq).trimmed();
      const QString value = line.mid(eq + 1).trimmed();
      if (!key.isEmpty() && !value.isEmpty()) {
        tokens.insert(key, value);
      }
    }
    iniFile.close();

    // Longer keys first so @primary_bg wins over @primary when both exist
    QStringList keys = tokens.keys();
    std::sort(keys.begin(), keys.end(),
              [](const QString &a, const QString &b) { return a.size() > b.size(); });
    for (const QString &key : keys) {
      qss.replace(QLatin1Char('@') + key, tokens.value(key));
    }
  }

  return qss;
}

bool SkinManager::applyFromName(const QString &name)
{
  const QString safe = name.isEmpty() ? QString::fromUtf8(kDefaultSkin) : name;
  const QString skinDir = resolveSkinDir(safe);
  if (skinDir.isEmpty()) {
    // Fall back to clearing stylesheet if even default is missing
    if (qApp) {
      qApp->setStyleSheet(QString());
    }
    current_ = QString::fromUtf8(kDefaultSkin);
    return false;
  }

  const QString qssPath = skinDir + QStringLiteral("/skin.qss");
  const QString qss = readAndSubstitute(qssPath, skinDir);
  if (qss.isNull()) {
    return false;
  }

  if (qApp) {
    qApp->setStyleSheet(qss);
  }
  current_ = safe;
  return true;
}

bool SkinManager::load(const QString &name)
{
  return applyFromName(name);
}

bool SkinManager::loadSaved()
{
  return applyFromName(savedSkin());
}

bool SkinManager::saveSkin(const QString &name)
{
  const QString safe = name.isEmpty() ? QString::fromUtf8(kDefaultSkin) : name;
  QSettings settings(QString::fromUtf8(kOrg), QString::fromUtf8(kApp));
  settings.setValue(QString::fromUtf8(kSkinKey), safe);
  settings.sync();
  return applyFromName(safe);
}

bool SkinManager::revert()
{
  return loadSaved();
}
