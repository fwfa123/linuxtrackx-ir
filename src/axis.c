#include <assert.h>
#include <string.h>
#include <pthread.h>
#include "axis.h"
#include "spline.h"
#include "utils.h"
#include "pref.h"
#include "math_utils.h"

//The "singleton" solution was chosen to allow easy monitoring of axis changes - 
//  - there is no need to track other apps that might have open the same axes...

static pthread_mutex_t axes_mutex = PTHREAD_MUTEX_INITIALIZER;

struct axis_def{
  bool enabled;
  bool inverted;
  splines curves;
  float factor;
  float l_limit, r_limit;
  float filter_factor;
  char *prefix;
};

struct ltr_axes {
  struct axis_def pitch_axis;
  struct axis_def yaw_axis;
  struct axis_def roll_axis;
  struct axis_def tx_axis;
  struct axis_def ty_axis;
  struct axis_def tz_axis;
  bool initialized;
  bool axes_changed_flag;
  char *section;
};

char *def_section[][2] = {
  {"Title", "Default"},
  {"Pitch-enabled", "Yes"},
  {"Pitch-inverted", "No"},
  {"Pitch-deadzone", "0.0"},
  {"Pitch-left-curvature", "0.5"},
  {"Pitch-right-curvature", "0.5"},
  {"Pitch-sensitivity", "5.000000"},
  {"Pitch-left-limit", "80.000000"},
  {"Pitch-right-limit", "80.000000"},
  {"Pitch-filter", "0.3"},
  {"Pitch-left-points", LTR_CURVE_DEFAULT_TEXT},
  {"Pitch-right-points", LTR_CURVE_DEFAULT_TEXT},
  {"Yaw-enabled", "Yes"},
  {"Yaw-inverted", "No"},
  {"Yaw-deadzone", "0.0"},
  {"Yaw-left-curvature", "0.5"},
  {"Yaw-right-curvature", "0.5"},
  {"Yaw-sensitivity", "5.000000"},
  {"Yaw-left-limit", "130.000000"},
  {"Yaw-right-limit", "130.000000"},
  {"Yaw-filter", "0.3"},
  {"Yaw-left-points", LTR_CURVE_DEFAULT_TEXT},
  {"Yaw-right-points", LTR_CURVE_DEFAULT_TEXT},
  {"Roll-enabled", "Yes"},
  {"Roll-inverted", "No"},
  {"Roll-deadzone", "0.0"},
  {"Roll-left-curvature", "0.5"},
  {"Roll-right-curvature", "0.5"},
  {"Roll-sensitivity", "1.500000"},
  {"Roll-left-limit", "45.000000"},
  {"Roll-right-limit", "45.000000"},
  {"Roll-filter", "0.3"},
  {"Roll-left-points", LTR_CURVE_DEFAULT_TEXT},
  {"Roll-right-points", LTR_CURVE_DEFAULT_TEXT},
  {"Xtranslation-enabled", "Yes"},
  {"Xtranslation-inverted", "No"},
  {"Xtranslation-deadzone", "0.0"},
  {"Xtranslation-left-curvature", "0.5"},
  {"Xtranslation-right-curvature", "0.5"},
  {"Xtranslation-sensitivity", "5.000000"},
  {"Xtranslation-left-limit", "300.000000"},
  {"Xtranslation-right-limit", "300.000000"},
  {"Xtranslation-filter", "0.3"},
  {"Xtranslation-left-points", LTR_CURVE_DEFAULT_TEXT},
  {"Xtranslation-right-points", LTR_CURVE_DEFAULT_TEXT},
  {"Ytranslation-enabled", "Yes"},
  {"Ytranslation-inverted", "No"},
  {"Ytranslation-deadzone", "0.0"},
  {"Ytranslation-left-curvature", "0.5"},
  {"Ytranslation-right-curvature", "0.5"},
  {"Ytranslation-sensitivity", "5.000000"},
  {"Ytranslation-left-limit", "300.000000"},
  {"Ytranslation-right-limit", "300.000000"},
  {"Ytranslation-filter", "0.3"},
  {"Ytranslation-left-points", LTR_CURVE_DEFAULT_TEXT},
  {"Ytranslation-right-points", LTR_CURVE_DEFAULT_TEXT},
  {"Ztranslation-enabled", "Yes"},
  {"Ztranslation-inverted", "No"},
  {"Ztranslation-deadzone", "0.0"},
  {"Ztranslation-left-curvature", "0.5"},
  {"Ztranslation-right-curvature", "0.5"},
  {"Ztranslation-sensitivity", "2.000000"},
  {"Ztranslation-left-limit", "300.000000"},
  {"Ztranslation-right-limit", "1.000000"},
  {"Ztranslation-filter", "0.7"},
  {"Ztranslation-left-points", LTR_CURVE_DEFAULT_TEXT},
  {"Ztranslation-right-points", LTR_CURVE_DEFAULT_TEXT},
  {NULL, NULL}
};


typedef enum{
  SENTRY1, DEADZONE, INVERTED, LCURV, RCURV, MULT, LMULT, RMULT, LIMITS, LLIMIT, RLIMIT, FILTER, 
  ENABLED, SENTRY_2
}axis_fields;
static const char *fields[] = {NULL, "-deadzone", "-inverted",
				"-left-curvature", "-right-curvature", 
				"-sensitivity", "-left-multiplier", "-right-multiplier",
				"-limits", "-left-limit", "-right-limit", "-filter", "-enabled", NULL};
static const char *axes_desc[] = {"PITCH", "ROLL", "YAW", "TX", "TY", "TZ"};
static const char *axis_param_desc[] = {"Deadzone", "Inverted", "Left Curvature", "Right Curvature",
                   "Sensitivity", "Left Sensitivity", "Right Sensitivity", "Limit", "Left Limit", "Right Limit", 
                   "Filter Factor", "Enabled", "FULL"};

//static struct lt_axes ltr_int_axes;
//static bool ltr_int_axes_changed_flag = false;
//static bool initialized = false;

const char *ltr_int_axis_get_desc(enum axis_t id)
{
  return axes_desc[id];
}

const char *ltr_int_axis_param_get_desc(enum axis_param_t id)
{
  return axis_param_desc[id];
}

static struct axis_def *get_axis(ltr_axes_t axes, enum axis_t id)
{
  switch(id){
    case PITCH:
      return &(axes->pitch_axis);
      break;
    case ROLL:
      return &(axes->roll_axis);
      break;
    case YAW:
      return &(axes->yaw_axis);
      break;
    case TX:
      return &(axes->tx_axis);
      break;
    case TY:
      return &(axes->ty_axis);
      break;
    case TZ:
      return &(axes->tz_axis);
      break;
    default:
      assert(0);
      break;
  }
}

static char *get_axis_prefix(enum axis_t id)
{
  switch(id){
    case PITCH:
      return "Pitch";
      break;
    case ROLL:
      return "Roll";
      break;
    case YAW:
      return "Yaw";
      break;
    case TX:
      return "Xtranslation";
      break;
    case TY:
      return "Ytranslation";
      break;
    case TZ:
      return "Ztranslation";
      break;
    default:
      assert(0);
      break;
  }
}

/*
bool ltr_int_get_axes_ff(ltr_axes_t axes, double ffs[])
{
  ffs[0] = ffs[1] = ffs[2] = ffs[3] = ffs[4] = ffs[5] = 0.0;
  pthread_mutex_lock(&axes_mutex);
  int i;
  struct axis_def *axis;
  for(i = PITCH; i <= TZ; ++i){
    axis = get_axis(axes, i);
    ffs[i] = axis->filter_factor;
  }
  pthread_mutex_unlock(&axes_mutex);
  return true;
}
*/

float ltr_int_filter_axis(ltr_axes_t axes, enum axis_t id, float x, float *y_minus_1)
{
  pthread_mutex_lock(&axes_mutex);
  struct axis_def *axis = get_axis(axes, id);
  if(!axis->enabled){
    pthread_mutex_unlock(&axes_mutex);
    return 0.0f;
  }
  
  pthread_mutex_unlock(&axes_mutex);
  float trans_koef;
  switch(id){
//    case TX:
//    case TY:
    case TZ:
      trans_koef = 10;
      break;
    default:
      trans_koef = 1.0;
      break;
  }
  float ff = trans_koef * (axis->filter_factor) * (axis->l_limit > axis->r_limit ? axis->l_limit : axis->r_limit);
  return *y_minus_1 = ltr_int_nonlinfilt(x, *y_minus_1, ff);
}

float ltr_int_val_on_axis(ltr_axes_t axes, enum axis_t id, float x)
{
  pthread_mutex_lock(&axes_mutex);
  struct axis_def *axis = get_axis(axes, id);
  if(!axis->enabled){
    pthread_mutex_unlock(&axes_mutex);
    return 0.0f;
  }
  if(axis->inverted){
    x = -x;
  }
  float mf = axis->factor;
  float lim = x < 0 ? axis->l_limit : axis->r_limit;
  if(lim < 1e-5){
    pthread_mutex_unlock(&axes_mutex);
    return 0.0;
  }
  x *= mf; //apply factor (sensitivity)
  x /= lim; //normalize to apply the spline
  if(x < -1.0){
    x = -1.0;
  }
  if(x > 1.0){
    x = 1.0;
  }
  float raw = ltr_int_spline_point(&(axis->curves), x) * lim;
//  float res = ltr_int_nonlinfilt(raw, axis->last_val, axis->filter_factor);
  pthread_mutex_unlock(&axes_mutex);
  return raw;
}

static void signal_change(ltr_axes_t axes)
{
  axes->axes_changed_flag = true;
}

static bool save_val_flt(ltr_axes_t axes, enum axis_t id, axis_fields field, float val)
{
  const char *field_name = ltr_int_my_strcat(get_axis_prefix(id), fields[field]);
  bool res = ltr_int_change_key_flt(axes->section, field_name, val);
  free((void*)field_name);
  return res;
}

static bool save_val_str(ltr_axes_t axes, enum axis_t id, axis_fields field, const char *val)
{
  const char *field_name = ltr_int_my_strcat(get_axis_prefix(id), fields[field]);
  bool res = ltr_int_change_key(axes->section, field_name, val);
  free((void*)field_name);
  return res;
}

bool ltr_int_is_symetrical(ltr_axes_t axes, enum axis_t id)
{
  pthread_mutex_lock(&axes_mutex);
  struct axis_def *axis = get_axis(axes, id);
  
  if(ltr_int_curve_halves_equal(&axis->curves.left, &axis->curves.right) &&
     (fabsf(axis->l_limit - axis->r_limit) < 1e-5)){
    pthread_mutex_unlock(&axes_mutex);
    return true;
  }else{
    pthread_mutex_unlock(&axes_mutex);
    return false;
  }
}

bool ltr_int_set_axis_param(ltr_axes_t axes, enum axis_t id, enum axis_param_t param, float val)
{
  if(param == AXIS_DEADZONE || param == AXIS_LCURV || param == AXIS_RCURV){
    return true;
  }
  pthread_mutex_lock(&axes_mutex);
  struct axis_def *axis = get_axis(axes, id);
  
  switch(param){
    case AXIS_MULT:
      axis->factor = val;
      save_val_flt(axes, id, MULT, val);
      signal_change(axes);
      break;
    case AXIS_LLIMIT:
      axis->l_limit = val;
      save_val_flt(axes, id, LLIMIT, val);
      signal_change(axes);
      break;
    case AXIS_RLIMIT:
      axis->r_limit = val;
      save_val_flt(axes, id, RLIMIT, val);
      signal_change(axes);
      break;
    case AXIS_FILTER:
      axis->filter_factor = val;
      save_val_flt(axes, id, FILTER, val);
      signal_change(axes);
      break;
    default:
      pthread_mutex_unlock(&axes_mutex);
      return false;
      break;
  }
  pthread_mutex_unlock(&axes_mutex);
  return true;
}

float ltr_int_get_axis_param(ltr_axes_t axes, enum axis_t id, enum axis_param_t param)
{
  pthread_mutex_lock(&axes_mutex);
  struct axis_def *axis = get_axis(axes, id);
  float res;
  switch(param){
    case AXIS_DEADZONE:
      res = ltr_int_curve_flat_prefix(&axis->curves.left);
      {
        float right_prefix = ltr_int_curve_flat_prefix(&axis->curves.right);
        if(right_prefix > res){
          res = right_prefix;
        }
      }
      break;
    case AXIS_LCURV:
    case AXIS_RCURV:
      res = 0.0f;
      break;
    case AXIS_MULT:
      res = axis->factor;
      break;
    case AXIS_LLIMIT:
      res = axis->l_limit;
      break;
    case AXIS_RLIMIT:
      res = axis->r_limit;
      break;
    case AXIS_FILTER:
      res = axis->filter_factor;
      break;
    default:
      res = 0.0;
      break;
  }
  pthread_mutex_unlock(&axes_mutex);
  return res;
}

bool ltr_int_set_axis_bool_param(ltr_axes_t axes, enum axis_t id, enum axis_param_t param, bool val)
{
  pthread_mutex_lock(&axes_mutex);
  struct axis_def *axis = get_axis(axes, id);

  switch(param){
    case AXIS_ENABLED:
      axis->enabled = val;
      if(val){
        save_val_str(axes, id, ENABLED, "Yes");
      }else{
        save_val_str(axes, id, ENABLED, "No");
      }
      signal_change(axes);
      break;
    case AXIS_INVERTED:
      axis->inverted = val;
      if(val){
        save_val_str(axes, id, INVERTED, "Yes");
      }else{
        save_val_str(axes, id, INVERTED, "No");
      }
      signal_change(axes);
      break;
    default:
      pthread_mutex_unlock(&axes_mutex);
      return false;
      break;
  }
  pthread_mutex_unlock(&axes_mutex);
  return true;
}

bool ltr_int_get_axis_bool_param(ltr_axes_t axes, enum axis_t id, enum axis_param_t param)
{
  pthread_mutex_lock(&axes_mutex);
  bool res = false;
  struct axis_def *axis = get_axis(axes, id);
  switch(param){
    case AXIS_ENABLED:
      res = axis->enabled;
      break;
    case AXIS_INVERTED:
      res = axis->inverted;
      break;
    default:
      res = false;
      break;
  }
  pthread_mutex_unlock(&axes_mutex);
  return res;
}

static bool ltr_int_axis_get_key_flt(const char *section, const char *key_name, float *res)
{
  if(ltr_int_get_key_flt(section, key_name, res)){
    return true;
  }
  if(ltr_int_get_key_flt("Default", key_name, res)){
    ltr_int_change_key_flt(section, key_name, *res);
    return true;
  }
  return false;
}

static char *ltr_int_axis_get_key(const char *section, const char *key_name)
{
  char *res = NULL;
  res = ltr_int_get_key(section, key_name);
  if(res != NULL){
    return res;
  }
  res = ltr_int_get_key("Default", key_name);
  if(res != NULL){
    ltr_int_change_key(section, key_name, res);
    return res;
  }
  return false;
}




static const char *curve_suffix(int right)
{
  return right ? "-right-points" : "-left-points";
}

static bool save_curve_half_to_section(struct axis_def *axis, enum axis_t id, int right,
                                       const char *dest_sec)
{
  char text[LTR_CURVE_HALF_TEXT];
  char *field_name;
  bool res;
  const ltr_curve_half *half = right ? &axis->curves.right : &axis->curves.left;

  if(ltr_int_curve_format_half(half, text, sizeof(text)) < 0){
    return false;
  }
  field_name = ltr_int_my_strcat(get_axis_prefix(id), curve_suffix(right));
  res = ltr_int_change_key(dest_sec, field_name, text);
  free(field_name);
  return res;
}

static void load_curve_half(const char *sec_name, struct axis_def *axis, int right)
{
  char *field_name = ltr_int_my_strcat(axis->prefix, curve_suffix(right));
  char *string = ltr_int_axis_get_key(sec_name, field_name);
  ltr_curve_half *half = right ? &axis->curves.right : &axis->curves.left;
  ltr_curve_pt pts[LTR_CURVE_MAX_POINTS];
  int count = 0;

  if(string != NULL){
    count = ltr_int_curve_parse_points(string, pts, LTR_CURVE_MAX_POINTS);
  }
  if(count >= 2){
    ltr_int_curve_set_points(half, pts, count);
  }else{
    ltr_int_curve_set_default(half);
  }
  if(string != NULL){
    free(string);
  }
  free(field_name);
}

static void copy_curve_key(const char *from_sec, const char *to_sec, enum axis_t id, int right)
{
  char *field_name = ltr_int_my_strcat(get_axis_prefix(id), curve_suffix(right));
  char *val = ltr_int_get_key(from_sec, field_name);
  const char *use = (val != NULL) ? val : LTR_CURVE_DEFAULT_TEXT;
  ltr_int_change_key(to_sec, field_name, use);
  if(val != NULL){
    free(val);
  }
  free(field_name);
}

static void ltr_int_init_axis(const char *sec_name, struct axis_def *axis, const char *prefix)
{
  assert(axis != NULL);
  axis->enabled = true;
  axis->inverted = false;
  axis->prefix = ltr_int_my_strdup(prefix);
  axis->factor = 1.0f;
  axis->r_limit = 50.0f;
  axis->l_limit = 50.0f;
  axis->filter_factor = 0.2f;
  //Either exists -> default gets overwritten normally,
  // or not -> default stays...
  ltr_int_axis_get_key_flt(sec_name, "Filter-factor", &(axis->filter_factor));
  if(axis->filter_factor > LTR_AXIS_FILTER_MAX){
    axis->filter_factor = LTR_AXIS_FILTER_MAX;
  }
  ltr_int_curve_set_default(&axis->curves.left);
  ltr_int_curve_set_default(&axis->curves.right);
}

static void ltr_int_close_axis(ltr_axes_t axes, enum axis_t id)
{
  struct axis_def *axis = get_axis(axes, id);
  assert(axis != NULL);
  
  free(axis->prefix);
  axis->prefix = NULL;
}


static void set_axis_field(struct axis_def *axis, axis_fields field, float val, enum axis_t id)
{
  assert(axis != NULL);
  switch(field){
    case(DEADZONE):
    case(LCURV):
    case(RCURV):
      (void)val;
      break;
    case(MULT):
      axis->factor = val;
      break;
    case(LMULT):
      axis->factor += val/2;
      break;
    case(RMULT):
      axis->factor += val/2;
      break;
    case(LLIMIT):
      axis->l_limit = val;
      break;
    case(RLIMIT):
      axis->r_limit = val;
      break;
    case(LIMITS):
      //Set some sensible defaults...
      if((id == PITCH) || (id == ROLL) || (id == YAW)){
        axis->l_limit = 90.0f;
        axis->r_limit = 90.0f;
      }else{
        axis->l_limit = 300.0f;
        axis->r_limit = 300.0f;
      }
      break;
    case(FILTER):
      axis->filter_factor = (val > 2.0) ? 2.0 : val;
      break;
    default:
      assert(0);
      break;
  }
}

static bool ltr_int_get_axis(const char *sec_name, enum axis_t id, struct axis_def *axis)
{
  axis_fields i;
  char *field_name = NULL;
  char *string;
  float val;
  char *prefix = get_axis_prefix(id);
  assert(prefix != NULL);
  assert(axis != NULL);
  
  ltr_int_init_axis(sec_name, axis, prefix);
//  axis->prefix = ltr_int_my_strdup(prefix);
  for(i = DEADZONE; i <= ENABLED; ++i){
    field_name = ltr_int_my_strcat(prefix, fields[i]);
    if((i != ENABLED) && (i != INVERTED)){
      if(ltr_int_axis_get_key_flt(sec_name, field_name, &val)){
        set_axis_field(axis, i, val, id);
      }
    }else{
      string = ltr_int_axis_get_key(sec_name, field_name);
      switch(i){
        case ENABLED:
          if((string != NULL) && (strcasecmp(string, "No") == 0)){
            axis->enabled = false;
          }
          break;
        case INVERTED:
          if((string != NULL) && (strcasecmp(string, "Yes") == 0)){
            axis->inverted = true;
          }
          break;
        default:
          break;
      }
      if(string != NULL){
        free(string);
      }
    }
    free(field_name);
    field_name = NULL;
  }

  load_curve_half(sec_name, axis, 0);
  load_curve_half(sec_name, axis, 1);

  //Shouldn't be needed... (and causes deadlock now)
  //ltr_int_val_on_axis(id, 0.0f);
  return true;
}

bool ltr_int_axes_changed(ltr_axes_t axes, bool reset_flag)
{
  bool flag = axes->axes_changed_flag;
  if(reset_flag){
    axes->axes_changed_flag = false;
  }
  return flag;
}

static void ltr_int_create_default()
{
  int i = 0;
  while(def_section[i][0] != NULL){
    ltr_int_change_key("Default", def_section[i][0], def_section[i][1]);
    ++i;
  }
}

static char *ltr_int_prepare_section(const char *profile)
{
  //first check for the existence of "Default" profile
  char *sec_name = ltr_int_find_section("Title", "Default");
  if(sec_name == NULL){
    //"Default" doesn't exist, create one...
    ltr_int_log_message("Default profile not existing, creating a new one...\n");
    ltr_int_create_default();
  }else{
    free(sec_name);
  }

  sec_name = ltr_int_find_section("Title", profile);
  if(sec_name != NULL){
    return sec_name;
  }
  //Default exists now, so create the new profile
  ltr_int_log_message("Creating new profile '%s'.\n", profile);
  sec_name = ltr_int_add_unique_section("Profile");
  if(sec_name != NULL){
    ltr_int_change_key(sec_name, "Title", profile);
  }
  return sec_name;
}

void ltr_int_init_axes(ltr_axes_t *axes, const char *profile)
{
  if(axes == NULL){
    ltr_int_log_message("Don't pass NULL to ltr_int_init_axes!\n");
    return;
  }
  ltr_int_log_message("Initializing axes for profile '%s'!\n", profile);
  if(((*axes) != NULL) && ((*axes)->initialized)){
    ltr_int_close_axes(axes);
  }
  *axes = ltr_int_my_malloc(sizeof(struct ltr_axes));
  pthread_mutex_lock(&axes_mutex);
  bool res = true;
  (*axes)->axes_changed_flag = false;
  
  char *sec_name = ltr_int_prepare_section(profile);
  if(sec_name != NULL){
    //printf("Inititializing axes of %s (section %s)\n", profile, sec_name);
    res &= ltr_int_get_axis(sec_name, PITCH, &((*axes)->pitch_axis));
    res &= ltr_int_get_axis(sec_name, YAW, &((*axes)->yaw_axis));
    res &= ltr_int_get_axis(sec_name, ROLL, &((*axes)->roll_axis));
    res &= ltr_int_get_axis(sec_name, TX, &((*axes)->tx_axis));
    res &= ltr_int_get_axis(sec_name, TY, &((*axes)->ty_axis));
    res &= ltr_int_get_axis(sec_name, TZ, &((*axes)->tz_axis));
    (*axes)->initialized = res;
    //now the section should exist anyway...
    (*axes)->section = sec_name;
  }
  pthread_mutex_unlock(&axes_mutex);
}

const char *ltr_int_axes_get_section(ltr_axes_t axes)
{
  if(axes == NULL)
    return NULL;
  return axes->section;
}

void ltr_int_close_axes(ltr_axes_t *axes)
{
  if(axes == NULL){
    ltr_int_log_message("Don't pass NULL to ltr_int_close_axes!\n");
    return;
  }
  if(((*axes == NULL)) || (!(*axes)->initialized)){
    return;
  }
  pthread_mutex_lock(&axes_mutex);
  ltr_int_log_message("Closing axes!\n");
  ltr_int_close_axis(*axes, PITCH);
  ltr_int_close_axis(*axes, ROLL);
  ltr_int_close_axis(*axes, YAW);
  ltr_int_close_axis(*axes, TX);
  ltr_int_close_axis(*axes, TY);
  ltr_int_close_axis(*axes, TZ);
  free((*axes)->section);
  free(*axes);
  *axes = NULL;
  pthread_mutex_unlock(&axes_mutex);
}

void ltr_int_axes_from_default(ltr_axes_t *axes)
{
  char *profile = ltr_int_my_strdup((*axes)->section);
  ltr_int_init_axes(axes, "Default");
  free((*axes)->section);
  (*axes)->section = profile;
}

static bool copy_axis_key_between_sections(const char *from_sec, const char *to_sec,
                                           enum axis_t id, axis_fields field)
{
  char *prefix = get_axis_prefix(id);
  char *field_name = ltr_int_my_strcat(prefix, fields[field]);
  bool ok = false;

  if(field == ENABLED || field == INVERTED){
    char *val = ltr_int_get_key(from_sec, field_name);
    if(val != NULL){
      ok = ltr_int_change_key(to_sec, field_name, val);
      free(val);
    }
  }else if(field != LMULT && field != RMULT && field != LIMITS){
    float val;
    if(ltr_int_get_key_flt(from_sec, field_name, &val)){
      ok = ltr_int_change_key_flt(to_sec, field_name, val);
    }
  }
  free(field_name);
  return ok;
}

bool ltr_int_axes_copy_profile_keys(const char *from_profile_title,
                                    const char *to_profile_title)
{
  int field;
  enum axis_t id;

  if(from_profile_title == NULL || to_profile_title == NULL){
    return false;
  }

  if(ltr_int_find_section("Title", "Default") == NULL){
    ltr_int_create_default();
  }

  char *from_sec = ltr_int_find_section("Title", from_profile_title);
  if(from_sec == NULL){
    return false;
  }

  char *to_sec = ltr_int_find_section("Title", to_profile_title);
  if(to_sec == NULL){
    free(from_sec);
    return false;
  }

  pthread_mutex_lock(&axes_mutex);
  for(id = PITCH; id <= TZ; id = (enum axis_t)(id + 1)){
    for(field = DEADZONE; field <= ENABLED; ++field){
      if(field == DEADZONE || field == LCURV || field == RCURV){
        continue;
      }
      copy_axis_key_between_sections(from_sec, to_sec, id, (axis_fields)field);
    }
    copy_curve_key(from_sec, to_sec, id, 0);
    copy_curve_key(from_sec, to_sec, id, 1);
  }
  pthread_mutex_unlock(&axes_mutex);

  free(from_sec);
  free(to_sec);
  return true;
}

void ltr_int_ensure_axis_baseline(void)
{
  int i;

  if(ltr_int_find_section("Title", LTR_AXIS_BASELINE_PROFILE) != NULL){
    return;
  }

  char *sec = ltr_int_add_unique_section("AxisBaseline");
  if(sec == NULL){
    return;
  }
  ltr_int_change_key(sec, "Title", LTR_AXIS_BASELINE_PROFILE);
  i = 0;
  while(def_section[i][0] != NULL){
    if(strcmp(def_section[i][0], "Title") != 0){
      ltr_int_change_key(sec, def_section[i][0], def_section[i][1]);
    }
    ++i;
  }
  free(sec);
}

static bool write_axis_state_to_section(struct axis_def *axis, enum axis_t id,
                                       const char *dest_sec)
{
  char *prefix = get_axis_prefix(id);
  char *field_name;
  bool ok = true;

  field_name = ltr_int_my_strcat(prefix, fields[ENABLED]);
  ok &= ltr_int_change_key(dest_sec, field_name, axis->enabled ? "Yes" : "No");
  free(field_name);

  field_name = ltr_int_my_strcat(prefix, fields[INVERTED]);
  ok &= ltr_int_change_key(dest_sec, field_name, axis->inverted ? "Yes" : "No");
  free(field_name);

  ok &= save_curve_half_to_section(axis, id, 0, dest_sec);
  ok &= save_curve_half_to_section(axis, id, 1, dest_sec);

  field_name = ltr_int_my_strcat(prefix, fields[MULT]);
  ok &= ltr_int_change_key_flt(dest_sec, field_name, axis->factor);
  free(field_name);

  field_name = ltr_int_my_strcat(prefix, fields[LLIMIT]);
  ok &= ltr_int_change_key_flt(dest_sec, field_name, axis->l_limit);
  free(field_name);

  field_name = ltr_int_my_strcat(prefix, fields[RLIMIT]);
  ok &= ltr_int_change_key_flt(dest_sec, field_name, axis->r_limit);
  free(field_name);

  field_name = ltr_int_my_strcat(prefix, fields[FILTER]);
  ok &= ltr_int_change_key_flt(dest_sec, field_name, axis->filter_factor);
  free(field_name);

  return ok;
}

bool ltr_int_axes_persist_state_to_profile(ltr_axes_t axes, const char *profile_title)
{
  char *dest_sec;

  if(axes == NULL || profile_title == NULL || !axes->initialized){
    return false;
  }

  if(ltr_int_find_section("Title", "Default") == NULL){
    ltr_int_create_default();
  }

  dest_sec = ltr_int_find_section("Title", profile_title);
  if(dest_sec == NULL){
    return false;
  }

  pthread_mutex_lock(&axes_mutex);
  write_axis_state_to_section(&(axes->pitch_axis), PITCH, dest_sec);
  write_axis_state_to_section(&(axes->yaw_axis), YAW, dest_sec);
  write_axis_state_to_section(&(axes->roll_axis), ROLL, dest_sec);
  write_axis_state_to_section(&(axes->tx_axis), TX, dest_sec);
  write_axis_state_to_section(&(axes->ty_axis), TY, dest_sec);
  write_axis_state_to_section(&(axes->tz_axis), TZ, dest_sec);
  pthread_mutex_unlock(&axes_mutex);

  free(dest_sec);
  return true;
}

int ltr_int_axis_get_curve(ltr_axes_t axes, enum axis_t id, int right,
                           ltr_curve_pt *pts, int max_pts)
{
  struct axis_def *axis;
  const ltr_curve_half *half;
  int n;
  int i;

  if(axes == NULL || pts == NULL || max_pts <= 0){
    return 0;
  }
  pthread_mutex_lock(&axes_mutex);
  axis = get_axis(axes, id);
  half = right ? &axis->curves.right : &axis->curves.left;
  n = half->count;
  if(n > max_pts){
    n = max_pts;
  }
  for(i = 0; i < n; ++i){
    pts[i] = half->pts[i];
  }
  pthread_mutex_unlock(&axes_mutex);
  return n;
}

bool ltr_int_axis_set_curve(ltr_axes_t axes, enum axis_t id,
                            const ltr_curve_pt *left, int nleft,
                            const ltr_curve_pt *right, int nright,
                            bool persist)
{
  struct axis_def *axis;

  if(axes == NULL || left == NULL || right == NULL){
    return false;
  }
  pthread_mutex_lock(&axes_mutex);
  axis = get_axis(axes, id);
  ltr_int_curve_set_points(&axis->curves.left, left, nleft);
  ltr_int_curve_set_points(&axis->curves.right, right, nright);
  if(persist){
    save_curve_half_to_section(axis, id, 0, axes->section);
    save_curve_half_to_section(axis, id, 1, axes->section);
  }
  signal_change(axes);
  pthread_mutex_unlock(&axes_mutex);
  return true;
}

bool ltr_int_axis_set_curve_string(ltr_axes_t axes, enum axis_t id, const char *text)
{
  ltr_curve_pt left[LTR_CURVE_MAX_POINTS];
  ltr_curve_pt right[LTR_CURVE_MAX_POINTS];
  int nleft = 0;
  int nright = 0;

  if(!ltr_int_curve_parse_both_points(text, left, &nleft, right, &nright, LTR_CURVE_MAX_POINTS)){
    return false;
  }
  return ltr_int_axis_set_curve(axes, id, left, nleft, right, nright, true);
}

int ltr_int_axis_format_curve(ltr_axes_t axes, enum axis_t id, char *buf, int buflen)
{
  struct axis_def *axis;
  int wrote;

  if(axes == NULL || buf == NULL){
    return -1;
  }
  pthread_mutex_lock(&axes_mutex);
  axis = get_axis(axes, id);
  wrote = ltr_int_curve_format_both(&axis->curves, buf, buflen);
  pthread_mutex_unlock(&axes_mutex);
  return wrote;
}

float ltr_int_axis_flat_prefix(ltr_axes_t axes, enum axis_t id)
{
  struct axis_def *axis;
  float left_prefix;
  float right_prefix;

  if(axes == NULL){
    return 0.f;
  }
  pthread_mutex_lock(&axes_mutex);
  axis = get_axis(axes, id);
  left_prefix = ltr_int_curve_flat_prefix(&axis->curves.left);
  right_prefix = ltr_int_curve_flat_prefix(&axis->curves.right);
  pthread_mutex_unlock(&axes_mutex);
  return (right_prefix > left_prefix) ? right_prefix : left_prefix;
}


