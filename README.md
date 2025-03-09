	
This is a simple project with the goal of recreating the following bash command structure in C:
```
infile < command1 | command2 > outfile
```
For example, running the project executable like this:
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
It is a stepping stone to a larger project which will require creating a simple bash-like shell.\
\
It was fascinating to learn about the workings of the pipes. What struck me the most is the fact that pipes allow to processes to run concurently while reading/writing from/to the same filestream.\
For my current level of understanding, this honestly still feels like magic. I hope I will have the chance later to explore how this is achieved on the technical level.\
This project also introduced the basic tools for proper error management, and forced me to rethink my approach towards erros to both be more clear and flexible.
In all previous projects, I treated erros as causes for program to immideately quit. This was because most of the projects I made were either very small scale or not designed to behave like real programs with UX concerns, rather just illustrations of a particular concept or development tools.\
In the case of bash, most errors are instead ignored, and the pipelien will attempt to continue execution unless its functionality is completely impeded. So I had to figure out a clear way to express those errors to the user, and make sure the program can continue execution regardless of what goes wrong along the way. This turned out to not be difficult on the programmatic level (at least for this project). Instead, the difficulty lies in the decision process behind which erros need or don't need to be shown and where, and the need to keep track of error return values throughout the program without causing bloat. I found the process satisfying and interesting. I also breifly looked into the source code for bash when I was trying to figure out which errors are displayed in the original software, and found this little snippet in the job control code:
```
if (tcsetpgrp (shell_tty, pgrp) < 0)
	{
	  /* Maybe we should print an error message? */
#if 0
	  sys_error ("tcsetpgrp(%d) failed: pid %ld to pgrp %ld",
	    shell_tty, (long)getpid(), (long)pgrp);
#endif
	  r = -1;
	  e = errno;
	}
```
Which amused me and gave me a little boost of confidence - it's not just me, looks like experienced programmers also have doubts on proper error management!\
I also learned the very basics of forking and managing multiple concurrent processes. This part did not cause much trouble, but was a head-scratcher initially.
