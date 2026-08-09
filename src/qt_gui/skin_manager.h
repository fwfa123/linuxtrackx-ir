#ifndef SKIN_MANAGER_H
#define SKIN_MANAGER_H

#include <QString>
#include <QStringList>

/**
 * Loads and applies Qt Style Sheets (skins) for ltr_gui.
 *
 * Special skin name "native": clear stylesheet and keep the platform style.
 * Other skins search order:
 *   1. ~/.config/linuxtrack/skins/<name>/skin.qss  (user override)
 *   2. :/ltr/skins/<name>/skin.qss                 (bundled)
 *
 * Preference is stored in QSettings("linuxtrack","ltr_gui") under appearance/skin.
 */
class SkinManager
{
public:
  static constexpr const char *kNativeSkin = "native";
  static constexpr const char *kDefaultSkin = "default";
  static constexpr const char *kExampleSkin = "example";
  static constexpr const char *kF35Skin = "f35";
  static constexpr const char *kXWingSkin = "xwing";

  static SkinManager &instance();

  /** "native", then "default", demo skins, then other skins (sorted). */
  QStringList availableSkins() const;

  /** Currently applied skin name (may be a preview). */
  QString currentSkin() const;

  /** Skin name persisted in QSettings (empty means "native"). */
  QString savedSkin() const;

  /** Apply a skin without writing QSettings. */
  bool load(const QString &name);

  /** Load and apply the saved skin. */
  bool loadSaved();

  /** Persist name to QSettings and apply it. */
  bool saveSkin(const QString &name);

  /** Revert the applied stylesheet to the saved skin. */
  bool revert();

  /**
   * Path to ~/.config/linuxtrack/skins/.
   * When create is true, the directory is created and a README.md guide is
   * seeded there if missing (e.g. Open Skins Folder).
   */
  QString userSkinsDir(bool create = false) const;

  /** Copy/refresh the skins README and demo skins into the user skins dir. */
  void ensureUserSkinsGuide() const;

  /** True for the built-in "native" choice (no QSS, platform style). */
  static bool isNativeSkin(const QString &name);

private:
  SkinManager() = default;
  SkinManager(const SkinManager &) = delete;
  SkinManager &operator=(const SkinManager &) = delete;

  void capturePlatformStyle();
  void applyPlatformStyle();
  void applyFusionStyle();
  QString resolveSkinDir(const QString &name) const;
  QString readAndSubstitute(const QString &qssPath, const QString &skinDir) const;
  bool applyFromName(const QString &name);
  QStringList bundledSkinNames() const;
  QStringList userSkinNames() const;

  QString current_;
  QString platformStyleName_;
  bool capturedPlatformStyle_ = false;
};

#endif
