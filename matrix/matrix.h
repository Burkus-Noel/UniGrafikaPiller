#ifndef MATRIX_H
#define MATRIX_H

/**
 * Initializes all elements of the matrix to zero.
 */
void init_zero_matrix(float matrix[3][3]);

/**
 * Print the elements of the matrix.
 */
void print_matrix(const float matrix[3][3]);

/**
 * Add matrices.
 */
void add_matrices(const float a[3][3], const float b[3][3], float c[3][3]);

/**
 * Make a 3X3 identity matrix.
 */

void init_identity_matrix(float identity_matrix[3][3]);

/**
 *  Multiple a matrix with a scalar.
 */

void matrix_scalar_multplication(float matrix[3][3], float scalar);

/**
  * Multiple 2 matrices into a third matrix.
  */

 void multiply_matrices(const float a[3][3], const float b[3][3], float c[3][3]);
 /**
 * Multiples a matrix with a vektor
 */

 void transform_point(const float a[3][3],const float b[3], float result[3]);

 #endif // MATRIX_H

