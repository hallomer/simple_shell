#include "shell.h"

/**
 * get_command_path - Get the full path of the command
 * @command: the command to be executed
 * @prog_name: name of the program
 * @argc: number of arguments passed to the program
 *
 * Return: the full path of the command, or NULL if not found
*/
char *get_command_path(char *command, char *prog_name, int argc)
{
	char *command_path = command;

	if (access(command_path, X_OK) == -1)
	{
		command_path = get_path(command);
		if (command_path == NULL && (strcmp(command, "exit") != 0))
		{
			fprintf(stderr, "%s: %d: %s: not found\n", prog_name, argc, command);
			return (NULL);
		}
	}

	return (command_path);
}

/**
 * run_command - Run the command with the given arguments
 * @argv_cmd: array containing the command and its arguments
 * @prog_name: name of the program
 * @command_path: the full path of the command
 *
 * Return: the exit status of the command
*/
int run_command(char **argv_cmd, char *prog_name, char *command_path)
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
 * execute_exit - Exits the shell with an optional status code
 * @argv_cmd: array of command arguments
 * @prog_name: name of the program
 * @argc: number of arguments passed to the program
 * @last_status: the exit status of the previously executed command
 *
 * Return: exit status or -1 on error
*/
int execute_exit(char **argv_cmd, char *prog_name, int argc, int last_status)
{
	int status = last_status;

	if (argv_cmd[1])
	{
		status = atoi(argv_cmd[1]);
		if (status == 0 && strcmp(argv_cmd[1], "0") != 0)
		{
			fprintf(stderr, "%s: %d: exit: Illegal number: %s\n",
					 prog_name, argc, argv_cmd[1]);
			return (2);
		}
	}

	exit(status);
}

/**
 * execute_command - Execute the user input command
 * @argv_cmd: array containing the command and its arguments
 * @prog_name: name of the program
 * @argc: number of arguments passed to the program
 * @last_status: the exit status of the previously executed command
 *
 * Return: the status code of the executed command
 */
int execute_command(char **argv_cmd, char *prog_name,
					int argc, int *last_status)
{
	int status;
	char *command_path = NULL;

	if (strcmp(argv_cmd[0], "exit") == 0)
	{
		status = execute_exit(argv_cmd, prog_name, argc, *last_status);
		return (status);
	}

	command_path = get_command_path(argv_cmd[0], prog_name, argc);
	if (command_path == NULL)
	{
		*last_status = 127;
		return (127);
	}

	status = run_command(argv_cmd, prog_name, command_path);
	*last_status = status;

	if (command_path != argv_cmd[0])
		free(command_path);

	return (status);
}
