/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   here_doc_bonus.c                                   :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: macbookair <macbookair@student.42.fr>      +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/03/19 00:00:40 by macbookair        #+#    #+#             */
/*   Updated: 2025/03/19 02:52:09 by macbookair       ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../header_bonus.h"

int	open_here_doc(t_cmd *tmp, t_cmd *cmd, t_other *other)
{
	other->open1 = open("/tmp/here_doc", O_CREAT | O_RDWR | O_APPEND, 0644);
	if (other->open1 == -1)
	{
		close_fds(tmp->pipefd, -1);
		free_all(cmd, other);
		perror("open: ");
		exit(FAILED);
	}
	return (SUCCESSFUL);
}

int	is_limiter(char *line, char *limiter)
{
    t_ind   ind;

	ind.i = 0;
	while (limiter[ind.i] && limiter[ind.i] == line[ind.i])
        ind.i ++;
    if (limiter[ind.i])
        return (FALSE);
    return (TRUE);
}

int	make_heredoc(t_cmd *tmp, t_cmd *cmd, t_other *other)
{
	char	*line;
	t_ind	ind;

	open_here_doc(tmp, cmd, other);
	ind.c = 0;
	while (1)
	{
		myputstr("pipe heredoc> ", 1);
		line = get_next_line(0);
		if (line == NULL && ind.c == 0)
		{
			close_fds(tmp->pipefd, other->open1);
			unlink("/tmp/here_doc");
			free_all(NULL, other);
			return (myputstr("gnl failed\n", 2), 1);
		}
		if (line == NULL && ind.c == 1)
			break ;
		ind.c = 1;
		if (is_limiter(line, other->limiter) == TRUE)
			return (free(line), SUCCESSFUL);
		write (other->open1, line, mystrlen(line));
		free(line);
	}
	return (SUCCESSFUL);
}
