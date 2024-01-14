#ifndef SHELL_H
#define SHELL_H

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <sys/wait.h>

#define MAX_ARGS 100

extern char **environ;

void print_env(void);
char *read_command(const char *prompt);
void tokenize_command(char *command, char **args);
char *find_executable(char *command);
int execute_command(char **args);
int is_builtin(char **args);

#endif
