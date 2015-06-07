#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <errno.h>
#include <string.h>
#include <limits.h>

#include "internal_cmd.h"

// Zone prototype
int create_new_process(char *input_cmd, char *const argv[]);
char **get_path_array(char *path, int *path_array_size);
int check_file(char **, int, char *);
char **get_input_args(char *);
int is_internal(char *);
char *get_prompt();
void analyze(char *);

int execv(const char *path, char *const argv[]) ;
int fork(void);
char *strerror(int);
pid_t wait(int *);
char *getenv(const char*);
char* strtok(char *, const char*);

// Déclaration des constantes
char *PATH;
char *HOST_NAME = "JeSuisCharlie@JKMG:";



// Déclaration des variables globales
char *SPECIAL_CHARS = "<>|&";

int main(int argc, char* const argv[])
{
    
    

    PATH = getenv("PATH");

    int *path_array_size = malloc(sizeof(int));
    char **path_array = get_path_array(PATH, path_array_size);

    char *input_cmd = malloc(sizeof(char) * 80);

    for (;;) {
        int res, index;
        printf("%s>$  ", get_prompt());
        gets(input_cmd);

        // Test
        analyze(input_cmd);

        // Si l'entrée utilisateur est vide, on continue
        if (strlen(input_cmd) < 1)
            continue;
        // Sinon si la commande commence par un /, on créé le processus à partir du chemin absolu fourni
        else if (*input_cmd == '/') {
            char **input_args = get_input_args(input_cmd);

            if ((res = create_new_process(input_args[0], input_args)) == -1)
            {
                printf("%s\n", strerror(errno));
            }
        }
        // Sinon, on recherche dans la variable d'environnement PATH s'il existe un executable du nom donné
        // afin de l'executer
        else {
            if ((res = is_internal(input_cmd)) != -1) {
                if (internal_cmds[res].has_argument == 1) {
                    char **input_args = get_input_args(input_cmd);
                    internal_cmds[res].func(input_args+1);
                }
                continue;    
            }
            if ((index = check_file(path_array, *path_array_size, input_cmd)) == -1) {
                printf("%s\n", strerror(errno));
                continue;
            }

            char **input_args = get_input_args(input_cmd);

            char *tmp_cmd = malloc(sizeof(char) * strlen(input_cmd));
            strcpy(tmp_cmd, input_cmd);
            tmp_cmd = strtok(tmp_cmd, " ");
            char *cmd = malloc(sizeof(char) * strlen(path_array[index]) + strlen(tmp_cmd) + 1 );
            strcpy(cmd, path_array[index]);
            strcat(cmd, "/");
            strcat(cmd, tmp_cmd);
            free(tmp_cmd);
            if ((res = create_new_process(cmd, input_args)) == -1)
                printf("%s\n", strerror(errno));
        }
    }
}

void analyze(char *input_cmd) {
    // Création tableau
    char **input_array = NULL;
    int i, last_index = 0, n_spaces = 0;


    for (i = 0; i < (int) strlen(input_cmd) ; i++) {
        int j;
        for (j = 0; j < (int) strlen(SPECIAL_CHARS) ; j++) {
            if (input_cmd[i] == SPECIAL_CHARS[j]) {
                n_spaces += 2;
                input_array = realloc(input_array, sizeof(char*) * n_spaces );
                char *tmp = malloc(sizeof(char)*(i - last_index) + 1);
                memcpy(tmp, &input_cmd[last_index], i - last_index);
                input_array[n_spaces-2] = tmp;
                last_index = i;
                //input_array = realloc(input_array, sizeof(char*) * ++n_spaces );
                tmp = malloc(sizeof(char) + 1);
                memcpy(tmp, &input_cmd[last_index], 1);
                input_array[n_spaces-1] = tmp;
                last_index++;
            }
        }
    }
    n_spaces += 2;
    input_array = realloc(input_array, sizeof(char*) * n_spaces );
    char *tmp = malloc(sizeof(char)*(i - last_index) + 1);
    memcpy(tmp, &input_cmd[last_index], i - last_index);
    input_array[n_spaces-2] = tmp;

    input_array[n_spaces -1] = NULL;

    // Print Tableau
    //char **c = input_array;
    //while (*c != NULL){
    //    printf("%s\n", *c);
    //    c++;
    //}
    //printf("Fin analyse\n");
    //printf("Fin analyse\n");
    //printf("Fin analyse\n");
    //printf("Fin analyse\n");
}

char *get_prompt() {

    char *cwd;

    cwd = malloc(sizeof(char) * PATH_MAX + 1);
    cwd = getcwd(cwd, sizeof(char)*256);
    char *prompt = malloc(sizeof(char) * strlen(HOST_NAME) + sizeof(char) * strlen(cwd) + 1 );
    strcpy(prompt, HOST_NAME);
    strcat(prompt, cwd);

    return prompt;
}

int is_internal(char *input_cmd) {
    char *tmp_input_cmd = malloc(sizeof(char) * strlen(input_cmd));
    strcpy(tmp_input_cmd, input_cmd);
    
    char *token = strtok (tmp_input_cmd, " ");

    int i;
    for (i = 0; i < (int) (sizeof(internal_cmds)/sizeof(internal_cmd)); i++) {
        if (internal_cmds[i].action != NULL && strcmp(internal_cmds[i].action, token) == 0)
            return i;
    }

    return -1;
    
}

char **get_input_args(char *input_cmd) {
    /*
        Renvoi les arguments associé à la ligne passé en paramètre. Le premier elemnt de input_cmd est
        considéré comme étant le nom du programme. (utilisé par execv, voir man execv).
    */

    char *tmp_input_cmd = malloc(sizeof(char) * strlen(input_cmd));
    char **args = NULL;

    strcpy(tmp_input_cmd, input_cmd);

    char *token = strtok (tmp_input_cmd, " ");
    
    int n_spaces = 0;

    while (token) {
        args = realloc (args, sizeof (char*) * ++n_spaces);

        if (args == NULL)
            exit (-1); /* memory allocation failed */

        args[n_spaces-1] = token;

        token = strtok (NULL, " ");
    }

    // On ajoute NULL à la fin du tableau
    args = realloc (args, sizeof (char*) * ++n_spaces);
    args[n_spaces-1] = NULL;

    return args;
}

int check_file(char **path_array, int path_array_size, char *cmd_line) {
    char *path = NULL, *tmp_cmd = NULL, *cmd = NULL;
    int i, res;
    // Récupération de la commande
    tmp_cmd = malloc(sizeof(char) * strlen(cmd_line));
    strcpy(tmp_cmd, cmd_line);

    cmd = strtok (tmp_cmd, " ");
    
    // Test de l'existance de la commande pour tous les chemins du PATH
    for (i = 0; i < path_array_size; i++) {
        path = malloc(sizeof(char) * strlen(path_array[i]) + strlen(cmd) + 1);
        strcpy(path, path_array[i]);
        strcat(path, "/");
        strcat(path, cmd);
        if ((res = access(path, X_OK)) == 0) {
            free(path);
            break;
        }
        free(path);
    }
    if (res == 0)
        return i;
    else
        return -1;
}

char **get_path_array(char *path, int *path_array_size) {

    char **path_array = NULL;

    char *tmp_path = malloc(sizeof(char) * strlen(path));
    strcpy(tmp_path, path);

    char *token = strtok (tmp_path, ":");
    int n_spaces = 0;

    while (token) {
        path_array = realloc (path_array, sizeof (char*) * ++n_spaces);

        if (path_array == NULL)
            exit (-1); /* memory allocation failed */

        path_array[n_spaces-1] = token;

        token = strtok (NULL, ":");
    }

    *path_array_size = n_spaces;

    return path_array;
}

int create_new_process(char *cmd_line, char *const argv[])
{
    // Création d'un processus fils
    int n;
    n=fork();
    // Si le fork renvoi 0, c'est le fils. Sinon si il renvoi > 0, c'est le père. (Inferieur à 0 pour erreur).
    if(n==0)
    {   
        return (execv(cmd_line, argv));
    }   
    else if(n>0)
    { 
        wait(0);
    }   
    else printf("erreur de creation\n");
    return(1);
}
