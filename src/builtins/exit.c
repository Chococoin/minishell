/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   exit.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: glugo-mu <glugo-mu@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/10/08 15:33:47 by siellage          #+#    #+#             */
/*   Updated: 2025/11/07 10:53:57 by glugo-mu         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell2.h"

int	isallnumeric(char *text)
{
	while (text && *text)
	{
		if (!(*text >= '0' && *text <= '9'))
			return (0);
		text++;
	}
	return (1);
}

int	runexit(t_cmdlist *cmdnode)
{
	int	arraylen;

	arraylen = getarraylen(&cmdnode->path[1]);
	if (arraylen > 1)
	{
		print_error("bash: exit: too many arguments\n", NULL, NULL);
		g_core.exec_output = 1;
		return (1);
	}
	else if (arraylen == 1)
	{
		if (is_all_numeric(cmdnode->path[1]))
			g_core.exec_output = ft_atoi(cmdnode->path[1]);
		else
		{
			print_error("bash: exit: ",
				cmdnode->path[1], ": numeric argument required\n");
			g_core.exec_output = 255;
		}
	}
	else
		g_core.exec_output = 0;
	free_for_loop();
	free_core();
	exit(g_core.exec_output % 256);
}
