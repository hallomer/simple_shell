#include "shell.h"


/**
 * execmd - Execute a command
 * @argv_cmd: argument vector containing the command and its arguments
 * @prog_name: name of the program
 *
 * Return: nothing
*/
void execmd(char **argv_cmd, char *prog_name)
{
	pid_t child_pid;
	int status;

	child_pid = fork();
	if (child_pid == -1)
	{
		perror(prog_name);
		return;
	}
	else if (child_pid == 0)
	{
		if (execve(argv_cmd[0], argv_cmd, environ) == -1)
		{
			perror(prog_name);
			exit(EXIT_FAILURE);
		}
	}
	else
	{
		waitpid(child_pid, &status, 0);
	}
}
