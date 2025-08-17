#include "log_view.h"
#include <iostream>
#include <QDir>
#include "utils.h"

LogView::LogView(QWidget *parent) : QWidget(parent), watcher(parent)
{
  ui.setupUi(this);
  setWindowTitle(QString::fromUtf8("Logfile viewer"));
  size = 0;
  changed = true;
  //To make sure the logfile exists and we can get its name
  ltr_int_log_message("Opening logfile viewer.\n");
  const char *log_name = ltr_int_get_logfile_name();
  const QString logPath = QString::fromUtf8(log_name ? log_name : "");
  if (!logPath.isEmpty()) {
    watcher.addPath(logPath);
  }
  QObject::connect(&watcher, SIGNAL(fileChanged(const QString&)), 
                   this, SLOT(fileChanged(const QString&)));
  lf = new QFile(logPath);
  if (!lf->open(QIODevice::ReadOnly | QIODevice::Text)) {
    viewer = new QPlainTextEdit(this);
    ui.verticalLayout->insertWidget(0, viewer);
    viewer->setReadOnly(true);
    viewer->setPlainText(QString::fromUtf8("Failed to open log file: ") + (logPath.isEmpty() ? QString::fromUtf8("<unknown>") : logPath));
    // still set up timer to try to read later if file appears
    ts = new QTextStream(lf);
    timer = new QTimer(this);
    QObject::connect(timer, SIGNAL(timeout()), this, SLOT(readChanges()));
    timer->start(250);
    return;
  }
  ts = new QTextStream(lf);
  timer = new QTimer(this);
  viewer = new QPlainTextEdit(this);
  ui.verticalLayout->insertWidget(0, viewer);
  // Ensure user cannot edit and show initial contents immediately
  viewer->setReadOnly(true);
  viewer->setPlainText(ts->readAll());
  changed = false;
  QObject::connect(timer, SIGNAL(timeout()), 
                   this, SLOT(readChanges()));
  timer->start(250);
}

void LogView::on_CloseButton_pressed()
{
  close();
}

void LogView::readChanges()
{
  // If file isn't open yet, try to resolve it now
  if (lf && !lf->isOpen()) {
    const char *log_name = ltr_int_get_logfile_name();
    QString candidate = QString::fromUtf8(log_name ? log_name : "");
    if (candidate.isEmpty()) {
      // Fallback: look for linuxtrack*.log in /tmp
      const QStringList files = QDir(QStringLiteral("/tmp")).entryList(
        QStringList() << QStringLiteral("linuxtrack*.log"), QDir::Files, QDir::Time);
      if (!files.isEmpty()) {
        candidate = QStringLiteral("/tmp/") + files.first();
      }
    }
    if (!candidate.isEmpty()) {
      lf->setFileName(candidate);
      if (lf->open(QIODevice::ReadOnly | QIODevice::Text)) {
        // Populate initial contents
        viewer->setPlainText(ts->readAll());
        changed = false;
      }
    }
  }

  if(changed) {
    viewer->appendPlainText(ts->readAll());
  }
  changed = false;
}



void LogView::fileChanged(const QString &path)
{
  (void) path;
  changed = true;
}



LogView::~LogView()
{
  timer->stop();
  delete(timer);
  delete(ts);
  delete(lf);
  ui.verticalLayout->removeWidget(viewer);
  delete(viewer);
}


