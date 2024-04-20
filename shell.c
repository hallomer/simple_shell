#include "shell.h"

int main(int argc, char **argv)
{
    char *prompt = "$ ";
    char *lineptr, *lineptr_copy = NULL;
    size_t n = 0;
    ssize_t nchars_read;
    const char *delim = " \n";
    int num_tokens = 0, i;
    char *token, **argv_cmd;
    char *prog_name = argv[0];
    pid_t child_pid;
    int status;

    if (argc < 1)
    {
        perror(prog_name);
        return (-1);
    }

    while (1)
    {
        printf("%s", prompt);
        nchars_read = getline(&lineptr, &n, stdin);
        if (nchars_read == -1)
            return (-1);

        lineptr_copy = malloc(sizeof(char) * nchars_read);
        if (lineptr_copy == NULL)
        {
            perror(prog_name);
            return (-1);
        }
        strcpy(lineptr_copy, lineptr);

        token = strtok(lineptr, delim);
        while (token)
        {
            num_tokens++;
            token = strtok(NULL, delim);
        }
        num_tokens++;

        argv_cmd = malloc(sizeof(char *) * num_tokens);
        token = strtok(lineptr_copy, delim);
        for (i = 0; token != NULL; i++)
        {
            argv_cmd[i] = malloc(sizeof(char) * strlen(token));
            strcpy(argv_cmd[i], token);
            token = strtok(NULL, delim);
        }

        argv_cmd[i] = NULL;

        child_pid = fork();
        if (child_pid == -1)
		{
            perror(prog_name);
            return (-1);
        }
		else if (child_pid == 0)
		{
            execmd(argv_cmd, prog_name);
            exit(EXIT_SUCCESS);
        }
		else
		{
            waitpid(child_pid, &status, 0);
        }

        for (i = 0; argv_cmd[i] != NULL; i++)
            free(argv_cmd[i]);
        free(argv_cmd);
    }

    free(lineptr);
    free(lineptr_copy);

    return (0);
}
