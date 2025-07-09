#include <ltr_tracking.h>
#include <ltr_profiles.h>
#include <ltr_gui_prefs.h>
#include <QInputDialog>
#include <iostream>
#include "tracker.h"

// Axis configuration structure for reducing code duplication
struct AxisConfig {
  int axis_id;
  QCheckBox* enable_widget;
  QDoubleSpinBox* left_widget;
  QDoubleSpinBox* right_widget;
};

static bool state2bool(int state);
static Qt::CheckState bool2state(bool v);

// Static axis configuration array to reduce code duplication
static const AxisConfig AXIS_CONFIGS[] = {
  {PITCH, nullptr, nullptr, nullptr},  // Will be initialized in constructor
  {ROLL, nullptr, nullptr, nullptr},
  {YAW, nullptr, nullptr, nullptr},
  {TX, nullptr, nullptr, nullptr},
  {TY, nullptr, nullptr, nullptr},
  {TZ, nullptr, nullptr, nullptr}
};

static constexpr size_t AXIS_COUNT = sizeof(AXIS_CONFIGS) / sizeof(AXIS_CONFIGS[0]);

LtrTracking::LtrTracking(const Ui::LinuxtrackMainForm &ui) : gui(ui), initializing(false)
{
  // Initialize axis configuration array with actual widget pointers
  const_cast<AxisConfig*>(AXIS_CONFIGS)[0] = {PITCH, gui.PitchEnable, gui.PitchUpSpin, gui.PitchDownSpin};
  const_cast<AxisConfig*>(AXIS_CONFIGS)[1] = {ROLL, gui.RollEnable, gui.TiltLeftSpin, gui.TiltRightSpin};
  const_cast<AxisConfig*>(AXIS_CONFIGS)[2] = {YAW, gui.YawEnable, gui.YawLeftSpin, gui.YawRightSpin};
  const_cast<AxisConfig*>(AXIS_CONFIGS)[3] = {TX, gui.XEnable, gui.MoveLeftSpin, gui.MoveRightSpin};
  const_cast<AxisConfig*>(AXIS_CONFIGS)[4] = {TY, gui.YEnable, gui.MoveUpSpin, gui.MoveDownSpin};
  const_cast<AxisConfig*>(AXIS_CONFIGS)[5] = {TZ, gui.ZEnable, gui.MoveBackSpin, gui.MoveForthSpin};
  
  Connect();
  initializing = true;
  gui.Profiles->addItems(Profile::getProfiles().getProfileNames());
  initializing = false;
}

LtrTracking::~LtrTracking()
{
}

void LtrTracking::refresh()
{
  PROFILE.setCurrent("Default");
  emit customSectionChanged();
}

void LtrTracking::Connect()
{
  QObject::connect(&TRACKER, SIGNAL(axisChanged(int, int)), 
                    this, SLOT(axisChanged(int, int)));
  QObject::connect(gui.Profiles, SIGNAL(currentIndexChanged(const QString &)), 
                    this, SLOT(on_Profiles_currentIndexChanged(const QString &)));
  QObject::connect(gui.CreateNewProfile, SIGNAL(pressed()), 
                    this, SLOT(on_CreateNewProfile_pressed()));
  
  // Connect all axis enable widgets
  for (const auto& config : AXIS_CONFIGS) {
    QObject::connect(config.enable_widget, SIGNAL(stateChanged(int)),
                      this, SLOT(on_AxisEnable_stateChanged(int)));
  }
  
  // Connect all axis value widgets
  QObject::connect(gui.PitchUpSpin, SIGNAL(valueChanged(double)),
                    this, SLOT(on_PitchUpSpin_valueChanged(double)));
  QObject::connect(gui.PitchDownSpin, SIGNAL(valueChanged(double)),
                    this, SLOT(on_PitchDownSpin_valueChanged(double)));
  QObject::connect(gui.YawLeftSpin, SIGNAL(valueChanged(double)),
                    this, SLOT(on_YawLeftSpin_valueChanged(double)));
  QObject::connect(gui.YawRightSpin, SIGNAL(valueChanged(double)),
                    this, SLOT(on_YawRightSpin_valueChanged(double)));
  QObject::connect(gui.TiltLeftSpin, SIGNAL(valueChanged(double)),
                    this, SLOT(on_TiltLeftSpin_valueChanged(double)));
  QObject::connect(gui.TiltRightSpin, SIGNAL(valueChanged(double)),
                    this, SLOT(on_TiltRightSpin_valueChanged(double)));
  QObject::connect(gui.MoveLeftSpin, SIGNAL(valueChanged(double)),
                    this, SLOT(on_MoveLeftSpin_valueChanged(double)));
  QObject::connect(gui.MoveRightSpin, SIGNAL(valueChanged(double)),
                    this, SLOT(on_MoveRightSpin_valueChanged(double)));
  QObject::connect(gui.MoveUpSpin, SIGNAL(valueChanged(double)),
                    this, SLOT(on_MoveUpSpin_valueChanged(double)));
  QObject::connect(gui.MoveDownSpin, SIGNAL(valueChanged(double)),
                    this, SLOT(on_MoveDownSpin_valueChanged(double)));
  QObject::connect(gui.MoveBackSpin, SIGNAL(valueChanged(double)),
                    this, SLOT(on_MoveBackSpin_valueChanged(double)));
  QObject::connect(gui.MoveForthSpin, SIGNAL(valueChanged(double)),
                    this, SLOT(on_MoveForthSpin_valueChanged(double)));
  QObject::connect(&TRACKER, SIGNAL(setCommonFF(float)),
                    this, SLOT(setCommonFF(float)));
  QObject::connect(gui.CommonFF, SIGNAL(valueChanged(int)),
                    this, SLOT(on_CommonFF_valueChanged(int)));
  QObject::connect(&TRACKER, SIGNAL(initAxes(void)),
                    this, SLOT(initAxes(void)));
}

void LtrTracking::axisChanged(int axis, int elem)
{
  // Find the axis configuration
  const AxisConfig* config = nullptr;
  for (const auto& ax : AXIS_CONFIGS) {
    if (ax.axis_id == axis) {
      config = &ax;
      break;
    }
  }
  
  if (!config) return;
  
  // Update widgets based on what changed
  if ((elem == AXIS_ENABLED) || (elem == AXIS_FULL)) {
    config->enable_widget->setCheckState(bool2state(TRACKER.axisGetEnabled(axis)));
  }
  if ((elem == AXIS_LMULT) || (elem == AXIS_FULL)) {
    config->left_widget->setValue(TRACKER.axisGet(axis, AXIS_LMULT));
  }
  if ((elem == AXIS_RMULT) || (elem == AXIS_FULL)) {
    config->right_widget->setValue(TRACKER.axisGet(axis, AXIS_RMULT));
  }
}

void LtrTracking::on_Profiles_currentIndexChanged(const QString &text)
{
  const bool prev = initializing;
  initializing = true;
  PROFILE.setCurrent(text);
  emit customSectionChanged();
  initializing = prev;
}

void LtrTracking::on_CreateNewProfile_pressed()
{
  bool done;
  const QString newSec = QInputDialog::getText(nullptr, QStringLiteral("New Section Name:"), 
                        QStringLiteral("Enter name of the new section:"), 
                        QLineEdit::Normal, QString(), &done);
  if (done && !newSec.isEmpty()) {
    const int i = PROFILE.isProfile(newSec);
    if (i == -1) {
      PROFILE.addProfile(newSec);
      gui.Profiles->clear();
      const QStringList &sl = Profile::getProfiles().getProfileNames();
      gui.Profiles->addItems(sl);
      gui.Profiles->setCurrentIndex(sl.size() - 1);
    } else {
      gui.Profiles->setCurrentIndex(i);
    }
  }
}

static bool state2bool(int state)
{
  return state == Qt::Checked;
}

// Generic axis enable handler - will be connected to all enable widgets
void LtrTracking::on_AxisEnable_stateChanged(int state)
{
  if (initializing) return;
  
  // Find which axis this corresponds to
  QCheckBox* sender = qobject_cast<QCheckBox*>(this->sender());
  if (!sender) return;
  
  for (const auto& config : AXIS_CONFIGS) {
    if (config.enable_widget == sender) {
      TRACKER.axisChangeEnabled(config.axis_id, state2bool(state));
      break;
    }
  }
}

void LtrTracking::on_PitchEnable_stateChanged(int state)
{
  if (!initializing) TRACKER.axisChangeEnabled(PITCH, state2bool(state));
}

void LtrTracking::on_RollEnable_stateChanged(int state)
{
  if (!initializing) TRACKER.axisChangeEnabled(ROLL, state2bool(state));
}

void LtrTracking::on_YawEnable_stateChanged(int state)
{
  if (!initializing) TRACKER.axisChangeEnabled(YAW, state2bool(state));
}

void LtrTracking::on_XEnable_stateChanged(int state)
{
  if (!initializing) TRACKER.axisChangeEnabled(TX, state2bool(state));
}

void LtrTracking::on_YEnable_stateChanged(int state)
{
  if (!initializing) TRACKER.axisChangeEnabled(TY, state2bool(state));
}

void LtrTracking::on_ZEnable_stateChanged(int state)
{
  if (!initializing) TRACKER.axisChangeEnabled(TZ, state2bool(state));
}

void LtrTracking::on_PitchUpSpin_valueChanged(double d)
{
  if (!initializing) TRACKER.axisChange(PITCH, AXIS_LMULT, d);
}

void LtrTracking::on_PitchDownSpin_valueChanged(double d)
{
  if (!initializing) TRACKER.axisChange(PITCH, AXIS_RMULT, d);
}

void LtrTracking::on_YawLeftSpin_valueChanged(double d)
{
  if (!initializing) TRACKER.axisChange(YAW, AXIS_LMULT, d);
}

void LtrTracking::on_YawRightSpin_valueChanged(double d)
{
  if (!initializing) TRACKER.axisChange(YAW, AXIS_RMULT, d);
}

void LtrTracking::on_TiltLeftSpin_valueChanged(double d)
{
  if (!initializing) TRACKER.axisChange(ROLL, AXIS_LMULT, d);
}

void LtrTracking::on_TiltRightSpin_valueChanged(double d)
{
  if (!initializing) TRACKER.axisChange(ROLL, AXIS_RMULT, d);
}

void LtrTracking::on_MoveLeftSpin_valueChanged(double d)
{
  if (!initializing) TRACKER.axisChange(TX, AXIS_LMULT, d);
}

void LtrTracking::on_MoveRightSpin_valueChanged(double d)
{
  if (!initializing) TRACKER.axisChange(TX, AXIS_RMULT, d);
}

void LtrTracking::on_MoveUpSpin_valueChanged(double d)
{
  if (!initializing) TRACKER.axisChange(TY, AXIS_LMULT, d);
}

void LtrTracking::on_MoveDownSpin_valueChanged(double d)
{
  if (!initializing) TRACKER.axisChange(TY, AXIS_RMULT, d);
}

void LtrTracking::on_MoveBackSpin_valueChanged(double d)
{
  if (!initializing) TRACKER.axisChange(TZ, AXIS_LMULT, d);
}

void LtrTracking::on_MoveForthSpin_valueChanged(double d)
{
  if (!initializing) TRACKER.axisChange(TZ, AXIS_RMULT, d);
}

static Qt::CheckState bool2state(bool v)
{
  return v ? Qt::Checked : Qt::Unchecked;
}

void LtrTracking::setCommonFFVal(float val)
{
  // Use QStringLiteral for better performance and static string optimization
  gui.CommonFFVal->setText(QStringLiteral("%1%").arg(val * 100.0, 5, 'f', 1));
}

void LtrTracking::on_CommonFF_valueChanged(int value)
{
  const float val = static_cast<float>(value) / gui.CommonFF->maximum();
  if (!initializing) {
    TRACKER.setCommonFilterFactor(val);
  }
  setCommonFFVal(val);
}

void LtrTracking::setCommonFF(float val)
{
  gui.CommonFF->setValue(static_cast<int>(val * gui.CommonFF->maximum()));
  setCommonFFVal(val);
}

void LtrTracking::initAxes()
{
  initializing = true;
  
  // Use the axis configuration array to reduce code duplication
  for (const auto& config : AXIS_CONFIGS) {
    config.enable_widget->setCheckState(bool2state(TRACKER.axisGetEnabled(config.axis_id)));
    config.left_widget->setValue(TRACKER.axisGet(config.axis_id, AXIS_LMULT));
    config.right_widget->setValue(TRACKER.axisGet(config.axis_id, AXIS_RMULT));
  }
  
  setCommonFF(TRACKER.getCommonFilterFactor());
  initializing = false;
}

