#include "utils.h"

void print_letter(int letter[4][5]) {
    for (int i = 0; i < 4; i++) {
        for (int j = 0; j < 5; j++) {
            printf("%d ", letter[i][j]);
        }   
        puts("");
    }   
}

void lcpy(int (*dst)[4][5], int (*src)[4][5]) {
    for (int i = 0; i < 4; i++) {
            memcpy((*dst)[i], (*src)[i], 5*sizeof(int));
    }   
}

bool matrices_are_equal(int src[4][5], int dst[4][5]) {
    bool res = true;

    for (int i = 0; i < 4; i++) {
        for (int j = 0; j < 5; j++) {
            if (src[i][j] != dst[i][j]) {
                res = false;
                break;
            }
        }
        if (!res) break;
    }

    return res;
}

bool matrix_exists_in(int (*matrices)[4][5], int matrice_count, int matrice[4][5]) {
    bool differ = false;
    for (int i = 0; i < matrice_count; i++) {
        if (differ = matrices_are_equal(matrices[i], matrice) == true) 
            break;
    }
    return differ;
}

void matrixcpy_to_matrices(int (**matrices)[4][5], int *matrice_count, int matrice[4][5]) {
    *matrices = realloc(*matrices, ++(*matrice_count) * sizeof(int[4][5]));
    for (int i = 0; i < 4; i++) {
        for (int j = 0; j < 5; j++) {
            (*matrices)[(*matrice_count)-1][i][j] = matrice[i][j];
        }
    }

}
