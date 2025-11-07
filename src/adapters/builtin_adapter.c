/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   builtin_adapter.c                                  :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: glugo-mu <glugo-mu@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/11/05 21:30:00 by glugo-mu          #+#    #+#             */
/*   Updated: 2025/11/05 21:30:00 by glugo-mu         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"
#include "minishell2.h"

t_cmdlist	*cmd_to_cmdlist(t_cmd *cmd);
void		free_cmdlist_adapter(t_cmdlist *cmdlist);

static void	restore_fds(int saved_stdin, int saved_stdout)
{
	dup2(saved_stdin, STDIN_FILENO);
	dup2(saved_stdout, STDOUT_FILENO);
	close(saved_stdin);
	close(saved_stdout);
}

static int	run_builtin_cmd(t_cmd *cmd)
{
	t_cmdlist	*cmdlist;
	int			builtin_type;
	int			status;

	status = 0;
	cmdlist = cmd_to_cmdlist(cmd);
	if (cmdlist)
	{
		builtin_type = isbuiltin(cmd->argv[0]);
		if (builtin_type)
			status = runbuiltin(cmdlist, builtin_type, NULL, -1);
		free_cmdlist_adapter(cmdlist);
	}
	return (status);
}

int	execute_builtin_simple(t_cmd *cmd)
{
	int	saved_stdin;
	int	saved_stdout;
	int	status;

	saved_stdin = dup(STDIN_FILENO);
	saved_stdout = dup(STDOUT_FILENO);
	if (cmd->redirs && apply_redirections(cmd->redirs) < 0)
	{
		restore_fds(saved_stdin, saved_stdout);
		return (1);
	}
	status = run_builtin_cmd(cmd);
	restore_fds(saved_stdin, saved_stdout);
	return (status);
}
