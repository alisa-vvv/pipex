The goal of the project is to understand in more detail how pipes work in UNIX.\
\
The program has to be executed by this:

```
./pipex file1 cmd1 cmd2 file2
```
Which should produce behavior that is equivalent to running the following shell ocmmand:

```
 < file1 cmd1 | cmd2 > file2
```

The scope of this project only requires to reproduce the behaviour of this exact command structure. Meaning that it is not nescessary to implement redirections seprately from pipes, implement cases where less than two commands or files were given, or implement cases where redirections are not used.\
\
This project is simply meant to introduce the concept of pipes and teach the use of some C commands that interact with Unix OS features. It is useful as a stepping stone to the larger project to create a smaller-scope bash-like shell.
