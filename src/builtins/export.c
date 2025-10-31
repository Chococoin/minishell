/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   export.c                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: glugo-mu <glugo-mu@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/10/08 15:40:50 by siellage          #+#    #+#             */
/*   Updated: 2025/10/31 16:50:28 by glugo-mu         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell2.h"

void	runexport(t_cmdlist *cmdnode)
{
	int		arraylen;
	char	**temppath;

	arraylen = getarraylen(cmdnode->path);
	if (arraylen > 1)
	{
		temppath = cmdnode->path;
		while (*(++temppath))
			doubleexportarg(*temppath);
	}
	else
		singleexportarg(cmdnode);
}

void	singleexportarg(t_cmdlist *cmdnode)
{
	t_env	*env;

	env = g_core.env_table;
	while (env)
	{
		write(cmdnode->outfile, "declare -x ", 11);
		write(cmdnode->outfile, env->env_name, ft_strlen(env->env_name));
		if (!env->content)
		{
			write(cmdnode->outfile, "\n", 1);
			env = env->next;
			continue ;
		}
		write(cmdnode->outfile, "=\"", 2);
		write(cmdnode->outfile, env->content, ft_strlen(env->content));
		write(cmdnode->outfile, "\"\n", 2);
		env = env->next;
	}
}

void	doubleexportarg(char *envcmd)
{
	char	*arg;
	int		isequal;
	t_env	*env;
	char	*tempenvname;

	if (!env_arg_control(envcmd))
		return ;
	tempenvname = getenvname(envcmd);
	arg = envcmd + ft_strlen(tempenvname);
	isequal = 0;
	if (*arg == '=')
		isequal |= 1;
	if (changeenv(tempenvname, ++arg, isequal))
	{
		free(tempenvname);
		return ;
	}
	env = g_core.env_table;
	add_newenv(&env, envcmd);
	if (!isequal)
		updateenv(envcmd, NULL);
	free(tempenvname);
}

int	changeenv(char *envname, char *arg, int isequal)
{
	t_env	*env;

	env = g_core.env_table;
	while (env)
	{
		if (str_compare(envname, env->env_name))
		{
			if (isequal)
				updateenv(envname, arg);
			return (1);
		}
		env = env->next;
	}
	return (0);
}