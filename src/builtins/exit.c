/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   exit.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: glugo-mu <glugo-mu@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/10/08 15:40:50 by siellage          #+#    #+#             */
/*   Updated: 2025/11/07 11:07:21 by glugo-mu         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell2.h"

int	isallnumeric(char *text)
{
	int	i;

	i = 0;
	if (text[i] == '-' || text[i] == '+')
		i++;
	if (!text[i])
		return (0);
	while (text[i])
	{
		if (text[i] < '0' || text[i] > '9')
			return (0);
		i++;
	}
	return (1);
}

void	runexit(t_cmdlist *cmdnode)
{
	int		exit_code;
	int		arraylen;

	exit_code = g_core.exec_output;
	arraylen = getarraylen(cmdnode->path);
	if (arraylen > 2)
	{
		print_error("exit\n-bash: exit: too many arguments\n", NULL, NULL);
		g_core.exec_output = 1;
		return ;
	}
	if (arraylen == 2)
	{
		if (!isallnumeric(cmdnode->path[1]))
		{
			print_error("exit\n-bash: exit: ", cmdnode->path[1],
				": numeric argument required\n");
			exit_code = 2;
		}
		else
			exit_code = ft_atoi(cmdnode->path[1]);
	}
	write(1, "exit\n", 5);
	exit(exit_code);
}
