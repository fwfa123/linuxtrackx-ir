#ifndef LTR_MODEL__H
#define LTR_MODEL__H

#include <QObject>
#include <QString>
#include <QShowEvent>
#include "ui_model_creation.h"
#include "ui_model_edit.h"
#include "ui_cap_edit.h"
#include "ui_clip_edit.h"
#include "ui_single_edit.h"
#include "ui_clip_tweaking.h"
#include "ui_cap_tweaking.h"

typedef enum {MDL_1PT, MDL_3PT_CLIP, MDL_3PT_CAP, MDL_FACE, MDL_ABSOLUTE} modelType_t;
class Guardian;

class ModelTuningSync : public QObject
{
  Q_OBJECT
 public:
  static ModelTuningSync *inst();
 signals:
  void headCenterChanged(const QString &section);
};

class ModelCreate : public QDialog
{
  Q_OBJECT
 public:
  ModelCreate(QWidget *parent = 0);
  ~ModelCreate();
 protected:
 signals:
  void ModelCreated(const QString &section);
 public slots:
  int exec();
 private slots:
  void on_ModelTypeCombo_currentIndexChanged(int index);
  void on_CancelButton_pressed();
  void on_CreateButton_pressed();
 signals:
  void dump(const QString &sec);
 private:
  void removeEditor();
  void activateEditor(QWidget *editor);
  Ui::ModelCreation ui;
  QRegularExpressionValidator *validator;
  QWidget *modelEditor;
};

class ModelEdit : public QWidget
{
  Q_OBJECT
 public:
  ModelEdit(Guardian *grd, QWidget *parent = 0);
  ~ModelEdit();
  void refresh();
  void reloadHeadCenterTuning();
 protected:
  void showEvent(QShowEvent *event);
 private slots:
  void on_CreateModelButton_pressed();
  void ModelCreated(const QString &section);
  void modelSelectorActivated(const QString &text);
  void onHeadCenterChanged(const QString &section);
 signals:
  void modelSelected(int modelType);
 private:
  Ui::ModelEditForm ui;
  QWidget *modelTweaker;
  //void Connect();
  ModelCreate *mcw;
  QString currentSection;
  bool initializing;
};

class CapEdit : public QWidget
{
  Q_OBJECT
 public:
  CapEdit(QWidget *parent = 0);
  ~CapEdit();
  void refresh();
 public slots:
  void dump(const QString &sec);
 private:
  Ui::CapEditForm ui;
};


class ClipEdit : public QWidget
{
  Q_OBJECT
 public:
  ClipEdit(QWidget *parent = 0);
  ~ClipEdit();
 public slots:
  void dump(const QString &sec);
 private:
  Ui::ClipEditForm ui;
};

class SingleEdit : public QWidget
{
  Q_OBJECT
 public:
  SingleEdit(QWidget *parent = 0);
  ~SingleEdit();
 public slots:
  void dump(const QString &sec);
 private:
  Ui::SingleEditForm ui;
};

class ClipTweaking : public QWidget
{
  Q_OBJECT
 public:
  ClipTweaking(const QString &section, QWidget *parent = 0);
  ~ClipTweaking();
  void reloadFromPrefs();
  const QString &section() const { return currentSection; }
 public slots:
  void on_ClipHx_valueChanged(int val);
  void on_ClipHy_valueChanged(int val);
  void on_ClipHz_valueChanged(int val);
  void on_ClipLeft_toggled();
  void on_ClipRight_toggled();
 private:
  Ui::ClipTweakingForm ui;
  QString currentSection;
  bool initializing;
  void loadFromPrefs();
  void tweakHx();
  void headCenterTuned();
};

class CapTweaking : public QWidget
{
  Q_OBJECT
 public:
  CapTweaking(const QString &section, QWidget *parent = 0);
  ~CapTweaking();
  void reloadFromPrefs();
  const QString &section() const { return currentSection; }
 public slots:
  void on_CapHy_valueChanged(int val);
  void on_CapHz_valueChanged(int val);
 private:
  Ui::CapTweakingForm ui;
  QString currentSection;
  bool initializing;
  void loadFromPrefs();
  void headCenterTuned();
};

#endif
