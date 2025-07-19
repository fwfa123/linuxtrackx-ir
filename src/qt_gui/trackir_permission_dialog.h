#ifndef TRACKIR_PERMISSION_DIALOG_H
#define TRACKIR_PERMISSION_DIALOG_H

#include <QDialog>
#include <QCheckBox>
#include <QPushButton>
#include <QLabel>
#include <QVBoxLayout>
#include <QHBoxLayout>
#include <QProcess>
#include <QMessageBox>
#include <QSettings>

class TrackIRPermissionDialog : public QDialog
{
    Q_OBJECT

public:
    explicit TrackIRPermissionDialog(QWidget *parent = nullptr);
    ~TrackIRPermissionDialog();

    static bool shouldShowDialog();
    static void setDialogShown();

private slots:
    void onInstallRulesClicked();
    void onSkipClicked();
    void onHelpClicked();

private:
    void setupUI();
    bool installUdevRules();
    bool addUserToGroups();
    bool installUdevRulesAndGroups();
    bool checkIfUserInGroup(const QString &group);
    void showInstallationResult(bool success, const QString &message);
    void showLogoutDialog();
    void updateStatus();

    QCheckBox *dontShowAgainCheckBox;
    QPushButton *installRulesButton;
    QPushButton *skipButton;
    QPushButton *helpButton;
    QLabel *statusLabel;
    
    static const QString CONFIG_FILE;
    static const QString DONT_SHOW_KEY;
};

#endif // TRACKIR_PERMISSION_DIALOG_H 