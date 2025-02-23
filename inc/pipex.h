/* ************************************************************************** */
/*                                                                            */
/*                                                       ::::::::             */
/*   pipex.h                                           :+:    :+:             */
/*                                                    +:+                     */
/*   By: avaliull <avaliull@student.codam.nl>        +#+                      */
/*                                                  +#+                       */
/*   Created: 2025/02/13 18:12:39 by avaliull     #+#    #+#                  */
/*   Updated: 2025/02/23 12:42:34 by avaliull     ########   odam.nl          */
/*                                                                            */
/* ************************************************************************** */

#ifndef PIPEX_H
# define PIPEX_H

# define MALLOC_ERR "malloc"
# define DUP2_ERR "dup2"
# define CLOSE_ERR "close"
# define FORK_ERR "fork"
# define FD_ERR "invalid fd"

char		*pipex(char *const cmd_argv[2],
				const int pipe_fd[2], const int io_fd[2]);
const char	**find_env_path(void);
char		*try_execve(const char **path_arr, char *const argv[]);
int			close_errcheck(int fd);
int			dup2_errcheck(int oldfd, int newfd);

#endif
