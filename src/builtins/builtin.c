/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   builtin.c                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: glugo-mu <glugo-mu@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/10/06 10:51:30 by siellage          #+#    #+#             */
/*   Updated: 2025/11/07 10:55:39 by glugo-mu         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell2.h"

int	isbuiltin(char *cmd)
{
	if (str_compare(cmd, "echo"))
		return (ECHO);
	else if (str_compare(cmd, "cd"))
		return (CD);
	else if (str_compare(cmd, "pwd"))
		return (PWD);
	else if (str_compare(cmd, "export"))
		return (EXPORT);
	else if (str_compare(cmd, "unset"))
		return (UNSET);
	else if (str_compare(cmd, "env"))
		return (ENV);
	else if (str_compare(cmd, "exit"))
		return (EXIT);
	return (0);
}

void	runbuiltin(t_cmdlist *cmdnode, int builtin, int *fd, int fd_index)
{
	if (fd)
		createdup(cmdnode, fd, fd_index);
	if (builtin == ECHO)
		runecho(cmdnode);
	else if (builtin == CD)
		runcd(cmdnode);
	else if (builtin == PWD)
		runpwd(cmdnode);
	else if (builtin == EXPORT)
		runexport(cmdnode);
	else if (builtin == UNSET)
		rununset(cmdnode);
	else if (builtin == ENV)
		runenv(cmdnode);
	else if (builtin == EXIT)
		runexit(cmdnode);
}