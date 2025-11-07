/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   unset.c                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: glugo-mu <glugo-mu@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/10/08 15:34:23 by siellage          #+#    #+#             */
/*   Updated: 2025/11/07 11:11:53 by glugo-mu         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell2.h"

void	rununset(t_cmdlist *cmdnode)
{
	char	**tempname;
	int		arraylen;

	arraylen = getarraylen(cmdnode->path);
	if (arraylen > 1)
	{
		tempname = cmdnode->path;
		while (*(++tempname))
		{
			if (!envargcontrol(*tempname))
			{
				print_error("-bash: unset: `", *tempname,
					"': not a valid identifier\n");
				g_core.exec_output = 1;
				continue ;
			}
			else
				deleteenv(*tempname);
		}
		sync_my_env();
	}
}

void	deleteenv(char *name)
{
	t_env	*env;
	t_env	*tempenv;

	env = g_core.env_table;
	while (env)
	{
		if (str_compare(env->env_name, name))
		{
			if (env->content)
				free(env->content);
			free(env->env_name);
			if (g_core.env_table == env)
				g_core.env_table = g_core.env_table->next;
			else
				tempenv->next = env->next;
			free(env);
			break ;
		}
		tempenv = env;
		env = env->next;
	}
}
