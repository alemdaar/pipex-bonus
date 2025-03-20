/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   execution_bonus.c                                  :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: macbookair <macbookair@student.42.fr>      +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/03/16 18:03:38 by oelhasso          #+#    #+#             */
/*   Updated: 2025/03/20 01:24:14 by macbookair       ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../header_bonus.h"

static int	exec(t_cmd *tmp, t_cmd *cmd, t_other *other)
{
	if (tmp->path_cmd == NULL)
	{
		
		if (access(other->infile, F_OK) == 0)
			unlink(other->infile);
		free_all(cmd, other);
		myputstr ("Error: command not found\n", 2);
		exit(1);
	}
	if (execve(tmp->path_cmd, tmp->argument, NULL) == ERROR)
	{
		if (access(other->infile, F_OK) == 0)
			unlink(other->infile);
		free_all(cmd, other);
		perror ("execve: ");
		exit(1);
	}
	return (SUCCESSFUL);
}

static int	pipping(t_cmd *tmp, t_cmd *cmd, t_other *other)
{
	t_ind	ind;

	ind.r = pipe(tmp->pipefd);
	if (ind.r == -1)
	{
		free_all(cmd, other);
		why_exit("Error: pipe failed\n", FAILED);
	}
	return (SUCCESSFUL);
}

static int	dupping(t_cmd *tmp, t_other *other, int position)
{
	t_ind	ind;

	if (position == 0 && tmp->next != NULL)
	{
		ind.r = dup2(other->open1, 0);
		if (ind.r == -1)
			return (close_fds(tmp->pipefd, other->open1), ERROR);
		ind.r = dup2(tmp->pipefd[WRITE], 1);
		if (ind.r == -1)
			return (close_fds(tmp->pipefd, other->open1), ERROR);
		close_fds(tmp->pipefd, other->open1);
		return (SUCCESSFUL);
	}
	else if (position != 0 && tmp->next != NULL)
	{
		ind.r = dup2(other->prev_read, 0);
		if (ind.r == -1)
			return (close_fds(tmp->pipefd, other->prev_read), ERROR);
		ind.r = dup2(tmp->pipefd[WRITE], 1);
		if (ind.r == -1)
			return (close_fds(tmp->pipefd, other->prev_read), ERROR);
		close_fds(tmp->pipefd, other->prev_read);
	}
	else
	{
		ind.r = dup2(other->prev_read, 0);
		if (ind.r == -1)
			return (close(other->prev_read), close(other->open2), ERROR);
		ind.r = dup2(other->open2, 1);
		if (ind.r == -1)
			return (close(other->prev_read), close(other->open2), ERROR);
		close(other->prev_read);
		close(other->open2);
	}
	return (SUCCESSFUL);
}

static int	child_process(t_cmd *tmp, t_cmd *cmd, t_other *other, int position)
{
	t_ind	ind;

	ind.r = 0;
	check_file(tmp, cmd, other, position);
	ind.r = dupping(tmp, other, position);
	if (ind.r == -1)
	{
		if (position == 0)
		{
			if (access(other->infile, F_OK) == 0)
				unlink(other->infile);
		}
		free_all(cmd, other);
		return (perror("Error dup2: "), exit(1), 1);
	}
	exec(tmp, cmd, other);
	exit(1);
}

int	execution(t_cmd *cmd, t_other *other)
{
	t_cmd	*tmp;
	t_ind	ind;

	ind.i = 0;
	tmp = cmd;
	while (tmp)
	{
		if (tmp->next)
			pipping(tmp, cmd, other);
		tmp->pid = fork();
		if (tmp->pid == -1)
		{
			free_all(cmd, other);
			why_exit("Error: fork failed\n", FAILED);
		}
		if (tmp->pid == 0)
			child_process(tmp, cmd, other, ind.i);
		if (ind.i != 0)
			close(other->prev_read);
		if (tmp->next)
		{
			close(tmp->pipefd[WRITE]);
			other->prev_read = tmp->pipefd[READ];
		}
		ind.i++;
		tmp = tmp->next;
	}
	ind.t = 0;
	while (ind.t < ind.i)
	{
		wait(NULL);
		ind.t ++;
	}
	if (access(other->infile, F_OK) == 0)
		unlink(other->infile);
	return (SUCCESSFUL);
}
