/* ************************************************************************** */
/*                                                                            */
/*                                                       ::::::::             */
/*   pipex_utils.c                                     :+:    :+:             */
/*                                                    +:+                     */
/*   By: avaliull <avaliull@student.codam.nl>        +#+                      */
/*                                                  +#+                       */
/*   Created: 2025/02/15 13:05:22 by avaliull     #+#    #+#                  */
/*   Updated: 2025/02/28 12:52:23 by avaliull     ########   odam.nl          */
/*                                                                            */
/* ************************************************************************** */

#include "pipex.h"
#include "ft_printf.h"
#include <unistd.h>
#include <string.h>
#include <errno.h>
#include <stdio.h>

int	dup2_errcheck(int oldfd, int newfd)
{
	if (dup2(oldfd, newfd) < 0)
	{
		perror(DUP2_ERR);
		return (-1);
	}
	return (0);
}

void	free_string_array(char **arr)
{
	int	i;

	if (arr)
	{
		i = -1;
		while (arr[++i])
			free((arr[i]));
		free(arr);
	}
}

char	**find_env_path(void)
{
	int		i;
	char	**path_arr;

	i = -1;
	path_arr = NULL;
	while (__environ[++i])
	{
		if (ft_strncmp(__environ[i], "PATH", 4) == 0)
			path_arr = ft_split(&__environ[i][5], ':');
	}
	if (!path_arr)
		return (NULL);
	return (path_arr);
}

static int	perror_return(char *err_msg)
{
	perror(err_msg);
	return (errno);
}

int	try_execve(const char **path, char *const argv[])
{
	char	*tmp_slash;
	char	*command_path;

	if (execve(argv[0], argv, __environ) != -1)
		return (0);
	tmp_slash = ft_strjoin("/", argv[0]);
	if (!tmp_slash)
		return (perror_return(MALLOC_ERR));
	while (path[0])
	{
		command_path = ft_strjoin(path[0], tmp_slash);
		if (!command_path)
			return (perror_return(MALLOC_ERR));
		if (execve(command_path, argv, __environ) == -1)
			free(command_path);
		else
		{
			free(command_path);
			free(tmp_slash);
			return (0);
		}
		path++;
	}
	free(tmp_slash);
	return (perror_return(argv[0]));
}
