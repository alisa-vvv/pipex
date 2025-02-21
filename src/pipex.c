/* ************************************************************************** */
/*                                                                            */
/*                                                       ::::::::             */
/*   pipex.c                                           :+:    :+:             */
/*                                                    +:+                     */
/*   By: avaliull <avaliull@student.codam.nl>        +#+                      */
/*                                                  +#+                       */
/*   Created: 2025/02/19 14:51:28 by avaliull     #+#    #+#                  */
/*   Updated: 2025/02/20 20:31:11 by avaliull     ########   odam.nl          */
/*                                                                            */
/* ************************************************************************** */

#include "pipex.h"
#include "sys/wait.h"
#define _GNU_SOURCE

//plan for error checking:
//should have a clean_exit function that is called from main or pipex (they both have all the vars).
//any functions below pipex should return a value on error to see if an exit
//is required, also write the error value to strerror
//they should also clean up all memory they allocate independant of the clean exit
//func
//clean_exit then frees and closes eveyrthing, outputs error msg from sterror,
//and exit(1)s the program. clean_exit is also used when no error is encountered.

int	cmd2_process(char *const command_argv[], const int pipe_fd[2],
				  const int fd_out, const char **path_arr)
{
	if (fd_out < 0)
		return (-1);
	if (close(pipe_fd[1]) < 0)
		perror("Close in cmd2 process");
	if (dup2(pipe_fd[0], STDIN_FILENO) < 0)
		perror("dup2 pipe_fd[0]");
	if (dup2(fd_out, STDOUT_FILENO) < 0)
		perror("dup2 fd_out");
	try_execve(path_arr, command_argv);
	return (0);
}

int	cmd1_process(char *const command_argv[], const int pipe_fd[2],
				  const int fd_in, const char **path_arr)
{
	if (fd_in < 0)
		return (-1);
	if (close(pipe_fd[0]) < 0)
		ft_printf("close err in cmd1\n");
	if (dup2(fd_in, STDIN_FILENO) < 0)
		perror("dup2 fd_in");
	if (dup2(pipe_fd[1], STDOUT_FILENO) < 0)
		perror("dup2 pipe_fd[1]");
	try_execve(path_arr, command_argv);
	return (0);
}

int	free_pipex(char *const *cmd1, char *const *cmd2, const char **path_arr,
			   int err_check)
{
	int	i;

	if (cmd1)
	{
		i = -1;
		while (cmd1[++i])
			free(cmd1[i]);
		free((void *) cmd1);
	}
	if (cmd2)
	{
		i = -1;
		while (cmd2[++i])
			free(cmd2[i]);
		free((void *) cmd2);
	}
	if (path_arr)
	{
		i = -1;
		while (path_arr[++i])
			free((void *) path_arr[i]);
		free(path_arr);
	}
	return (err_check);
}

int	pipex(char *const cmd_argv[2], const int pipe_fd[2], const int io_fd[2])
{
	const char **path_arr = find_env_path();
	char *const	*cmd1 = ft_split(cmd_argv[0], ' ');
	char *const	*cmd2 = ft_split(cmd_argv[1], ' ');
	int	fork_check;
	int	perror_check;

	if (!cmd1 || !cmd2 || !path_arr)
		return (free_pipex(cmd1, cmd2, path_arr, -1));
	fork_check = fork();
	perror_check = 0;
	if (fork_check == -1)
		return (free_pipex(cmd1, cmd2, path_arr, -1));
	else if (fork_check == 0)
		perror_check = cmd1_process(cmd1, pipe_fd, io_fd[0], path_arr);
	else
	{
		fork_check = fork();
		if (fork_check == -1)
			return (free_pipex(cmd1, cmd2, path_arr, -1));
		else if (fork_check == 0)
			perror_check = cmd2_process(cmd2, pipe_fd, io_fd[1], path_arr);
		wait(NULL);
	}
	return (free_pipex(cmd1, cmd2, path_arr, perror_check));
}

void	clean_exit(int *pipe_fd, int const io_fd[2])
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
	exit(0);
}

int	*setup_pipe(void)
{
	int	*pipefd;

	pipefd = (int *) malloc (sizeof (int) * 2);
	if (!pipefd)
		return (NULL);
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
	int	io_fd[2];
	int	*pipe_fd;

	pipe_fd = setup_pipe();
	if (!pipe_fd)
		clean_exit(NULL, io_fd);
	io_fd[0] = open(argv[1], O_RDONLY);
	if (io_fd[0] < 0)
		perror(argv[1]);
	io_fd[1] = open(argv[4], O_WRONLY | O_CREAT | O_TRUNC, 0777);
	if (io_fd[1] < 0)
		perror(argv[4]);
	if (argc != 5)
	{
		perror("Argument count not 4");
		exit(1);
	}
	pipex((char *const[]){argv[2], argv[3]}, pipe_fd, io_fd);
	clean_exit(pipe_fd, io_fd);
}
