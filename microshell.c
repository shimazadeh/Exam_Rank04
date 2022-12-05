#include <stdlib.h>
#include <unistd.h>
#include <sys/types.h>
#include <signal.h>
#include <string.h>
#include <sys/wait.h>
#include <stdio.h>

int	ft_puterror(char *str, char *arg)
{
	while(*str)
		write(2, str++, 1);
	if (arg)
	{
		write (2, " ", 1);
		while(*arg)
			write(2, arg++, 1);
	}
	write(2, "\n", 1);
	return (1);
}

int	ft_exec(char **av, int tmp, int i, char **env)
{
	av[i] = NULL;
	dup2(tmp, STDIN_FILENO);
	close(tmp);
	execve(av[0], av, env);
	return (ft_puterror("error: cannot execute", av[0]));
}

int	main(int ac, char **av, char **env)
{
	int	i;
	int	tmp;
	int	fd[2];
	(void)ac;

	i = 0;
	tmp = dup(STDIN_FILENO);
	while(av[i] && av[i + 1])
	{
		av = &av[i + 1];
		i = 0;
		while(av[i] && strcmp(av[i], ";") && strcmp(av[i], "|"))
			i++;
		if (strcmp(av[0], "cd") == 0)
		{
			if (i != 2)
				return (ft_puterror("error: cd: bad arguments", NULL));
			else if (chdir(av[1]) != 0)
				return (ft_puterror("error: cd: cannot change directory to ", av[1]));
		}
		else if (i != 0 && (av[i] == NULL || strcmp(av[i], ";") == 0))
		{
			if (fork() == 0)
			{
				if (ft_exec(av, tmp, i, env))
					return (1);
			}
			else
			{
				close(tmp);
				while (waitpid(-1, NULL, WUNTRACED) == -1)
					;
				tmp = dup(STDIN_FILENO);
			}
		}
		else if (i != 0 && strcmp(av[i], "|") == 0)
		{
			pipe(fd);
			if (fork() == 0)
			{
				dup2(fd[1], STDOUT_FILENO);
				close(fd[1]);
				close(fd[0]);
				if (ft_exec(av, tmp, i, env))
					return (1);
			}
			else
			{
				close(fd[1]);
				close(tmp);
				tmp = fd[0];
			}
		}
	}
	close(tmp);
	return (0);
}
