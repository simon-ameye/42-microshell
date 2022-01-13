/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   microshell.c                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: sameye <sameye@student.42.fr>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2022/01/12 12:13:30 by sameye            #+#    #+#             */
/*   Updated: 2022/01/13 21:39:28 by sameye           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include <unistd.h>
#include <sys/wait.h>
#include <string.h>
#include <stdlib.h>

void ft_putstr_fd(char *s)
{
	int i;

	i = 0;
	while (s[i])
		i++;
	write(2, s, i);
}

void ft_exec(char **av, int tmp_fd, int len, char **env)
{
	av[len] = NULL;
	dup2(tmp_fd, STDIN_FILENO);
	close(tmp_fd);
	execve(av[0], av, env);
	ft_putstr_fd("error: cannot execute ");
	ft_putstr_fd(av[0]);
	ft_putstr_fd("\n");
	exit (1);
}

int main(int ac, char **av, char **env)
{
	int len;
	int pid;
	int tmp_fd;
	int fd[2];

	(void)ac;
	tmp_fd = dup(STDIN_FILENO);
	while (av[0] && av[1])
	{
		av++;
		len = 0;
		while (av[len] && strcmp(av[len], "|") && strcmp(av[len], ";"))
			len++;
		if (!strcmp(*av, "cd"))
		{
			if (len != 2)
				ft_putstr_fd("error: cd: bad arguments\n");
			else if (chdir(av[1]) != 0)
			{
				ft_putstr_fd("error: cd: cannot change directory to ");
				ft_putstr_fd(av[1]);
				ft_putstr_fd("\n");
			}
		}
		else if (av[len] == NULL || !strcmp(av[len], ";"))
		{
			pid = fork();
			if (pid == 0)
				ft_exec(av, tmp_fd, len, env);
			close (tmp_fd);
			waitpid(pid, NULL, 0);
			tmp_fd = dup(STDIN_FILENO);
		}
		else if (!strcmp(av[len], "|"))
		{
			pipe(fd);
			pid = fork();
			if (pid == 0)
			{
				
				dup2(fd[1], STDOUT_FILENO);
				close(fd[0]);
				close(fd[1]);
				ft_exec(av, tmp_fd, len, env);
			}
			close(fd[1]);
			close(tmp_fd);
			waitpid(pid, NULL, 0);
			tmp_fd = dup(fd[0]);
			close(fd[0]);
		}
		av += len;
	}
	close(tmp_fd);
	return (0);
}
