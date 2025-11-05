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

void	execute_builtin_simple(t_cmd *cmd)
{
	t_cmdlist	*cmdlist;
	int			builtin_type;

	cmdlist = cmd_to_cmdlist(cmd);
	if (!cmdlist)
		return ;
	builtin_type = isbuiltin(cmd->argv[0]);
	if (builtin_type)
		runbuiltin(cmdlist, builtin_type, NULL, -1);
	free_cmdlist_adapter(cmdlist);
}
