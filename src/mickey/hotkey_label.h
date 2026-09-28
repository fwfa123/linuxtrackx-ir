#ifndef HOTKEY_LABEL__H
#define HOTKEY_LABEL__H

#include <QString>

/* Human-readable hotkey label for UI. Stored prefs stay Qt PortableText
 * (e.g. "Num+2"); this turns keypad bindings into "NumPad 2". */
QString hotkeyDisplayString(const QString &stored);

#endif
