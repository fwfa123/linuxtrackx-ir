#ifndef HELP_VIEWER__H
#define HELP_VIEWER__H

#include <QTextBrowser>

class HelpViewWidget : public QTextBrowser
{
  Q_OBJECT
 public:
  explicit HelpViewWidget(QWidget *parent = nullptr);
};

#endif
