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

void	exit_parent(const int pipe_fd[2], const int err_check, char **path)
{
	if (path)
		free_string_array(path);
	if (pipe_fd)
	{
		close(pipe_fd[0]);
		close(pipe_fd[1]);
	}
	exit(err_check);
}

void	pipex(char *const cmd_argv[2], char *const files_argv[2],
			const int pipe_fd[2])
{
	char	**path_arr;
	pid_t	child1_error;
	pid_t	child2_error;
	int		err_check;
	int		status;

	path_arr = find_env_path();
	if (!path_arr)
	{
		perror(MALLOC_ERR);
		exit_parent(pipe_fd, errno, path_arr);
	}
	child1_error = read_fork(cmd_argv, files_argv, pipe_fd, path_arr);
	child2_error = write_fork(cmd_argv, files_argv, pipe_fd, path_arr);
	close(pipe_fd[0]);
	close(pipe_fd[1]);
	err_check = errno * (child1_error < 0 || child2_error < 0);
	while (waitpid(-1, &status, 0) > 0)
	{
		if (WIFEXITED(status))
			if (WEXITSTATUS(status) != 0)
				err_check = WEXITSTATUS(status);
	}
	exit_parent(NULL, err_check, path_arr);
}

int	main(int argc, char *argv[])
{
	int	pipe_fd[2];

	if (argc != 5)
	{
		perror("Argument count not 4");
		exit(1);
	}
	if (pipe(pipe_fd) < 0)
	{
		perror(PIPE_ERR);
		exit_parent(NULL, errno, NULL);
	}
	pipex((char *const [2]){argv[2], argv[3]},
		(char *const [2]){argv[1], argv[4]}, pipe_fd);
}
