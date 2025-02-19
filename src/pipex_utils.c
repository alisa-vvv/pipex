/* ************************************************************************** */
/*                                                                            */
/*                                                       ::::::::             */
/*   utils.c                                           :+:    :+:             */
/*                                                    +:+                     */
/*   By: avaliull <avaliull@student.codam.nl>        +#+                      */
/*                                                  +#+                       */
/*   Created: 2025/02/15 13:05:22 by avaliull     #+#    #+#                  */
/*   Updated: 2025/02/15 13:06:21 by avaliull     ########   odam.nl          */
/*                                                                            */
/* ************************************************************************** */

#include "pipex.h"

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
			{
				ft_printf("REPLACE THIS WITH PROPER ERROR HANDLING\n");
			}
			break;
		}
	}
	if (!path_var)
	{
		ft_printf("REPLACE THIS WITH PROPER ERROR HANDLING\n");
	}
	path_arr = ft_split(path_var, ':');
	free(path_var);
	if (!path_arr)
	{
		ft_printf("REPLACE THIS WITH PROPER ERROR HANDLING\n");
	}
	return ((const char**) path_arr);
}

int	try_execve(const char **path_arr, char *const argv[])
{
	int		i;
	char	*tmp_slash;
	char	*command_path;

	i = 0;
	// replace ft_strjoin with a local thing so you don't malloc twice for this
	// also maybe split this into a "where"? and add access check
	if (access(argv[0], F_OK) == 0)
	{
		execve(argv[0], argv, __environ);
		return (0);
	}
	tmp_slash = ft_strjoin("/", argv[0]);
	while (path_arr[i])
	{
		if (!tmp_slash)
			ft_printf("REPLACE THIS WITH PROPER ERROR HANDLING\n");
		command_path = ft_strjoin(path_arr[i], tmp_slash);
		if (!command_path)
			ft_printf("REPLACE THIS WITH PROPER ERROR HANDLING\n");
		if (execve(command_path, argv, __environ) != -1)
		{
			free(command_path);
			free(tmp_slash);
			return (0);
		}
		else
			free(command_path);
		i++;
	}
	free(tmp_slash);
	return (-1);
}
