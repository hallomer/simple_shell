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
	{
		printf("(H) ");
	}

	nread = getline(line, n, input_stream);
	if (nread == -1)
	{
		if (*line)
		{
			free(*line);
			*line = NULL;
		}
	}
	return (nread);
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
	char *command_path = argv_cmd[0];

	if (access(command_path, X_OK) == -1)
	{
		command_path = get_path(argv_cmd[0]);
		if (command_path == NULL)
		{
			fprintf(stderr, "%s: %d: %s: not found\n", prog_name, argc, argv_cmd[0]);
			return (127);
		}
	}

	child_pid = fork();
	if (child_pid == -1)
	{
		perror(prog_name);
		if (command_path != argv_cmd[0])
			free(command_path);
		return (126);
	}
	else if (child_pid == 0)
	{
		if (execve(command_path, argv_cmd, environ) == -1)
		{
			perror(prog_name);
			exit(127);
		}
	}
	else
	{
		wait(&status);
		if (command_path != argv_cmd[0])
			free(command_path);
		return (WEXITSTATUS(status));
	}

	return (0);
}


/**
 * parse_command - Parse a command line string into an array of arguments
 * @line: command line string to parse
 *
 * Return: array of strings containing the command, or NULL on failure
*/
char **parse_command(char *line)
{
	char *token, **argv_cmd, *line_copy = strdup(line);
	int num_tokens = 0, i, j;
	const char *delim = " \t\r\n";

	if (line_copy == NULL)
		return (NULL);
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
	for (i = 0; token != NULL; i++)
	{
		argv_cmd[i] = strdup(token);
		if (argv_cmd[i] == NULL)
		{
			for (j = 0; j < i; j++)
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
