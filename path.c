#include "shell.h"

/**
 * get_path - Finds the full path of a command in the system's PATH
 * @cmd: The command name for which to find the full path
 *
 * Return: full path of the command or NULL or an error
*/
char *get_path(char *cmd)
{
	char *path, *path_copy, *path_tokens, *file_path;
	int cmd_len, dir_len;
	struct stat buffer;

	path = getenv("PATH");
	if (path)
	{
		path_copy = strdup(path);
		cmd_len = strlen(cmd);
		path_tokens = strtok(path_copy, ":");
		while (path_tokens)
		{
			dir_len = strlen(path_tokens);
			file_path = malloc(cmd_len + dir_len + 2);

			strcpy(file_path, path_tokens);
			strcat(file_path, "/");
			strcat(file_path, cmd);
			strcat(file_path, "\0");
			if (stat(file_path, &buffer) == 0)
			{
				free(path_copy);
				return (file_path);
			}
			else
			{
				free(file_path);
				path_tokens = strtok(NULL, ":");
			}
		}

		free(path_copy);

		if (stat(cmd, &buffer) == 0)
			return (cmd);

		return (NULL);
	}
	return (NULL);
}
