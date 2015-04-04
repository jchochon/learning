#include <stdlib.h>

int get_length_of_list_of_pointer(void** list) {
    int n = 0;

    if (list == NULL)
        return -1;

    while (*list != NULL) {
        n++;
        list++;
    }

    return n;
    
}
