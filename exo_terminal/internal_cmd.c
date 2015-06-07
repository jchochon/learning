#include <unistd.h>
#include <stdio.h>
#include <error.h>

void change_directory (char** param) {
    printf("Changing working directory for %s\n", *param);
    if (chdir(*param) != 0)
        perror(NULL);
}

void call_charlie (char** param) {
    
}
