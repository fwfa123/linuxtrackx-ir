#ifndef PROFILE_SETUP__H
#define PROFILE_SETUP__H

#include <QWidget>
#include "ui_profile_setup.h"

class QTextStream;
class ScpForm;

class ProfileSetup : public QWidget
{
 Q_OBJECT
 public:
  ProfileSetup(const QString &name, QWidget *parent = 0);
  ~ProfileSetup();
  // Qt6 RADICAL FIX: allow reusing a single ProfileSetup instance for different profiles
  // instead of deleting/recreating the widget (which was crashing inside Qt layouts).
  void loadProfile(const QString &name);
   void copyFromDefault();
   void restoreAxisDefaults();
   void saveAxisDefaults();
   void importProfile(QTextStream &tf);
   void exportProfile(QTextStream &tf);
 private:
   void connect();
   Ui::ProfileSetupForm ui;
   ScpForm *sc;
   QString profileName;
   bool initializing;
 public slots:
   bool close();
 private slots:
   void on_CopyFromDefault_clicked();
   void on_RestoreAxisDefaults_clicked();
   void on_SaveAxisDefaults_clicked();
   void on_Smoothing_valueChanged(int val);
   void on_TrackingRate_valueChanged(int val);
   void setCommonFF(float val);
   void initAxes();
};

#endif
