/* ************************************************************************** */
/*                                                                            */
/*                                                       ::::::::             */
/*   pipex.c                                           :+:    :+:             */
/*                                                    +:+                     */
/*   By: avaliull <avaliull@student.codam.nl>        +#+                      */
/*                                                  +#+                       */
/*   Created: 2025/02/19 14:51:28 by avaliull     #+#    #+#                  */
/*   Updated: 2025/02/27 17:48:54 by avaliull     ########   odam.nl          */
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
#include <errno.h>

char	*cmd2_process(char *const command_argv[], const char **path_arr,
						const int pipe_fd[2], char *outfile)
{
	const int	fd_out = open(outfile, O_WRONLY | O_CREAT | O_TRUNC, 0777);
	char		*result;

	close_errcheck(pipe_fd[1]);
	if (fd_out < 0)
	{
		perror(outfile);
		return (NULL);
	}
	dup2_errcheck(pipe_fd[0], STDIN_FILENO);
	dup2_errcheck(fd_out, STDOUT_FILENO);
	result = try_execve(path_arr, command_argv);
	close(pipe_fd[0]);
	return (result);
}

char	*cmd1_process(char *const command_argv[], const char **path_arr,
						const int pipe_fd[2], char *infile)
{
	const int	fd_in = open(infile, O_RDONLY);
	char		*result;

	close_errcheck(pipe_fd[0]);
	if (fd_in < 0)
	{
		perror(infile);
		return (NULL);
	}
	dup2_errcheck(fd_in, STDIN_FILENO);
	dup2_errcheck(pipe_fd[1], STDOUT_FILENO);
	result = try_execve(path_arr, command_argv);
	close(pipe_fd[1]);
	return (result);
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

char	*pipex(char *const cmd_argv[2], char *const files_argv[2],
			const int pipe_fd[2])
{
	const char	**path_arr = find_env_path();
	char *const	*cmd1 = ft_split(cmd_argv[0], ' ');
	char *const	*cmd2 = ft_split(cmd_argv[1], ' ');
	int			fork_check;
	char		*perror_check;

	if (!cmd1 || !cmd2 || !path_arr)
		return (free_pipex(cmd1, cmd2, path_arr, MALLOC_ERR));
	perror_check = NULL;
	fork_check = fork();
	if (fork_check == -1)
		return (free_pipex(cmd1, cmd2, path_arr, FORK_ERR));
	else if (fork_check == 0)
		perror_check = cmd1_process(cmd1, path_arr, pipe_fd, files_argv[0]);
	else
	{
		fork_check = fork();
		if (fork_check == -1)
			return (free_pipex(cmd1, cmd2, path_arr, FORK_ERR));
		else if (fork_check == 0)
			perror_check = cmd2_process(cmd2, path_arr, pipe_fd, files_argv[1]);
		else
		{
			close(pipe_fd[0]);
			close(pipe_fd[1]);
			while (waitpid(-1, NULL, 0) > 0)
			{
			}
		}
	}
	return (free_pipex(cmd1, cmd2, path_arr, perror_check));
}
