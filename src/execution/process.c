/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   process.c                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: glugo-mu <glugo-mu@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/11/06 10:35:14 by siellage          #+#    #+#             */
/*   Updated: 2025/11/07 14:10:16 by glugo-mu         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell2.h"

void	runprocess(t_cmdlist *cmdlist, int *fd, int fdindex)
{
	if (cmdlist->infile != SSTDERR && cmdlist->outfile != SSTDERR)
		exec_command(cmdlist, fd, fdindex);
	else
		clearpipe(fd);
	freeforloop();
	freecore();
	exit(g_core.exec_output);
}

void	waitall(void)
{
	t_cmdlist	*cmdlist;

	cmdlist = g_core.cmd_table;
	while (cmdlist)
	{
		if (!cmdlist->next)
			waitpid(cmdlist->pid, &g_core.exec_output, 0);
		else
			waitpid(cmdlist->pid, 0, 0);
		cmdlist = cmdlist->next;
	}
	g_core.exec_output = WEXITSTATUS(g_core.exec_output);
}
