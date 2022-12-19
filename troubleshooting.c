#include <stdlib.h>
#include <unistd.h>
#include <sys/types.h>
#include <signal.h>
#include <string.h>
#include <sys/wait.h>
#include <stdio.h>


int	main(int ac, char **av, char **env)
{
printf("parent : %d\n", getpid());

	int pid = fork();
	if (pid == 0)
	{
		printf("child %d\n", getpid());
		kill(getpid(), 19);
		printf("child out\n");
		return (0);
	}
	getchar();
	kill(pid, 18);
	if(fork() == 0)
	{

	}
	else
	{
		while (waitpid(-1, NULL, WUNTRACED) != -1)
			;
		printf("prout\n");
		getchar();
	}
}
