#include "shell.h"

/**
 * read_command - Read a command from the user
 * @prompt: $
 *
 * Return: command
*/
char *read_command(char *prompt)
{
	char *cmd = NULL;
	size_t len = 0;

	printf("%s", prompt);

	if (getline(&cmd, &len, stdin) == -1)
		return (NULL);

	return (cmd);
}

/**
 * tokenize_command - Tokenize the command into arguments
 * @cmd: command
 * @args: arguments
 *
 * Return: nothing
*/
void tokenize_command(char *cmd, char **args)
{
	int i = 0;

	args[i] = strtok(cmd, " \t\n");

	if (args[i] == NULL || strcmp(args[i], "") == 0)
		return;

	while ((args[++i] = strtok(NULL, " \t\n")) != NULL)
		;

	args[i] = NULL;
}

/**
 * check_builtin_commands - Check if the command is a built-in command
 * @args: arguments
 *
 * Return: 1 if the command is a built-in command, 0 otherwise
*/
int check_builtin_commands(char **args)
{
	if (strcmp(args[0], "exit") == 0)
		return (1);
	else if (strcmp(args[0], "env") == 0)
	{
		print_environment();
		return (1);
	}

	return (0);
}

/**
 * execute_command - Execute the command
 * @args: argument
 *
 * Return: 0
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
		wait(NULL);
	}

	return (0);
}
