typedef struct {
    float potential;
    float threshold;
    int output;
    
} t_neuron;

typedef struct {
    float output;
} t_sensor;

typedef struct {
    t_sensor* source;
    t_neuron* target;
    float weight;
} t_synapse;

t_sensor* create_sensor();
void feed_neural_network(t_synapse (*synapses)[20], int letter[4][5]);
void calculate_potential(t_synapse (*synapses)[20], t_neuron *neuron);
void learning_phase(t_synapse (*synapses)[20], float eps, float Yd, float Yi);
int random(int max_value);
void generate_noise(int (*L)[4][5], int n);
