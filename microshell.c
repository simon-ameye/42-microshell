/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   microshell.c                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: sameye <sameye@student.42.fr>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2022/01/12 12:13:30 by sameye            #+#    #+#             */
/*   Updated: 2022/01/12 14:55:08 by sameye           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include <unistd.h>
#include <sys/wait.h>
#include <string.h>

void ft_putstr_fd(char *s, int stream)
{
	while (*s)
	{
		write(stream, *s, 1);
		s++;
	}
}

int main(int ac, char **av, char **env)
{
	int len;

	av++;
	while (*av)
	{
		len = 0;
		while (av[len] 	&& av[len] != '|' && av[len] != ';')
			len++;
		if (strcmp(*av, 'cd' == 0))
		{
			if (len != 2)
				ft_putstr_fd("error: cd: bad arguments\n", 2);
			else if (chdir(av[1] != 0))
			{
				ft_putstr_fd("error: cd: cannot change directory to ", 2);
				ft_putstr_fd(av[1], 2);
				ft_putstr_fd("\n", 2);
			}
		}







		av++;
	}
}