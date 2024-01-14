#include "shell.h"

/**
 * read_command - Reads a command from the user
 * @prompt: the prompt to display before reading the command
 *
 * Return: the command entered by the user
*/
char *read_command(const char *prompt)
{
	char *command = NULL;
	ssize_t len = 0;

	printf("%s", prompt);
	if (getline(&command, (size_t *)&len, stdin) == -1)
	{
		free(command);
		return (NULL);
	}

	return (command);
}

/**
 * tokenize_command - Tokenizes the command string into arguments
 * @command: the command string to tokenize
 * @args: an array of strings to store the arguments
 *
 * Return: nothing
*/
void tokenize_command(char *command, char **args)
{
	size_t i = 0;

	args[i] = strtok(command, " \t\n");
	if (args[i] == NULL || strcmp(args[i], "") == 0)
		return;

	while ((args[++i] = strtok(NULL, " \t\n")) != NULL)
		;

	args[i] = NULL;
}

/**
 * is_builtin - Checks if the command is a built-in command and executes
 * @args: an array of strings representing the command and its arguments
 *
 * Return: 1 on success
*/
int is_builtin(char **args)
{
	if (strcmp(args[0], "exit") == 0)
		return (1);
	else if (strcmp(args[0], "env") == 0)
	{
		print_env();
		return (1);
	}

	return (0);
}

/**
 * execute_command - Executes a command with the given arguments
 * @args: An array of strings representing the command and its arguments
 *
 * Return: 0 on success
*/
int execute_command(char **args)
{
	pid_t pid = fork();

	if (pid == -1)
	{
		perror("fork");
		return (1);
	}
	else if (pid == 0)
	{
		if (execvp(args[0], args) == -1)
		{
			perror("execvp");
			exit(1);
		}
	}
	else
	{
		int status;

		if (wait(&status) == -1)
		{
			perror("wait");
			return (1);
		}
	}

	return (0);
}
