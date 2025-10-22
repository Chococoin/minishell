/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   command_utils.c                                    :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: glugo-mu <glugo-mu@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/10/09 12:27:52 by glugo-mu          #+#    #+#             */
/*   Updated: 2025/10/15 19:39:51 by glugo-mu         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

static void	free_argv(char **argv)
{
	size_t	i;

	if (!argv)
		return ;
	i = 0;
	while (argv[i])
	{
		free(argv[i]);
		i++;
	}
	free(argv);
}

static void	free_redirs(t_redir *redir)
{
	t_redir	*next;

	while (redir)
	{
		next = redir->next;
		free(redir->target);
		free(redir);
		redir = next;
	}
}

void	cmd_clear(t_cmd **cmds)
{
	t_cmd	*next;

	if (!cmds)
		return ;
	while (*cmds)
	{
		next = (*cmds)->next;
		free_argv((*cmds)->argv);
		free_redirs((*cmds)->redirs);
		free(*cmds);
		*cmds = next;
	}
}
