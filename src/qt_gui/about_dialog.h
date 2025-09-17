#ifndef ABOUT_DIALOG_H
#define ABOUT_DIALOG_H

#include <QDialog>
#include <QWidget>

class AboutDialog : public QDialog
{
    Q_OBJECT

public:
    explicit AboutDialog(QWidget *parent = nullptr);

private:
    void setupUI();
    void setupLayout();
    void setupVersionInfo();
    void setupCredits();
};

#endif // ABOUT_DIALOG_H
