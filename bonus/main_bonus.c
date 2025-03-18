/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   main_bonus.c                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: oelhasso <elhassounioussama2@gmail.com>    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/03/16 16:57:09 by oelhasso          #+#    #+#             */
/*   Updated: 2025/03/18 02:45:50 by oelhasso         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "header_bonus.h"

static void	is_here_doc(char *str, t_other *other)
{
	other->limiter = 0;
	if (str[0] == 'h' && str[1] == 'e' && str[2] == 'r')
	{
		if (str[3] == 'e' && str[4] == '_' && str[5] == 'd')
		{
			if (str[6] == 'o' && str[7] == 'c' && str[8] == 0)
			{
				other->limiter = 1;
				return ;
			}
		}
	}
	return ;
}

static void set_up(int ac, char **av, t_other *other)
{
	other->paths = NULL;
	other->ac = ac;
	other->count_proc = ac - 3;
	if (other->limiter == TRUE)
		other->count_proc --;
	other->infile = av[1];
	other->outfile = av[ac - 1];
}

int	main(int ac, char **av, char **envp)
{
	t_cmd	*cmd;
	t_other	other;

	if (ac < 5)
		return (myputstr("args nb < 5 !\n", 2), FAILED);
	// if (other.debug == -1)
	// {
	// 	myputstr("open debug: \n", 2);
	// 	return (1);
	// }
	is_here_doc(av[1], &other);
	cmd = NULL;
	set_up(ac, av, &other);
	edit_paths(&other, envp);
	parcing(&cmd, av, &other);
	execution(cmd, &other);
	free_all(cmd, &other);
}
