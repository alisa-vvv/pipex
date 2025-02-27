/* ************************************************************************** */
/*                                                                            */
/*                                                       ::::::::             */
/*   init_exit.c                                       :+:    :+:             */
/*                                                    +:+                     */
/*   By: avaliull <avaliull@student.codam.nl>        +#+                      */
/*                                                  +#+                       */
/*   Created: 2025/02/23 12:27:48 by avaliull     #+#    #+#                  */
/*   Updated: 2025/02/27 18:20:11 by avaliull     ########   odam.nl          */
/*                                                                            */
/* ************************************************************************** */

#include "pipex.h"
#include <sys/wait.h>
#include <stdio.h>
#include <stdlib.h>
#include <fcntl.h>
#include <unistd.h>
#include <string.h>
#include <errno.h>

void	clean_exit(int *pipe_fd, const int err_check)
{
	if (pipe_fd)
	{
		close(pipe_fd[0]);
		close(pipe_fd[1]);
	}
	exit(err_check);
}

int	main(int argc, char *argv[])
{
	int		pipe_fd[2];
	char	*pipex_errcheck;

	if (argc != 5)
	{
		perror("Argument count not 4");
		exit(1);
	}
	if (pipe(pipe_fd) < 0)
	{
		perror(strerror(errno));
		clean_exit(NULL, EXIT_FAILURE);
	}
	pipex_errcheck = pipex((char *const [2]){argv[2], argv[3]},
						(char *const [2]){argv[1], argv[4]}, pipe_fd);
	if (pipex_errcheck)
	{
		perror(pipex_errcheck);
		clean_exit(pipe_fd, EXIT_FAILURE);
	}
	clean_exit(pipe_fd, EXIT_SUCCESS);
}
