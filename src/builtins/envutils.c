/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   envutils.c                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: glugo-mu <glugo-mu@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/10/16 11:23:23 by siellage          #+#    #+#             */
/*   Updated: 2025/11/07 11:00:40 by glugo-mu         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell2.h"

int	getenvlen(void)
{
	t_env	*envlist;
	int		count;

	envlist = g_core.env_table;
	count = 0;
	while (envlist)
	{
		if (envlist->content)
			count++;
		envlist = envlist->next;
	}
	return (count);
}

char	**getenvcpy(void)
{
	int		envlen;
	char	**envlist;
	char	*tempenv;
	t_env	*tempenvlist;

	envlen = getenvlen();
	envlist = (char **)malloc(sizeof(char *) * (envlen + 1));
	envlist[envlen] = NULL;
	tempenvlist = g_core.env_table;
	while (tempenvlist)
	{
		if (tempenvlist->content)
		{
			tempenv = NULL;
			ownstrjoin(&tempenv, tempenvlist->env_name);
			straddchar(&tempenv, '=');
			ownstrjoin(&tempenv, tempenvlist->content);
			envlist[--envlen] = tempenv;
		}
		tempenvlist = tempenvlist->next;
	}
	return (envlist);
}

void	freeenvcpy(char **envlist)
{
	char	**tempenvlist;

	tempenvlist = envlist;
	while (*tempenvlist)
		free(*(tempenvlist++));
	free(envlist);
}

void	fillenvs(char **env)
{
	while (env && *env)
		addnewenv(&g_core.env_table, *(env++));
}

t_env	*addnewenv(t_env **envtable, char *env)
{
	t_env	*lastnode;
	char	*content;

	if (!*envtable)
	{
		*envtable = (t_env *)malloc(sizeof(t_env));
		lastnode = *envtable;
	}
	else
	{
		lastnode = *envtable;
		while (lastnode->next)
			lastnode = lastnode->next;
		lastnode->next = (t_env *)malloc(sizeof(t_env));
		lastnode = lastnode->next;
	}
	lastnode->env_name = getenvname(env);
	content = env + ft_strlen(lastnode->env_name);
	if (*content == '=' && *(content + 1))
		lastnode->content = ft_strdup(content + 1);
	else
		lastnode->content = NULL;
	lastnode->next = NULL;
	return (lastnode);
}
