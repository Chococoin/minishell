/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   execcommand.c                                      :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: glugo-mu <glugo-mu@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/11/03 16:26:21 by siellage          #+#    #+#             */
/*   Updated: 2025/11/07 11:20:42 by glugo-mu         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell2.h"

void	execcommand(t_cmdlist *cmdnode, int *fd, int fdindex)
{
	char	*cmd;
	int		builtinindex;

	if (cmdnode && !cmdnode->cmd)
		return ;
	cmd = getcmd(cmdnode->cmd);
	builtinindex = isbuiltin(cmd);
	if (builtinindex)
		runbuiltin(cmdnode, builtinindex, fd, fdindex);
	else
		runexecve(cmdnode, fd, fdindex);
}

char	*getcmd(char *cmd)
{
	int		commandlen;

	commandlen = 0;
	while (*cmd)
	{
		commandlen++;
		if (*cmd == '/')
			commandlen = 0;
		cmd++;
	}
	return (cmd - commandlen);
}
