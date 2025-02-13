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

//./pipex file1 cmd1 cmd2 file2

// Step 1: get fds of both files and strings as names of both cmds
// Step 2: create to more files for cmd1 out and cmd2 input ? needed?
// Step 3: fork and run cmd1 using excve in the child process
// Step 3.5: make sure to split the command into command and it's args
// pipe() between all parent and child processes?
// Step 4: write output of child process to cmd2 input
// Step 5: fork (maybe do it inside child?) and run cmd2 in new child
// Step 6: get output
// Step 7: close all processess I guess?
// Step 8: write output to outfile_name
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

int	main(int argc, char *argv[], char *envp[])
{
	if (argc != 5)
		exit(1);

	const char	*infile_name = argv[1];
	const char	*outfile_name = argv[4];
	const char	*cmd1 = argv[2];
	const char	*cmd2 = argv[3];
	check_files(infile_name, outfile_name);
	const int fd_in = open(infile_name, O_RDONLY);
	const int fd_out = open(outfile_name, O_WRONLY);

	ft_printf("what's in env: %s\n", envp[3]);
	ft_printf("infile name: %s\n", infile_name);
	ft_printf("outfile name: %s\n", outfile_name);
	ft_printf("cmd1: %s\n", cmd1);
	ft_printf("cmd2: %s\n", cmd2);
	ft_printf("fd_in: %d\n", fd_in);
	ft_printf("fd_out: %d\n", fd_out);

	close(fd_in);
	close(fd_out);
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
