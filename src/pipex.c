/* ************************************************************************** */
/*                                                                            */
/*                                                       ::::::::             */
/*   pipex.c                                           :+:    :+:             */ /*                                                    +:+                     */ /*   By: avaliull <avaliull@student.codam.nl>        +#+                      */
/*                                                  +#+                       */
/*   Created: 2025/02/13 18:11:38 by avaliull     #+#    #+#                  */
/*   Updated: 2025/02/13 20:11:14 by avaliull     ########   odam.nl          */
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

int	check_files(const char *infile_name, const char *outfile_name)
{
	//this should be replaced with whatever error management is required
	ft_printf("checking if exists file1: %d\n", access(infile_name, F_OK));
	ft_printf("checking if can read file1: %d\n", access(infile_name, R_OK));
	ft_printf("checking if exists file2: %d\n", access(outfile_name, F_OK));
	ft_printf("checking if can write file2: %d\n", access(infile_name, W_OK));
	return (0);
}

int	*setup_pipe(void)
{
	int	*pipefd;

	pipefd = (int *) malloc (sizeof (int) * 2);
	if (pipe(pipefd) == -1) {
		ft_printf("%s\n", strerror(errno));
		exit (1);
	}
	ft_printf("pipefd after: %d, %d\n", pipefd[0], pipefd[1]);
	return (pipefd);
}

void	cmd2_process(char *const command_argv[], int *pipefd, int fd_out, const char **path_arr)
{
	close(pipefd[1]);
	dup2(pipefd[0], STDIN_FILENO);
	dup2(fd_out, STDOUT_FILENO);
	try_execve(path_arr, command_argv);
}

void	cmd1_process(char *const command_argv[], int *pipefd, int fd_in, const char **path_arr)
{
	close(pipefd[0]);
	dup2(fd_in, STDIN_FILENO);
	dup2(pipefd[1], STDOUT_FILENO);
	try_execve(path_arr, command_argv);
}

void	pipex(char *const cmd1[], char *const cmd2[], int *pipefd, const int io_fd[2])
{
	const char **path_arr = find_env_path();
	int	fork_check;

	fork_check = fork();
	if (fork_check == -1)
		ft_printf("fork error\n");
	else if (fork_check == 0)
		cmd1_process(cmd1, pipefd, io_fd[0], path_arr);
	else
	{
		fork_check = fork();
		if (fork_check == -1)
			ft_printf("fork error\n");
		else if (fork_check == 0)
			cmd2_process(cmd2, pipefd, io_fd[1], path_arr);
		wait(NULL);
	}
	int i = -1;
	if (path_arr)
	{
		while (path_arr[++i])
			free((void *) path_arr[i]);
		free(path_arr);
	}
	return ;
}

int	main(int argc, char *argv[])
{
	char *const	*cmd1 = ft_split(argv[2], ' ');
	char *const	*cmd2 = ft_split(argv[3], ' ');
	int const	io_fd[2] = {open(argv[1], O_RDONLY),
		open(argv[4], O_WRONLY | O_CREAT | O_TRUNC, 0777)};
	int			*pipefd;

	if (argc != 5)
	{
		perror("Argument count not 4");
		exit(1);
	}

	// we probablyt don't need this since open will check for erros
	check_files(argv[1], argv[4]);

	pipefd = setup_pipe();
	pipex(cmd1, cmd2, pipefd, io_fd);

	int	i;

	i = -1;
	if (cmd1)
	{
		while (cmd1[++i])
			free(cmd1[i]);
		free((void *) cmd1);
	}
	i = -1;
	if (cmd2)
	{
		while (cmd2[++i])
			free(cmd2[i]);
		free((void *) cmd2);
	}
	close(io_fd[0]);
	close(io_fd[1]);
	close(pipefd[0]);
	close(pipefd[1]);
	free(pipefd);
	exit(0);
	//dup();
	//dup2();
	//fork();
	//unlink();
	//wait();
	//waitpid();
}
