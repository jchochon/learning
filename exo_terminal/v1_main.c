#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <errno.h>

// Zone prototype
int execv(const char *path, char *const argv[]) ;
int fork(void);
char *strerror(int);

int main(int argc, char* const argv[])
{
    char *line = malloc(sizeof(char) * 80);

    for (;;)
    {
        int res;
        printf(">$  ");
        gets(line);
        res = create_new_process(line, argv);
        if (res == -1)
        {
            //printf("Aucun binaire à cet emplacement n'a été trouvé.\n");
            printf("%s\n", strerror(errno));
        }
        //break;
    }
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
