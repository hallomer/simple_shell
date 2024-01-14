#ifndef SHELL_H
#define SHELL_H


#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <sys/types.h>
#include <sys/wait.h>
#include <ctype.h>

extern char **environ;

int is_empty(const char *str);
void is_newline(char *cmd);
int is_env(char *cmd);
void execute(char *cmd);

#endif
