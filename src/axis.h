#ifndef AXIS__H
#define AXIS__H

#ifdef __cplusplus
extern "C" {
#endif

#include <stdbool.h>
#include "spline.h"

struct ltr_axes;
typedef struct ltr_axes *ltr_axes_t;
#define LTR_AXES_T_INITIALIZER NULL
#define LTR_AXIS_FILTER_MAX 4.0

enum axis_t {PITCH, ROLL, YAW, TX, TY, TZ, MISC};
enum axis_param_t {AXIS_ENABLED, AXIS_DEADZONE,
                   AXIS_LCURV, AXIS_RCURV,
                   AXIS_MULT,
                   AXIS_LLIMIT, AXIS_RLIMIT,
                   AXIS_FILTER,
                   AXIS_INVERTED,
                   AXIS_FULL, MISC_LEGR, MISC_ALTER, MISC_ALIGN, MISC_FOCAL_LENGTH,
                   AXIS_CURVE,
                   AXIS_DEFAULT = 1024};

void ltr_int_init_axes(ltr_axes_t *axes, const char *profile);
void ltr_int_close_axes(ltr_axes_t *axes);
const char *ltr_int_axes_get_section(ltr_axes_t axes);
float ltr_int_val_on_axis(ltr_axes_t axes, enum axis_t id, float x);
float ltr_int_filter_axis(ltr_axes_t axes, enum axis_t id, float x, float *y_minus_1);

bool ltr_int_is_symetrical(ltr_axes_t axes, enum axis_t id);

bool ltr_int_set_axis_param(ltr_axes_t axes, enum axis_t id, enum axis_param_t param, float val);
float ltr_int_get_axis_param(ltr_axes_t axes, enum axis_t id, enum axis_param_t param);

bool ltr_int_set_axis_bool_param(ltr_axes_t axes, enum axis_t id, enum axis_param_t param, bool val);
bool ltr_int_get_axis_bool_param(ltr_axes_t axes, enum axis_t id, enum axis_param_t param);

bool ltr_int_axes_changed(ltr_axes_t axes, bool reset_flag);
bool ltr_int_get_axes_ff(ltr_axes_t axes, double ffs[]);

const char *ltr_int_axis_get_desc(enum axis_t id);
const char *ltr_int_axis_param_get_desc(enum axis_param_t id);

#define LTR_AXIS_BASELINE_PROFILE "AxisBaseline"

void ltr_int_axes_from_default(ltr_axes_t *axes);

void ltr_int_ensure_axis_baseline(void);
bool ltr_int_axes_copy_profile_keys(const char *from_profile_title,
                                    const char *to_profile_title);
bool ltr_int_axes_persist_state_to_profile(ltr_axes_t axes,
                                           const char *profile_title);

/* Points are normalized [0,1] per half; right=0 is the negative-input half. */
int ltr_int_axis_get_curve(ltr_axes_t axes, enum axis_t id, int right,
                           ltr_curve_pt *pts, int max_pts);
bool ltr_int_axis_set_curve(ltr_axes_t axes, enum axis_t id,
                            const ltr_curve_pt *left, int nleft,
                            const ltr_curve_pt *right, int nright,
                            bool persist);
bool ltr_int_axis_set_curve_string(ltr_axes_t axes, enum axis_t id, const char *text);
int ltr_int_axis_format_curve(ltr_axes_t axes, enum axis_t id, char *buf, int buflen);
float ltr_int_axis_flat_prefix(ltr_axes_t axes, enum axis_t id);

#ifdef __cplusplus
}
#endif

#endif
