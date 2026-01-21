#include <QApplication>
#include <QTranslator>
#include <QLibraryInfo>

#include "ltr_gui.h"
#include <utils.h>
#include <locale.h>
#include <stdlib.h>
#include <signal.h>

static void catch_sigpipe(int sig)
{
    (void) sig;
}

 int main(int argc, char *argv[])
 {
     // Ignore SIGPIPE to prevent crashes when pipes break (e.g., when tester program exits)
     // This is especially important on newer Qt6/Wine/kernel combinations (e.g., Arch Linux)
     // where broken pipes can cause the application to terminate unexpectedly
     // Follow the same pattern as ltr_pipe.c for consistency
     signal(SIGPIPE, catch_sigpipe);
     
     ltr_int_check_root();
     ltr_int_log_message("Starting ltr_gui\n");
     // Keep C locale for deterministic logs while allowing UI translations
     setenv("LC_ALL", "C", 1);
     setlocale(LC_ALL, "C");
     QApplication app(argc, argv);

     // Load UI translations if available (embedded in resources)
     QTranslator appTranslator;
     const QString locale = QLocale::system().name();
     if (!appTranslator.load(QStringLiteral(":/ltr/translations/ltr_gui_") + locale)) {
         (void)appTranslator.load(QStringLiteral(":/ltr/translations/ltr_gui_") + QLocale::system().name().left(2));
     }
     app.installTranslator(&appTranslator);

     LinuxtrackGui gui;
     gui.show();
     return app.exec();
 }

