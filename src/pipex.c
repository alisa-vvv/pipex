/* ************************************************************************** */
/*                                                                            */
/*                                                       ::::::::             */
/*   init_exit.c                                       :+:    :+:             */
/*                                                    +:+                     */
/*   By: avaliull <avaliull@student.codam.nl>        +#+                      */
/*                                                  +#+                       */
/*   Created: 2025/02/23 12:27:48 by avaliull     #+#    #+#                  */
/*   Updated: 2025/02/28 13:23:42 by avaliull     ########   odam.nl          */
/*                                                                            */
/* ************************************************************************** */

#include "pipex.h"
#include "ft_printf.h"
#include <sys/wait.h>
#include <stdio.h>
#include <stdlib.h>
#include <fcntl.h>
#include <unistd.h>
#include <string.h>
#include <errno.h>

void	parent_exit(int *pipe_fd, const int err_check)
{
	if (pipe_fd)
	{
		close(pipe_fd[0]);
		close(pipe_fd[1]);
	}
	exit(err_check);
}

int	pipex(char *const cmd_argv[2], char *const files_argv[2],
			const int pipe_fd[2])
{
	char	**path_arr;
	pid_t	child_error_check;
	int		status;

	path_arr = find_env_path();
	if (!path_arr)
	{
		perror(MALLOC_ERR);
		free_string_array(path_arr);
		return (errno);
	}
	child_error_check = read_fork(cmd_argv, files_argv, pipe_fd, path_arr);
	if (child_error_check > 0)
		child_error_check = write_fork(cmd_argv, files_argv, pipe_fd, path_arr);
	close(pipe_fd[0]);
	close(pipe_fd[1]);
	if (child_error_check > 0)
		child_error_check = 0;
	while (waitpid(-1, &status, 0) > 0)
	{
		if (WIFEXITED(status))
			if (WEXITSTATUS(status) != 0)
				child_error_check = WEXITSTATUS(status);
	}
	free_string_array(path_arr);
	return (child_error_check);
}

int	main(int argc, char *argv[])
{
	int	pipe_fd[2];
	int	pipex_errcheck;

	if (argc != 5)
	{
		perror("Argument count not 4");
		exit(1);
	}
	if (pipe(pipe_fd) < 0)
	{
		perror(PIPE_ERR);
		parent_exit(NULL, errno);
	}
	pipex_errcheck = pipex((char *const [2]){argv[2], argv[3]},
			(char *const [2]){argv[1], argv[4]}, pipe_fd);
	parent_exit(pipe_fd, pipex_errcheck);
}
