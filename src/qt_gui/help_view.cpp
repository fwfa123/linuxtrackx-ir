#ifdef HAVE_CONFIG_H
  #include "config.h"
#endif

#include "help_view.h"
#include "ltr_gui_prefs.h"
#include <QSettings>
#include <QDesktopServices>
#include <QSplitter>
#include <QTreeWidget>
#include <QDir>
#include <QApplication>
#include <QFileInfo>
#include <QMessageBox>
#include <QHBoxLayout>
#include <QPushButton>
#include <QUrl>

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
static const TocEntry toc[] = {
    {"Welcome", "welcome.htm"},
    {"What's new", "news.htm"},
    {"Hall Of Fame", "hall_of_fame.htm"},
    {"Tracker Setup", "dev_setup.htm"},
    {"TrackIR Permissions", "trackir_permissions.htm"},
    {"Model Setup", "model_setup.htm"},
    {"Tracking Setup", "axes_setup.htm"},
    {"TrackIR firmware and Wine integration", "extractor.htm"},
    {"Gaming Tab", "gaming_tab.htm"},
    {"Advanced Tab", "advanced_tab.htm"},
    {"XPlane plugin setup", "xpl_setup.htm"},
    {"Linuxtrack Interface", "interface.htm"},
    {"Wine games support", "wine.htm"},
    {"Virtual mouse setup", "mickey.htm"},
    {"System Information", "misc.htm"},
};
static const int tocCount = sizeof(toc) / sizeof(toc[0]);
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
  for (int i = 0; i < tocCount; ++i) {
    QTreeWidgetItem *item = new QTreeWidgetItem(tocTree);
    item->setText(0, QString::fromUtf8(toc[i].title));
    item->setData(0, Qt::UserRole, QString::fromUtf8(toc[i].file));
  }
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

  if (tocCount > 0)
    showPage(QString::fromUtf8(toc[0].file));
  else if (!helpDir.isEmpty())
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
  } else {
    viewer->setSource(url);
  }
}
