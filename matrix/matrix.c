#include "matrix.h"

#include <stdio.h>

void init_zero_matrix(float matrix[3][3])
{
    int i;
    int j;

    for (i = 0; i < 3; ++i) {
        for (j = 0; j < 3; ++j) {
            matrix[i][j] = 0.0;
        }
    }
}

void print_matrix(const float matrix[3][3])
{
    int i;
    int j;

    for (i = 0; i < 3; ++i) {
        for (j = 0; j < 3; ++j) {
            printf("%4.4f ", matrix[i][j]);
        }
        printf("\n");
    }
}

void add_matrices(const float a[3][3], const float b[3][3], float c[3][3])
{
    int i;
    int j;

    for (i = 0; i < 3; ++i) {
        for (j = 0; j < 3; ++j) {
            c[i][j] = a[i][j] + b[i][j];
        }
    }
}

void init_identity_matrix(float identity_matrix[3][3])
{
    int i;
    int j;
    for (i = 0; i < 3; i++)
    {
      for (j = 0; j < 3; ++j) {
            if(i==j)
             identity_matrix[i][j] = 1;
            else 
            identity_matrix[i][j] = 0;
        }  
    }
}

void matrix_scalar_multplication(float matrix[3][3], float scalar)
{
    for (int i = 0; i < 3; i++)
        {
            for (int j = 0; j < 3; j++)
            {
                matrix[i][j] *= scalar;
            }
        }
}

void multiply_matrices(const float a[3][3], const float b[3][3], float c[3][3])
{
for (int i = 0; i < 3; i++)
    {
        for (int j = 0; j < 3; j++)
        {
            c[i][j] = 0.0f;  

            for (int k = 0; k < 3; k++)
            {
                c[i][j] += a[i][k] * b[k][j];
            }
        }
    }
}

void transform_point(const float a[3][3],const float b[3], float result[3])
{
for (int i = 0; i < 3; i++)
    {
        result[i] = 0.0f;

        for (int k = 0; k < 3; k++)
        {
            result[i] += a[i][k] * b[k];
        }
    }
}
