#include "shell.h"

/**
 * handle_empty_path - Helper function
 * @cmd: command to check
 *
 * Return: command path if it or NULL
*/
char *handle_empty_path(char *cmd)
{
	struct stat buffer;

	if (stat(cmd, &buffer) == 0)
		return (strdup(cmd));

	return (NULL);
}

/**
 * search_path - Helper function
 * @path: PATH string from the environment
 * @cmd: command to search for
 *
 * Return:full path of the or NULL
*/

char *search_path(char *path, char *cmd)
{
	char *path_copy, *path_tokens, *file_path;
	int cmd_len, dir_len;
	struct stat buffer;

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

		free(file_path);
		path_tokens = strtok(NULL, ":");
	}

	free(path_copy);
	return (NULL);
}


/**
 * get_path - Finds the full path of a command in the system's PATH
 * @cmd: command to find the full path for
 *
 * Return: full path or NULL if not found
*/
char *get_path(char *cmd)
{
	char *path, *full_path;
	struct stat buffer;

	path = getenv("PATH");
	if (!path || *path == '\0')
		return (handle_empty_path(cmd));

	full_path = search_path(path, cmd);
	if (full_path)
		return (full_path);

	if (stat(cmd, &buffer) == 0)
		return (strdup(cmd));

	return (NULL);
}
