#ifndef LUTRIS_PATH_CONFIG_DIALOG_H
#define LUTRIS_PATH_CONFIG_DIALOG_H

#include <QDialog>
#include <QLineEdit>
#include <QPushButton>
#include <QLabel>
#include <QVBoxLayout>
#include <QHBoxLayout>
#include <QGroupBox>
#include <QFileDialog>
#include <QMessageBox>
#include <QSettings>

class LutrisPathConfigDialog : public QDialog
{
    Q_OBJECT

public:
    explicit LutrisPathConfigDialog(QWidget *parent = nullptr);
    ~LutrisPathConfigDialog();

    QString getDatabasePath() const { return databasePathEdit->text(); }
    QString getConfigPath() const { return configPathEdit->text(); }

private slots:
    void onDatabaseBrowseClicked();
    void onConfigBrowseClicked();
    void onResetClicked();
    void onAcceptClicked();

private:
    void setupUI();
    void loadCurrentSettings();
    void validatePaths();

    QLabel *statusLabel;
    QLineEdit *databasePathEdit;
    QLineEdit *configPathEdit;
    QPushButton *databaseBrowseButton;
    QPushButton *configBrowseButton;
    QPushButton *resetButton;
    QPushButton *okButton;
    QPushButton *cancelButton;

    static const QString SETTINGS_GROUP;
    static const QString DATABASE_PATH_KEY;
    static const QString CONFIG_PATH_KEY;
};

#endif // LUTRIS_PATH_CONFIG_DIALOG_H
