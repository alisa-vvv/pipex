/* ************************************************************************** */
/*                                                                            */
/*                                                       ::::::::             */
/*   pipex.c                                           :+:    :+:             */
/*                                                    +:+                     */
/*   By: avaliull <avaliull@student.codam.nl>        +#+                      */
/*                                                  +#+                       */
/*   Created: 2025/02/13 18:11:38 by avaliull     #+#    #+#                  */
/*   Updated: 2025/02/13 20:11:14 by avaliull     ########   odam.nl          */
/*                                                                            */
/* ************************************************************************** */

#include "pipex.h"
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

int	*setup_pipe(int fd_in, int fd_out)
{
	int	*pipefd;

	pipefd = (int *) malloc (sizeof (int) * 2);
	pipefd[0] = fd_in;
	pipefd[1] = fd_out;
	ft_printf("pipefd before: %d, %d\n", pipefd[0], pipefd[1]);
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
void	cmd2_process(const char *outfile_name, const char *cmd2, int fd_out)
{
	ft_printf("sweet child o mine (2)\n");
	ft_printf("outfile name: %s\n", outfile_name);
	ft_printf("cmd2: %s\n", cmd2);
	ft_printf("fd_out: %d\n", fd_out);
}

void	cmd1_process(const char *infile_name, const char *cmd1, int fd_in,
				  const char *outfile_name, const char *cmd2, int fd_out)
{
	pid_t	child2;

	ft_printf("sweet child o mine (1)\n");
	ft_printf("infile name: %s\n", infile_name);
	ft_printf("cmd1: %s\n", cmd1);
	ft_printf("fd_in: %d\n", fd_in);

	child2 = fork();
	if (child2 < 0)
	{
		perror("Fork: ");
		exit (1);
	}
	if (child2 == 0)
	{
		ft_printf ("this is the parent if child2, pipipe: %d\n", child2);
	}
	else
	{
		ft_printf("this should be child2 : %d\n", child2);
		cmd2_process(outfile_name, cmd2, fd_out);
	}
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
	const char	*cmd1 = argv[2];
	const char	*cmd2 = argv[3];

	//check that files are available
	check_files(infile_name, outfile_name);

	// get fds
	const int fd_in = open(infile_name, O_RDONLY);
	const int fd_out = open(outfile_name, O_WRONLY);

	int	*pipefd;
	pipefd = setup_pipe(fd_in, fd_out);

	pid_t	child1;

	child1 = fork();
	if (child1 < 0)
	{
		perror("Fork: ");
		exit (1);
	}
	if (child1 == 0)
		ft_printf("this is parent of child1: %d\n", child1);
	else
	{
		ft_printf("this is child1: %d\n", child1);
		cmd1_process(infile_name, cmd1, fd_in, outfile_name, cmd2, fd_out);
	}
	// get content of input file
	//const char *in_content = get_file_content(fd_in);

	//ft_printf("file content: %s\n", in_content);
	//ft_printf("Enviroment:\n");
	//int i = 0;
	//while (__environ[i])
	//{
	//	ft_printf("%d: %s\n", i, __environ[i]);
	//	i++;
	//}

	//free((void *) in_content);
	close(fd_in);
	close(fd_out);
	free(pipefd);
	//read();
	//perror();
	//strerror();
	//dup();
	//dup2();
	//execve();
	//fork();
	//pipe();
	//unlink();
	//wait();
	//waitpid();
}
