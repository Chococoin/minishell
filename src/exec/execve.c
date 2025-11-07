/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   execve.c                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: glugo-mu <glugo-mu@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/11/03 12:40:18 by siellage          #+#    #+#             */
/*   Updated: 2025/11/07 11:14:58 by glugo-mu         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell2.h"

void	runexecve(t_cmdlist *cmdnode, int *fd, int fdindex)
{
	char	**envlist;

	cmdnode->pid = fork();
	if (!cmdnode->pid)
	{
		createdup(cmdnode, fd, fdindex);
		envlist = getenvcpy();
		if (execve(cmdnode->cmd, cmdnode->path, envlist) == -1)
		{
			if (cmdnode->cmd)
				print_error(cmdnode->cmd, " command not found\n", NULL);
			freeenvcpy(envlist);
			freeforloop();
			freecore();
			exit(127);
		}
	}
	if (fd && cmdnode->pid)
		clearpipe(fd);
	waitpid(cmdnode->pid, &g_core.exec_output, 0);
	g_core.exec_output = WEXITSTATUS(g_core.exec_output);
}
