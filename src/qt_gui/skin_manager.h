#ifndef SKIN_MANAGER_H
#define SKIN_MANAGER_H

#include <QString>
#include <QStringList>

/**
 * Loads and applies Qt Style Sheets (skins) for ltr_gui.
 *
 * Search order for a skin named <name>:
 *   1. ~/.config/linuxtrack/skins/<name>/skin.qss  (user override)
 *   2. :/ltr/skins/<name>/skin.qss                 (bundled)
 *
 * Preference is stored in QSettings("linuxtrack","gui") under appearance/skin.
 */
class SkinManager
{
public:
  static SkinManager &instance();

  /** Merge user + bundled skin names (sorted, "default" first when present). */
  QStringList availableSkins() const;

  /** Currently applied skin name (may be a preview). */
  QString currentSkin() const;

  /** Skin name persisted in QSettings (empty means "default"). */
  QString savedSkin() const;

  /** Apply a skin without writing QSettings. Returns false if QSS not found. */
  bool load(const QString &name);

  /** Load and apply the saved skin (or "default"). */
  bool loadSaved();

  /** Persist name to QSettings and apply it. */
  bool saveSkin(const QString &name);

  /** Revert the applied stylesheet to the saved skin. */
  bool revert();

  /** Absolute path to ~/.config/linuxtrack/skins/ (created if missing). */
  QString userSkinsDir() const;

private:
  SkinManager() = default;
  SkinManager(const SkinManager &) = delete;
  SkinManager &operator=(const SkinManager &) = delete;

  QString resolveSkinDir(const QString &name) const;
  QString resolveSkinQssPath(const QString &name) const;
  QString readAndSubstitute(const QString &qssPath, const QString &skinDir) const;
  bool applyFromName(const QString &name);

  QString current_;
};

#endif
