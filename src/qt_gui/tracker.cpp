#include "tracker.h"
#include <tracking.h>
#include <pref.hpp>
#include <pref.h>
#include <axis.h>
#include <ltlib.h>
#include <../ltr_srv_master.h>
#include <../ltr_srv_slave.h>
#include <cstdio>
#include "ltr_gui_prefs.h"
#include <iostream>
#include <unistd.h>
#include <QApplication>
#include <QMessageBox>
#include <QTimer>
#include <QDateTime>

Tracker *Tracker::trr = NULL;
char *com_fname = NULL;

class MasterThread : public QThread{
  public:
    void run();
};

void MasterThread::run()
{
  ltr_int_master(false);
}

Tracker& Tracker::trackerInst()
{
  if(trr == NULL){
    trr = new Tracker();
  }
  return *trr;
}

static void ltr_int_new_slave(char *name)
{
  TRACKER.signalNewSlave(name);
}

static void ltr_int_state_changed(void *param)
{
  (void) param;
  TRACKER.signalStateChange(ltr_int_get_tracking_state());
}

static buffering buf(3);
static bool initBuffers = true;
static void ltr_int_new_frame(struct frame_type *frame, void *param)
{
  (void) param;
  static struct frame_type local_frame;
  local_frame.bloblist = frame->bloblist;
  local_frame.width = frame->width;
  local_frame.height = frame->height;
  local_frame.counter = frame->counter;

  if(initBuffers){
    buf.resizeBuffers(frame->width, frame->height);
    initBuffers = false;
  }

  if(frame->bitmap != NULL){
    buf.bufferWritten();
  }
  buffer *b;
  if(buf.writeBuffer(&b)){
    frame->bitmap = b->getBuffer();
  }else{
    frame->bitmap = NULL;
  }

  TRACKER.signalNewFrame(&local_frame);
  static linuxtrack_full_pose_t current_pose;
  ltr_int_get_camera_update(&current_pose);
  TRACKER.signalNewPose(&current_pose);
}

buffering *Tracker::getBuffers()
{
  return &buf;
}

Tracker::Tracker() : axes(LTR_AXES_T_INITIALIZER), axes_valid(false),
  currentProfile(QString::fromUtf8("Default")), common_ff(0.0), trackingRateFps(120),
  trackingRateLastEmitUs_(0), trackingEmitThisCycle_(false)
{
  if(!ltr_int_gui_lock(true)){
    QMessageBox::warning(NULL, QObject::tr("Linuxtrack"),
      QObject::tr("Another linuxtrack gui is running already!"),
                         QMessageBox::Ok);
  }
  master = new MasterThread();
  saveDebounceTimer = new QTimer(this);
  saveDebounceTimer->setSingleShot(true);
  connect(saveDebounceTimer, &QTimer::timeout, this, &Tracker::performDeferredSave);
  ltr_int_set_callback_hooks(ltr_int_new_frame, ltr_int_state_changed, ltr_int_new_slave);
  setProfile(QString::fromUtf8("Default"));
  axes_valid = true;
}

void Tracker::performDeferredSave()
{
  if(ltr_int_need_saving()){
    ltr_int_save_prefs(NULL);
  }
}

void Tracker::schedulePrefsSave()
{
  saveDebounceTimer->start(800);
}

Tracker::~Tracker()
{
  if(master->isRunning()){
    ltr_int_request_shutdown();
    master->wait();
  }
}

void Tracker::signalStateChange(linuxtrack_state_type current_state)
{
  emit stateChanged(current_state);
}

bool Tracker::trackingRateGateOpen()
{
  if(trackingRateFps >= 120){
    return true;
  }
  const qint64 min_interval_us = 1000000LL / trackingRateFps;
  const qint64 t = QDateTime::currentMSecsSinceEpoch() * 1000;
  if(t - trackingRateLastEmitUs_ < min_interval_us){
    return false;
  }
  trackingRateLastEmitUs_ = t;
  return true;
}

void Tracker::signalNewFrame(struct frame_type *frame)
{
  // Same gate as pose: camera FPS counter and preview follow profile rate (see signalNewPose).
  trackingEmitThisCycle_ = trackingRateGateOpen();
  if(!trackingEmitThisCycle_){
    return;
  }
  emit newFrame(frame);
}

void Tracker::signalNewPose(linuxtrack_full_pose_t *full_pose)
{
  if(!trackingEmitThisCycle_){
    return;
  }
  trackingEmitThisCycle_ = false;

  static linuxtrack_pose_t processed;
  static linuxtrack_pose_t unfiltered;
  processed = full_pose->pose;
  ltr_int_postprocess_axes(axes, &processed, &unfiltered);
  emit newPose(full_pose, &unfiltered, &processed);
}


void Tracker::signalNewSlave(const char *name)
{
  ltr_axes_t tmp_axes = LTR_AXES_T_INITIALIZER;
  ltr_int_init_axes(&tmp_axes, name);
  for(int i = PITCH; i <= TZ; ++i){
    ltr_int_change(name, i, AXIS_ENABLED, ltr_int_get_axis_bool_param(tmp_axes, (axis_t)i, AXIS_ENABLED)?1.0:0.0);
    ltr_int_change(name, i, AXIS_INVERTED, ltr_int_get_axis_bool_param(tmp_axes, (axis_t)i, AXIS_INVERTED)?1.0:0.0);
    for(int j = AXIS_DEADZONE; j <= AXIS_FILTER; ++j){
      ltr_int_change(name, i, j, ltr_int_get_axis_param(tmp_axes, (axis_t)i, (axis_param_t)j));
    }
  }
  ltr_int_change(NULL, MISC, MISC_LEGR, ltr_int_use_oldrot()?1.0:0.0);
  ltr_int_change(NULL, MISC, MISC_ALTER, ltr_int_use_alter()?1.0:0.0);
  ltr_int_change(NULL, MISC, MISC_ALIGN, ltr_int_do_tr_align()?1.0:0.0);
  ltr_int_change(NULL, MISC, MISC_FOCAL_LENGTH, ltr_int_get_focal_length());
  ltr_int_close_axes(&tmp_axes);
}

static float limit_ff(float val)
{
  if(val < 0.0) return 0.0;
  if(val > LTR_AXIS_FILTER_MAX) return LTR_AXIS_FILTER_MAX;
  return val;
}

void Tracker::broadcastAxesToSlaves()
{
  const QByteArray sec = profileSection.toUtf8();
  const char *profile = sec.constData();
  for(int i = PITCH; i <= TZ; ++i){
    ltr_int_change(profile, i, AXIS_ENABLED,
      ltr_int_get_axis_bool_param(axes, (axis_t)i, AXIS_ENABLED) ? 1.0 : 0.0);
    ltr_int_change(profile, i, AXIS_INVERTED,
      ltr_int_get_axis_bool_param(axes, (axis_t)i, AXIS_INVERTED) ? 1.0 : 0.0);
    for(int j = AXIS_DEADZONE; j <= AXIS_FILTER; ++j){
      float val = ltr_int_get_axis_param(axes, (axis_t)i, (axis_param_t)j);
      if((axis_param_t)j == AXIS_FILTER){
        val = limit_ff(ffs[i] + common_ff);
      }
      ltr_int_change(profile, i, j, val);
    }
  }
}

void Tracker::reloadAxesState()
{
  ltr_int_close_axes(&axes);
  ltr_int_init_axes(&axes, currentProfile.toUtf8().constData());
  const char *sec = ltr_int_axes_get_section(axes);
  if(sec != NULL){
    profileSection = QString::fromUtf8(sec);
  }

  common_ff = 1.0;
  int i;
  for(i = PITCH; i <= TZ; ++i){
    ffs[i] = ltr_int_get_axis_param(axes, (axis_t)i, AXIS_FILTER);
    if(common_ff > ffs[i]){
      common_ff = ffs[i];
    }
  }
  for(i = PITCH; i <= TZ; ++i){
    ffs[i] -= common_ff;
  }

  axes_valid = true;
  emit setCommonFF(common_ff);
  emit initAxes();
}

void Tracker::restoreAxisDefaults()
{
  ltr_int_ensure_axis_baseline();
  if(!ltr_int_axes_copy_profile_keys(LTR_AXIS_BASELINE_PROFILE,
                                     currentProfile.toUtf8().constData())){
    return;
  }
  reloadAxesState();
  broadcastAxesToSlaves();
  ltr_int_prefs_changed();
  ltr_int_save_prefs(NULL);
}

void Tracker::saveAxisDefaults()
{
  ltr_int_ensure_axis_baseline();
  if(!ltr_int_axes_persist_state_to_profile(axes, LTR_AXIS_BASELINE_PROFILE)){
    return;
  }
  ltr_int_prefs_changed();
  ltr_int_save_prefs(NULL);
}


void Tracker::setProfile(QString p)
{
  axes_valid = false;
  ltr_int_close_axes(&axes);
  //PREF.setCustomSection(p);
  currentProfile = p;
  PREF.getProfileSection(currentProfile, profileSection);
  //std::cout<<"Set profile "<<currentProfile.toStdString()<<" - "<<p.toStdString()<<"\n";
  ltr_int_init_axes(&axes, currentProfile.toUtf8().constData());
  // Keep profileSection in sync with axes section so ltr_int_change() and axis
  // persistence (save_val_flt) always use the same prefs section.
  const char *sec = ltr_int_axes_get_section(axes);
  if(sec != NULL){
    profileSection = QString::fromUtf8(sec);
  }

  common_ff = 1.0;
  int i;
  for(i = PITCH; i <= TZ; ++i){
    ffs[i] = ltr_int_get_axis_param(axes, (axis_t)i, AXIS_FILTER);
    if(common_ff > ffs[i]){
      common_ff = ffs[i];
    }

  }
  for(i = PITCH; i <= TZ; ++i){
    ffs[i] -= common_ff;
  }

  // Load tracking rate from profile prefs (default 120)
  {
    int tr = 120;
    if(ltr_int_get_key_int(profileSection.toUtf8().constData(), "TrackingRateFps", &tr)){
      trackingRateFps = tr;
    }else{
      trackingRateFps = 120;
    }
    if(trackingRateFps < 30) trackingRateFps = 30;
    if(trackingRateFps > 120) trackingRateFps = 120;
  }
  
  axes_valid = true;
  emit initAxes();
}

void Tracker::fromDefault()
{
  if(!ltr_int_axes_copy_profile_keys("Default", currentProfile.toUtf8().constData())){
    return;
  }
  reloadAxesState();
  broadcastAxesToSlaves();
  ltr_int_prefs_changed();
  ltr_int_save_prefs(NULL);
}

void Tracker::start(QString &section)
{
  (void) section;
  initBuffers = true;
  buf.init();
  master->start();
}

void Tracker::pause()
{
  ltr_int_suspend_cmd();
}

void Tracker::wakeup()
{
  ltr_int_wakeup_cmd();
}

void Tracker::recenter()
{
  ltr_int_recenter_cmd();
}

void Tracker::stop()
{
  ltr_int_request_shutdown();
  if(master->isRunning()){
    ltr_int_request_shutdown();
    master->wait();
  }
}

bool Tracker::axisChange(axis_t axis, axis_param_t elem, bool enabled)
{
  ltr_int_set_axis_bool_param(axes, axis, elem, enabled);
  emit axisChanged(axis, elem);
  ltr_int_change(profileSection.toUtf8().constData(), axis, elem, enabled?1.0:0.0);
  ltr_int_prefs_changed();
  saveDebounceTimer->start(800);
  return true;
}

bool Tracker::miscChange(axis_param_t elem, bool enabled)
{
  ltr_int_change(NULL, MISC, elem, enabled?1.0:0.0);
  ltr_int_prefs_changed();
  saveDebounceTimer->start(800);
  return true;
}

bool Tracker::miscChange(axis_param_t elem, float val)
{
  ltr_int_change(NULL, MISC, elem, val);
  ltr_int_prefs_changed();
  saveDebounceTimer->start(800);
  return true;
}

bool Tracker::axisChange(axis_t axis, axis_param_t elem, float val)
{
  if(elem == AXIS_FILTER){
    ffs[axis] = val;
    val = limit_ff(val + common_ff);
  }
  bool res = ltr_int_set_axis_param(axes, axis, elem, val);
  emit axisChanged(axis, elem);
  ltr_int_change(profileSection.toUtf8().constData(), axis, elem, val);
  ltr_int_prefs_changed();
  saveDebounceTimer->start(800);
  return res;
}

bool Tracker::axisGetBool(axis_t axis, axis_param_t elem)
{
  return ltr_int_get_axis_bool_param(axes, axis, elem);
}

float Tracker::axisGet(axis_t axis, axis_param_t elem)
{
  if(elem == AXIS_FILTER){
    return ffs[axis];
  }else{
    return ltr_int_get_axis_param(axes, axis, elem);
  }
}

float Tracker::axisGetValue(axis_t axis, float val)
{
  return ltr_int_val_on_axis(axes, axis, val);
}

bool Tracker::axisIsSymetrical(axis_t axis)
{
  return ltr_int_is_symetrical(axes, axis);
}

bool Tracker::setCommonFilterFactor(float c_f)
{
  common_ff = c_f;
  bool res = true;
  float val;
  for(int i = PITCH; i <= TZ; ++i){
    val = limit_ff(ffs[i] + common_ff);
    res &= ltr_int_set_axis_param(axes, (axis_t)i, AXIS_FILTER, val);
    emit axisChanged(i, AXIS_FILTER);
    ltr_int_change(profileSection.toUtf8().constData(), i, AXIS_FILTER, val);
  }
  emit setCommonFF(c_f);
  ltr_int_prefs_changed();
  saveDebounceTimer->start(800);
  return res;
}

float Tracker::getCommonFilterFactor()
{
  return common_ff;
}

int Tracker::getTrackingRateFps()
{
  return trackingRateFps;
}

void Tracker::setTrackingRateFps(int fps)
{
  if(fps < 30) fps = 30;
  if(fps > 120) fps = 120;
  trackingRateFps = fps;
  ltr_int_change_key_int(profileSection.toUtf8().constData(), "TrackingRateFps", fps);
  ltr_int_prefs_changed();
  saveDebounceTimer->start(800);
}

