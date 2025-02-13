#include "s21_matrix.h"
#define SUCCESS 1
#define FAILURE 0
#define EPSILON 1e-7

int s21_create_matrix(int rows, int columns, matrix_t *result) {
  int status = 0;
  if (rows < 1 || columns < 1 || result == NULL) {
    status = 1;
  } else {
    result->rows = rows;
    result->columns = columns;

    result->matrix = malloc(rows * sizeof(double *));
    if (result->matrix == NULL) {
      status = 1;
    }

    for (int i = 0; i < rows && status == 0; i++) {
      result->matrix[i] = malloc(columns * sizeof(double));
      if (result->matrix[i] == NULL) {
        status = 1;
        for (int j = 0; j < i; j++) {
          free(result->matrix[j]);
        }
        free(result->matrix);
        result->matrix = NULL;
      }
    }
  }

  return status;
}

void s21_remove_matrix(matrix_t *A) {
  if (A != NULL && A->matrix != NULL) {
    for (int i = 0; i < A->rows; i++) {
      if (A->matrix[i] != NULL) {
        free(A->matrix[i]);
        A->matrix[i] = NULL;
      }
    }

    free(A->matrix);
    A->matrix = NULL;

    A->rows = 0;
    A->columns = 0;
  }
}

int s21_eq_matrix(matrix_t *A, matrix_t *B) {
  int a = SUCCESS;
  if (checkMatrix(A) == 0 && checkMatrix(B) == 0) {
    if (A->rows == B->rows && A->columns == B->columns) {
      for (int i = 0; i < A->rows && a == SUCCESS; i++) {
        for (int j = 0; j < A->columns; j++) {
          if (fabs(A->matrix[i][j] - B->matrix[i][j]) > EPSILON) {
            a = FAILURE;
            break;
          }
        }
      }
    } else
      a = FAILURE;
  } else
    a = FAILURE;
  return a;
}

int s21_sum_matrix(matrix_t *A, matrix_t *B, matrix_t *result) {
  int a = 0;
  if (checkMatrix(A) == 0 && checkMatrix(B) == 0) {
    if (A->rows == B->rows && A->columns == B->columns) {
      if (s21_create_matrix(A->rows, A->columns, result) == 0) {
        for (int i = 0; i < A->rows && a == 0; i++) {
          for (int j = 0; j < B->columns; j++) {
            result->matrix[i][j] = A->matrix[i][j] + B->matrix[i][j];
            if (!isfinite(result->matrix[i][j])) {
              s21_remove_matrix(result);
              a = 1;
              break;
            }
          }
        }
      } else
        a = 1;
    } else
      a = 2;
  } else
    a = 1;
  return a;
}

int s21_sub_matrix(matrix_t *A, matrix_t *B, matrix_t *result) {
  int a = 0;
  if (checkMatrix(A) == 0 && checkMatrix(B) == 0) {
    if (A->rows == B->rows && A->columns == B->columns) {
      if (s21_create_matrix(A->rows, A->columns, result) == 0) {
        for (int i = 0; i < A->rows && a == 0; i++) {
          for (int j = 0; j < B->columns; j++) {
            result->matrix[i][j] = A->matrix[i][j] - B->matrix[i][j];
            if (!isfinite(result->matrix[i][j])) {
              s21_remove_matrix(result);
              a = 1;
              break;
            }
          }
        }
      } else
        a = 1;
    } else
      a = 2;
  } else
    a = 1;
  return a;
}

int s21_mult_number(matrix_t *A, double number, matrix_t *result) {
  int a = 0;
  if (checkMatrix(A) == 0) {
    if (s21_create_matrix(A->rows, A->columns, result) == 0) {
      for (int i = 0; i < A->rows && a == 0; i++) {
        for (int j = 0; j < A->columns; j++) {
          result->matrix[i][j] = A->matrix[i][j] * number;
          if (!isfinite(result->matrix[i][j])) {
            s21_remove_matrix(result);
            a = 1;
            break;
          }
        }
      }
    } else
      a = 1;
  } else
    a = 1;
  return a;
}

int s21_mult_matrix(matrix_t *A, matrix_t *B, matrix_t *result) {
  int a = 0;
  if (checkMatrix(A) == 0 && checkMatrix(B) == 0) {
    if (A->columns == B->rows) {
      if (s21_create_matrix(A->rows, B->columns, result) == 0) {
        for (int i = 0; i < A->rows && a == 0; i++) {
          for (int j = 0; j < B->columns && a == 0; j++) {
            result->matrix[i][j] = 0;
            for (int k = 0; k < A->columns; k++) {
              result->matrix[i][j] += A->matrix[i][k] * B->matrix[k][j];
              if (!isfinite(result->matrix[i][j])) {
                s21_remove_matrix(result);
                a = 1;
                break;
              }
            }
          }
        }
      } else
        a = 1;
    } else
      a = 2;
  } else
    a = 1;
  return a;
}

int s21_transpose(matrix_t *A, matrix_t *result) {
  int a = 0;
  if (checkMatrix(A) == 0) {
    if (s21_create_matrix(A->columns, A->rows, result) == 0) {
      for (int i = 0; i < A->rows && a == 0; i++) {
        for (int j = 0; j < A->columns; j++) {
          result->matrix[j][i] = A->matrix[i][j];
          if (!isfinite(result->matrix[j][i])) {
            s21_remove_matrix(result);
            a = 1;
            break;
          }
        }
      }
    } else
      a = 1;
  } else
    a = 1;
  return a;
}

int s21_calc_complements(matrix_t *A, matrix_t *result) {
  int a = 0;
  if (checkMatrix(A) == 0) {
    if (A->rows == A->columns) {
      if (s21_create_matrix(A->rows, A->columns, result) == 0) {
        if (A->rows == 1) {
          result->matrix[0][0] = A->matrix[0][0];
        } else {
          matrix_t minor;
          for (int i = 0; i < A->rows; i++) {
            for (int j = 0; j < A->columns; j++) {
              s21_get_minor(A, i, j, &minor);
              double det = 0;
              s21_determinant(&minor, &det);
              result->matrix[i][j] = det * pow(-1, i + j);
              s21_remove_matrix(&minor);
            }
          }
        }
      } else
        a = 1;
    } else
      a = 2;
  } else
    a = 1;
  return a;
}

int s21_determinant(matrix_t *A, double *result) {
  int a = 0;
  if (checkMatrix(A) == 0) {
    if (A->rows == A->columns) {
      switch (A->rows) {
        case 1:
          *result = A->matrix[0][0];
          break;
        case 2:
          *result = A->matrix[0][0] * A->matrix[1][1] -
                    A->matrix[0][1] * A->matrix[1][0];
          if (!isfinite(*result)) {
            a = 1;
          }
          break;
        default:
          *result = 0;
          matrix_t minor;
          for (int j = 0; j < A->columns; j++) {
            s21_get_minor(A, 0, j, &minor);
            double det = 0;
            s21_determinant(&minor, &det);
            *result += A->matrix[0][j] * det * pow(-1, j);
            s21_remove_matrix(&minor);
          }
          break;
      }
    } else
      a = 2;
  } else
    a = 1;
  return a;
}

int s21_inverse_matrix(matrix_t *A, matrix_t *result) {
  int a = 0;
  if (checkMatrix(A) == 0) {
    if (A->rows == A->columns) {
      double det = 0;
      s21_determinant(A, &det);
      if (fabs(det) > EPSILON) {
        matrix_t compl, transp;
        s21_calc_complements(A, &compl );
        s21_transpose(&compl, &transp);
        s21_mult_number(&transp, 1.0 / det, result);

        s21_remove_matrix(&compl );
        s21_remove_matrix(&transp);
      } else
        a = 2;
    } else
      a = 2;
  } else
    a = 1;
  return a;
}

void s21_get_minor(matrix_t *A, int row, int col, matrix_t *result) {
  s21_create_matrix(A->rows - 1, A->columns - 1, result);
  for (int i = 0, r = 0; i < A->rows; i++) {
    if (i == row) continue;
    for (int j = 0, c = 0; j < A->columns; j++) {
      if (j == col) continue;
      result->matrix[r][c] = A->matrix[i][j];
      c++;
    }
    r++;
  }
}

int checkMatrix(matrix_t *matrix) {
  int result = 0;
  if (matrix == NULL || matrix->matrix == NULL) {
    result = 1;
  }
  return result;
};