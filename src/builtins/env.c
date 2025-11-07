/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   env.c                                              :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: glugo-mu <glugo-mu@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/10/08 15:33:28 by siellage          #+#    #+#             */
/*   Updated: 2025/11/07 09:48:53 by glugo-mu         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell2.h"

int	runenv(t_cmdlist *cmdnode)
{
	t_env	*tempenv;

	tempenv = g_core.env_table;
	while (tempenv)
	{
		if (tempenv->content)
		{
			write(cmdnode->outfile, tempenv->env_name,
				ft_strlen(tempenv->env_name));
			write(cmdnode->outfile, "=", 1);
			write(cmdnode->outfile, tempenv->content,
				ft_strlen(tempenv->content));
			write(cmdnode->outfile, "\n", 1);
		}
		tempenv = tempenv->next;
	}
	return (0);
}
