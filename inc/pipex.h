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

# define MALLOC_ERR "malloc"
# define DUP2_ERR "dup2"
# define FORK_ERR "fork"
# define PIPE_ERR "pipe"
# define FD_ERR "invalid fd"

/*	Forking	*/
int		read_fork(char *const cmd_argv[2], char *const files_argv[2],
			const int pipe_fd[2], char **path_arr);
int		write_fork(char *const cmd_argv[2], char *const files_argv[2],
			const int pipe_fd[2], char **path_arr);
/*	Utils	*/
char	**find_env_path(void);
int		try_execve(const char **path_arr, char *const argv[]);
int		close_errcheck(int fd);
int		dup2_errcheck(int oldfd, int newfd);
void	free_string_array(char **arr);

#endif
