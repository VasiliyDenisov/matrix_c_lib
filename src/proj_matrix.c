#include "proj_matrix.h"

int proj_create_matrix(int rows, int columns, matrix_t *result) {
  int flag = OK;
  if (rows <= 0 || columns <= 0)
    flag = INCORRECT_MATRIX;
  else {
    result->rows = rows;
    result->columns = columns;
    result->matrix = calloc(rows, sizeof(double *));
    for (int i = 0; i < rows; i++)
      result->matrix[i] = (double *)calloc(columns, sizeof(double));
  }
  return flag;
}

void proj_remove_matrix(matrix_t *A) {
  for (int i = 0; i < A->rows; i++) free(A->matrix[i]);
  free(A->matrix);
  A->rows = A->columns = 0;
}

int proj_eq_matrix(matrix_t *A, matrix_t *B) {
  int flag = is_matrix_size_equal(A, B);
  if (flag == SUCCESS) {
    int i = 0, j = 0, idx = 0;
    while (i < A->rows && double_equal(A->matrix[i][j], B->matrix[i][j])) {
      idx++;
      i = idx / A->columns;
      j = idx % A->columns;
    }
    if (i < A->rows) flag = FAILURE;
  }
  return flag;
}

int proj_sum_matrix(matrix_t *A, matrix_t *B, matrix_t *result) {
  int flag = is_incorrect_matrix(A) || is_incorrect_matrix(B);
  if (flag == OK) flag = is_matrix_size_equal(A, B) ? OK : CALCULATION_ERROR;
  if (flag == OK) {
    proj_create_matrix(A->rows, A->columns, result);
    for (int i = 0; i < A->rows; i++)
      for (int j = 0; j < A->columns; j++)
        result->matrix[i][j] = A->matrix[i][j] + B->matrix[i][j];
  }
  return flag;
}

int proj_sub_matrix(matrix_t *A, matrix_t *B, matrix_t *result) {
  int flag = is_incorrect_matrix(B);
  if (flag == OK) {
    for (int i = 0; i < B->rows; i++)
      for (int j = 0; j < B->columns; j++) B->matrix[i][j] *= -1.0;
    flag = proj_sum_matrix(A, B, result);
  }
  return flag;
}

int proj_mult_number(matrix_t *A, double number, matrix_t *result) {
  int flag = is_incorrect_matrix(A);
  if (flag == OK) {
    proj_create_matrix(A->rows, A->columns, result);
    for (int i = 0; i < A->rows; i++)
      for (int j = 0; j < A->columns; j++)
        result->matrix[i][j] = A->matrix[i][j] * number;
  }
  return flag;
}

int proj_mult_matrix(matrix_t *A, matrix_t *B, matrix_t *result) {
  int flag = is_incorrect_matrix(A) || is_incorrect_matrix(B);
  if (flag == OK) flag = A->columns != B->rows ? CALCULATION_ERROR : 0;
  if (flag == OK) {
    proj_create_matrix(A->rows, B->columns, result);
    for (int i = 0; i < A->rows; i++)
      for (int j = 0; j < B->columns; j++)
        for (int l = 0; l < A->columns; l++)
          result->matrix[i][j] += A->matrix[i][l] * B->matrix[l][j];
  }
  return flag;
}

int proj_transpose(matrix_t *A, matrix_t *result) {
  int flag = is_incorrect_matrix(A);
  if (flag == OK) {
    proj_create_matrix(A->columns, A->rows, result);
    for (int i = 0; i < A->rows; i++)
      for (int j = 0; j < A->columns; j++)
        result->matrix[j][i] = A->matrix[i][j];
  }
  return flag;
}

int proj_calc_complements(matrix_t *A, matrix_t *result) {
  int flag = is_incorrect_matrix(A);
  if (flag == OK)
    if (A->columns != A->rows) flag = CALCULATION_ERROR;
  if (flag == OK) {
    proj_create_matrix(A->columns, A->rows, result);
    if (A->columns == 1)
      result->matrix[0][0] = A->matrix[0][0];
    else if (A->columns > 1) {
      for (int i = 0; i < A->rows; i++) {
        for (int j = 0; j < A->columns; j++) {
          double tmp_result = 0;
          matrix_t tmp_matrix;
          proj_create_matrix(A->rows - 1, A->columns - 1, &tmp_matrix);
          for (int k = 0, m = 0; k < A->rows - 1; k++, m++) {
            for (int l = 0, n = 0; l < A->columns - 1; l++, n++) {
              m += (m == i);
              n += (n == j);
              tmp_matrix.matrix[k][l] = A->matrix[m][n];
            }
          }
          proj_determinant(&tmp_matrix, &tmp_result);
          result->matrix[i][j] = (1 - 2 * ((i + j) % 2)) * tmp_result;
          proj_remove_matrix(&tmp_matrix);
        }
      }
    }
  }
  return flag;
}

int proj_determinant(matrix_t *A, double *result) {
  int flag = is_incorrect_matrix(A);
  *result = 0;
  if (flag == OK)
    if (A->columns != A->rows) flag = CALCULATION_ERROR;
  if (flag == OK) {
    if (A->columns == 1)
      *result = A->matrix[0][0];
    else {
      for (int k = 0; k < A->columns; k++) {
        double tmp_result = 0;
        matrix_t tmp_matrix = {};
        proj_create_matrix(A->rows - 1, A->columns - 1, &tmp_matrix);
        for (int i = 0; i < A->rows - 1; i++)
          for (int j = 0, x = 0; j < A->columns - 1; j++, x++) {
            x += (x == k);
            tmp_matrix.matrix[i][j] = A->matrix[i + 1][x];
          }
        proj_determinant(&tmp_matrix, &tmp_result);
        *result += (1 - 2 * (k % 2)) * tmp_result * A->matrix[0][k];
        proj_remove_matrix(&tmp_matrix);
      }
    }
  }
  return flag;
}

int proj_inverse_matrix(matrix_t *A, matrix_t *result) {
  int flag = OK;
  double determinant = 0;

  if (is_incorrect_matrix(A))
    flag = INCORRECT_MATRIX;
  else if (A->columns != A->rows)
    flag = CALCULATION_ERROR;
  else if (!proj_determinant(A, &determinant) && !determinant)
    flag = CALCULATION_ERROR;
  else {
    matrix_t complements, transpose;
    proj_calc_complements(A, &complements);
    proj_transpose(&complements, &transpose);
    proj_create_matrix(A->columns, A->rows, result);
    for (int i = 0; i < result->rows; i++)
      for (int j = 0; j < result->columns; j++)
        result->matrix[i][j] = 1 / determinant * transpose.matrix[i][j];
    proj_remove_matrix(&complements);
    proj_remove_matrix(&transpose);
  }
  return flag;
}

int double_equal(double first, double second) {
  return fabs(first - second) <= 1e-7;
}

int is_incorrect_matrix(const matrix_t *A) {
  int flag = A == NULL;
  if (!flag) flag = A->rows <= 0 || A->columns <= 0;
  return flag;
}

int is_matrix_size_equal(const matrix_t *A, const matrix_t *B) {
  return (A->columns == B->columns) && (A->rows == B->rows);
}
