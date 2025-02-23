/* ************************************************************************** */
/*                                                                            */
/*                                                       ::::::::             */
/*   pipex_utils.c                                     :+:    :+:             */
/*                                                    +:+                     */
/*   By: avaliull <avaliull@student.codam.nl>        +#+                      */
/*                                                  +#+                       */
/*   Created: 2025/02/15 13:05:22 by avaliull     #+#    #+#                  */
/*   Updated: 2025/02/22 14:09:47 by avaliull     ########   odam.nl          */
/*                                                                            */
/* ************************************************************************** */

#include "pipex.h"
#include "ft_printf.h"
#include <unistd.h>
#include <string.h>
#include <errno.h>

const char	**find_env_path(void)
{
	int		i;
	char	*path_var;
	char	**path_arr;

	i = -1;
	path_var = NULL;
	while (__environ[++i])
	{
		if (ft_strncmp(__environ[i], "PATH", 4) == 0)
		{
			path_var = ft_strdup(&__environ[i][5]);
			if (!path_var)
				return (NULL);
			break ;
		}
	}
	path_arr = ft_split(path_var, ':');
	free(path_var);
	if (!path_arr)
		return (NULL);
	return ((const char **) path_arr);
}

char	*try_execve(const char **path_arr, char *const argv[])
{
	char	*tmp_slash;
	char	*command_path;

	if (execve(argv[0], argv, __environ) != -1)
		return (NULL);
	tmp_slash = ft_strjoin("/", argv[0]);
	if (!tmp_slash)
		return ("malloc error");
	while (path_arr)
	{
		command_path = ft_strjoin(path_arr[0], tmp_slash);
		if (!command_path)
			return ("malloc error");
		if (execve(command_path, argv, __environ) != -1)
		{
			free(command_path);
			free(tmp_slash);
			return (NULL);
		}
		else
			free(command_path);
		path_arr++;
	}
	free(tmp_slash);
	return (strerror(errno));
}
