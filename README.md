	
This is a simple project with the goal of recreating the following bash command structure in C:
```
infile < command1 | command2 > outfile
```
For example, running a project like this:
```
./pipex infile "cat" "wc -l" outfile
```
Should produce results equivalent to the following shell commands:
```
< infile cat | wc -l > outfile
```
For this project, the list of allowed fnctions includes any pritnf variant written by me, reactiations of functions from the standard C library written by me (in this case libft), and the following:
```
open, close, read, write,
malloc, free, perror,
strerror, access, dup, dup2,
execve, exit, fork, pipe,
unlink, wait, waitpid
```
The project is deliberately limited in scope, and its purpose is to explore the functioning of pipes in C, as well as the basics of managing parent and child processes.\
It is a stepping stone to a larger project which will require creating a simple bash-like shell.
