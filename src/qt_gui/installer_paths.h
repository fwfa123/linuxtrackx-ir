#ifndef INSTALLER_PATHS_H
#define INSTALLER_PATHS_H

#include <QString>

namespace InstallerPaths {
// Directory with prebuilt PE bridge files (NPClient.dll, etc.), or empty if missing
QString resolveWineBridgePayloadDir();
}

#endif // INSTALLER_PATHS_H
