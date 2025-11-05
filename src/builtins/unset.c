/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   unset.c                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: glugo-mu <glugo-mu@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/10/08 15:34:23 by siellage          #+#    #+#             */
/*   Updated: 2025/10/31 16:24:32 by glugo-mu         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell2.h"

void	rununset(t_cmdlist *cmdnode)
{
	char	**tempname;
	int		array_len;

	array_len = get_array_len(cmdnode->path);
	if (array_len > 1)
	{
		tempname = cmdnode->path;
		while (*(++tempname))
		{
			if (!env_arg_control(*tempname))
			{
				print_error("-bash: unset: `", *tempname,
					"': not a valid identifier\n");
				g_core.exec_output = 1;
				continue ;
			}
			else
				deleteenv(*tempname);
		}
	}
}

void	deleteenv(char *name)
{
	t_env	*env;
	t_env	*temp_env;

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
				temp_env->next = env->next;
			free(env);
			break ;
		}
		temp_env = env;
		env = env->next;
	}
}
