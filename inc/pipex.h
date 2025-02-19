/* ************************************************************************** */
/*                                                                            */
/*                                                       ::::::::             */
/*   pipex.h                                           :+:    :+:             */
/*                                                    +:+                     */
/*   By: avaliull <avaliull@student.codam.nl>        +#+                      */
/*                                                  +#+                       */
/*   Created: 2025/02/13 18:12:39 by avaliull     #+#    #+#                  */
/*   Updated: 2025/02/13 19:38:28 by avaliull     ########   odam.nl          */
/*                                                                            */
/* ************************************************************************** */

#ifndef	PIPEX_H
# define PIPEX_H

# include "ft_printf.h"
//for close, access, read, execve, fork:
# include <unistd.h>
//for open:
# include <fcntl.h>
//for perror:
# include <stdio.h>
//for errno:
# include <errno.h>
// for  strerror:
# include <string.h>

const char	**find_env_path(void);
int			try_execve(const char **path_arr, char *const argv[]);

#endif
