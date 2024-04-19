#include "shell.h"

void execmd(char **argv)
{
    char *cmd = NULL;

    if (argv)
    {
        cmd = argv[0];

        if (execve(cmd, argv, NULL) == -1)
            perror(argv[0]);
    }
}
