#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <stdbool.h>

void print_letter(int letter[4][5]);
void lcpy(int (*dst)[4][5], int (*src)[4][5]);
bool matrix_exists_in(int (*matrices)[4][5], int matrice_count, int matrice[4][5]);
void matrixcpy_to_matrices(int (**matrices)[4][5], int *matrice_count, int matrice[4][5]);
bool matrices_are_equal(int src[4][5], int dst[4][5]);
