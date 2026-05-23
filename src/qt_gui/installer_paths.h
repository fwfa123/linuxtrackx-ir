#ifndef INSTALLER_PATHS_H
#define INSTALLER_PATHS_H

#include <QString>

namespace InstallerPaths {
// Returns full path to linuxtrack-wine.exe if found, otherwise empty string
QString resolveWineBridgeInstallerPath();
// Directory with prebuilt PE bridge files (NPClient.dll, etc.), or empty if missing
QString resolveWineBridgePayloadDir();
}

#endif // INSTALLER_PATHS_H


