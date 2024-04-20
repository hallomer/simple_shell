#ifndef SHELL_H
#define SHELL_H

#include <stdio.h>
#include <stdlib.h>
#include <sys/types.h>
#include <string.h>
#include <unistd.h>
#include <sys/stat.h>
#include <sys/wait.h>

extern char **environ;

int parse_command(char *line, char ***argv_cmd);
void free_argv(char **argv);
void execmd(char **argv_cmd, char *prog_name);

#endif
