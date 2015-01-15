void change_directory(char*);

typedef struct internal_cmd {
    char *action;
    void (* func) ();
    int has_argument;
} internal_cmd;

const internal_cmd internal_cmds[] = { 
    {"cd", &change_directory, 1 },
    //{"foreground", &foreground },
    //{"jobs" ,&job_list},
    {NULL, NULL, 0}
};

