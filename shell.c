#include "shell.h"

/**
 * main - Entry point for the shell
 *
 * Return: 0 on sucsess
*/

int main(void)
{
	char *cmd, *new_line = NULL, *token;
	size_t cmd_size = 0;
	ssize_t read_cmd;
	int status = 0;

	while (1)
	{
		if (isatty(STDIN_FILENO))
			write(1, "$ ", 2);

		read_cmd = getline(&cmd, &cmd_size, stdin);
		if (read_cmd == -1 || is_empty(cmd))
		{
			free(cmd);
			exit(status);
		}
		is_newline(cmd);
		if (is_env(cmd) != 1)
		{
			new_line = malloc(strlen(cmd) + 1);
			strcpy(new_line, cmd);
			if (is_empty(new_line))
			{
				free(cmd);
				free(new_line);
				exit(status);
			}

			token = strtok(new_line, " \t\n");
			while (token)
				token = strtok(NULL, " \t\n");

			free(new_line);
		}
		 execute(cmd);
	}
	return (0);
}


/**
 * is_empty - Check if string is empty or space
 * @str: string
 *
 * Return: 1 if not empty
*/

int is_empty(const char *str)
{
	if (str == NULL)
		return (1);

	while (*str)
	{
		if (*str != ' ' && *str != '\t' && *str != '\n' &&
			*str != '\v' && *str != '\f' && *str != '\r')
			return (0);
		str++;
	}

	return (1);
}
/**
 * is_newline - If there is \n char, it removes it
 * @cmd: command
 *
 * Return: nothing
*/
void is_newline(char *cmd)
{
	int i = 0;

	while (cmd[i] != '\0')
	{
		if (cmd[i] == '\n')
			cmd[i] = '\0';
		i++;
	}
}



/**
 * is_env - Check if command is env
 * @cmd: command
 *
 * Return: 1 if env
*/
int is_env(char *cmd)
{
	int i = 0;

	if (strncmp(cmd, "env", 3) == 0)
	{
		for (i = 0; environ[i]; i++)
		{
			write(STDOUT_FILENO, environ[i], strlen(environ[i]));
			write(STDOUT_FILENO, "\n", 1);
		}
		return (1);
	}
	return (0);
}


/**
 * execute - Executes commands
 * @cmd: command
 *
 * Return: nothing
*/
void execute(char *cmd)
{
	char *args[1024], *token = strtok(cmd, " \t\n");
	int i = 0;
	pid_t pid;

	while (token != NULL)
	{
		args[i] = token;
		i++;
		token = strtok(NULL, " \t\n");
	}
	args[i] = NULL;
	pid = fork();

	if (pid == -1)
	{

		perror("fork");
		exit(EXIT_FAILURE);
	}
	else if (pid == 0)
	{

		if (execvp(args[0], args) == -1)
		{
			perror("execvp");
			exit(EXIT_FAILURE);
		}
	}
	else
	{
		int status;

		waitpid(pid, &status, 0);
	}
}
