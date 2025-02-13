#include <check.h>
#include <math.h>
#include <stdlib.h>
#define SUCCESS 0
#define FAILURE 1
#define FAILURE2 2
#include "s21_matrix.h"

START_TEST(test_s21_create_matrix_valid) {
  matrix_t A;
  int status = s21_create_matrix(3, 3, &A);
  ck_assert_int_eq(status, SUCCESS);
  ck_assert_ptr_nonnull(A.matrix);
  s21_remove_matrix(&A);
}
END_TEST

START_TEST(test_s21_create_matrix_invalid) {
  matrix_t A;
  int status = s21_create_matrix(-1, 3, &A);
  ck_assert_int_eq(status, FAILURE);
  status = s21_create_matrix(3, -3, &A);
  ck_assert_int_eq(status, FAILURE);
}
END_TEST

START_TEST(test_create_matrix_valid) {
  matrix_t result;
  int res = s21_create_matrix(3, 3, &result);

  ck_assert_int_eq(res, 0);
  ck_assert_int_eq(result.rows, 3);
  ck_assert_int_eq(result.columns, 3);

  ck_assert_ptr_nonnull(result.matrix);
  for (int i = 0; i < result.rows; i++) {
    ck_assert_ptr_nonnull(result.matrix[i]);
  }

  s21_remove_matrix(&result);
}
END_TEST

START_TEST(test_s21_remove_matrix_valid) {
  matrix_t A;
  s21_create_matrix(3, 3, &A);
  s21_remove_matrix(&A);
  ck_assert_ptr_null(A.matrix);
}
END_TEST

START_TEST(test_s21_sum_matrix_valid) {
  matrix_t A, B, result;
  s21_create_matrix(2, 2, &A);
  s21_create_matrix(2, 2, &B);

  A.matrix[0][0] = 1.0;
  A.matrix[0][1] = 2.0;
  A.matrix[1][0] = 3.0;
  A.matrix[1][1] = 4.0;

  B.matrix[0][0] = 5.0;
  B.matrix[0][1] = 6.0;
  B.matrix[1][0] = 7.0;
  B.matrix[1][1] = 8.0;

  int status = s21_sum_matrix(&A, &B, &result);
  ck_assert_int_eq(status, SUCCESS);
  ck_assert_double_eq(result.matrix[0][0], 6.0);
  ck_assert_double_eq(result.matrix[0][1], 8.0);
  ck_assert_double_eq(result.matrix[1][0], 10.0);
  ck_assert_double_eq(result.matrix[1][1], 12.0);

  s21_remove_matrix(&A);
  s21_remove_matrix(&B);
  s21_remove_matrix(&result);
}
END_TEST

START_TEST(test_s21_sum_matrix_invalid) {
  matrix_t A, B, result;
  s21_create_matrix(2, 2, &A);
  s21_create_matrix(3, 3, &B);

  int status = s21_sum_matrix(&A, &B, &result);
  ck_assert_int_eq(status, FAILURE2);

  s21_remove_matrix(&A);
  s21_remove_matrix(&B);
}
END_TEST
START_TEST(test_sum_matrix_invalid_matrix_A) {
  matrix_t A, B, result;
  s21_create_matrix(2, 2, &B);

  A.matrix = NULL;

  int status = s21_sum_matrix(&A, &B, &result);

  ck_assert_int_eq(status, FAILURE);

  s21_remove_matrix(&B);
}
END_TEST

START_TEST(test_sum_matrix_invalid_matrix_B) {
  matrix_t A, B, result;
  s21_create_matrix(2, 2, &A);

  B.matrix = NULL;

  int status = s21_sum_matrix(&A, &B, &result);

  ck_assert_int_eq(status, FAILURE);

  s21_remove_matrix(&A);
}
END_TEST

START_TEST(test_sum_matrix_invalid_size) {
  matrix_t A, B, result;
  s21_create_matrix(2, 2, &A);
  s21_create_matrix(3, 3, &B);

  int status = s21_sum_matrix(&A, &B, &result);

  ck_assert_int_eq(status, FAILURE2);

  s21_remove_matrix(&A);
  s21_remove_matrix(&B);
}
END_TEST

START_TEST(test_sum_matrix_all_valid) {
  matrix_t A, B, result;
  s21_create_matrix(2, 2, &A);
  s21_create_matrix(2, 2, &B);

  A.matrix[0][0] = 1.0;
  A.matrix[0][1] = 2.0;
  A.matrix[1][0] = 3.0;
  A.matrix[1][1] = 4.0;

  B.matrix[0][0] = 5.0;
  B.matrix[0][1] = 6.0;
  B.matrix[1][0] = 7.0;
  B.matrix[1][1] = 8.0;

  int status = s21_sum_matrix(&A, &B, &result);

  ck_assert_int_eq(status, SUCCESS);

  ck_assert_double_eq(result.matrix[0][0], 6.0);
  ck_assert_double_eq(result.matrix[0][1], 8.0);
  ck_assert_double_eq(result.matrix[1][0], 10.0);
  ck_assert_double_eq(result.matrix[1][1], 12.0);

  s21_remove_matrix(&A);
  s21_remove_matrix(&B);
  s21_remove_matrix(&result);
}
END_TEST

START_TEST(test_sum_matrix_infinite_values) {
  matrix_t A, B, result;
  s21_create_matrix(2, 2, &A);
  s21_create_matrix(2, 2, &B);

  A.matrix[0][0] = INFINITY;
  A.matrix[0][1] = 2.0;
  A.matrix[1][0] = 3.0;
  A.matrix[1][1] = 4.0;

  B.matrix[0][0] = 5.0;
  B.matrix[0][1] = 6.0;
  B.matrix[1][0] = 7.0;
  B.matrix[1][1] = 8.0;

  int status = s21_sum_matrix(&A, &B, &result);

  ck_assert_int_eq(status, FAILURE);

  s21_remove_matrix(&A);
  s21_remove_matrix(&B);
}
END_TEST

START_TEST(test_sum_matrix_null_pointer_result) {
  matrix_t A, B;
  s21_create_matrix(2, 2, &A);
  s21_create_matrix(2, 2, &B);

  int status = s21_sum_matrix(&A, &B, NULL);

  ck_assert_int_eq(status, FAILURE);

  s21_remove_matrix(&A);
  s21_remove_matrix(&B);
}
END_TEST

START_TEST(test_s21_sub_matrix_valid) {
  matrix_t A, B, result;
  s21_create_matrix(2, 2, &A);
  s21_create_matrix(2, 2, &B);

  A.matrix[0][0] = 5.0;
  A.matrix[0][1] = 6.0;
  A.matrix[1][0] = 7.0;
  A.matrix[1][1] = 8.0;

  B.matrix[0][0] = 1.0;
  B.matrix[0][1] = 2.0;
  B.matrix[1][0] = 3.0;
  B.matrix[1][1] = 4.0;

  int status = s21_sub_matrix(&A, &B, &result);
  ck_assert_int_eq(status, SUCCESS);
  ck_assert_double_eq(result.matrix[0][0], 4.0);
  ck_assert_double_eq(result.matrix[0][1], 4.0);
  ck_assert_double_eq(result.matrix[1][0], 4.0);
  ck_assert_double_eq(result.matrix[1][1], 4.0);

  s21_remove_matrix(&A);
  s21_remove_matrix(&B);
  s21_remove_matrix(&result);
}
END_TEST

START_TEST(test_sub_matrix_valid) {
  matrix_t A, B, result;
  s21_create_matrix(2, 2, &A);
  s21_create_matrix(2, 2, &B);

  A.matrix[0][0] = 5;
  A.matrix[0][1] = 3;
  A.matrix[1][0] = 7;
  A.matrix[1][1] = 2;

  B.matrix[0][0] = 2;
  B.matrix[0][1] = 1;
  B.matrix[1][0] = 4;
  B.matrix[1][1] = 1;

  int res = s21_sub_matrix(&A, &B, &result);

  ck_assert_int_eq(res, SUCCESS);
  ck_assert(fabs(result.matrix[0][0] - 3) < 1e-7);
  ck_assert(fabs(result.matrix[0][1] - 2) < 1e-7);
  ck_assert(fabs(result.matrix[1][0] - 3) < 1e-7);
  ck_assert(fabs(result.matrix[1][1] - 1) < 1e-7);

  s21_remove_matrix(&A);
  s21_remove_matrix(&B);
  s21_remove_matrix(&result);
}
END_TEST
START_TEST(test_sub_matrix_infinite_result) {
  matrix_t A, B, result;
  s21_create_matrix(2, 2, &A);
  s21_create_matrix(2, 2, &B);

  A.matrix[0][0] = 1;
  A.matrix[0][1] = 2;
  A.matrix[1][0] = 3;
  A.matrix[1][1] = 4;

  B.matrix[0][0] = 0;
  B.matrix[0][1] = 0;
  B.matrix[1][0] = 0;
  B.matrix[1][1] = 0;

  int res = s21_sub_matrix(&A, &B, &result);

  ck_assert_int_eq(res, SUCCESS);
  ck_assert(fabs(result.matrix[0][0] - 1) < 1e-7);
  ck_assert(fabs(result.matrix[0][1] - 2) < 1e-7);
  ck_assert(fabs(result.matrix[1][0] - 3) < 1e-7);
  ck_assert(fabs(result.matrix[1][1] - 4) < 1e-7);

  s21_remove_matrix(&A);
  s21_remove_matrix(&B);
  s21_remove_matrix(&result);
}
END_TEST

START_TEST(test_sub_matrix_invalid_size) {
  matrix_t A, B, result;
  s21_create_matrix(2, 2, &A);
  s21_create_matrix(3, 3, &B);

  int res = s21_sub_matrix(&A, &B, &result);

  ck_assert_int_eq(res, FAILURE2);

  s21_remove_matrix(&A);
  s21_remove_matrix(&B);
}
END_TEST

START_TEST(test_sub_matrix_invalid_matrix) {
  matrix_t A, B, result;
  s21_create_matrix(2, 2, &A);
  s21_create_matrix(2, 2, &B);

  A.matrix[0][0] = NAN;
  A.matrix[0][1] = 0;
  A.matrix[1][0] = 3;
  A.matrix[1][1] = 4;

  B.matrix[0][0] = 1;
  B.matrix[0][1] = 2;
  B.matrix[1][0] = 1;
  B.matrix[1][1] = 1;

  int res = s21_sub_matrix(&A, &B, &result);

  ck_assert_int_eq(res, FAILURE);

  s21_remove_matrix(&A);
  s21_remove_matrix(&B);
  s21_remove_matrix(&result);
}
END_TEST

START_TEST(test_s21_mult_number_valid) {
  matrix_t A, result;
  s21_create_matrix(2, 2, &A);

  A.matrix[0][0] = 1.0;
  A.matrix[0][1] = 2.0;
  A.matrix[1][0] = 3.0;
  A.matrix[1][1] = 4.0;

  int status = s21_mult_number(&A, 2.0, &result);
  ck_assert_int_eq(status, SUCCESS);
  ck_assert_double_eq(result.matrix[0][0], 2.0);
  ck_assert_double_eq(result.matrix[0][1], 4.0);
  ck_assert_double_eq(result.matrix[1][0], 6.0);
  ck_assert_double_eq(result.matrix[1][1], 8.0);

  s21_remove_matrix(&A);
  s21_remove_matrix(&result);
}
END_TEST

START_TEST(test_s21_transpose_valid) {
  matrix_t A, result;
  s21_create_matrix(2, 3, &A);

  A.matrix[0][0] = 1.0;
  A.matrix[0][1] = 2.0;
  A.matrix[0][2] = 3.0;
  A.matrix[1][0] = 4.0;
  A.matrix[1][1] = 5.0;
  A.matrix[1][2] = 6.0;

  int status = s21_transpose(&A, &result);
  ck_assert_int_eq(status, SUCCESS);
  ck_assert_double_eq(result.matrix[0][0], 1.0);
  ck_assert_double_eq(result.matrix[1][0], 2.0);
  ck_assert_double_eq(result.matrix[2][0], 3.0);
  ck_assert_double_eq(result.matrix[0][1], 4.0);
  ck_assert_double_eq(result.matrix[1][1], 5.0);
  ck_assert_double_eq(result.matrix[2][1], 6.0);

  s21_remove_matrix(&A);
  s21_remove_matrix(&result);
}
END_TEST

START_TEST(test_s21_transpose_invalid) {
  matrix_t A, result;
  s21_create_matrix(-1, 3, &A);
  int status = s21_transpose(&A, &result);
  ck_assert_int_eq(status, FAILURE);
}
END_TEST

START_TEST(test_s21_eq_matrix_valid_equal) {
  matrix_t A, B;
  s21_create_matrix(2, 2, &A);
  s21_create_matrix(2, 2, &B);

  A.matrix[0][0] = 1.0;
  A.matrix[0][1] = 2.0;
  A.matrix[1][0] = 3.0;
  A.matrix[1][1] = 4.0;

  B.matrix[0][0] = 1.0;
  B.matrix[0][1] = 2.0;
  B.matrix[1][0] = 3.0;
  B.matrix[1][1] = 4.0;

  int status = s21_eq_matrix(&A, &B);
  ck_assert_int_eq(status, 1);

  s21_remove_matrix(&A);
  s21_remove_matrix(&B);
}
END_TEST

START_TEST(test_s21_eq_matrix_valid_not_equal) {
  matrix_t A, B;
  s21_create_matrix(2, 2, &A);
  s21_create_matrix(2, 2, &B);

  A.matrix[0][0] = 1.0;
  A.matrix[0][1] = 2.0;
  A.matrix[1][0] = 3.0;
  A.matrix[1][1] = 4.0;

  B.matrix[0][0] = 5.0;
  B.matrix[0][1] = 6.0;
  B.matrix[1][0] = 7.0;
  B.matrix[1][1] = 8.0;

  int status = s21_eq_matrix(&A, &B);
  ck_assert_int_eq(status, 0);

  s21_remove_matrix(&A);
  s21_remove_matrix(&B);
}
END_TEST

START_TEST(test_s21_eq_matrix_invalid_size) {
  matrix_t A, B;
  s21_create_matrix(2, 2, &A);
  s21_create_matrix(3, 3, &B);

  int status = s21_eq_matrix(&A, &B);
  ck_assert_int_eq(status, 0);

  s21_remove_matrix(&A);
  s21_remove_matrix(&B);
}
END_TEST

START_TEST(test_s21_mult_matrix_valid) {
  matrix_t A, B, result;
  s21_create_matrix(2, 3, &A);
  s21_create_matrix(3, 2, &B);

  A.matrix[0][0] = 1.0;
  A.matrix[0][1] = 2.0;
  A.matrix[0][2] = 3.0;
  A.matrix[1][0] = 4.0;
  A.matrix[1][1] = 5.0;
  A.matrix[1][2] = 6.0;

  B.matrix[0][0] = 7.0;
  B.matrix[0][1] = 8.0;
  B.matrix[1][0] = 9.0;
  B.matrix[1][1] = 10.0;
  B.matrix[2][0] = 11.0;
  B.matrix[2][1] = 12.0;

  int status = s21_mult_matrix(&A, &B, &result);
  ck_assert_int_eq(status, SUCCESS);

  ck_assert_double_eq_tol(result.matrix[0][0], 58.0, 1e-6);
  ck_assert_double_eq_tol(result.matrix[0][1], 64.0, 1e-6);
  ck_assert_double_eq_tol(result.matrix[1][0], 139.0, 1e-6);
  ck_assert_double_eq_tol(result.matrix[1][1], 154.0, 1e-6);

  s21_remove_matrix(&A);
  s21_remove_matrix(&B);
  s21_remove_matrix(&result);
}
END_TEST

START_TEST(test_s21_mult_matrix_invalid_size) {
  matrix_t A, B, result;
  s21_create_matrix(2, 3, &A);
  s21_create_matrix(4, 2, &B);

  int status = s21_mult_matrix(&A, &B, &result);
  ck_assert_int_eq(status, FAILURE2);

  s21_remove_matrix(&A);
  s21_remove_matrix(&B);
}
END_TEST

START_TEST(test_s21_mult_matrix_invalid_matrix) {
  matrix_t A, B, result;

  A.rows = -1;
  A.columns = 3;
  A.matrix = NULL;

  s21_create_matrix(3, 2, &B);

  int status = s21_mult_matrix(&A, &B, &result);
  ck_assert_int_eq(status, FAILURE);

  s21_remove_matrix(&B);
}
END_TEST

START_TEST(test_s21_mult_matrix_result_nan) {
  matrix_t A, B, result;
  s21_create_matrix(2, 2, &A);
  s21_create_matrix(2, 2, &B);

  A.matrix[0][0] = INFINITY;
  A.matrix[0][1] = 2.0;
  A.matrix[1][0] = 3.0;
  A.matrix[1][1] = 4.0;

  B.matrix[0][0] = 1.0;
  B.matrix[0][1] = 2.0;
  B.matrix[1][0] = 3.0;
  B.matrix[1][1] = 4.0;

  int status = s21_mult_matrix(&A, &B, &result);
  ck_assert_int_eq(status, FAILURE);

  s21_remove_matrix(&A);
  s21_remove_matrix(&B);
}
END_TEST

START_TEST(test_s21_calc_complements_non_square) {
  matrix_t A, result;

  s21_create_matrix(2, 3, &A);

  int status = s21_calc_complements(&A, &result);

  ck_assert_int_eq(status, FAILURE2);

  s21_remove_matrix(&A);
}
END_TEST

START_TEST(test_s21_calc_complements_invalid_matrix) {
  matrix_t A, result;

  A.rows = -1;
  A.columns = -1;
  A.matrix = NULL;

  int status = s21_calc_complements(&A, &result);

  ck_assert_int_eq(status, FAILURE);
}
END_TEST
START_TEST(test_s21_calc_complements_1x1) {
  matrix_t A, result;
  s21_create_matrix(1, 1, &A);
  A.matrix[0][0] = 5.0;

  int status = s21_calc_complements(&A, &result);
  ck_assert_int_eq(status, SUCCESS);
  ck_assert_ldouble_eq(result.matrix[0][0], 5.0);

  s21_remove_matrix(&A);
  s21_remove_matrix(&result);
}
END_TEST

START_TEST(test_s21_calc_complements_2x2) {
  matrix_t A, result;

  s21_create_matrix(2, 2, &A);

  A.matrix[0][0] = 3;
  A.matrix[0][1] = 8;
  A.matrix[1][0] = 4;
  A.matrix[1][1] = 6;

  int status = s21_calc_complements(&A, &result);

  ck_assert_int_eq(status, SUCCESS);

  ck_assert_double_eq_tol(result.matrix[0][0], 6.0, 1e-6);
  ck_assert_double_eq_tol(result.matrix[0][1], -4.0, 1e-6);
  ck_assert_double_eq_tol(result.matrix[1][0], -8.0, 1e-6);
  ck_assert_double_eq_tol(result.matrix[1][1], 3.0, 1e-6);

  s21_remove_matrix(&A);
  s21_remove_matrix(&result);
}
END_TEST

START_TEST(test_determinant_single_element) {
  matrix_t A;
  s21_create_matrix(1, 1, &A);
  A.matrix[0][0] = 5.0;

  double result = 0.0;
  int status = s21_determinant(&A, &result);

  ck_assert_int_eq(status, SUCCESS);
  ck_assert_double_eq(result, 5.0);

  s21_remove_matrix(&A);
}
END_TEST

START_TEST(test_determinant_2x2) {
  matrix_t A;
  s21_create_matrix(2, 2, &A);

  A.matrix[0][0] = 1.0;
  A.matrix[0][1] = 2.0;
  A.matrix[1][0] = 3.0;
  A.matrix[1][1] = 4.0;

  double result = 0.0;
  int status = s21_determinant(&A, &result);

  ck_assert_int_eq(status, SUCCESS);
  ck_assert_double_eq_tol(result, -2.0, 1e-7);

  s21_remove_matrix(&A);
}
END_TEST

START_TEST(test_determinant_invalid_matrix) {
  matrix_t A;
  A.rows = 0;
  A.columns = 0;
  A.matrix = NULL;

  double result = 0.0;
  int status = s21_determinant(&A, &result);

  ck_assert_int_eq(status, FAILURE);
}
END_TEST

START_TEST(test_determinant_non_square_matrix) {
  matrix_t A;
  s21_create_matrix(2, 3, &A);

  double result = 0.0;
  int status = s21_determinant(&A, &result);

  ck_assert_int_eq(status, FAILURE2);

  s21_remove_matrix(&A);
}
END_TEST

START_TEST(test_determinant_large_matrix) {
  matrix_t A;
  s21_create_matrix(4, 4, &A);

  double values[4][4] = {{1.0, 2.0, 3.0, 4.0},
                         {5.0, 6.0, 7.0, 8.0},
                         {9.0, 10.0, 11.0, 12.0},
                         {13.0, 14.0, 15.0, 16.0}};

  for (int i = 0; i < 4; i++) {
    for (int j = 0; j < 4; j++) {
      A.matrix[i][j] = values[i][j];
    }
  }

  double result = 0.0;
  int status = s21_determinant(&A, &result);

  ck_assert_int_eq(status, SUCCESS);
  ck_assert_double_eq_tol(result, 0.0, 1e-7);

  s21_remove_matrix(&A);
}
END_TEST

START_TEST(test_inverse_matrix_valid_2x2) {
  matrix_t A, result;
  s21_create_matrix(2, 2, &A);

  A.matrix[0][0] = 4;
  A.matrix[0][1] = 7;
  A.matrix[1][0] = 2;
  A.matrix[1][1] = 6;

  int status = s21_inverse_matrix(&A, &result);

  ck_assert_int_eq(status, SUCCESS);
  ck_assert(fabs(result.matrix[0][0] - 0.6) < 1e-7);
  ck_assert(fabs(result.matrix[0][1] - -0.7) < 1e-7);
  ck_assert(fabs(result.matrix[1][0] - -0.2) < 1e-7);
  ck_assert(fabs(result.matrix[1][1] - 0.4) < 1e-7);

  s21_remove_matrix(&A);
  s21_remove_matrix(&result);
}
END_TEST

START_TEST(test_inverse_matrix_valid) {
  matrix_t A, result;
  s21_create_matrix(2, 2, &A);

  A.matrix[0][0] = 4;
  A.matrix[0][1] = 7;
  A.matrix[1][0] = 2;
  A.matrix[1][1] = 6;

  int res = s21_inverse_matrix(&A, &result);

  ck_assert_int_eq(res, SUCCESS);
  ck_assert(fabs(result.matrix[0][0] - 0.6) < 1e-7);
  ck_assert(fabs(result.matrix[0][1] + 0.7) < 1e-7);
  ck_assert(fabs(result.matrix[1][0] + 0.2) < 1e-7);
  ck_assert(fabs(result.matrix[1][1] - 0.4) < 1e-7);

  s21_remove_matrix(&A);
  s21_remove_matrix(&result);
}
END_TEST

START_TEST(test_inverse_matrix_valid_3x3) {
  matrix_t A, result;
  s21_create_matrix(3, 3, &A);

  A.matrix[0][0] = 3;
  A.matrix[0][1] = 0;
  A.matrix[0][2] = 2;
  A.matrix[1][0] = 2;
  A.matrix[1][1] = 0;
  A.matrix[1][2] = -2;
  A.matrix[2][0] = 0;
  A.matrix[2][1] = 1;
  A.matrix[2][2] = 1;

  int status = s21_inverse_matrix(&A, &result);

  ck_assert_int_eq(status, SUCCESS);
  ck_assert(fabs(result.matrix[0][0] - 0.2) < 1e-7);
  ck_assert(fabs(result.matrix[0][1] - 0.2) < 1e-7);
  ck_assert(fabs(result.matrix[0][2] - 0.0) < 1e-7);
  ck_assert(fabs(result.matrix[1][0] - -0.2) < 1e-7);
  ck_assert(fabs(result.matrix[1][1] - 0.3) < 1e-7);
  ck_assert(fabs(result.matrix[1][2] - 1.0) < 1e-7);
  ck_assert(fabs(result.matrix[2][0] - 0.2) < 1e-7);
  ck_assert(fabs(result.matrix[2][1] - -0.3) < 1e-7);
  ck_assert(fabs(result.matrix[2][2] - 0.0) < 1e-7);

  s21_remove_matrix(&A);
  s21_remove_matrix(&result);
}
END_TEST

Suite *matrix_suite(void) {
  Suite *s;
  TCase *tc_core;

  s = suite_create("Matrix");

  tc_core = tcase_create("Core");

  tcase_add_test(tc_core, test_inverse_matrix_valid_2x2);
  tcase_add_test(tc_core, test_inverse_matrix_valid_3x3);
  tcase_add_test(tc_core, test_inverse_matrix_valid);

  tcase_add_test(tc_core, test_determinant_single_element);
  tcase_add_test(tc_core, test_determinant_2x2);
  tcase_add_test(tc_core, test_determinant_invalid_matrix);
  tcase_add_test(tc_core, test_determinant_non_square_matrix);
  tcase_add_test(tc_core, test_determinant_large_matrix);

  tcase_add_test(tc_core, test_s21_calc_complements_non_square);
  tcase_add_test(tc_core, test_s21_calc_complements_invalid_matrix);
  tcase_add_test(tc_core, test_s21_calc_complements_1x1);
  tcase_add_test(tc_core, test_s21_calc_complements_2x2);

  tcase_add_test(tc_core, test_s21_mult_matrix_valid);
  tcase_add_test(tc_core, test_s21_mult_matrix_invalid_matrix);
  tcase_add_test(tc_core, test_s21_mult_matrix_invalid_size);
  tcase_add_test(tc_core, test_s21_mult_matrix_result_nan);

  tcase_add_test(tc_core, test_s21_create_matrix_valid);
  tcase_add_test(tc_core, test_s21_create_matrix_invalid);
  tcase_add_test(tc_core, test_create_matrix_valid);

  tcase_add_test(tc_core, test_s21_remove_matrix_valid);

  tcase_add_test(tc_core, test_s21_sum_matrix_valid);
  tcase_add_test(tc_core, test_s21_sum_matrix_invalid);
  tcase_add_test(tc_core, test_sum_matrix_null_pointer_result);
  tcase_add_test(tc_core, test_sum_matrix_infinite_values);
  tcase_add_test(tc_core, test_sum_matrix_all_valid);
  tcase_add_test(tc_core, test_sum_matrix_invalid_size);
  tcase_add_test(tc_core, test_sum_matrix_invalid_matrix_B);
  tcase_add_test(tc_core, test_sum_matrix_invalid_matrix_A);

  tcase_add_test(tc_core, test_sub_matrix_valid);
  tcase_add_test(tc_core, test_sub_matrix_invalid_matrix);
  tcase_add_test(tc_core, test_sub_matrix_invalid_size);
  tcase_add_test(tc_core, test_sub_matrix_infinite_result);
  tcase_add_test(tc_core, test_s21_sub_matrix_valid);

  tcase_add_test(tc_core, test_s21_mult_number_valid);

  tcase_add_test(tc_core, test_s21_transpose_valid);
  tcase_add_test(tc_core, test_s21_transpose_invalid);

  tcase_add_test(tc_core, test_s21_eq_matrix_valid_equal);
  tcase_add_test(tc_core, test_s21_eq_matrix_valid_not_equal);
  tcase_add_test(tc_core, test_s21_eq_matrix_invalid_size);

  suite_add_tcase(s, tc_core);

  return s;
}

int main(void) {
  int no_failed = 0;
  Suite *s;
  SRunner *runner;

  s = matrix_suite();
  runner = srunner_create(s);

  srunner_run_all(runner, CK_NORMAL);
  no_failed = srunner_ntests_failed(runner);
  srunner_free(runner);

  return (no_failed == 0) ? EXIT_SUCCESS : EXIT_FAILURE;
}
