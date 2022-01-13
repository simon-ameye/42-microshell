/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   shackbei_microshell.c                              :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: sameye <sameye@student.42.fr>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2021/12/15 12:15:17 by shackbei          #+#    #+#             */
/*   Updated: 2022/01/13 20:30:21 by sameye           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include <unistd.h>
#include <sys/wait.h>
#include <string.h>

/*not needed in exam, but necessary if you want to use this tester:
https://github.com/Glagan/42-exam-rank-04/blob/master/microshell/test.sh*/
// #ifdef TEST_SH
// # define TEST		1
// #else
// # define TEST		0
// #endif

void	ft_putstr_fd2(char *str)
{
	int	i;

	i = 0;
	while (str[i])
		i++;
	write(2, str, i);
}

int ft_execute(char *argv[], int i, int tmp_fd, char *env[])
{
	//overwrite ; or | or NULL whith NULL to use the array as input for execve.
	//we are here in the child so it has no impact in the Parrent Proces.
	argv[i] = NULL;
	close(tmp_fd);
	execve(argv[0], argv, env);
	ft_putstr_fd2("error: cannot execute ");
	ft_putstr_fd2(argv[0]);
	write(2, "\n", 1);
	return (1);
}

int	main(int argc, char *argv[], char *env[])
{
	int	i;
	int pid;
	int fd[2];
	int tmp_fd;
	(void)argc;	// not needed in exam, but necessary if ou compile with -Wall -Wextra -Werror Flags

	pid = 0;
	i = 0;
	tmp_fd = dup(STDIN_FILENO);
	while (argv[i] && argv[i + 1]) //check if the end is reached
	{
		argv = &argv[i + 1];	//the new argv start after the ; or |
		i = 0;
		//count untill we have all invormations to execute the next child;
		while (argv[i] && strcmp(argv[i], ";") && strcmp(argv[i], "|"))
			i++;
		if (strcmp(argv[0], "cd") == 0) //cd
		{
			if (i != 2)
				ft_putstr_fd2("error: cd: bad arguments\n");
			else if (chdir(argv[1]) != 0)
			{
				ft_putstr_fd2("error: cd: cannot change directory to ");
				ft_putstr_fd2(argv[1]);
				write(2, "\n", 1);
			}
		}
		else if (argv != &argv[i] && (argv[i] == NULL || strcmp(argv[i], ";") == 0)) //exec in stdout
		{
			pid = fork();
			if ( pid == 0)
			{
				dup2(tmp_fd, STDIN_FILENO);
				if (ft_execute(argv, i , tmp_fd, env))
					return (1);
			}
			else
			{
				close(tmp_fd);
				waitpid(-1, NULL, WUNTRACED);
				tmp_fd = dup(STDIN_FILENO);
			}
		}
		else if(argv != &argv[i] && strcmp(argv[i], "|") == 0) //pipe
		{
			pipe(fd);
			pid = fork();
			if ( pid == 0)
			{
				dup2(tmp_fd, STDIN_FILENO);
				dup2(fd[1], STDOUT_FILENO);
				close(fd[0]);
				close(fd[1]);
				if (ft_execute(argv, i , tmp_fd, env))
					return (1);
			}
			else
			{
				close(fd[1]);
				close(tmp_fd);
				waitpid(-1, NULL, WUNTRACED);
				tmp_fd = dup(fd[0]);
				close(fd[0]);
			}
		}
	}
	close(tmp_fd);
	// if (TEST)		// not needed in exam, but necessary if you want to use this tester:
	// 	while (1);	// https://github.com/Glagan/42-exam-rank-04/blob/master/microshell/test.sh
	return (0);
}