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
state** get_childs_of(state* s);
bidimensional_index get_position_of_zero(state* s);
bidimensional_index** get_swapable_positions(state* s, bidimensional_index index_of_zero);
