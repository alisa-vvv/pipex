/* ************************************************************************** */
/*                                                                            */
/*                                                       ::::::::             */
/*   pipex.c                                           :+:    :+:             */
/*                                                    +:+                     */
/*   By: avaliull <avaliull@student.codam.nl>        +#+                      */
/*                                                  +#+                       */
/*   Created: 2025/02/19 14:51:28 by avaliull     #+#    #+#                  */
/*   Updated: 2025/02/28 13:54:11 by avaliull     ########   odam.nl          */
/*                                                                            */
/* ************************************************************************** */

#include "pipex.h"
#include "ft_printf.h"
#include <sys/wait.h>
#include <sys/types.h>
#include <stdio.h>
#include <string.h>
#include <unistd.h>
#include <fcntl.h>
#include <stdlib.h>
#include <errno.h>

static void	exit_child(int fds[3], char **cmd_arr, char **path_arr, int status)
{
	close(fds[0]);
	close(fds[1]);
	close(fds[2]);
	if (cmd_arr)
		free_string_array(cmd_arr);
	if (path_arr)
		free_string_array(path_arr);
	exit(status);
}

static void	cmd2_process(char *const cmd_argv[2], char **path,
						const int pipe_fd[2], const char *outfile)
{
	const int	fd_out = open(outfile, O_WRONLY | O_CREAT | O_TRUNC, 0777);
	char		**cmd2;
	int			err;

	close(pipe_fd[1]);
	if (fd_out < 0)
	{
		perror(outfile);
		exit_child((int []){pipe_fd[0], pipe_fd[1], fd_out}, NULL, path, errno);
	}
	cmd2 = ft_split(cmd_argv[1], ' ');
	if (!cmd2)
	{
		perror(MALLOC_ERR);
		exit_child((int []){pipe_fd[0], pipe_fd[1], fd_out}, NULL, path, errno);
	}
	dup2(pipe_fd[0], STDIN_FILENO);
	dup2(fd_out, STDOUT_FILENO);
	close(pipe_fd[0]);
	close(fd_out);
	err = try_execve((const char **) path, cmd2);
	exit_child((int []){pipe_fd[0], pipe_fd[1], fd_out}, cmd2, path, err);
}

static void	cmd1_process(char *const cmd_argv[2], char **path_arr,
						const int pipe_fd[2], const char *infile)
{
	const int	fd_in = open(infile, O_RDONLY);
	int			err;
	char		**cmd1;

	close(pipe_fd[0]);
	if (fd_in < 0)
	{
		perror(infile);
		exit_child((int []){pipe_fd[0], pipe_fd[1], fd_in}, NULL, path_arr, 1);
	}
	cmd1 = ft_split(cmd_argv[0], ' ');
	if (!cmd1)
	{
		perror(MALLOC_ERR);
		exit_child((int []){pipe_fd[0], pipe_fd[1], fd_in}, NULL, path_arr, 1);
	}
	dup2(fd_in, STDIN_FILENO);
	dup2(pipe_fd[1], STDOUT_FILENO);
	close(pipe_fd[1]);
	close(fd_in);
	err = try_execve((const char **) path_arr, cmd1);
	exit_child((int []){pipe_fd[0], pipe_fd[1], fd_in}, cmd1, path_arr, err);
}

pid_t	write_fork(char *const cmd_argv[2], char *const files_argv[2],
						const int pipe_fd[2], char **path_arr)
{
	int	fork_check;

	fork_check = fork();
	if (fork_check == 0)
		cmd2_process(cmd_argv, path_arr, pipe_fd, files_argv[1]);
	else if (fork_check < 0)
		perror(FORK_ERR);
	return (fork_check);
}

pid_t	read_fork(char *const cmd_argv[2], char *const files_argv[2],
						const int pipe_fd[2], char **path_arr)
{
	int		fork_check;

	fork_check = fork();
	if (fork_check < 0)
	{
		perror(FORK_ERR);
		free_string_array((char **) path_arr);
	}
	else if (fork_check == 0)
		cmd1_process(cmd_argv, path_arr, pipe_fd, files_argv[0]);
	else
		return (fork_check);
	return (fork_check);
}
