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

#ifndef PIPEX_H
# define PIPEX_H

# include "ft_printf.h"
# include <errno.h>
# include <string.h>

# define _GNU_SOURCE
# define MALLOC_ERR "malloc"
# define DUP2_ERR "dup2"

const char	**find_env_path(void);
char		*try_execve(const char **path_arr, char *const argv[]);

#endif
