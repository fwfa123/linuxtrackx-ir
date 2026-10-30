#include <math.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include "spline.h"

static int cmp_pt(const void *va, const void *vb)
{
  const ltr_curve_pt *a = (const ltr_curve_pt *)va;
  const ltr_curve_pt *b = (const ltr_curve_pt *)vb;
  if(a->x < b->x) return -1;
  if(a->x > b->x) return 1;
  return 0;
}

static double clampd(double v, double lo, double hi)
{
  if(v < lo) return lo;
  if(v > hi) return hi;
  return v;
}

static float clampf(float v, float lo, float hi)
{
  if(v < lo) return lo;
  if(v > hi) return hi;
  return v;
}

/* Real roots of a·t³ + b·t² + c·t + d = 0 (OpenTrack Catmull-Rom reduction). */
static unsigned solve_cubic(double a, double b, double c, double d, double roots[3])
{
  const double eps = 1e-9;
  double scale = fabs(a);
  if(fabs(b) > scale) scale = fabs(b);
  if(fabs(c) > scale) scale = fabs(c);

  if(!(scale > 1e-12)){
    return 0;
  }

  if(fabs(a) <= eps * scale){
    if(fabs(b) <= eps * scale){
      if(fabs(c) <= eps * scale){
        return 0;
      }
      roots[0] = -d / c;
      return 1;
    }
    {
      const double disc = c * c - 4.0 * b * d;
      double q;
      if(disc < 0.0){
        return 0;
      }
      q = -0.5 * (c + copysign(sqrt(disc), c));
      roots[0] = q / b;
      roots[1] = (q != 0.0) ? d / q : -c / b - roots[0];
      return 2;
    }
  }

  {
    const double B = b / a;
    const double C = c / a;
    const double D = d / a;
    const double ofs = B / 3.0;
    const double p = C - B * B / 3.0;
    const double q = B * (2.0 * B * B - 9.0 * C) / 27.0 + D;
    const double disc = q * q / 4.0 + p * p * p / 27.0;

    if(disc > 0.0){
      const double s = sqrt(disc);
      roots[0] = cbrt(-q / 2.0 + s) + cbrt(-q / 2.0 - s) - ofs;
      return 1;
    }
    if(p >= 0.0){
      roots[0] = cbrt(-q) - ofs;
      return 1;
    }
    {
      const double m = 2.0 * sqrt(-p / 3.0);
      const double arg = clampd(3.0 * q / (p * m), -1.0, 1.0);
      const double theta = acos(arg) / 3.0;
      unsigned k;
      for(k = 0; k < 3; ++k){
        roots[k] = m * cos(theta - 2.0 * 3.14159265358979323846 * (double)k / 3.0) - ofs;
      }
      return 3;
    }
  }
}

/* Largest t in [0,1] with x(t)=X for cx Catmull-Rom coeffs, or -1. */
static double solve_segment_time(const double cx[4], double X)
{
  double roots[3];
  const unsigned n = solve_cubic(cx[3] / 2.0, cx[2] / 2.0, cx[1] / 2.0,
                                  cx[0] / 2.0 - X, roots);
  const double eps = 1e-4;
  double t = -1.0;
  unsigned i;
  for(i = 0; i < n; ++i){
    if(roots[i] >= -eps && roots[i] <= 1.0 + eps){
      const double clamped = clampd(roots[i], 0.0, 1.0);
      if(clamped > t){
        t = clamped;
      }
    }
  }
  return t;
}

static double poly4(const double c[4], double t)
{
  const double t2 = t * t;
  return 0.5 * (c[0] + c[1] * t + c[2] * t2 + c[3] * t2 * t);
}

static void sample_at(const ltr_curve_half *half, int index, double *x, double *y)
{
  if(index < 0){
    index = 0;
  }
  if(index >= half->count){
    index = half->count - 1;
  }
  *x = half->pts[index].x;
  *y = half->pts[index].y;
}

static void rebuild_half(ltr_curve_half *half)
{
  const double c = (double)(LTR_CURVE_BUCKETS - 1);
  int i;
  unsigned k;

  for(k = 0; k < LTR_CURVE_BUCKETS; ++k){
    half->table[k] = 0.f;
  }
  if(half->count < 2){
    return;
  }

  for(i = 0; i + 1 < half->count; ++i){
    double p0x, p0y, p1x, p1y, p2x, p2y, p3x, p3y;
    double cx[4], cy[4];
    unsigned start, end;

    sample_at(half, i - 1, &p0x, &p0y);
    sample_at(half, i, &p1x, &p1y);
    sample_at(half, i + 1, &p2x, &p2y);
    sample_at(half, i + 2, &p3x, &p3y);

    cx[0] = 2.0 * p1x;
    cx[1] = -p0x + p2x;
    cx[2] = 2.0 * p0x - 5.0 * p1x + 4.0 * p2x - p3x;
    cx[3] = -p0x + 3.0 * p1x - 3.0 * p2x + p3x;
    cy[0] = 2.0 * p1y;
    cy[1] = -p0y + p2y;
    cy[2] = 2.0 * p0y - 5.0 * p1y + 4.0 * p2y - p3y;
    cy[3] = -p0y + 3.0 * p1y - 3.0 * p2y + p3y;

    start = (unsigned)ceil(p1x * c);
    end = (unsigned)floor(p2x * c);
    if(end >= LTR_CURVE_BUCKETS){
      end = LTR_CURVE_BUCKETS - 1;
    }
    if(start >= LTR_CURVE_BUCKETS || start > end){
      continue;
    }
    for(k = start; k <= end; ++k){
      const double X = clampd((double)k / c, p1x, p2x);
      double t = solve_segment_time(cx, X);
      double y;
      if(t < 0.0){
        const double span = p2x - p1x;
        t = (span > 1e-9) ? clampd((X - p1x) / span, 0.0, 1.0) : 1.0;
      }
      y = poly4(cy, t);
      if(isfinite(y)){
        half->table[k] = (float)y;
      }
    }
  }

  {
    const float prefix = ltr_int_curve_flat_prefix(half);
    unsigned max;
    if(prefix > 0.f){
      max = (unsigned)(prefix * c);
      if(max > LTR_CURVE_BUCKETS){
        max = LTR_CURVE_BUCKETS;
      }
      for(k = 0; k < max; ++k){
        half->table[k] = 0.f;
      }
    }
  }

  for(k = 0; k < LTR_CURVE_BUCKETS; ++k){
    half->table[k] = clampf(half->table[k], 0.f, 1.f);
  }
}

void ltr_int_curve_set_points(ltr_curve_half *half, const ltr_curve_pt *pts, int count)
{
  ltr_curve_pt sorted[LTR_CURVE_MAX_POINTS];
  ltr_curve_pt out[LTR_CURVE_MAX_POINTS];
  int n = 0;
  int m = 0;
  int i;

  if(half == NULL){
    return;
  }
  if(pts != NULL){
    for(i = 0; i < count && n < LTR_CURVE_MAX_POINTS; ++i){
      sorted[n].x = clampf(pts[i].x, 0.f, 1.f);
      sorted[n].y = clampf(pts[i].y, 0.f, 1.f);
      ++n;
    }
  }
  if(n > 1){
    qsort(sorted, (size_t)n, sizeof(sorted[0]), cmp_pt);
  }

  out[0].x = 0.f;
  out[0].y = 0.f;
  m = 1;
  for(i = 0; i < n; ++i){
    if(sorted[i].x < LTR_CURVE_MIN_GAP){
      continue;
    }
    if(sorted[i].x > 1.f - LTR_CURVE_MIN_GAP){
      continue;
    }
    if(m >= LTR_CURVE_MAX_POINTS - 1){
      break;
    }
    if(sorted[i].x - out[m - 1].x < LTR_CURVE_MIN_GAP){
      continue;
    }
    out[m] = sorted[i];
    ++m;
  }
  out[m].x = 1.f;
  out[m].y = 1.f;
  ++m;

  half->count = m;
  memcpy(half->pts, out, (size_t)m * sizeof(ltr_curve_pt));
  rebuild_half(half);
}

void ltr_int_curve_set_default(ltr_curve_half *half)
{
  ltr_curve_pt pts[2];
  pts[0].x = 0.f;
  pts[0].y = 0.f;
  pts[1].x = 1.f;
  pts[1].y = 1.f;
  ltr_int_curve_set_points(half, pts, 2);
}

int ltr_int_curve_halves_equal(const ltr_curve_half *a, const ltr_curve_half *b)
{
  int i;
  if(a == NULL || b == NULL || a->count != b->count){
    return 0;
  }
  for(i = 0; i < a->count; ++i){
    if(fabsf(a->pts[i].x - b->pts[i].x) > 1e-4f) return 0;
    if(fabsf(a->pts[i].y - b->pts[i].y) > 1e-4f) return 0;
  }
  return 1;
}

float ltr_int_curve_flat_prefix(const ltr_curve_half *half)
{
  int i;
  if(half == NULL){
    return 0.f;
  }
  for(i = 0; i < half->count; ++i){
    if(half->pts[i].x >= 1e-4f && half->pts[i].y >= 1e-4f){
      if(i == 0){
        return 0.f;
      }
      return half->pts[i - 1].x;
    }
  }
  return 0.f;
}

int ltr_int_curve_format_half(const ltr_curve_half *half, char *buf, int buflen)
{
  int used = 0;
  int i;
  if(half == NULL || buf == NULL || buflen < 4 || half->count < 2){
    return -1;
  }
  buf[0] = '\0';
  for(i = 0; i < half->count; ++i){
    int wrote = snprintf(buf + used, (size_t)(buflen - used),
                         "%s%.4f,%.4f", (i == 0) ? "" : ";",
                         half->pts[i].x, half->pts[i].y);
    if(wrote < 0 || wrote >= buflen - used){
      buf[0] = '\0';
      return -1;
    }
    used += wrote;
  }
  return used;
}

int ltr_int_curve_parse_points(const char *str, ltr_curve_pt *pts, int max_pts)
{
  const char *p;
  int n = 0;
  if(str == NULL || pts == NULL || max_pts < 2){
    return 0;
  }
  p = str;
  while(*p != '\0' && n < max_pts){
    char *end = NULL;
    float x, y;
    while(*p == ' ' || *p == '\t'){
      ++p;
    }
    if(*p == '\0'){
      break;
    }
    x = strtof(p, &end);
    if(end == p || *end != ','){
      return 0;
    }
    p = end + 1;
    y = strtof(p, &end);
    if(end == p){
      return 0;
    }
    pts[n].x = x;
    pts[n].y = y;
    ++n;
    p = end;
    if(*p == ';'){
      ++p;
    }else if(*p != '\0'){
      return 0;
    }
  }
  return (n >= 2) ? n : 0;
}

int ltr_int_curve_format_both(const splines *curves, char *buf, int buflen)
{
  char left[LTR_CURVE_HALF_TEXT];
  char right[LTR_CURVE_HALF_TEXT];
  int wrote;
  if(curves == NULL || buf == NULL || buflen < 8){
    return -1;
  }
  if(ltr_int_curve_format_half(&curves->left, left, sizeof(left)) < 0){
    return -1;
  }
  if(ltr_int_curve_format_half(&curves->right, right, sizeof(right)) < 0){
    return -1;
  }
  wrote = snprintf(buf, (size_t)buflen, "L=%s|R=%s", left, right);
  if(wrote < 0 || wrote >= buflen){
    if(buflen > 0){
      buf[0] = '\0';
    }
    return -1;
  }
  return wrote;
}

int ltr_int_curve_parse_both_points(const char *str,
                                    ltr_curve_pt *left, int *nleft,
                                    ltr_curve_pt *right, int *nright,
                                    int max_pts)
{
  const char *bar;
  const char *left_str;
  const char *right_str;
  char left_buf[LTR_CURVE_HALF_TEXT];
  int nl, nr;
  size_t left_len;

  if(str == NULL || left == NULL || right == NULL || nleft == NULL || nright == NULL){
    return 0;
  }
  if(strncmp(str, "L=", 2) == 0 && (bar = strchr(str, '|')) != NULL && strncmp(bar, "|R=", 3) == 0){
    left_str = str + 2;
    left_len = (size_t)(bar - left_str);
    if(left_len >= sizeof(left_buf)){
      return 0;
    }
    memcpy(left_buf, left_str, left_len);
    left_buf[left_len] = '\0';
    right_str = bar + 3;
    nl = ltr_int_curve_parse_points(left_buf, left, max_pts);
    nr = ltr_int_curve_parse_points(right_str, right, max_pts);
  }else{
    nl = ltr_int_curve_parse_points(str, left, max_pts);
    nr = ltr_int_curve_parse_points(str, right, max_pts);
  }
  if(nl < 2 || nr < 2){
    return 0;
  }
  *nleft = nl;
  *nright = nr;
  return 1;
}

float ltr_int_spline_point(const splines *curves, float x)
{
  const ltr_curve_half *half;
  float sign = 1.f;
  float q, frac, y;
  int i;

  if(curves == NULL){
    return 0.f;
  }
  if(x < 0.f){
    half = &curves->left;
    sign = -1.f;
    x = -x;
  }else{
    half = &curves->right;
  }
  if(half->count < 2){
    return sign * clampf(x, 0.f, 1.f);
  }
  if(x > 1.f){
    x = 1.f;
  }
  q = x * (float)(LTR_CURVE_BUCKETS - 1);
  i = (int)q;
  if(i >= LTR_CURVE_BUCKETS - 1){
    return sign * half->table[LTR_CURVE_BUCKETS - 1];
  }
  if(i < 0){
    i = 0;
  }
  frac = q - (float)i;
  y = half->table[i] * (1.f - frac) + half->table[i + 1] * frac;
  return sign * clampf(y, 0.f, 1.f);
}

#ifdef LTR_SPLINE_TEST
static int g_fails = 0;

static void expect_near(const char *name, float got, float want, float tol)
{
  if(fabsf(got - want) > tol){
    fprintf(stderr, "FAIL %s: got %g want %g\n", name, got, want);
    ++g_fails;
  }
}

int main(void)
{
  splines curves;
  ltr_curve_pt pts[4];
  char text[LTR_CURVE_TEXT];
  ltr_curve_pt left[LTR_CURVE_MAX_POINTS];
  ltr_curve_pt right[LTR_CURVE_MAX_POINTS];
  int nl = 0, nr = 0;
  int i;

  ltr_int_curve_set_default(&curves.left);
  ltr_int_curve_set_default(&curves.right);
  expect_near("linear 0", ltr_int_spline_point(&curves, 0.f), 0.f, 1e-4f);
  expect_near("linear 0.5", ltr_int_spline_point(&curves, 0.5f), 0.5f, 0.01f);
  expect_near("linear 1", ltr_int_spline_point(&curves, 1.f), 1.f, 1e-3f);
  expect_near("linear -0.25", ltr_int_spline_point(&curves, -0.25f), -0.25f, 0.01f);
  expect_near("prefix linear", ltr_int_curve_flat_prefix(&curves.right), 0.f, 1e-6f);

  pts[0].x = 0.f; pts[0].y = 0.f;
  pts[1].x = 0.20f; pts[1].y = 0.f;
  pts[2].x = 1.f; pts[2].y = 1.f;
  ltr_int_curve_set_points(&curves.right, pts, 3);
  expect_near("dead 0.10", ltr_int_spline_point(&curves, 0.10f), 0.f, 1e-4f);
  expect_near("dead prefix", ltr_int_curve_flat_prefix(&curves.right), 0.20f, 1e-4f);
  if(!(ltr_int_spline_point(&curves, 0.60f) > ltr_int_spline_point(&curves, 0.30f))){
    fprintf(stderr, "FAIL deadzone should rise after the flat prefix\n");
    ++g_fails;
  }
  expect_near("dead end", ltr_int_spline_point(&curves, 1.f), 1.f, 0.02f);

  pts[0].x = 0.f; pts[0].y = 0.f;
  pts[1].x = 0.30f; pts[1].y = 0.90f;
  pts[2].x = 1.f; pts[2].y = 1.f;
  ltr_int_curve_set_points(&curves.right, pts, 3);
  expect_near("through point", ltr_int_spline_point(&curves, 0.30f), 0.90f, 0.05f);
  if(ltr_int_spline_point(&curves, 0.30f) > 1.f || ltr_int_spline_point(&curves, 0.15f) < 0.f){
    fprintf(stderr, "FAIL curve left the 0..1 range\n");
    ++g_fails;
  }

  if(ltr_int_curve_format_both(&curves, text, sizeof(text)) < 0){
    fprintf(stderr, "FAIL format\n");
    ++g_fails;
  }else if(!ltr_int_curve_parse_both_points(text, left, &nl, right, &nr, LTR_CURVE_MAX_POINTS)){
    fprintf(stderr, "FAIL parse '%s'\n", text);
    ++g_fails;
  }else if(nr < 3 || fabsf(right[1].x - 0.30f) > 1e-3f || fabsf(right[1].y - 0.90f) > 1e-3f){
    fprintf(stderr, "FAIL round trip count %d\n", nr);
    ++g_fails;
  }

  expect_near("left still linear", ltr_int_spline_point(&curves, -0.5f), -0.5f, 0.01f);

  for(i = 0; i < curves.right.count; ++i){
    if(curves.right.pts[i].y < 0.f || curves.right.pts[i].y > 1.f){
      fprintf(stderr, "FAIL point y out of range\n");
      ++g_fails;
    }
  }

  if(g_fails != 0){
    fprintf(stderr, "%d failure(s)\n", g_fails);
    return 1;
  }
  printf("spline tests ok\n");
  return 0;
}
#endif
