/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   tools_bonus.c                                      :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: macbookair <macbookair@student.42.fr>      +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/03/16 20:03:29 by oelhasso          #+#    #+#             */
/*   Updated: 2025/03/20 01:27:05 by macbookair       ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../header_bonus.h"

int	mystrlen(char *str)
{
	int	i;

	i = 0;
	while (str[i])
		i++;
	return (i);
}

char	*mixem(t_cmd *cmd, t_other *other, int path_ind)
{
	char	*str;
	int		i;
	int		j;

	str = malloc (mystrlen(other->paths[path_ind]) + mystrlen(cmd->cmd) + 2);
	if (!str)
		return (NULL);
	i = 0;
	j = 0;
	while (other->paths[path_ind][j])
		str[i++] = other->paths[path_ind][j++];
	j = 0;
	str[i++] = '/';
	while (cmd->cmd[j])
		str[i++] = cmd->cmd[j++];
	str[i] = 0;
	return (str);
}

int	close_fds(int fds[2], int file)
{
	close(fds[READ]);
	close(fds[WRITE]);
	if (file != -1)
		close(file);
	return (SUCCESSFUL);
}

int	check_file(t_cmd *tmp, t_cmd *cmd, t_other *other, int flag)
{
	if (flag == 0)
	{
		if (other->is_limiter == FALSE)
			other->open1 = open(other->infile, O_RDWR);
		else {	
			if (access(other->infile, F_OK) == 0)
				open ("yes", O_CREAT | O_RDWR, 0777);
			else
				open ("NO", O_CREAT | O_RDWR, 0777);
			other->open1 = open(other->infile, O_RDWR | O_APPEND);
		}
		if (other->open1 == -1)
		{
			close_fds(tmp->pipefd, -1);
			return (free_all(cmd, other), perror("open: "), exit(1), 1);
		}
		return (SUCCESSFUL);
	}
	else if (flag == other->count_proc - 1)
	{
		if (other->is_limiter == TRUE)
			other->open2 = open(other->outfile, O_WRONLY | O_APPEND | O_CREAT, 0644);
		else
			other->open2 = open(other->outfile, O_WRONLY | O_TRUNC | O_CREAT, 0644);
		if (other->open2 == -1)
		{
			close(other->prev_read);
			return (free_all(cmd, other), perror("open: "), exit(1), 1);
		}
		return (SUCCESSFUL);
	}
	else
		return (SUCCESSFUL);
}

void	protect_it(t_cmd *cmd)
{
	cmd->cmd = NULL;
	cmd->opt = NULL;
	cmd->path_cmd = NULL;
	cmd->argument = NULL;
}
