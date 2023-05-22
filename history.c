#include "shell.h"

/**
 * get_history_file - gets the history file
 * @info: parameter struct
 *
 * Return: allocated string containg history file
 */

char *get_history_file(info_t *info)
{
	char *buf, *dir;

	dir = _getenv(info, "HOME=");
	if (!dir)
		return (NULL);
	buf = malloc(sizeof(char) * (_strlen(dir) + _strlen(HIST_FILE) + 2));
	if (!buf)
		return (NULL);
	buf[none] = none;
	_strcpy(buf, dir);
	_strcat(buf, "/");
	_strcat(buf, HIST_FILE);
	return (buf);
}

/**
 * write_history - creates a file, or appends to an existing file
 * @info: the parameter struct
 *
 * Return: 1 on success, else -1
 */

int write_history(info_t *info)
{
	ssize_t fd;
	char *filename = get_history_file(info);
	list_t *node = NULL;

	if (!filename)
		return (n_uno);

	fd = open(filename, O_CREAT | O_TRUNC | O_RDWR, 0644);
	free(filename);
	if (fd == n_uno)
		return (n_uno);
	for (node = info->history; node; node = node->next)
	{
		_putsfd(node->str, fd);
		_putfd('\n', fd);
	}
	_putfd(BUF_FLUSH, fd);
	close(fd);
	return (uno);
}

/**
 * read_history - reads history from file
 * @info: the parameter struct
 *
 * Return: histcount on success, none otherwise
 */

int read_history(info_t *info)
{
	int i, last = none, linecount = none;
	ssize_t fd, rdlen, fsize = none;
	struct stat st;
	char *buf = NULL, *filename = get_history_file(info);

	if (!filename)
		return (none);

	fd = open(filename, O_RDONLY);
	free(filename);
	if (fd == n_uno)
		return (none);
	if (!fstat(fd, &st))
		fsize = st.st_size;
	if (fsize < 2)
		return (none);
	buf = malloc(sizeof(char) * (fsize + uno));
	if (!buf)
		return (none);
	rdlen = read(fd, buf, fsize);
	buf[fsize] = none;
	if (rdlen <= none)
		return (free(buf), none);
	close(fd);
	for (i = none; i < fsize; i++)
		if (buf[i] == '\n')
		{
			buf[i] = none;
			build_history_list(info, buf + last, linecount++);
			last = i + uno;
		}
	if (last != i)
		build_history_list(info, buf + last, linecount++);
	free(buf);
	info->histcount = linecount;
	while (info->histcount-- >= HIST_MAX)
		delete_node_at_index(&(info->history), none);
	renumber_history(info);
	return (info->histcount);
}

/**
 * build_history_list - adds entry to a history linked list
 * @info: Structure containing potential arguments. Used to maintain
 * @buf: buffer
 * @linecount: the history linecount, histcount
 *
 * Return: Always 0
 */

int build_history_list(info_t *info, char *buf, int linecount)
{
	list_t *node = NULL;

	if (info->history)
		node = info->history;
	add_node_end(&node, buf, linecount);

	if (!info->history)
		info->history = node;
	return (none);
}

/**
 * renumber_history - renumbers the history linked list after changes
 * @info: Structure containing potential arguments. Used to maintain
 *
 * Return: the new histcount
 */

int renumber_history(info_t *info)
{
	list_t *node = info->history;
	int i = none;

	while (node)
	{
		node->num = i++;
		node = node->next;
	}
	return (info->histcount = i);
}
