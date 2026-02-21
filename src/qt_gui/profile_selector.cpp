#include <QMessageBox>
#include <QFileDialog>
#include <QTextStream>

#include "profile_selector.h"
#include "profile_setup.h"
#include "ltr_profiles.h"
#include "ltr_gui_prefs.h"
#include "utils.h"
#include "tracker.h"
#include <QTimer>
#include <iostream>

ProfileSelector::ProfileSelector(QWidget *parent) : QWidget(parent), ps(NULL), initializing(true)
{
  ui.setupUi(this);

  // Explicit signal/slot connection for Qt6 compatibility
  connect(ui.Profiles, &QComboBox::currentTextChanged,
          this, &ProfileSelector::profilesCurrentTextChanged);

  //To make sure that at least default exists
  TRACKER.setProfile(QString::fromUtf8("Default"));

  QStringList profiles;
  ui.Profiles->addItems(Profile::getProfiles().getProfileNames());
  initializing = false;
  // Qt6: Defer ProfileSetup creation to avoid layout crashes during widget initialization
  // Initialize to "Default" profile after a short delay
  int targetIndex = ui.Profiles->findText(QString::fromUtf8("Default"));
  if (targetIndex != -1) {
    // Set the current index first
    ui.Profiles->setCurrentIndex(targetIndex);
    // Defer ProfileSetup creation to avoid crashes
    // Qt6.5: Add null check and validate UI widget before access
    QTimer::singleShot(50, this, [this]() {
      if (this && ui.Profiles) {
        if (ui.Profiles->currentText() == QString::fromUtf8("Default")) {
          profilesCurrentTextChanged(QString::fromUtf8("Default"));
        }
      }
    });
  }
}


ProfileSelector::~ProfileSelector()
{
  // Qt6 RADICAL FIX: Do not manually delete ps here; it is a child of
  // ProfileSelector and will be destroyed automatically. Manual deletion,
  // combined with layout removal, was contributing to layout crashes.
  ps = NULL;
}

void ProfileSelector::refresh()
{
  QString currentItem = ui.Profiles->currentText();
  QStringList profiles;
  initializing = true;
  ui.Profiles->clear();
  ui.Profiles->addItems(Profile::getProfiles().getProfileNames());
  initializing = false;
  setCurrentProfile(currentItem);
}

bool ProfileSelector::setCurrentProfile(QString prof)
{
  int index = ui.Profiles->findText(prof);
  if(index == -1){
    //std::cout<<"Profile "<<prof.toStdString()<<" not found!!!\n";
    return false;
  }
  //std::cout<<"Profile "<<prof.toStdString()<<" found, setting it!!!\n";
  ui.Profiles->setCurrentIndex(index);
  return true;
}

void ProfileSelector::profilesCurrentTextChanged(const QString &text)
{
  if((PROFILE.isProfile(text)) < 0){
    return;
  }
  if (ps == NULL) {
    // First-time creation: create the ProfileSetup widget and add it to layout
    ps = new ProfileSetup(text, this);
    // Qt6 RADICAL FIX: use addWidget() instead of insertWidget() to avoid
    // edge cases with out-of-range indexes confusing QBoxLayout internals.
    ui.AxesSetup->addWidget(ps);
  } else {
    // Qt6 RADICAL FIX: reuse existing ProfileSetup instance for new profile
    // instead of deleting/recreating it (which was crashing inside Qt layouts).
    ps->loadProfile(text);
  }
}

void ProfileSelector::on_CopyFromDefault_pressed()
{
  if (ps != NULL) {
    ps->copyFromDefault();
  }
}

void ProfileSelector::on_ImportProfile_pressed()
{
  QString home = QString::fromUtf8(qgetenv("HOME").constData());
  QString mask(QString::fromUtf8("Profile (*.profile)"));
  QString fname = QFileDialog::getOpenFileName(this, QString::fromUtf8("Import Profile..."), home, mask, &mask);
  if(fname == QString::fromUtf8("")){
    return;
  }
  QFile f(fname);
  if(!f.open(QIODevice::ReadOnly)){
    QMessageBox::warning(this, tr("Problem importing profile!"),
      tr("There was a problem opening the profile file '%1'!").arg(fname) );
    return;
  }
  QTextStream tf(&f);

  QString newName(tf.readLine());
  //std::cout<<"Importing profile '"<<newName.toStdString()<<"'\n";

int ccc = PROFILE.isProfile(newName);
  //std::cout<<"..."<<ccc<<"\n";
  if(ccc < 0){
    //std::cout<<"Creating new profile!\n";
    PROFILE.addProfile(newName);
    refresh();
  }
  setCurrentProfile(newName);
  if (ps != NULL) {
    ps->importProfile(tf);
    PREF.savePrefs();
  }
  refresh();
}

void ProfileSelector::on_ExportProfile_pressed()
{
  QString home = QString::fromUtf8(qgetenv("HOME").constData());
  QString mask(QString::fromUtf8("Profile (*.profile)"));
  QString fname = QFileDialog::getSaveFileName(this, QString::fromUtf8("Export Profile..."), home, mask, &mask);
  if(fname == QString::fromUtf8("")){
    return;
  }
  if(!fname.endsWith(QString::fromUtf8(".profile"))){
    fname += QString::fromUtf8(".profile");
  }
  QFile f(fname);
  if(!f.open(QIODevice::WriteOnly | QFile::Truncate)){
    QMessageBox::warning(this, tr("Problem exporting profile!"),
      tr("There was a problem saving the profile to file '%1'!").arg(fname) );
    return;
  }
  QTextStream tf(&f);

  if (ps != NULL) {
    ps->exportProfile(tf);
  }
}

bool ProfileSelector::close()
{
  // Guard against null pointer - ProfileSetup may not be created yet
  if (ps != NULL) {
    ps->close();
  }
  return QWidget::close();
}


