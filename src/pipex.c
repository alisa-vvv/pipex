/* ************************************************************************** */
/*                                                                            */
/*                                                       ::::::::             */
/*   pipex.c                                           :+:    :+:             */
/*                                                    +:+                     */
/*   By: avaliull <avaliull@student.codam.nl>        +#+                      */
/*                                                  +#+                       */
/*   Created: 2025/02/19 14:51:28 by avaliull     #+#    #+#                  */
/*   Updated: 2025/02/23 12:46:10 by avaliull     ########   odam.nl          */
/*                                                                            */
/* ************************************************************************** */

#include "pipex.h"
#include "ft_printf.h"
#include <sys/wait.h>
#include <stdio.h>
#include <string.h>
#include <unistd.h>
#include <fcntl.h>
#include <stdlib.h>

char	*cmd2_process(char *const command_argv[], const char **path_arr,
						const int pipe_fd[2], const int fd_out)
{
	if (fd_out < 0)
		return (NULL);
	close_errcheck(pipe_fd[1]);
	dup2_errcheck(pipe_fd[0], STDIN_FILENO);
	dup2_errcheck(fd_out, STDOUT_FILENO);
	return (try_execve(path_arr, command_argv));
}

char	*cmd1_process(char *const command_argv[], const char **path_arr,
						const int pipe_fd[2], const int fd_in)
{
	if (fd_in < 0)
		return (NULL);
	close_errcheck(pipe_fd[0]);
	dup2_errcheck(fd_in, STDIN_FILENO);
	dup2_errcheck(pipe_fd[1], STDOUT_FILENO);
	return (try_execve(path_arr, command_argv));
}

char	*free_pipex(char *const *cmd1, char *const *cmd2, const char **path_arr,
					char *err_check)
{
	int	i;

	if (cmd1)
	{
		i = -1;
		while (cmd1[++i])
			free(cmd1[i]);
		free((void *) cmd1);
	}
	if (cmd2)
	{
		i = -1;
		while (cmd2[++i])
			free(cmd2[i]);
		free((void *) cmd2);
	}
	if (path_arr)
	{
		i = -1;
		while (path_arr[++i])
			free((void *) path_arr[i]);
		free(path_arr);
	}
	return (err_check);
}

char	*pipex(char *const cmd_argv[2],
			const int pipe_fd[2], const int io_fd[2])
{
	const char	**path_arr = find_env_path();
	char *const	*cmd1 = ft_split(cmd_argv[0], ' ');
	char *const	*cmd2 = ft_split(cmd_argv[1], ' ');
	int			fork_check;
	char		*perror_check;

	if (!cmd1 || !cmd2 || !path_arr)
		return (free_pipex(cmd1, cmd2, path_arr, MALLOC_ERR));
	fork_check = fork();
	perror_check = EXIT_SUCCESS;
	if (fork_check == -1)
		return (free_pipex(cmd1, cmd2, path_arr, FORK_ERR));
	else if (fork_check == 0)
		perror_check = cmd1_process(cmd1, path_arr, pipe_fd, io_fd[0]);
	else
	{
		fork_check = fork();
		if (fork_check == -1)
			return (free_pipex(cmd1, cmd2, path_arr, FORK_ERR));
		else if (fork_check == 0)
			perror_check = cmd2_process(cmd2, path_arr, pipe_fd, io_fd[1]);
		wait(NULL);
	}
	return (free_pipex(cmd1, cmd2, path_arr, perror_check));
}
