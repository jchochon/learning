#include <stdio.h>
#include <stdlib.h>
#include <math.h>
#include <stdbool.h>
#include <unistd.h>

#include "neuron.h"
#include "utils.h"

#define EPSILON 0.01

int main() {
    int k = 1, err = 10, iter = 0;
    float ratio = 0;
    int A[4][5] = {
                { 1, 1, 1, 1, 1},
                { 1, 0, 0, 0, 1},
                { 1, 1, 1, 1, 1},
                { 1, 0, 0, 0, 1} };
    int C[4][5] = {
                { 1, 1, 1, 1, 1},
                { 1, 0, 0, 0, 0},
                { 1, 0, 0, 0, 0},
                { 1, 1, 1, 1, 1} };
    int L[4][5];
    int last_used_matrix[4][5] = {
                { -1, -1, -1, -1, -1},
                { -1, -1, -1, -1, -1},
                { -1, -1, -1, -1, -1},
                { -1, -1, -1, -1, -1} };
    t_neuron d_neuron = { .potential = 0, .threshold = 0.5, .output = 0 };
    t_synapse synapses[20];

    // Création du réseau
    for (int i = 0; i < 20; i++) {
        synapses[i].source = create_sensor();
        synapses[i].target = &d_neuron;
        synapses[i].weight = 0;
    }


    // Phase d'apprentissage
    do {
        feed_neural_network(&synapses, A);
        learning_phase(&synapses, EPSILON, 1, 0);

        feed_neural_network(&synapses, C);
        learning_phase(&synapses, EPSILON, 0, 1);
        
        feed_neural_network(&synapses, A);
        calculate_potential(&synapses, &d_neuron);
        if (d_neuron.output != 1)
            err++;

        feed_neural_network(&synapses, C);
        calculate_potential(&synapses, &d_neuron);
        if (d_neuron.output != 0)
            err++;

        iter++;
        ratio = (float) iter / (float) err;
        
        //printf("iter: %d && err: %d\n", iter, err);
        //printf("ratio:%f\n", ratio);
    } while ( ratio < 0.8);


    // Phase de test pour A
    while (k < 20) {
        int i = 0;
        int success = 0;
        while (i < 100000) {
            while (true) {
                // Generer copie d'une lettre
                lcpy(&L, &A);
                // Generer du bruit sur cette copie
                generate_noise(&L, k);
                //print_letter(L);
                //printf("\n");
                if (!matrices_are_equal(L, last_used_matrix)) {
                    lcpy(&last_used_matrix, &L);
                    break;
                }
            }

            feed_neural_network(&synapses, L);
            calculate_potential(&synapses, &d_neuron);
            if (d_neuron.output == 1)
                success++;
            
            i++;
        }
        success /= 1000;
        printf("%d%% de succes sur %d essais avec %d%% de bruit.\n", success, i, k*5);
        k++;
    }

    // Phase de test pour C
    k = 1;
    while (k < 20) {
        int i = 0;
        int success = 0;
        while (i < 100000) {
            while (true) {
                // Generer copie d'une lettre
                lcpy(&L, &C);
                // Generer du bruit sur cette copie
                generate_noise(&L, k);
                if (!matrices_are_equal(L, last_used_matrix)) {
                    lcpy(&last_used_matrix, &L);
                    break;
                }
            }

            feed_neural_network(&synapses, L);
            calculate_potential(&synapses, &d_neuron);
            if (d_neuron.output == 0)
                success++;
            
            i++;
        }
        success /= 1000;
        printf("%d%% de succes sur %d essais avec %d%% de bruit.\n", success, i, k*5);
        k++;
    }
}
