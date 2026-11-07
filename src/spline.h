#ifndef SPLINE__H
#define SPLINE__H

#ifdef __cplusplus
extern "C" {
#endif

/* One half of an axis response: x,y in [0,1], endpoints pinned at (0,0) and (1,1). */
#define LTR_CURVE_MAX_POINTS 16
#define LTR_CURVE_BUCKETS 4096
#define LTR_CURVE_MIN_GAP 0.01f
#define LTR_CURVE_HALF_TEXT 256
#define LTR_CURVE_TEXT 500
#define LTR_CURVE_DEFAULT_TEXT "0.0000,0.0000;1.0000,1.0000"

typedef struct{
  float x;
  float y;
}ltr_curve_pt;

typedef struct{
  int count;
  ltr_curve_pt pts[LTR_CURVE_MAX_POINTS];
  float table[LTR_CURVE_BUCKETS];
}ltr_curve_half;

typedef struct{
  ltr_curve_half left;
  ltr_curve_half right;
}splines;

void ltr_int_curve_set_default(ltr_curve_half *half);
void ltr_int_curve_set_points(ltr_curve_half *half, const ltr_curve_pt *pts, int count);
int ltr_int_curve_halves_equal(const ltr_curve_half *a, const ltr_curve_half *b);
float ltr_int_curve_flat_prefix(const ltr_curve_half *half);

int ltr_int_curve_format_half(const ltr_curve_half *half, char *buf, int buflen);
int ltr_int_curve_parse_points(const char *str, ltr_curve_pt *pts, int max_pts);
int ltr_int_curve_format_both(const splines *curves, char *buf, int buflen);
int ltr_int_curve_parse_both_points(const char *str,
                                    ltr_curve_pt *left, int *nleft,
                                    ltr_curve_pt *right, int *nright,
                                    int max_pts);

float ltr_int_spline_point(const splines *curves, float x);

#ifdef __cplusplus
}
#endif

#endif
