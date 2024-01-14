#include "shell.h"

/**
 * main - Entry point
 *
 * Return: nothing
*/
int main(void)
{
	const char *prompt = "$ ";
	char *command = NULL, *args[MAX_ARGS], *executable;

	while (1)
	{
		command = read_command(prompt);
		if (command == NULL)
			return (1);

		tokenize_command(command, args);
		if (args[0] == NULL)
		{
			free(command);
			command = NULL;
			continue;
		}

		if (is_builtin(args))
		{
			free(command);
			command = NULL;
			continue;
		}

		executable = find_executable(args[0]);
		if (executable == NULL)
		{
			fprintf(stderr, "%s: No such file or directory\n", args[0]);
			free(command);
			command = NULL;
			continue;
		}

		args[0] = executable;
		execute_command(args);

		free(command);
		command = NULL;
	}
	return (0);
}

/**
 * print_env - Prints the current environment variables
 *
 * Return: nothing
*/
void print_env(void)
{
	char **env = environ;

	while (*env != NULL)
	{
		printf("%s\n", *env);
		env++;
	}
}

/**
 * find_executable - Executes a command with the given arguments
 * @command: An array of strings representing the command and its arguments
 *
 * Return: 0 on success
*/
char *find_executable(char *command)
{
	if (access(command, F_OK | X_OK) == 0)
	{
		return (strdup(command));
	}

	return (NULL);
}

