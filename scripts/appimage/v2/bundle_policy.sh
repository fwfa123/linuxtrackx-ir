#!/usr/bin/env bash
# AppImage v2 — standard bundle expectations and path constants (single source of truth).
# Standard pipeline uses CMake -DDISABLE_WIIMOTE=ON; images do not ship wii_server.
# shellcheck disable=SC2034
# Paths are relative to AppDir root or to squashfs-root when validating a packaged AppImage.

LTR_HELP_LTR_GUI_QCH_REL="usr/share/linuxtrack/help/ltr_gui/help.qch"
LTR_HELP_LTR_GUI_QHC_REL="usr/share/linuxtrack/help/ltr_gui/help.qhc"
LTR32_LIB_REL="usr/lib/i386-linux-gnu/linuxtrack/liblinuxtrack.so.0"
LIBXCB_GLX_REL="usr/lib/libxcb-glx.so.0"
