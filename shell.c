#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <sys/types.h>
#include <sys/wait.h>

#define BUFFER_SIZE 1024

/**
 * display_prompt - Displays the prompt
 *
 * Return: the command entered by the user
*/
void display_prompt(void)
{
	if (isatty(STDIN_FILENO))
		write(STDOUT_FILENO, "$ ", 2);
}

/**
 * read_cmd - Reads a command from the user
 *
 * Return: the command entered by the user
*/
char *read_cmd(void)
{
	char *cmd = NULL;
	size_t cmd_size = 0;
	ssize_t read_cmd;

	read_cmd = getline(&cmd, &cmd_size, stdin);
	if (read_cmd == -1)
	{
		free(cmd);
		exit(EXIT_SUCCESS);
	}

	cmd[strcspn(cmd, "\n")] = '\0';

	return (cmd);
	}

/**
 * execute_cmd - Executes a command with the given arguments
 * @cmd: command
 *
 * Return: 0 on success
*/
void execute_cmd(char *cmd)
{
	pid_t pid;
	char *envp[] = {NULL};

	pid = fork();
	if (pid == -1)
	{
		perror("fork");
		exit(EXIT_FAILURE);
	}
	else if (pid == 0)
	{
		char **args = malloc(sizeof(char *) * 2);

		args[0] = cmd;
		args[1] = NULL;

		if (execve(cmd, args, envp) == -1)
		{
			perror(cmd);
			exit(EXIT_FAILURE);
		}
	}
	else
	{
		int child_status;

		if (waitpid(pid, &child_status, 0) == -1)
		{
			perror("waitpid");
			exit(EXIT_FAILURE);
		}
	}
}

/**
 * main - Entry point
 *
 * Return: nothing
*/
int main(void)
{
	char *cmd;

	while (1)
	{
		display_prompt();
		cmd = read_cmd();
		execute_cmd(cmd);
		free(cmd);
	}

	return (0);
}
