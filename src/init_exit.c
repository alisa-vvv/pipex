/* ************************************************************************** */
/*                                                                            */
/*                                                       ::::::::             */
/*   init_exit.c                                       :+:    :+:             */
/*                                                    +:+                     */
/*   By: avaliull <avaliull@student.codam.nl>        +#+                      */
/*                                                  +#+                       */
/*   Created: 2025/02/23 12:27:48 by avaliull     #+#    #+#                  */
/*   Updated: 2025/02/23 12:33:29 by avaliull     ########   odam.nl          */
/*                                                                            */
/* ************************************************************************** */

#include "pipex.h"
#include <stdio.h>
#include <stdlib.h>
#include <fcntl.h>
#include <unistd.h>
#include <string.h>
#include <errno.h>

void	clean_exit(int *pipe_fd, int const io_fd[2], const int err_check)
{
	if (io_fd[0] > 0)
		close(io_fd[0]);
	if (io_fd[1] > 0)
		close(io_fd[1]);
	if (pipe_fd)
	{
		close(pipe_fd[0]);
		close(pipe_fd[1]);
		free(pipe_fd);
	}
	exit(err_check);
}

int	*setup_pipe(void)
{
	int	*pipefd;

	pipefd = (int *) malloc (sizeof (int) * 2);
	if (!pipefd)
	{
		return (NULL);
		perror(MALLOC_ERR);
	}
	if (pipe(pipefd) < 0)
	{
		perror(strerror(errno));
		free(pipefd);
		return (NULL);
	}
	return (pipefd);
}

int	main(int argc, char *argv[])
{
	int		io_fd[2];
	int		*pipe_fd;
	char	*pipex_errcheck;

	if (argc != 5)
	{
		perror("Argument count not 4");
		exit(1);
	}
	io_fd[0] = open(argv[1], O_RDONLY);
	if (io_fd[0] < 0)
		perror(argv[1]);
	io_fd[1] = open(argv[4], O_WRONLY | O_CREAT | O_TRUNC, 0777);
	if (io_fd[1] < 0)
		perror(argv[4]);
	pipe_fd = setup_pipe();
	if (!pipe_fd)
		clean_exit(NULL, io_fd, EXIT_FAILURE);
	pipex_errcheck = pipex((char *const []){argv[2], argv[3]}, pipe_fd, io_fd);
	if (pipex_errcheck)
	{
		perror(pipex_errcheck);
		clean_exit(pipe_fd, io_fd, EXIT_FAILURE);
	}
	clean_exit(pipe_fd, io_fd, EXIT_SUCCESS);
}
