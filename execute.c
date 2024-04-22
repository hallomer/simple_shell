#include "shell.h"

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
	char *command_path;
	int status;

	if (handle_builtins(argv_cmd))
		return (0);

	command_path = get_command_path(argv_cmd[0], prog_name, argc);
	if (command_path == NULL)
		return (127);

	status = fork_and_execute(command_path, argv_cmd, prog_name);
	free(command_path);

	return (status);
}


/**
 * handle_builtins - Handle built-in commands
 * @argv_cmd: array containing the command and its arguments
 *
 * Return: exit status, or 0 if command is not a built-in
 */
int handle_builtins(char **argv_cmd)
{
	int status = 0;

	if (strcmp(argv_cmd[0], "exit") == 0)
	{
		if (argv_cmd[1])
		{
			status = atoi(argv_cmd[1]);
			if (status == 0 && strcmp(argv_cmd[1], "0") != 0)
			{
				fprintf(stderr, "exit: %s: numeric argument required\n", argv_cmd[1]);
				return (2);
			}
		}
		exit(status);
	}
	else if (strcmp(argv_cmd[0], "env") == 0)
	{
		print_environment();
		return (0);
	}
	return (0);
}

/**
 * get_command_path - Get the full path of the command
 * @command: command to find the path for
 * @prog_name: name of the program
 * @argc: number of arguments passed to the program
 *
 * Return: full path of the command, or NULL if not found
*/
char *get_command_path(char *command, char *prog_name, int argc)
{
	char *command_path = command;

	if (access(command_path, X_OK) == -1)
	{
		command_path = get_path(command);
		if (command_path == NULL)
		{
			fprintf(stderr, "%s: %d: %s: not found\n", prog_name, argc, command);
			return (NULL);
		}
	}

	return (command_path);
}

/**
 * fork_and_execute - Fork and execute the command
 * @command_path: full path of the command
 * @argv_cmd: array containing the command and its arguments
 * @prog_name: name of the program
 *
 * Return: exit status of the command, or 126 on failure
*/
int fork_and_execute(char *command_path, char **argv_cmd, char *prog_name)
{
	pid_t child_pid;
	int status;

	child_pid = fork();
	if (child_pid == -1)
	{
		perror(prog_name);
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
		return (WEXITSTATUS(status));
	}

	return (0);
}

/**
 * print_environment - Print the current environment variables
 *
 * Return - nothing
*/
void print_environment(void)
{
	int i;

	for (i = 0; environ[i] != NULL; i++)
	{
		write(STDOUT_FILENO, environ[i], strlen(environ[i]));
		write(STDOUT_FILENO, "\n", 1);
	}
}
