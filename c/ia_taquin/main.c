#define DEBUG
//#define TEST

#include <stdio.h>
#include <stdlib.h>

#include "state.h"
#include "utils.h"


int main() {
    // Initialisation de l'algo
    state** opened_states = NULL;
    state** closed_states = NULL;
    state* start = create_new_state();
    start->row = 3;
    start->col = 3;

    start->matrix[0][0] = 2;
    start->matrix[0][1] = 8;
    start->matrix[0][2] = 3;
    start->matrix[1][0] = 1;
    start->matrix[1][1] = 6;
    start->matrix[1][2] = 4;
    start->matrix[2][0] = 7;
    start->matrix[2][1] = 0;
    start->matrix[2][2] = 5;

    state* end = create_new_state();
    end->row = 3;
    end->col = 3;
    end->matrix[0][0] = 1;
    end->matrix[0][1] = 2;
    end->matrix[0][2] = 3;
    end->matrix[1][0] = 8;
    end->matrix[1][1] = 0;
    end->matrix[1][2] = 4;
    end->matrix[2][0] = 7;
    end->matrix[2][1] = 6;
    end->matrix[2][2] = 5;

    #ifdef TEST
    // Test des fonctions first_state_by_f, add_state_into_list & remove_state_from_list

    // Test de add_state_into_list
    printf("Début du test d'ajout d'un état dans une liste d'états\n");
    state* a = create_new_state();
    a->f = 12;
    state* b = create_new_state();
    b->f = 7;
    state* c = create_new_state();
    c->f = 10;
    state* d = create_new_state();
    d->f = 2;
    state* e = create_new_state();
    e->f = 17;
    state* f = create_new_state();
    f->f = 20;
    state* g = create_new_state();
    g->f = 22;
    opened_states = add_state_into_list(opened_states, a);
    opened_states = add_state_into_list(opened_states, b);
    opened_states = add_state_into_list(opened_states, c);
    opened_states = add_state_into_list(opened_states, d);
    opened_states = add_state_into_list(opened_states, e);
    opened_states = add_state_into_list(opened_states, f);
    opened_states = add_state_into_list(opened_states, g);
    int opened_states_length = get_length_of_list_of_pointer((void**)opened_states);
    if (opened_states_length == 7) 
        printf("Succès");
    else
        printf("Echec");
    printf("\n");
    
    // Test de first_state_by_f
    printf("Début du test de récupération de l'état de F "
                        "minimal dans une liste d'états\n");
    state* first_state_test = first_state_by_f(opened_states);
    if (first_state_test == d)
        printf("Succès");
    else
        printf("Echec");
    printf("\n");
    printf("Début du test de suppression d'un état parmis une "
                                            "liste d'états\n");
    opened_states = remove_state_from_list(opened_states, d);
    first_state_test = first_state_by_f(opened_states);
    opened_states_length = get_length_of_list_of_pointer((void**)opened_states);
    printf("Nombre d'élément présent: %d\n", opened_states_length);
    state** tmp = opened_states;
    while (*tmp != NULL) {
        printf("%d\n", (*tmp)->f);
        tmp++;
    }
    printf("oooooooooo\n");
    if (first_state_test != d)
        printf("Succès");
    else
        printf("Echec");
    printf("\n");
    #endif

    opened_states = add_state_into_list(opened_states, start);

    
    printf("Start\n");
    printf("____________\n");
    print_state(start);

    

    // Début de l'algo
    #ifdef DEBUG
    printf("Début de l'algoooooooooooooooooooooooooooooooooo\n");
    printf("opened_states = %p\n", opened_states);
    #endif
    while (get_length_of_list_of_pointer((void**) opened_states) > 0) {
        // Recherche du noeud le plus faible dans opened_states
        state* current_state = first_state_by_f(opened_states);
        // Génération des fils
        //state** childs = get_next_states(current_state);

        printf("%p\n", opened_states);
    }
    #ifdef DEBUG
    printf("fin algooooooooooooooo\n");
    #endif
    
    return 0;
}
