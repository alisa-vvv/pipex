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
//contains the environ variable
#define _GNU_SOURCE

//./pipex file1 cmd1 cmd2 file2

// Step 1: get fds of both files and strings as names of both cmds
// Step 2: pipe()
// Step 3: fork and run cmd1 using excve in the child process
// Step 3.25: make sure to do waitdpid so the program does not close before cmd1 is complete
// Step 3.5: make sure to split the command into command and it's args
// Step 4: write output of child process to cmd2 input (via the pipe (redirect using dup2))
// step 4.5 fork and run cmd2 in the second child process
// Step 6: write output to outfile_name (redirect using dup2)
// Step 7: close all processess I guess (should hppen by itself)?
// Step 9: profit

int	check_files(const char *infile_name, const char *outfile_name)
{
	//this should be replaced with whatever error management is required
	ft_printf("checking if exists file1: %d\n", access(infile_name, F_OK));
	ft_printf("checking if can read file1: %d\n", access(infile_name, R_OK));
	ft_printf("checking if exists file2: %d\n", access(outfile_name, F_OK));
	ft_printf("checking if can write file2: %d\n", access(infile_name, W_OK));
	return (0);
}

// might not need this?
char	*get_file_content(int fd)
{
	char	*file_content;
	char	*next_line;
	char	*tmp_str;

	file_content = get_next_line(fd);
	next_line = get_next_line(fd);
	while (next_line)
	{
		tmp_str = file_content;
		file_content = ft_strjoin(file_content, next_line);
		free(tmp_str);
		free(next_line);
		next_line = get_next_line(fd);
	}
	return (file_content);
}

int	*setup_pipe()
{
	int	*pipefd;

	pipefd = (int *) malloc (sizeof (int) * 2);
	if (pipe(pipefd) == -1)
	{
		ft_printf("%s\n", strerror(errno));
		exit (1);
	}
	ft_printf("pipefd after: %d, %d\n", pipefd[0], pipefd[1]);
	return (pipefd);
}

// I think the processes should be flipped maybe
// cause parent has to wait for child to be over, so
// we create first child for cmd2
// then inside there we create second child for cmd1
// then first child waitpids for cmd1 to be executed inside second child
// second child writes the output to input of first child via the pipe
// then first child executes cmd2 and pipes the output to outfile
// i think
// do we need another pipe? I don't think so? maybe? feels like we shouldn't need one, we are coding a single pipe after all
void	cmd2_process(char *const *command_argv, int *pipefd, int fd_out, char **path_arr)
{
	close(pipefd[1]);
	dup2(pipefd[0], STDIN_FILENO);
	dup2(fd_out, STDOUT_FILENO);
	try_execve(path_arr, command_argv);
}

void	cmd1_process(char *const *command_argv, int *pipefd, int fd_in, char **path_arr)
{
	close(pipefd[0]);
	dup2(fd_in, STDIN_FILENO);
	dup2(pipefd[1], STDOUT_FILENO);
	try_execve(path_arr, command_argv);
}

void	pipex(char *const *command_argv, int *pipefd, int fd_in, int fd_out)
{
	char **path_arr = find_env_path();
	int	fork_check;

	//dup2(fd_in, pipefd[0]);
	//dup2(fd_out, pipefd[1]);
	fork_check = fork();
	if (fork_check == -1)
		ft_printf("fork error\n");
	else if (fork_check == 0)
		cmd1_process(command_argv, pipefd, fd_in, path_arr);
	else
	{
		fork_check = fork();
		if (fork_check == -1)
			ft_printf("fork error\n");
		else if (fork_check == 0)
			cmd2_process(command_argv, pipefd, fd_out, path_arr);
		wait(NULL);
	}
	int i = -1;
	if (path_arr)
	{
		while (path_arr[++i])
			free(path_arr[i]);
		free(path_arr);
	}
	return ;
}

int	main(int argc, char *argv[])
{
	if (argc != 5)
	{
		perror("Argument count not 4");
		exit(1);
	}
	// save argvs to corr. vars
	// we probably don't actually need the file names, just the fds
	const char	*infile_name = argv[1];
	const char	*outfile_name = argv[4];
//	const char	*cmd1 = argv[2];
//	const char	*cmd2 = argv[3];

	//check that files are available
	check_files(infile_name, outfile_name);

	// get fds
	const int fd_in = open(infile_name, O_RDONLY);
	const int fd_out = open(outfile_name, O_WRONLY);

	int	*pipefd;
	pipefd = setup_pipe();

	char *const command_argv[] = {"cat", "-e", NULL};
	pipex(command_argv, pipefd, fd_in, fd_out);
	//pid_t	child1;

	//child1 = fork();
	//if (child1 < 0)
	//{
	//	perror("Fork: ");
	//	exit (1);
	//}
	//if (child1 == 0)
	//{
	//	ft_printf("this is child1: %d\n", child1);
	//	cmd2_process(infile_name, cmd1, fd_in, outfile_name, cmd2, fd_out);
	//}
	//else
	//	ft_printf("this is parent of child1: %d\n", child1);

	close(fd_in);
	close(fd_out);
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
