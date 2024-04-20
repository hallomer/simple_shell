#include "shell.h"
/**
 * read_command - Read user input command
 * @line: pointer to store the user input
 * @n: pointer to store the size of the line buffer
 *
 * Return: number of characters read, or -1 on error
*/
ssize_t read_command(char **line, size_t *n)
{
	printf("$ ");
	return (getline(line, n, stdin));
}

/**
 * execute_command - Execute the user input command
 * @argv_cmd: array containing the command and its arguments
 * @prog_name: name of the program
 * @argc: number of arguments passed to the program
 *
 * Return: 0 on success, 1 on failure
*/
int execute_command(char **argv_cmd, char *prog_name, int argc)
{
	pid_t child_pid;
	int status;

	child_pid = fork();
	if (child_pid == -1)
	{
		perror(prog_name);
		return (1);
	}
	else if (child_pid == 0)
	{
		if (execve(argv_cmd[0], argv_cmd, environ) == -1)
		{
			printf("%s: %d: %s: not found\n", prog_name, argc, argv_cmd[0]);
			exit(EXIT_FAILURE);
		}
	}
	else
	{
		wait(&status);
	}

	return (0);
}
