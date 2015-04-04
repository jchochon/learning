#include <stdlib.h>
#include <stdio.h>
#include <string.h>

#include "state.h"
#include "utils.h"

//#define DEBUG

void init_state(state* s) {
    s->g = 0;
    s->h = 0;
    s->f = 0;
    s->row = 0;
    s->col = 0;
    s->previous_state = NULL;
}

void print_state(state* s) {
    printf("G: %d\n", s->g);
    printf("H: %d\n", s->h);
    printf("F: %d\n", s->f);

    printf("State's grid: \n");

    int i, j;
    for (i = 0; i < 3; i++) {
        for (j = 0; j < 3; j++) {
            printf("%d ", s->matrix[i][j]);
        }   
        printf("\n");
    }
}

state* first_state_by_f(state** states) {
    // Renvoi l'état de F le plus faible
    state* min_state = *states;
    states++;
    while (*states != NULL) {
        state* current_state = *states;
        if (current_state->f < min_state->f)
            min_state = current_state;
        states++;
    }
    
    return min_state;
}

state** remove_state_from_list(state** states, state* s) {
    // On recupère le nombre d'élément dans la liste
    int n = get_length_of_list_of_pointer((void**) states);

    // Trouver l'emplacement de s
    state** header = states;
    int i = 0;
    while (*header != s && *header != NULL) {
        i++;
        header++;
    }

    // TODO: regarder comment gérer en c l'equivalent d'une exception POO pour enlever cet exit moche !
    if (*header == NULL) {
        printf("(remove_state_from_list) Le sommet n'est pas dans la liste\n");
        exit(1);
    }
    
    // On décale les successeurs de s de -1 rang
    while (*header != NULL) {
        *header = *(header+1);
        if (*header == NULL)
            break;
        header++;
    }

    // 
    states = realloc(states, n * sizeof(state*));
    
    return states;

}

state** add_state_into_list(state** states, state* s) {
     // /!!!!!!!!!!!!\ Toujours avoir null à la fin de la suite de structure /!!!!!!!!!!!!\

    /*
     * Si states est à null, on l'initialise. Vision alternative: Equivalent d'un "List<State> states = new List<State>();" en POO, un pointeur
     * de la structure state étant l'analogue d'une réf POO. On admet que states pointant vers un unique pointeur de state equivaut à une liste 
     * de longueur 0.
    */
    if (states == NULL) {
        // Initialisation de la liste à length == 0
        states = malloc(sizeof(state*));
        *states = NULL;
    }

    // récupération de la longueur de states
    int n = get_length_of_list_of_pointer((void**) states);

    // Allocation n+2 pointeurs de state. Un pour ajouter un élément, l'autre pour garder la trace de fin de liste
    states = realloc(states, n*sizeof(state*) + 2*sizeof(state*));
    // Ajout de l'élément à ajouter + d'un élément NULL
    *(states+n) = s;
    *(states+(n+1)) = NULL;
    
    // On retourne states qui à potentiellement changé d'adresse dû à la nature de realloc
    return states;
}

state** get_childs_of(state* state) {
    bidimensional_index index_of_zero = get_position_of_zero(state);

    if (index_of_zero.i == -1 || index_of_zero.j == -1) {
        printf("Pas de zero trouvé dans la grille: \n");
        print_state(state);
        exit(15);
    }
    
    bidimensional_index** swapable_positions = get_swapable_positions(state, index_of_zero);

    return malloc(sizeof(state**));
}

/*
state** get_next_states(state* state) {
    // On recupère l'index de la tuile 0
    //bidimensional_index index = { .i = -1, .j = -1 };
    //index.i = -1;
    //index.j = -1;
    //int index_of_zero[2] = { -1, -1 };
    //get_position_of_zero(s, &index_of_zero[0], &index_of_zero[1]);
    bidimensional_index index_of_zero = get_position_of_zero(state);

    // TODO: apprendre gestion des erreur en c POO style pour se débarasser de ce exit tout moche
    if (index_of_zero.i == -1 || index_of_zero.j == -1) {
        printf("Pas de zero trouvé dans la grille: \n");
        print_state(state);
        exit(15);
    }

    #ifdef DEBUG
    printf("row of zero: %d\n", index_of_zero.i);
    printf("column of zero: %d\n", index_of_zero.j);
    #endif

    // Recuperation des positions echangeable avec 0. TODO: Free swapable_positions
    //bidimensional_index** swapable_positions = NULL;
    //get_swapable_positions(s, index_of_zero, swapable_positions);
    // TODO: faire fonction pour free swapable_positions
    bidimensional_index** swapable_positions = get_swapable_positions(state, index_of_zero);

    state** childs = malloc(sizeof(state*));

    // Pour chaque position swapable, on créer un nouvel état
    while (*swapable_positions != NULL) {
        state* new_state = create_new_state();
        new_state->g = state->g + 1;
        new_state->row = state->row;
        new_state->col = state->col;
        for (int i = 0; i < new_state->row; i++) {
            memcpy(new_state->matrix[i], state->matrix[i], state->row*sizeof(int));
        }
        swapable_position++;
    }

    // Free des positions
    

    exit(15);
}
*/

bidimensional_index** get_swapable_positions(state* s, bidimensional_index index_of_zero) {
    bidimensional_index** swapable_positions = NULL;
    int i = index_of_zero.i, j = index_of_zero.j;
    // Nombre d'élément de swapable_positions
    int n = 0;

    // Initialisation de swapable_positions
    swapable_positions = realloc(swapable_positions, sizeof(bidimensional_index*));
    *swapable_positions = NULL;
    
    // Si il existe une tuile à gauche de 0
    if (j-1 > -1) {
        swapable_positions = realloc(swapable_positions, n*sizeof(bidimensional_index*) + sizeof(bidimensional_index*));
        *(swapable_positions+n) = malloc(sizeof(bidimensional_index));
        (*(swapable_positions+n))->i = i;
        (*(swapable_positions+n))->j = j - 1;
        *(swapable_positions+(n+1)) = NULL;
        n += 1;
    }
    // Si il existe une tuile au dessus de 0
    if (i-1 > -1) {
        swapable_positions = realloc(swapable_positions, n*sizeof(bidimensional_index*) + sizeof(bidimensional_index*));
        *(swapable_positions+n) = malloc(sizeof(bidimensional_index));
        (*(swapable_positions+n))->i = i - 1;
        (*(swapable_positions+n))->j = j;
        *(swapable_positions+(n+1)) = NULL;
        n += 1;
    }
    // Si il existe une tuile à droite de 0
    if (j+1 < s->col) {
        swapable_positions = realloc(swapable_positions, n*sizeof(bidimensional_index*) + sizeof(bidimensional_index*));
        *(swapable_positions+n) = malloc(sizeof(bidimensional_index));
        (*(swapable_positions+n))->i = i;
        (*(swapable_positions+n))->j = j + 1;
        *(swapable_positions+(n+1)) = NULL;
        n += 1;
    }
    // Si il existe une tuile en dessous de 0
    if (i+1 < s->row) {
        swapable_positions = realloc(swapable_positions, n*sizeof(bidimensional_index*) + sizeof(bidimensional_index*));
        *(swapable_positions+n) = malloc(sizeof(bidimensional_index));
        (*(swapable_positions+n))->i = i + 1;
        (*(swapable_positions+n))->j = j;
        *(swapable_positions+(n+1)) = NULL;
        n += 1;
    }
    
    return swapable_positions;
}

//void get_position_of_zero(state* s, int* row_num, int* col_num) {
bidimensional_index get_position_of_zero(state* s) {
    bidimensional_index index = { .i = -1, .j = -1 };

    int i, j;
    for (i = 0; i < s->row; i++) {
        for (j = 0; j < s-> col; j++) {
            if (s->matrix[i][j] == 0) {
                index.i = i;
                index.j = j;
            }
        }
    }

    return index;
}

state* create_new_state() {
    state* s = malloc(sizeof(state));
    init_state(s);
    return s;
}
