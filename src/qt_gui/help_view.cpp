#ifdef HAVE_CONFIG_H
  #include "config.h"
#endif

#include "help_view.h"
#include "ltr_gui_prefs.h"
#include <QSettings>
#include <QDesktopServices>
#include <QSplitter>
#include <QTreeWidget>
#include <QTreeWidgetItemIterator>
#include <QDir>
#include <QApplication>
#include <QFileInfo>
#include <QMessageBox>
#include <QHBoxLayout>
#include <QPushButton>
#include <QUrl>
#include <QSignalBlocker>

HelpViewer *HelpViewer::hlp = NULL;

static const char WIKI_URL[] = "https://gitlab.com/fwfa123/linuxtrackx-ir/-/wikis/home";

struct TocEntry { const char *title; const char *file; };

#if defined(HELP_TOC_MICKEY)
static const TocEntry toc[] = {
    {"Welcome", "welcome.htm"},
    {"What's new", "news.htm"},
    {"Tracking Setup", "tracking.htm"},
    {"Miscellaneous Setup", "misc.htm"},
};
static const int tocCount = sizeof(toc) / sizeof(toc[0]);
#else
static const TocEntry startHere[] = {
    {"Welcome", "welcome.htm"},
    {"What's new", "news.htm"},
};
static const TocEntry theWindow[] = {
    {"Device Setup", "dev_setup.htm"},
    {"Model Setup", "model_setup.htm"},
    {"Tracking Setup", "axes_setup.htm"},
    {"Gaming", "gaming_tab.htm"},
    {"System", "misc.htm"},
    {"Advanced", "advanced_tab.htm"},
};
static const TocEntry otherTasks[] = {
    {"TrackIR permissions", "trackir_permissions.htm"},
    {"Firmware and Wine bridge", "extractor.htm"},
    {"X-Plane plugin", "xpl_setup.htm"},
    {"Virtual mouse", "mickey.htm"},
    {"Linuxtrack interface", "interface.htm"},
    {"Wine reference", "wine.htm"},
    {"Hall of Fame", "hall_of_fame.htm"},
};

static void addTocGroup(QTreeWidget *tree, const char *title,
                        const TocEntry *children, int childCount)
{
  QTreeWidgetItem *parent = new QTreeWidgetItem(tree);
  parent->setText(0, QString::fromUtf8(title));
  parent->setFlags(parent->flags() & ~Qt::ItemIsSelectable);
  for (int i = 0; i < childCount; ++i) {
    QTreeWidgetItem *item = new QTreeWidgetItem(parent);
    item->setText(0, QString::fromUtf8(children[i].title));
    item->setData(0, Qt::UserRole, QString::fromUtf8(children[i].file));
  }
}
#endif

HelpViewer &HelpViewer::getHlp()
{
  if (hlp == NULL) {
    hlp = new HelpViewer();
  }
  return *hlp;
}

void HelpViewer::ShowWindow()
{
  getHlp().show();
}

void HelpViewer::RaiseWindow()
{
  getHlp().raise();
  getHlp().activateWindow();
}

void HelpViewer::ChangePage(QString name)
{
  getHlp().ChangeHelpPage(name);
}

void HelpViewer::ChangeHelpPage(QString name)
{
  showPage(name);
}

void HelpViewer::CloseWindow()
{
  getHlp().close();
}

void HelpViewer::LoadPrefs(QSettings &settings)
{
  HelpViewer &hv = getHlp();
  settings.beginGroup(QString::fromUtf8("HelpWindow"));
  hv.resize(settings.value(QString::fromUtf8("size"), QSize(800, 600)).toSize());
  hv.move(settings.value(QString::fromUtf8("pos"), QPoint(0, 0)).toPoint());
  settings.endGroup();
}

void HelpViewer::StorePrefs(QSettings &settings)
{
  HelpViewer &hv = getHlp();
  settings.beginGroup(QString::fromUtf8("HelpWindow"));
  settings.setValue(QString::fromUtf8("size"), hv.size());
  settings.setValue(QString::fromUtf8("pos"), hv.pos());
  settings.endGroup();
}

static QString resolveHelpDir()
{
  QString helpBase = QString::fromUtf8(HELP_BASE);
  if (helpBase.endsWith(QLatin1Char('/')))
    helpBase.chop(1);
  QStringList candidates;
  candidates << PREF.getDataPath(QString::fromUtf8("/help/") + helpBase + QLatin1Char('/'));
  candidates << QApplication::applicationDirPath() + QStringLiteral("/../share/linuxtrack/help/") + helpBase;
  candidates << QApplication::applicationDirPath() + QStringLiteral("/help");
  candidates << QStringLiteral("/usr/share/linuxtrack/help/") + helpBase;
  candidates << QStringLiteral("/opt/linuxtrack/share/linuxtrack/help/") + helpBase;
  for (const QString &dir : candidates) {
    QString can = QDir(dir).canonicalPath();
    if (!can.isEmpty() && QFileInfo(can).isDir()) {
      QFileInfo fi(can + QStringLiteral("/welcome.htm"));
      if (fi.exists())
        return can;
    }
  }
  return QString();
}

void HelpViewer::selectTocItem(const QString &fileName)
{
  if (!tocTree || fileName.isEmpty())
    return;
  QTreeWidgetItemIterator it(tocTree);
  while (*it) {
    if ((*it)->data(0, Qt::UserRole).toString() == fileName) {
      const QSignalBlocker blocker(tocTree);
      tocTree->setCurrentItem(*it);
      tocTree->scrollToItem(*it);
      return;
    }
    ++it;
  }
}

void HelpViewer::showPage(const QString &fileName)
{
  if (helpDir.isEmpty()) {
    viewer->setHtml(QStringLiteral("<p>Help files not found. <a href=\"") + QString::fromUtf8(WIKI_URL) +
                    QStringLiteral("\">View Online Wiki</a></p>"));
    return;
  }
  QString path = QDir(helpDir).absoluteFilePath(fileName);
  if (!QFileInfo::exists(path)) {
    viewer->setHtml(QStringLiteral("<p>Page not found: ") + fileName + QStringLiteral("</p>"));
    return;
  }
  viewer->setSource(QUrl::fromLocalFile(path));
  selectTocItem(fileName);
}

HelpViewer::HelpViewer(QWidget *parent) : QWidget(parent)
{
  ui.setupUi(this);
  setWindowTitle(QString::fromUtf8("Help viewer"));

  wikiButton = new QPushButton(QString::fromUtf8("View Online Wiki"), this);
  ui.horizontalLayout->insertWidget(0, wikiButton);

  helpDir = resolveHelpDir();

  tocTree = new QTreeWidget(this);
  tocTree->setHeaderHidden(true);
#if defined(HELP_TOC_MICKEY)
  for (int i = 0; i < tocCount; ++i) {
    QTreeWidgetItem *item = new QTreeWidgetItem(tocTree);
    item->setText(0, QString::fromUtf8(toc[i].title));
    item->setData(0, Qt::UserRole, QString::fromUtf8(toc[i].file));
  }
#else
  addTocGroup(tocTree, "Start here", startHere,
              static_cast<int>(sizeof(startHere) / sizeof(startHere[0])));
  addTocGroup(tocTree, "The window", theWindow,
              static_cast<int>(sizeof(theWindow) / sizeof(theWindow[0])));
  addTocGroup(tocTree, "Other tasks", otherTasks,
              static_cast<int>(sizeof(otherTasks) / sizeof(otherTasks[0])));
#endif
  tocTree->expandAll();

  viewer = new HelpViewWidget(this);
  viewer->setOpenLinks(false);
  if (!helpDir.isEmpty())
    viewer->setSearchPaths(QStringList(helpDir));

  splitter = new QSplitter();
  splitter->addWidget(tocTree);
  splitter->addWidget(viewer);
  splitter->setStretchFactor(1, 4);

  layout = new QHBoxLayout();
  layout->addWidget(splitter);
  ui.verticalLayout->insertLayout(0, layout);

  connect(ui.CloseButton, SIGNAL(pressed()), this, SLOT(on_CloseButton_pressed()));
  connect(wikiButton, SIGNAL(pressed()), this, SLOT(openWiki()));
  connect(tocTree, SIGNAL(itemClicked(QTreeWidgetItem*,int)), this, SLOT(tocItemClicked(QTreeWidgetItem*,int)));
  connect(viewer, SIGNAL(anchorClicked(QUrl)), this, SLOT(followLink(QUrl)));

#if defined(HELP_TOC_MICKEY)
  if (tocCount > 0)
    showPage(QString::fromUtf8(toc[0].file));
  else
#endif
  if (!helpDir.isEmpty())
    showPage(QStringLiteral("welcome.htm"));
  else
    viewer->setHtml(QStringLiteral("<p>Help files not found. <a href=\"") + QString::fromUtf8(WIKI_URL) +
                    QStringLiteral("\">View Online Wiki</a></p>"));
}

HelpViewer::~HelpViewer()
{
  ui.verticalLayout->removeItem(layout);
  layout->removeWidget(splitter);
  delete layout;
  delete splitter;
}

void HelpViewer::tocItemClicked(QTreeWidgetItem *item, int column)
{
  Q_UNUSED(column);
  if (item) {
    QString file = item->data(0, Qt::UserRole).toString();
    if (!file.isEmpty())
      showPage(file);
  }
}

void HelpViewer::on_CloseButton_pressed()
{
  close();
}

void HelpViewer::openWiki()
{
  QDesktopServices::openUrl(QUrl(QString::fromUtf8(WIKI_URL)));
}

void HelpViewer::followLink(const QUrl &url)
{
  if (url.scheme().compare(QLatin1String("http"), Qt::CaseInsensitive) == 0 ||
      url.scheme().compare(QLatin1String("https"), Qt::CaseInsensitive) == 0) {
    QDesktopServices::openUrl(url);
    return;
  }

  QString fileName = url.fileName();
  if (fileName.isEmpty() && url.isRelative())
    fileName = url.path();
  if (fileName.contains(QLatin1Char('/')))
    fileName = QFileInfo(fileName).fileName();

  if (!fileName.isEmpty() && !helpDir.isEmpty() &&
      QFileInfo::exists(QDir(helpDir).absoluteFilePath(fileName))) {
    showPage(fileName);
    return;
  }
  viewer->setSource(url);
}
