/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   createdup.c                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: glugo-mu <glugo-mu@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/10/08 17:01:55 by siellage          #+#    #+#             */
/*   Updated: 2025/11/07 10:57:08 by glugo-mu         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell2.h"

void	createdup(t_cmdlist *cmd_list, int *fd, int fdindex)
{
	int	newfd[2];

	if (cmd_list->infile == HERADOC)
	{
		pipe(newfd);
		write(newfd[1], cmd_list->heradoc_values,
			ft_strlen(cmd_list->heradoc_values));
		dup2(newfd[0], STDIN_FILENO);
		close (newfd[1]);
		close(newfd[0]);
	}
	else if (cmd_list->infile > SSTDERR)
		dup2(cmd_list->infile, STDIN_FILENO);
	else if (fd && cmd_list != g_core.cmd_table)
		dup2(fd[fdindex - 2], STDIN_FILENO);
	if (cmd_list->outfile > SSTDERR)
		dup2(cmd_list->outfile, STDOUT_FILENO);
	else if (fd && cmd_list->next)
		dup2(fd[fdindex + 1], STDOUT_FILENO);
	if (fdindex >= 0)
		clearpipe(fd);
}
