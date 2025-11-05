/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   redirections.c                                     :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: glugo-mu <glugo-mu@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/11/05 22:00:00 by glugo-mu          #+#    #+#             */
/*   Updated: 2025/11/05 22:00:00 by glugo-mu         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"
#include <fcntl.h>

static int	handle_input_redir(const char *file)
{
	int	fd;

	fd = open(file, O_RDONLY);
	if (fd < 0)
	{
		perror("minishell");
		return (-1);
	}
	if (dup2(fd, STDIN_FILENO) < 0)
	{
		close(fd);
		perror("minishell");
		return (-1);
	}
	close(fd);
	return (0);
}

static int	handle_output_redir(const char *file)
{
	int	fd;

	fd = open(file, O_WRONLY | O_CREAT | O_TRUNC, 0644);
	if (fd < 0)
	{
		perror("minishell");
		return (-1);
	}
	if (dup2(fd, STDOUT_FILENO) < 0)
	{
		close(fd);
		perror("minishell");
		return (-1);
	}
	close(fd);
	return (0);
}

static int	handle_append_redir(const char *file)
{
	int	fd;

	fd = open(file, O_WRONLY | O_CREAT | O_APPEND, 0644);
	if (fd < 0)
	{
		perror("minishell");
		return (-1);
	}
	if (dup2(fd, STDOUT_FILENO) < 0)
	{
		close(fd);
		perror("minishell");
		return (-1);
	}
	close(fd);
	return (0);
}

static int	handle_heredoc(const char *delimiter)
{
	int		pipefd[2];
	char	*line;

	if (pipe(pipefd) < 0)
		return (perror("minishell"), -1);
	while (1)
	{
		line = readline("> ");
		if (!line || ft_strcmp(line, delimiter) == 0)
		{
			free(line);
			break ;
		}
		write(pipefd[1], line, ft_strlen(line));
		write(pipefd[1], "\n", 1);
		free(line);
	}
	close(pipefd[1]);
	if (dup2(pipefd[0], STDIN_FILENO) < 0)
		return (close(pipefd[0]), perror("minishell"), -1);
	close(pipefd[0]);
	return (0);
}

int	apply_redirections(t_redir *redirs)
{
	t_redir	*current;
	int		result;

	current = redirs;
	while (current)
	{
		if (current->type == TOKEN_REDIR_IN)
			result = handle_input_redir(current->target);
		else if (current->type == TOKEN_REDIR_OUT)
			result = handle_output_redir(current->target);
		else if (current->type == TOKEN_REDIR_APPEND)
			result = handle_append_redir(current->target);
		else if (current->type == TOKEN_HEREDOC)
			result = handle_heredoc(current->target);
		else
			result = 0;
		if (result < 0)
			return (-1);
		current = current->next;
	}
	return (0);
}
