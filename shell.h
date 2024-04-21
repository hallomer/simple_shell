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

ssize_t read_command(char **line, size_t *n, FILE *input_stream);
int execute_command(char **argv_cmd, char *prog_name, int argc);
char **parse_command(char *line);
char *get_path(char *cmd);
void free_argv(char **argv);
char *search_path(char *path, char *cmd);

#endif
