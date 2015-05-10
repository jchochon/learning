#include <stdlib.h>
#include <stdio.h>
#include <time.h>
#include <stdbool.h>

#include "neuron.h"


static bool is_srand_initialized = false;


t_sensor* create_sensor() {
    t_sensor* sensor = malloc(sizeof(*sensor));
    sensor->output = 0;
    return sensor;
}

void feed_neural_network(t_synapse (*synapses)[20], int letter[4][5]) {
    int k = 0;
    for (int i = 0; i < 4; i++) {
        for (int j = 0; j < 5; j++) {
            ((*synapses)[k++].source)->output = letter[i][j];
        }
    }

}

void calculate_potential(t_synapse (*synapses)[20], t_neuron *neuron) {
    float potential = 0;
    for (int i = 0; i < 20; i++) {
        potential += ((*synapses)[i].source)->output * (*synapses)[i].weight;
    }
    neuron->potential = potential;
    // Heaviside
    neuron->output = potential - neuron->threshold < 0 ? 0 : 1;
}


void learning_phase(t_synapse (*synapses)[20], float eps, float Yd, float Yi) {

    for (int i = 0; i < 20; i++) {
        float xj = ((*synapses)[i].source)->output;
        (*synapses)[i].weight = (*synapses)[i].weight + eps * (Yd - Yi) * xj;
    }
    
}

int random(int max_value){
    if (!is_srand_initialized) {
        srand(time(NULL));
        is_srand_initialized = true;
    }
    return rand() % (max_value + 1);
}

void generate_noise(int (*L)[4][5], int n) {
    int row, col, i = 0, indexes_count = 0;
    bool found = false;
    int (*selected_indexes)[2] = NULL;

    while (i < n) {
        while (true) {
            row = random(3);
            col = random(4);
            found = false;
            for (int k = 0; k < indexes_count; k++) {
                if (selected_indexes[k][0] == row && selected_indexes[k][1] == col) {
                    found = true;
                    break;
}
            }

            if (!found) {
                selected_indexes = realloc(selected_indexes, ++indexes_count * sizeof(int[2]));
                selected_indexes[indexes_count-1][0] = row;
                selected_indexes[indexes_count-1][1] = col;
                break;
            }
        }
        
        (*L)[row][col] = (*L)[row][col] == 0 ? 1 : 0;

        i++;
    }
    free(selected_indexes);
}
