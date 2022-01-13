/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   microshell.c                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: sameye <sameye@student.42.fr>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2022/01/12 12:13:30 by sameye            #+#    #+#             */
/*   Updated: 2022/01/13 20:15:24 by sameye           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include <unistd.h>
#include <sys/wait.h>
#include <string.h>
#include <stdio.h>
#include <stdlib.h>

void ft_putstr_fd(char *s, int stream)
{
	int i;

	i = 0;
	while (*s)
		i++;
	write(stream, s, i);
}

int ft_exec(char **av, int tmp_fd, int len, char **env)
{
	av[len] = NULL;
	close(tmp_fd);
	fprintf(stderr, "lets execve %s\n", av[0]);
	execve(av[0], av, env);
	ft_putstr_fd("error: cannot execute ", 2);
	ft_putstr_fd(av[0], 2);
	ft_putstr_fd("\n", 2);
	return (1);
}

int main(int ac, char **av, char **env)
{
	int len;
	int pid;
	int tmp_fd;
	int fd[2];

	(void)ac;
	tmp_fd = dup(STDIN_FILENO);
	av++;
	while (*av)
	{
		len = 0;
		while (av[len] && strcmp(av[len], "|") != 0 && strcmp(av[len], ";") != 0)
			len++;
		fprintf(stderr, "len %i\n", len);
		if (strcmp(*av, "cd") == 0)
		{
			if (len != 2)
				ft_putstr_fd("error: cd: bad arguments\n", 2);
			else if (chdir(av[1]) != 0)
			{
				ft_putstr_fd("error: cd: cannot change directory to ", 2);
				ft_putstr_fd(av[1], 2);
				ft_putstr_fd("\n", 2);
			}
		}
		else if (av[len] == NULL || strcmp(av[len], ";") == 0)
		{
			fprintf(stderr, "end side cmd\n");
			pid = fork();
			if (pid == 0)
			{
				dup2(tmp_fd, STDIN_FILENO);
				ft_exec(av, tmp_fd, len, env);
				fprintf(stderr, "child returned\n");
				exit(1);
			}
			close (tmp_fd);
			fprintf(stderr, "waiting pid\n");
			waitpid(pid, NULL, 0);
			fprintf(stderr, "pid returned\n");
			tmp_fd = dup(STDIN_FILENO);
		}
		else if (strcmp(av[len], "|") == 0)
		{
			pipe(fd);
			pid = fork();
			if (pid == 0)
			{
				dup2(tmp_fd, STDIN_FILENO);
				dup2(fd[1], STDOUT_FILENO);
				close(fd[0]);
				close(fd[1]);
				if (ft_exec(av, tmp_fd, len, env))
					return (1);
			}
			else
			{
				close(tmp_fd);
				close(fd[1]);
				fprintf(stderr, "waiting pid\n");
				waitpid(pid, NULL, 0);
				fprintf(stderr, "pid returned\n");
				tmp_fd = dup(fd[2]);
				close(fd[0]);
			}
		}
		fprintf(stderr, "first loop\n");
		av += len;
	}
	close(tmp_fd);
}
