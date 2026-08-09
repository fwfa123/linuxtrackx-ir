#include "skin_manager.h"
#include "ltr_gui_prefs.h"

#include <QApplication>
#include <QDir>
#include <QDirIterator>
#include <QFile>
#include <QFileInfo>
#include <QMap>
#include <QSettings>
#include <QStyle>
#include <QStyleFactory>
#include <QTextStream>
#include <algorithm>

namespace {
constexpr auto kOrg = "linuxtrack";
constexpr auto kApp = "ltr_gui";
constexpr auto kSkinKey = "appearance/skin";

/** Bundled skins always advertised even if QRC dir listing fails. */
const QStringList kKnownBundledSkins = {
  QStringLiteral("default"),
  QStringLiteral("example"),
  QStringLiteral("f35"),
  QStringLiteral("xwing"),
};

constexpr auto kBundledReadme = ":/ltr/skins/README.md";
}

SkinManager &SkinManager::instance()
{
  static SkinManager mgr;
  return mgr;
}

bool SkinManager::isNativeSkin(const QString &name)
{
  return name.isEmpty() || name == QLatin1String(kNativeSkin);
}

QString SkinManager::userSkinsDir(bool create) const
{
  QString dir = PrefProxy::getRsrcDirPath() + QStringLiteral("skins");
  if (create) {
    QDir().mkpath(dir);
    ensureUserSkinsGuide();
  }
  return dir;
}

void SkinManager::ensureUserSkinsGuide() const
{
  const QString destDir = PrefProxy::getRsrcDirPath() + QStringLiteral("skins");
  QDir().mkpath(destDir);

  // Refresh README and official demo skins from the bundle.
  auto forceCopy = [](const QString &resourcePath, const QString &destPath) {
    QFile src(resourcePath);
    if (!src.open(QIODevice::ReadOnly)) {
      return;
    }
    QFileInfo fi(destPath);
    QDir().mkpath(fi.absolutePath());
    QFile out(destPath);
    if (!out.open(QIODevice::WriteOnly | QIODevice::Truncate)) {
      return;
    }
    out.write(src.readAll());
  };
  forceCopy(QString::fromUtf8(kBundledReadme),
            destDir + QStringLiteral("/README.md"));

  auto refreshDemoSkin = [&](const char *name) {
    const QString dir = destDir + QLatin1Char('/') + QLatin1String(name);
    QDir().mkpath(dir);
    forceCopy(QStringLiteral(":/ltr/skins/") + QLatin1String(name) +
                  QStringLiteral("/skin.qss"),
              dir + QStringLiteral("/skin.qss"));
    forceCopy(QStringLiteral(":/ltr/skins/") + QLatin1String(name) +
                  QStringLiteral("/skin.ini"),
              dir + QStringLiteral("/skin.ini"));
  };
  refreshDemoSkin(SkinManager::kExampleSkin);
  refreshDemoSkin(SkinManager::kF35Skin);
  refreshDemoSkin(SkinManager::kXWingSkin);
}

QString SkinManager::currentSkin() const
{
  return current_.isEmpty() ? QString::fromUtf8(kNativeSkin) : current_;
}

QString SkinManager::savedSkin() const
{
  QSettings settings(QString::fromUtf8(kOrg), QString::fromUtf8(kApp));
  if (!settings.contains(QString::fromUtf8(kSkinKey))) {
    // Migrate mistaken early key from QSettings("linuxtrack","gui")
    QSettings legacy(QString::fromUtf8(kOrg), QStringLiteral("gui"));
    if (legacy.contains(QString::fromUtf8(kSkinKey))) {
      const QString migrated =
          legacy.value(QString::fromUtf8(kSkinKey)).toString();
      settings.setValue(QString::fromUtf8(kSkinKey), migrated);
      settings.sync();
      legacy.remove(QString::fromUtf8(kSkinKey));
      legacy.sync();
    }
  }
  const QString name = settings.value(QString::fromUtf8(kSkinKey),
                                      QString::fromUtf8(kNativeSkin)).toString();
  return name.isEmpty() ? QString::fromUtf8(kNativeSkin) : name;
}

void SkinManager::capturePlatformStyle()
{
  if (capturedPlatformStyle_ || !qApp || !qApp->style()) {
    return;
  }
  platformStyleName_ = qApp->style()->name();
  capturedPlatformStyle_ = true;
}

void SkinManager::applyPlatformStyle()
{
  capturePlatformStyle();
  if (!qApp) {
    return;
  }
  if (!platformStyleName_.isEmpty() &&
      QStyleFactory::keys().contains(platformStyleName_, Qt::CaseInsensitive)) {
    QApplication::setStyle(platformStyleName_);
  }
}

void SkinManager::applyFusionStyle()
{
  capturePlatformStyle();
  if (qApp && QStyleFactory::keys().contains(QStringLiteral("Fusion"))) {
    QApplication::setStyle(QStringLiteral("Fusion"));
  }
}

QStringList SkinManager::bundledSkinNames() const
{
  QStringList names = kKnownBundledSkins;

  // Also discover any extra bundled skins via the resource tree
  QDirIterator it(QStringLiteral(":/ltr/skins"),
                  QStringList() << QStringLiteral("skin.qss"),
                  QDir::Files,
                  QDirIterator::Subdirectories);
  while (it.hasNext()) {
    it.next();
    // :/ltr/skins/<name>/skin.qss
    const QFileInfo fi(it.filePath());
    const QString name = fi.dir().dirName();
    if (!name.isEmpty() && !names.contains(name)) {
      names.append(name);
    }
  }

  // Keep only skins whose QSS actually exists
  QStringList verified;
  for (const QString &name : names) {
    if (QFile::exists(QStringLiteral(":/ltr/skins/") + name +
                      QStringLiteral("/skin.qss"))) {
      verified.append(name);
    }
  }
  return verified;
}

QStringList SkinManager::userSkinNames() const
{
  QStringList names;
  const QDir userDir(userSkinsDir(false));
  if (!userDir.exists()) {
    return names;
  }
  const QStringList entries =
      userDir.entryList(QDir::Dirs | QDir::NoDotAndDotDot);
  for (const QString &entry : entries) {
    if (QFile::exists(userDir.filePath(entry) + QStringLiteral("/skin.qss"))) {
      names.append(entry);
    }
  }
  return names;
}

QStringList SkinManager::availableSkins() const
{
  QStringList names;
  names.append(QString::fromUtf8(kNativeSkin));

  const QStringList bundled = bundledSkinNames();
  for (const QString &name : bundled) {
    if (!names.contains(name)) {
      names.append(name);
    }
  }

  const QStringList user = userSkinNames();
  for (const QString &name : user) {
    if (isNativeSkin(name)) {
      continue; // reserve the name
    }
    if (!names.contains(name)) {
      names.append(name);
    }
  }

  // Keep native, default, demos first; then locale-sorted remainder
  QStringList rest;
  for (const QString &name : names) {
    if (!isNativeSkin(name) &&
        name != QLatin1String(kDefaultSkin) &&
        name != QLatin1String(kExampleSkin) &&
        name != QLatin1String(kF35Skin) &&
        name != QLatin1String(kXWingSkin)) {
      rest.append(name);
    }
  }
  std::sort(rest.begin(), rest.end(),
            [](const QString &a, const QString &b) {
              return a.localeAwareCompare(b) < 0;
            });

  QStringList ordered;
  ordered.append(QString::fromUtf8(kNativeSkin));
  if (names.contains(QLatin1String(kDefaultSkin))) {
    ordered.append(QString::fromUtf8(kDefaultSkin));
  }
  if (names.contains(QLatin1String(kExampleSkin))) {
    ordered.append(QString::fromUtf8(kExampleSkin));
  }
  if (names.contains(QLatin1String(kF35Skin))) {
    ordered.append(QString::fromUtf8(kF35Skin));
  }
  if (names.contains(QLatin1String(kXWingSkin))) {
    ordered.append(QString::fromUtf8(kXWingSkin));
  }
  ordered.append(rest);
  return ordered;
}

QString SkinManager::resolveSkinDir(const QString &name) const
{
  if (isNativeSkin(name)) {
    return QString();
  }

  const QString userPath = userSkinsDir(false) + QLatin1Char('/') + name;
  if (QFile::exists(userPath + QStringLiteral("/skin.qss"))) {
    return userPath;
  }

  const QString bundledPath = QStringLiteral(":/ltr/skins/") + name;
  if (QFile::exists(bundledPath + QStringLiteral("/skin.qss"))) {
    return bundledPath;
  }

  return QString();
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

  const QString iniPath = skinDir + QStringLiteral("/skin.ini");
  QFile iniFile(iniPath);
  if (!iniFile.open(QIODevice::ReadOnly | QIODevice::Text)) {
    return qss;
  }

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

  QStringList keys = tokens.keys();
  std::sort(keys.begin(), keys.end(),
            [](const QString &a, const QString &b) { return a.size() > b.size(); });
  for (const QString &key : keys) {
    qss.replace(QLatin1Char('@') + key, tokens.value(key));
  }

  return qss;
}

bool SkinManager::applyFromName(const QString &name)
{
  const QString safe = name.isEmpty() ? QString::fromUtf8(kNativeSkin) : name;

  if (isNativeSkin(safe)) {
    applyPlatformStyle();
    if (qApp) {
      qApp->setStyleSheet(QString());
    }
    current_ = QString::fromUtf8(kNativeSkin);
    return true;
  }

  const QString skinDir = resolveSkinDir(safe);
  if (skinDir.isEmpty()) {
    return false;
  }

  const QString qssPath = skinDir + QStringLiteral("/skin.qss");
  const QString qss = readAndSubstitute(qssPath, skinDir);
  if (qss.isNull()) {
    return false;
  }

  applyFusionStyle();
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
  if (!applyFromName(savedSkin())) {
    // Corrupt/missing saved skin → fall back to native
    return applyFromName(QString::fromUtf8(kNativeSkin));
  }
  return true;
}

bool SkinManager::saveSkin(const QString &name)
{
  const QString safe = name.isEmpty() ? QString::fromUtf8(kNativeSkin) : name;
  if (!applyFromName(safe)) {
    return false;
  }
  QSettings settings(QString::fromUtf8(kOrg), QString::fromUtf8(kApp));
  settings.setValue(QString::fromUtf8(kSkinKey), safe);
  settings.sync();
  return true;
}

bool SkinManager::revert()
{
  return loadSaved();
}
