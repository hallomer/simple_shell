#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <string.h>
#include <sys/wait.h>

#define MAX_LINE 1024

int main() {
    char line[MAX_LINE];
    char *args[MAX_LINE / 2 + 1], *token;
    int argc, status;
    pid_t pid;

    while (1) {
        printf("$ ");
        fflush(stdout);

        if (fgets(line, sizeof(line), stdin) == NULL)
            break;

        line[strcspn(line, "\n")] = '\0';

        argc = 0;
        token = strtok(line, " ");
        while (token != NULL) {
            args[argc++] = token;
            token = strtok(NULL, " ");
        }
        args[argc] = NULL;

        pid = fork();
        if (pid == -1) {
            perror("fork");
            exit(EXIT_FAILURE);
        } else if (pid == 0) {

            if (execvp(args[0], args) == -1) {
                fprintf(stderr, "%s: Command not found\n", args[0]);
                exit(EXIT_FAILURE);
            }
        } else {
            waitpid(pid, &status, 0);
        }
    }
    printf("\n");
    return 0;
}