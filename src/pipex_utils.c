/* ************************************************************************** */
/*                                                                            */
/*                                                       ::::::::             */
/*   pipex_utils.c                                     :+:    :+:             */
/*                                                    +:+                     */
/*   By: avaliull <avaliull@student.codam.nl>        +#+                      */
/*                                                  +#+                       */
/*   Created: 2025/02/15 13:05:22 by avaliull     #+#    #+#                  */
/*   Updated: 2025/02/25 19:26:07 by avaliull     ########   odam.nl          */
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
	char	**path_arr;

	i = -1;
	while (__environ[++i])
	{
		if (ft_strncmp(__environ[i], "PATH", 4) == 0)
		{
			path_arr = ft_split(&__environ[i][5], ':');
			if (!path_arr)
				return (NULL);
			return ((const char **) path_arr);
		}
	}
	return (NULL);
}

char	*try_execve(const char **path_arr, char *const argv[])
{
	char	*tmp_slash;
	char	*command_path;

	if (execve(argv[0], argv, __environ) != -1)
		return (NULL);
	tmp_slash = ft_strjoin("/", argv[0]);
	if (!tmp_slash)
		return ("MALLOC_ERR");
	while (path_arr[0])
	{
		command_path = ft_strjoin(path_arr[0], tmp_slash);
		if (!command_path)
			return ("MALLOC_ERR");
		if (execve(command_path, argv, __environ) == -1)
			free(command_path);
		else
		{
			free(command_path);
			free(tmp_slash);
			return (NULL);
		}
		path_arr++;
	}
	free(tmp_slash);
	return (EXECVE_ERR);
}
