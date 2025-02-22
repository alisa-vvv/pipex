/* ************************************************************************** */
/*                                                                            */
/*                                                       ::::::::             */
/*   libfunc_wrappers.c                                :+:    :+:             */
/*                                                    +:+                     */
/*   By: avaliull <avaliull@student.codam.nl>        +#+                      */
/*                                                  +#+                       */
/*   Created: 2025/02/22 15:18:02 by avaliull     #+#    #+#                  */
/*   Updated: 2025/02/22 15:46:50 by avaliull     ########   odam.nl          */
/*                                                                            */
/* ************************************************************************** */

#include "pipex.h"
#include <unistd.h>
#include <stdio.h>

int dup2_errcheck (int oldfd, int newfd)
{
	if (dup2(oldfd, newfd) < 0)
	{
		perror(DUP2_ERR);
		return (-1);
	}
	return (0);
}

int close_errcheck (int fd)
{
	if (close(fd) < 0)
	{
		perror(CLOSE_ERR);
		return (-1);
	}
	return (0);
}
