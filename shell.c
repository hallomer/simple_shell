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
	char **argv_cmd = NULL;
	char *prog_name = argv[0];
	FILE *input_stream = stdin;
	int status = 0, last_status = 0;

	if (argc > 1)
		input_stream = fopen(argv[1], "r");

	if (input_stream == NULL)
	{
		perror(argv[0]);
		return (EXIT_FAILURE);
	}

	while (1)
	{
		nread = read_command(&line, &n, input_stream);
		if (nread == -1)
		{
			free(line);
			break;
		}
		line[nread - 1] = '\0';

		argv_cmd = parse_command(line);
		if (argv_cmd == NULL)
			continue;

		status = execute_command(argv_cmd, prog_name, argc, &last_status);
		free_argv(argv_cmd);
	}
	if (line)
		free(line);
	if (input_stream != stdin)
		fclose(input_stream);
	return (status);
}

/**
 * free_argv - Frees argv
 * @argv: argv to free
 *
 * Return: nothing
*/
void free_argv(char **argv)
{
	int i;

	for (i = 0; argv[i]; i++)
	{
		if (argv[i] != NULL)
			free(argv[i]);
	}
	if (argv != NULL)
		free(argv);
}
