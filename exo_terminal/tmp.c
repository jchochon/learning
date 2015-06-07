#include <unistd.h>
#include <stdlib.h>
#include <stdio.h>

#include "internal_cmd.h"

int main()
{
    char *buf = malloc(sizeof(char)*256);
    char *cwd = malloc(sizeof(char)*256);
    cwd = getcwd(buf, sizeof(char)*256);

    printf("%s\n", cwd);
    change_directory("test");
    printf("%s\n", getcwd(buf, sizeof(char)*256));

    return 0;
}
