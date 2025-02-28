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

void	exit_child(int fds[3], char **cmd_arr, char **path_arr, int status)
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

void	cmd2_process(char *const cmd_argv[2], char **path,
						const int pipe_fd[2], char *outfile)
{
	const int	fd_out = open(outfile, O_WRONLY | O_CREAT | O_TRUNC, 0777);
	char		**cmd2;
	int			err;

	close(pipe_fd[1]);
	if (fd_out < 0)
	{
		perror(outfile);
		exit_child((int[]){pipe_fd[0], pipe_fd[1], fd_out}, NULL, path, errno);
	}
	cmd2 = ft_split(cmd_argv[1], ' ');
	if (!cmd2)
	{
		perror(MALLOC_ERR);
		exit_child((int[]){pipe_fd[0], pipe_fd[1], fd_out}, NULL, path, errno);
	}
	dup2_errcheck(pipe_fd[0], STDIN_FILENO);
	dup2_errcheck(fd_out, STDOUT_FILENO);
	close(pipe_fd[0]);
	close(fd_out);
	err = try_execve((const char **) path, cmd2);
	exit_child((int[]){pipe_fd[0], pipe_fd[1], fd_out}, cmd2, path, err);
}

void	cmd1_process(char *const cmd_argv[2], char **path_arr,
						const int pipe_fd[2], char *infile)
{
	const int	fd_in = open(infile, O_RDONLY);
	int			err;
	char		**cmd1;

	close(pipe_fd[0]);
	if (fd_in < 0)
	{
		perror(infile);
		exit_child((int[]){pipe_fd[0], pipe_fd[1], fd_in}, NULL, path_arr, 1);
	}
	cmd1 = ft_split(cmd_argv[0], ' ');
	if (!cmd1)
	{
		perror(MALLOC_ERR);
		exit_child((int[]){pipe_fd[0], pipe_fd[1], fd_in}, NULL, path_arr, 1);
	}
	dup2_errcheck(fd_in, STDIN_FILENO);
	dup2_errcheck(pipe_fd[1], STDOUT_FILENO);
	close(pipe_fd[1]);
	close(fd_in);
	err = try_execve((const char **) path_arr, cmd1);
	exit_child((int[]){pipe_fd[0], pipe_fd[1], fd_in}, cmd1, path_arr, err);
}

int	write_fork(char *const cmd_argv[2], char *const files_argv[2],
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

int	read_fork(char *const cmd_argv[2], char *const files_argv[2],
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

int	pipex(char *const cmd_argv[2], char *const files_argv[2],
			const int pipe_fd[2])
{
	char	**path_arr;
	int		child_error_check;
	int		status;

	path_arr = find_env_path();
	if (!path_arr)
	{
		perror(MALLOC_ERR);
		free_string_array(path_arr);
	}
	child_error_check = read_fork(cmd_argv, files_argv, pipe_fd, path_arr);
	if (child_error_check > 0)
		child_error_check = write_fork(cmd_argv, files_argv, pipe_fd, path_arr);
	close(pipe_fd[0]);
	close(pipe_fd[1]);
	child_error_check = 0;
	while (waitpid(-1, &status, 0) > 0)
	{
		if (WIFEXITED(status))
			if (WEXITSTATUS(status) != 0)
				child_error_check = WEXITSTATUS(status);
	}
	ft_printf("exit status (REMOVE): %d\n", child_error_check);
	free_string_array(path_arr);
	return (child_error_check);
}
