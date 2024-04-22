#include "shell.h"

/**
 * read_command - Read user input command
 * @line: pointer to store the user input
 * @n: pointer to store the size of the line buffer
 * @input_stream: from which to read the user input command
 *
 * Return: number of characters read, or -1 on error
*/
ssize_t read_command(char **line, size_t *n, FILE *input_stream)
{
	ssize_t nread;

	if (isatty(fileno(input_stream)))
		printf("(H) ");
	nread = getline(line, n, input_stream);
	if (nread == -1)
	{
		free(*line);
		return (-1);
	}
	return (nread);
}

/**
 * parse_command - Parse a command line string into an array of arguments
 * @line: command line string to parse
 *
 * Return: array of strings containing the command, or NULL on failure
*/
char **parse_command(char *line)
{
	char *token;
	char **argv_cmd;
	int num_tokens = 0, i, j;
	const char *delim = " \t\r\n";
	char *line_copy = strdup(line);

	token = strtok(line_copy, delim);
	while (token != NULL)
	{
		num_tokens++;
		token = strtok(NULL, delim);
	}
	if (num_tokens == 0)
	{
		free(line_copy);
		return (NULL);
	}
	argv_cmd = malloc(sizeof(char *) * (num_tokens + 1));
	if (argv_cmd == NULL)
	{
		free(line_copy);
		return (NULL);
	}
	token = strtok(line, delim);
	if (token == NULL)
	{
		free(line_copy);
		return (NULL);
	}
	for (i = 0; token != NULL; i++)
	{
		argv_cmd[i] = strdup(token);
		if (argv_cmd[i] == NULL)
		{
			for (j = 0; j <= i; j++)
				free(argv_cmd[j]);
			free(argv_cmd);
			free(line_copy);
			return (NULL);
		}
		token = strtok(NULL, delim);
	}
	argv_cmd[i] = NULL;
	free(line_copy);
	return (argv_cmd);
}
