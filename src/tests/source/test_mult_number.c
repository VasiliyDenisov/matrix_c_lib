#include "../proj_test.h"

START_TEST(mult_number_test_1) {
  matrix_t A, R, R2;
  proj_create_matrix(1, 1, &A);
  proj_create_matrix(1, 1, &R2);

  A.matrix[0][0] = 1.25;
  R2.matrix[0][0] = -5;
  proj_mult_number(&A, -4, &R);

  ck_assert(fabs(R.matrix[0][0] - R2.matrix[0][0]) < 1e-7);
  proj_remove_matrix(&A);
  proj_remove_matrix(&R);
  proj_remove_matrix(&R2);
}
END_TEST

START_TEST(mult_number_test_2) {
  matrix_t A, R, R2;
  proj_create_matrix(3, 3, &A);
  proj_create_matrix(3, 3, &R2);

  A.matrix[0][0] = 0.25;
  A.matrix[0][1] = 1.25;
  A.matrix[0][2] = 2.25;
  A.matrix[1][0] = 3.25;
  A.matrix[1][1] = 4.25;
  A.matrix[1][2] = 5.25;
  A.matrix[2][0] = 6.25;
  A.matrix[2][1] = 7.25;
  A.matrix[2][2] = 8.25;

  R2.matrix[0][0] = 1;
  R2.matrix[0][1] = 5;
  R2.matrix[0][2] = 9;
  R2.matrix[1][0] = 13;
  R2.matrix[1][1] = 17;
  R2.matrix[1][2] = 21;
  R2.matrix[2][0] = 25;
  R2.matrix[2][1] = 29;
  R2.matrix[2][2] = 33;

  proj_mult_number(&A, 4, &R);

  ck_assert_int_eq(proj_eq_matrix(&R, &R2), SUCCESS);
  proj_remove_matrix(&A);
  proj_remove_matrix(&R);
  proj_remove_matrix(&R2);
}
END_TEST

START_TEST(mult_number_test_3) {
  matrix_t A, R, R2;
  proj_create_matrix(2, 3, &A);
  proj_create_matrix(2, 3, &R2);

  A.matrix[0][0] = 0.25;
  A.matrix[0][1] = 1.25;
  A.matrix[0][2] = 2.25;
  A.matrix[1][0] = 3.25;
  A.matrix[1][1] = 4.25;
  A.matrix[1][2] = 5.25;

  R2.matrix[0][0] = 1;
  R2.matrix[0][1] = 5;
  R2.matrix[0][2] = 9;
  R2.matrix[1][0] = 13;
  R2.matrix[1][1] = 17;
  R2.matrix[1][2] = 21;

  proj_mult_number(&A, 4, &R);
  ck_assert_int_eq(proj_eq_matrix(&R, &R2), SUCCESS);
  proj_remove_matrix(&A);
  proj_remove_matrix(&R);
  proj_remove_matrix(&R2);
}
END_TEST

START_TEST(null_mult_num) {
  matrix_t *B = NULL;
  double num = 0;
  matrix_t *R = NULL;
  int res = proj_mult_number(B, num, R);
  ck_assert_int_eq(res, INCORRECT_MATRIX);
}
END_TEST

Suite *test_mult_number() {
  Suite *s = suite_create("mult_number");
  TCase *tc = tcase_create("mult_number_tcase");

  tcase_add_test(tc, mult_number_test_1);
  tcase_add_test(tc, mult_number_test_2);
  tcase_add_test(tc, mult_number_test_3);
  tcase_add_test(tc, null_mult_num);

  suite_add_tcase(s, tc);
  return s;
}
