/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   builtin.c                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: glugo-mu <glugo-mu@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/10/06 10:51:30 by siellage          #+#    #+#             */
/*   Updated: 2025/11/07 10:53:29 by glugo-mu         ###   ########.fr       */
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

int	runbuiltin(t_cmdlist *cmdnode, int builtin, int *fd, int fd_index)
{
	int	status;

	status = 0;
	if (fd)
		createdup(cmdnode, fd, fd_index);
	if (builtin == ECHO)
		status = runecho(cmdnode);
	else if (builtin == CD)
		status = runcd(cmdnode);
	else if (builtin == PWD)
		status = runpwd(cmdnode);
	else if (builtin == EXPORT)
		status = runexport(cmdnode);
	else if (builtin == UNSET)
		status = rununset(cmdnode);
	else if (builtin == ENV)
		status = runenv(cmdnode);
	else if (builtin == EXIT)
		status = runexit(cmdnode);
	return (status);
}
