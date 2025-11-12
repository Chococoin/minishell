/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   executer.c                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: glugo-mu <glugo-mu@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/11/04 13:51:12 by siellage          #+#    #+#             */
/*   Updated: 2025/11/07 11:21:47 by glugo-mu         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell2.h"

void	runmultiplecommand(t_cmdlist *cmdlist)
{
	int			*fd;

	fd = createpipe();
	while (cmdlist)
	{
		switchpipe(&fd);
		pipe(&fd[2]);
		cmdlist->pid = fork();
		if (cmdlist->pid <= 0)
			runprocess(cmdlist, fd, 2);
		cmdlist = cmdlist->next;
		if (fd[4] && fd[5])
		{
			close(fd[4]);
			close(fd[5]);
			fd[4] = 0;
			fd[5] = 0;
		}
	}
	clearpipe(fd);
	waitall();
}

void	runsinglecommand(t_cmdlist *cmdlist, int *fd)
{
	if (cmdlist->infile != SSTDERR && cmdlist->outfile != SSTDERR)
		execcommand(cmdlist, fd, -1);
}

void	executer(void)
{
	if (!g_core.cmd_table)
		return ;
	else if (!g_core.cmd_table->next)
		runsinglecommand(g_core.cmd_table, NULL);
	else
		runmultiplecommand(g_core.cmd_table);
}
