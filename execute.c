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
