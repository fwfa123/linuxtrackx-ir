#include <assert.h>
#include <math.h>
#include <stdio.h>
#include <string.h>

#include "math_utils.h"

static bool near(double a, double b, double eps)
{
  return fabs(a - b) <= eps;
}

static bool vec_near(const double a[3], const double b[3], double eps)
{
  return near(a[0], b[0], eps) && near(a[1], b[1], eps) && near(a[2], b[2], eps);
}

static void test_align_disabled_passthrough(void)
{
  const double in[3] = {12.0, -4.0, 7.0};
  double out[3];
  assert(ltr_int_align_translation(30.0, 45.0, 10.0, in, out, false));
  assert(vec_near(in, out, 1e-9));
}

static void test_align_zero_rotation_passthrough(void)
{
  const double in[3] = {50.0, -20.0, 15.0};
  double out[3];
  assert(ltr_int_align_translation(0.0, 0.0, 0.0, in, out, true));
  assert(vec_near(in, out, 1e-9));
}

static void test_align_yaw_45_lateral_slide(void)
{
  const double in[3] = {50.0, 0.0, 0.0};
  double aligned[3];
  double transposed[3];
  double transform[3][3];

  assert(ltr_int_align_translation(0.0, 45.0, 0.0, in, aligned, true));

  ltr_int_euler_to_matrix(0.0, 45.0 / 180.0 * M_PI, 0.0, transform);
  ltr_int_transpose_in_place(transform);
  ltr_int_matrix_times_vec(transform, (double *)in, transposed);

  const double expected_x = 50.0 * cos(45.0 / 180.0 * M_PI);
  const double expected_z = -50.0 * sin(45.0 / 180.0 * M_PI);
  assert(near(aligned[0], expected_x, 1e-9));
  assert(near(aligned[1], 0.0, 1e-9));
  assert(near(aligned[2], expected_z, 1e-9));

  /* Old NPClient bug used R^T; ensure we no longer match that path. */
  assert(!vec_near(aligned, transposed, 1e-6));
  assert(!near(aligned[2], transposed[2], 1e-6));
}

int main(void)
{
  test_align_disabled_passthrough();
  test_align_zero_rotation_passthrough();
  test_align_yaw_45_lateral_slide();
  printf("test_align_translation passed\n");
  return 0;
}
