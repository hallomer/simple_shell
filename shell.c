#include "shell.h"

/**
 * main - Entry point of the shell program
 * @argc: number of arguments passed to the program
 * @argv: an array of strings containing the arguments
 *
 * Return: 0 on success, or the appropriate error code on failure
*/
int main(int argc, char **argv)
{
	char *prompt = "$ ";
	char *lineptr = NULL;
	size_t n = 0;
	ssize_t nchars_read;
	char **argv_cmd = NULL;
	int num_tokens = 0;

	if (argc < 1)
	{
		perror(argv[0]);
		return (EXIT_FAILURE);
	}

	while (1)
	{
		printf("%s", prompt);
		nchars_read = getline(&lineptr, &n, stdin);
		if (nchars_read == -1)
			break;
		lineptr[nchars_read - 1] = '\0';

		if (lineptr[0] != '\0')
		{
			num_tokens = parse_command(lineptr, &argv_cmd);
			if (num_tokens > 0)
				execmd(argv_cmd, argv[0]);
			free_argv(argv_cmd);
		}

		free(lineptr);
		lineptr = NULL;
		n = 0;
	}

	if (lineptr)
		free(lineptr);

	return (EXIT_SUCCESS);
}

/**
 * parse_command - Parse the command line and create an argument vector
 * @line: command line to parse
 * @argv_cmd: pointer to store the argument vector
 *
 * Return: number of tokens in the command line
*/
int parse_command(char *line, char ***argv_cmd)
{
	const char *delim = " \n";
	int num_tokens = 0;
	char *token, **tokens = NULL;

	token = strtok(line, delim);
	while (token)
	{
		tokens = realloc(tokens, (num_tokens + 1) * sizeof(char *));
		if (tokens == NULL)
			return (-1);
		tokens[num_tokens] = malloc(sizeof(char) * (strlen(token) + 1));
		if (tokens[num_tokens] == NULL)
			return (-1);
		strcpy(tokens[num_tokens], token);
		num_tokens++;
		token = strtok(NULL, delim);
	}

	*argv_cmd = tokens;
	return (num_tokens);
}

/**
 * free_argv - Free the memory allocated for the argument vector
 * @argv: argument vector to free
 *
 * Return: nothing
*/
void free_argv(char **argv)
{
	int i;

	if (argv == NULL)
		return;

	for (i = 0; argv[i] != NULL; i++)
		free(argv[i]);

	free(argv);
}
