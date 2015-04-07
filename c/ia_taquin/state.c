#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include <stdbool.h>

#include "state.h"
#include "utils.h"

#define DEBUG

void init_state(state* s) {
    s->g = 0;
    s->h = 0;
    s->f = 0;
    s->row = 0;
    s->col = 0;
    // TODO: a enlever
    for (int i = 0; i < 3; i++) {
        for (int j = 0; j < 3; j++) {
            s->matrix[i][j] = 0;
        }
    }
    s->previous_state = NULL;
}

void print_state(state* s) {
    if (s == NULL) {
        puts("print state: NULL");
        return;
    }
    printf("G: %d\n", s->g);
    printf("H: %d\n", s->h);
    printf("F: %d\n", s->f);
    printf("Dimensions: [%d][%d]\n", s->row, s->col);

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

    states = realloc(states, n * sizeof(state*));
    return states;

}

int compute_heuristic(state* s1, state* s2) {
    // Test l'égalité des matrices de s1 & s2
    if (s1->row != s2->row || s1->col != s2->col) {
        // TODO: Gestion erreur
        printf("compute_heuristic: Les dimensions des états ne sont pas égales\n");
        exit(15);
    }

    int diff = 0;
    for (int i = 0; i < s1->row; i++) {
        for (int j = 0; j < s1->col; j++) {
            if (s1->matrix[i][j] != s2->matrix[i][j])
                diff++;
        }
    }

    return diff;
}

state* states_contain_similar(state** states, state* s) {
    /* Renvoi un ptr vers k si la liste contient fi
    */
    if (states == NULL || *states == NULL)
        return NULL;

    while (*states != NULL) {
        if (are_states_equal(*states, s))
            return *states;
        states++;
    }

    return NULL;
}

bool are_states_equal(state* s1, state* s2) {
    // Test que les dimensions sont > à 0
    if (!(s1->row > 0 && s1->col > 0 && s2->row > 0 && s2->col > 0)) {
        puts("Les dimensions sont incorrectes");
        print_state(s1);
        print_state(s2);
        exit(15);
    }
    // Test l'égalité des matrices de s1 & s2
    if (s1->row != s2->row || s1->col != s2->col) {
        // TODO: Gestion erreur
        printf("are_states_equal: Les dimensions des états ne sont pas égales\n");
        exit(15);
    }


    bool res = true;
    for (int i = 0; i < s1->row; i++) {
        for (int j = 0; j < s1->col; j++) {
            if (s1->matrix[i][j] != s2->matrix[i][j]) {
                return false;
            }
        }
    }

    return res;
}

state** add_state_into_list(state** states, state* s) {
    //printf("début add !!!!\n");
    //printf("aaaaaaaaaaaaaaaaaaaaaaaa\n");
    //print_state(s);
    //printf("states==%p\n", states);
    //printf("aaaaaaaaaaaaaaaaaaaaaaaa\n");
    //print_list(states);
    // /!!!!!!!!!!!!\ Toujours avoir null à la fin de la suite de structure /!!!!!!!!!!!!\

    /*
     * Si states est à null, on l'initialise. Vision alternative: Equivalent d'un "List<State> states = new List<State>();" en POO, un pointeur
     * de la structure state étant l'analogue d'une réf POO. On admet que states pointant vers un unique pointeur de state equivaut à une liste 
     * de longueur 0.
    */
    if (states == NULL) {
        // Initialisation de la liste à length == 0
        state** ptr = malloc(sizeof(state*));
        *ptr = NULL;
        if (ptr == NULL) {
            fprintf(stderr, "failed to allocate memory.\n");
            exit(-1);
        }
        states = ptr;
        printf("states2==%p\n", states);
    }

    // récupération de la longueur de states
    int n = get_length_of_list_of_pointer((void**) states);
    //printf("n=%d\n", n);
    //printf("lala\n");
    //print_list(states);
    //printf("lala\n");

    // Allocation n+2 pointeurs de state. Un pour ajouter un élément, l'autre pour garder la trace de fin de liste
    states = realloc(states, n*sizeof(state*) + 2*sizeof(state*));
    // Ajout de l'élément à ajouter + d'un élément NULL
    *(states+n) = s;
    *(states+(n+1)) = NULL;

    puts("fin add!!!!");
    //print_list(states);
    
    // On retourne states qui à potentiellement changé d'adresse dû à la nature de realloc
    return states;
}

state** get_next_states(state* current) {
    // On recupère l'index de la tuile 0
    //bidimensional_index index = { .i = -1, .j = -1 };
    //index.i = -1;
    //index.j = -1;
    //int index_of_zero[2] = { -1, -1 };
    //get_position_of_zero(s, &index_of_zero[0], &index_of_zero[1]);
    bidimensional_index index_of_zero = get_position_of_zero(current);

    // TODO: apprendre gestion des erreur en c POO style pour se débarasser de ce exit tout moche
    if (index_of_zero.i == -1 || index_of_zero.j == -1) {
        printf("Pas de zero trouvé dans la grille: \n");
        print_state(current);
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
    bidimensional_index** swapable_positions = get_swapable_positions(current, index_of_zero);

    state** childs = malloc(sizeof(state*));
    childs = NULL;
    // n = nb d'élément dans la liste childs
    int n = 0;

    // Pour chaque position swapable, on créer un nouvel état
    while (*swapable_positions != NULL) {
        state* new_state = create_new_state();
        new_state->g = current->g + 1;
        new_state->row = current->row;
        new_state->col = current->col;
        new_state->previous_state = (struct state*) current;
        for (int i = 0; i < new_state->row; i++) {
            memcpy(new_state->matrix[i], current->matrix[i], current->row*sizeof(int));
        }
        // On swap les positions dans les nouveaux états
        new_state->matrix[index_of_zero.i][index_of_zero.j] = new_state->matrix[(*swapable_positions)->i][(*swapable_positions)->j];
        new_state->matrix[(*swapable_positions)->i][(*swapable_positions)->j] = 0;
        
        // Ajout à la liste des enfants
        if (childs == NULL) {
            childs = malloc(sizeof(state*));
            *childs = NULL;
        }

        childs = realloc(childs, n*sizeof(state*) + 2*sizeof(state*));
        *(childs+n) = new_state;
        *(childs+(n+1)) = NULL;
        
        swapable_positions++;
        n++;
    }

    // TODO: Free des positions

    printf("okkkkkkkkkkkkkkkkkkkkkkkkkkkkkkkkkkkkkk\n");
    return childs;
    
}

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
        swapable_positions = realloc(swapable_positions, n*sizeof(bidimensional_index*) + 2*sizeof(bidimensional_index*));
        *(swapable_positions+n) = malloc(sizeof(bidimensional_index));
        (*(swapable_positions+n))->i = i;
        (*(swapable_positions+n))->j = j - 1;
        *(swapable_positions+(n+1)) = NULL;
        n++;
    }
    // Si il existe une tuile au dessus de 0
    if (i-1 > -1) {
        swapable_positions = realloc(swapable_positions, n*sizeof(bidimensional_index*) + 2*sizeof(bidimensional_index*));
        *(swapable_positions+n) = malloc(sizeof(bidimensional_index));
        (*(swapable_positions+n))->i = i - 1;
        (*(swapable_positions+n))->j = j;
        *(swapable_positions+(n+1)) = NULL;
        n++;
    }
    // Si il existe une tuile à droite de 0
    if (j+1 < s->col) {
        swapable_positions = realloc(swapable_positions, n*sizeof(bidimensional_index*) + 2*sizeof(bidimensional_index*));
        *(swapable_positions+n) = malloc(sizeof(bidimensional_index));
        (*(swapable_positions+n))->i = i;
        (*(swapable_positions+n))->j = j + 1;
        *(swapable_positions+(n+1)) = NULL;
        n++;
    }
    // Si il existe une tuile en dessous de 0
    if (i+1 < s->row) {
        swapable_positions = realloc(swapable_positions, n*sizeof(bidimensional_index*) + 2*sizeof(bidimensional_index*));
        *(swapable_positions+n) = malloc(sizeof(bidimensional_index));
        (*(swapable_positions+n))->i = i + 1;
        (*(swapable_positions+n))->j = j;
        *(swapable_positions+(n+1)) = NULL;
        n++;
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

void print_list(state** list){ 
    #ifdef DEBUG
    printf("début affichage liste\n");
    #endif
    if (list == NULL || *list == NULL) {
        printf("liste non initialisée\n");
        return;
    }

    while (*list != NULL) {
        printf("______________________\n");
        print_state(*list);
        list++;
    }
    #ifdef DEBUG
    printf("fin affichage list\n");
    #endif
    
}

void print_result(state* s) {
    int n = s->g + 1;
    state *results[n];

    while(s != NULL) {
        results[s->g] = s;
        s = s->previous_state;
    }

    puts("___________________________________");
    for (int i = 0; i < n; i++)
        print_state(results[i]);
}
