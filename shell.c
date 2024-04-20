#include "shell.h"

/**
 * main - Entry point of the shell program
 * @argc: number of arguments passed to the program
 * @argv: array of strings containing the arguments
 *
 * Return: 0 on success, or the appropriate error code on failure
*/
int main(int argc, char **argv)
{
	char *line = NULL;
	size_t n = 0;
	ssize_t nread;
	char *argv_cmd[2];
	char *prog_name = argv[0];

	if (argc < 1)
	{
		perror(argv[0]);
		return (EXIT_FAILURE);
	}

	while (1)
	{
		nread = read_command(&line, &n);
		if (nread == -1)
			break;

		line[nread - 1] = '\0';

		argv_cmd[0] = line;
		argv_cmd[1] = NULL;

		if (execute_command(argv_cmd, prog_name, argc))
			break;
	}

	if (line)
		free(line);

	return (EXIT_SUCCESS);
}
