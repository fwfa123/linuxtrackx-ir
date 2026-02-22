#ifndef HELP_VIEW__H
#define HELP_VIEW__H

#include <QWidget>
#include "ui_logview.h"
#include "help_viewer.h"

class QSettings;
class QSplitter;
class QTreeWidget;
class QTreeWidgetItem;
class QPushButton;

class HelpViewer : public QWidget
{
  Q_OBJECT
  HelpViewer(QWidget *parent = 0);
  ~HelpViewer();
  static HelpViewer *hlp;
  static HelpViewer &getHlp();
  void ChangeHelpPage(QString name);

public:
  static void ShowWindow();
  static void ChangePage(QString name);
  static void CloseWindow();
  static void LoadPrefs(QSettings &settings);
  static void StorePrefs(QSettings &settings);
  static void RaiseWindow();

private slots:
  void on_CloseButton_pressed();
  void openWiki();
  void followLink(const QUrl &url);
  void tocItemClicked(QTreeWidgetItem *item, int column);

private:
  void showPage(const QString &fileName);

  Ui::LogViewerForm ui;
  QPushButton *wikiButton;
  QHBoxLayout *layout;
  QSplitter *splitter;
  QTreeWidget *tocTree;
  HelpViewWidget *viewer;
  QString helpDir;
};

#endif
