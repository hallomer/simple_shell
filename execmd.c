#include "shell.h"

void execmd(char **argv_cmd, char *prog_name)
{
    char *cmd = NULL;

    if (argv_cmd)
    {
        cmd = argv_cmd[0];

        if (execve(cmd, argv_cmd, NULL) == -1)
        {
            perror(prog_name);
        }
    }
}