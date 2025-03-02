/* ************************************************************************** */
/*                                                                            */
/*                                                       ::::::::             */
/*   pipex.h                                           :+:    :+:             */
/*                                                    +:+                     */
/*   By: avaliull <avaliull@student.codam.nl>        +#+                      */
/*                                                  +#+                       */
/*   Created: 2025/02/13 18:12:39 by avaliull     #+#    #+#                  */
/*   Updated: 2025/02/28 12:52:38 by avaliull     ########   odam.nl          */
/*                                                                            */
/* ************************************************************************** */

#ifndef PIPEX_H
# define PIPEX_H

# define MALLOC_ERR "malloc()"
# define DUP2_ERR "dup2()"
# define FORK_ERR "fork()"
# define PIPE_ERR "pipe()"
# define FD_ERR "invalid fd"
# define CLOSE_ERR "close()"

# include <aio.h>

/*	Forking	*/
pid_t	read_fork(char *const cmd_argv[2], char *const files_argv[2],
			const int pipe_fd[2], char **path_arr);
pid_t	write_fork(char *const cmd_argv[2], char *const files_argv[2],
			const int pipe_fd[2], char **path_arr);
/*	Utils	*/
int		try_execve(const char **path_arr, char *const argv[]);
void	free_string_array(char **arr);
char	**find_env_path(void);

#endif
