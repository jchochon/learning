#include <stdbool.h>

typedef struct {
    int g;
    int h;
    int f;
    int row;
    int col;
    struct state* previous_state;
    int matrix[3][3];
} state;

typedef struct {
    int i;
    int j;
} bidimensional_index;

void init_state(state* s);
void print_state(state* s);
state* first_state_by_f(state** states);
state** remove_state_from_list(state** states, state* s);
state** add_state_into_list(state** states, state* s);
state* create_new_state();
state** get_next_states(state* s);
bidimensional_index get_position_of_zero(state* s);
bidimensional_index** get_swapable_positions(state* s, bidimensional_index index_of_zero);
bool are_states_equal(state* s1, state* s2);
int compute_heuristic(state* s1, state* s2);
state* states_contain_similar(state** states, state* s);
void print_list(state** list);
