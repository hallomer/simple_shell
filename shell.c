#include "shell.h"

/**
 * main - Entry point
 *
 * Return: 0
*/
int main(void)
{
	char *prompt = "$ ", *cmd = NULL, *args[MAX_ARGS], *path;

	while (1)
	{
		cmd = read_command(prompt);
		if (cmd == NULL)
			return (1);
		tokenize_command(cmd, args);
		if (args[0] == NULL)
		{
			free(cmd);
			cmd = NULL;
			continue;
		}
		if (check_builtin_commands(args))
		{
			free(cmd);
			cmd = NULL;
			continue;
		}
		if (access(args[0], F_OK) == -1)
		{
			path = getenv("PATH");
			if (!find_path(args[0], path))
			{
				fprintf(stderr, "%s: Command not found\n", args[0]);
				free(cmd);
				cmd = NULL;
				continue;
			}
		}
		execute_command(args);
		free(cmd);
		cmd = NULL;
	}
	return (0);
}

extern char **environ;

/**
 * print_environment - Print environment variables
 *
 * Return: nothing
*/
void print_environment(void)
{
	char **env = environ;

	while (*env != NULL)
	{
		printf("%s\n", *env);
		env++;
	}
}

/**
 * find_path - Find the command's path
 * @command: user's command
 * @path: path
 *
 * Return: 1 if the command exists, 0 otherwise
*/
int find_path(char *command, char *path)
{
	char *dir = strtok(path, ":");
	int command_exists = 0;
	char command_path[256];

	while (dir != NULL)
	{
		snprintf(command_path, sizeof(command_path), "%s/%s", dir, command);

		if (access(command_path, F_OK) == 0)
		{
			command_exists = 1;
			strcpy(command, command_path);
			break;
		}

		dir = strtok(NULL, ":");
	}

	return (command_exists);
}
