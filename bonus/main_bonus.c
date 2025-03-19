/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   main_bonus.c                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: macbookair <macbookair@student.42.fr>      +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/03/16 16:57:09 by oelhasso          #+#    #+#             */
/*   Updated: 2025/03/19 19:25:18 by macbookair       ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "header_bonus.h"

static void	is_here_doc(char *str, t_other *other)
{
	other->is_limiter = 0;
	if (str[0] == 'h' && str[1] == 'e' && str[2] == 'r')
	{
		if (str[3] == 'e' && str[4] == '_' && str[5] == 'd')
		{
			if (str[6] == 'o' && str[7] == 'c' && str[8] == 0)
			{
				other->is_limiter = 1;
				unlink("/tmp/here_doc");
				return ;
			}
		}
	}
	return ;
}

static void set_up(int ac, char **av, t_other *other)
{
	other->paths = NULL;
	other->limiter = NULL;
	other->ac = ac;
	other->count_proc = ac - 3;
	if (other->is_limiter == TRUE)
		other->count_proc --;
	if (other->is_limiter == TRUE)
		other->limiter = av[2];
	other->infile = av[1];
	other->outfile = av[ac - 1];
}

int	main(int ac, char **av, char **envp)
{
	t_cmd	*cmd;
	t_other	other;

	if (ac < 5)
		return (myputstr("args nb < 5 !\n", 2), FAILED);
	// other.debug = open ("debug", O_CREAT | O_RDWR, 0777);
	// if (other.debug == -1)
	// {
	// 	myputstr("open debug: \n", 2);
	// 	return (1);
	// }
	is_here_doc(av[1], &other);
	if (other.is_limiter == TRUE)
			make_heredoc(tmp, cmd, other);
	cmd = NULL;
	set_up(ac, av, &other);
	edit_paths(&other, envp);
	parcing(&cmd, av, &other);
	// t_cmd *tmp;
	// tmp = cmd;
	// int i;
	// while (tmp)
	// {
	// 	printf("cmd = %s\n", tmp->cmd);
	// 	printf("opt = %s\n", tmp->opt);
	// 	printf("ar = %d\n", tmp->ar);
	// 	i = 0;
	// 	while (tmp->argument[i])
	// 		printf("argument = %s\n", tmp->argument[i++]);
	// 	printf("path_cmd = %s\n", tmp->path_cmd);
	// 	tmp = tmp->next;
	// }
	// printf("all path = %s\n", other.all_path);
	// printf("count path = %d\n", other.count_path);
	// printf("count proc = %d\n", other.count_proc);
	// printf("infile = %s\n", other.infile);
	// printf("outfile = %s\n", other.outfile);
	// i = 0;
	// while (i < other.count_path)
	// 	printf("paths = %s\n", other.paths[i++]);
	execution(cmd, &other);
	free_all(cmd, &other);
}
