/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   builtins_utils3.c                                  :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: glugo-mu <glugo-mu@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/11/05 17:00:00 by glugo-mu          #+#    #+#             */
/*   Updated: 2025/11/05 17:00:00 by glugo-mu         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell2.h"

static void	free_cmd_node(t_cmdlist *cmd)
{
	if (cmd->heradoc_values)
		free(cmd->heradoc_values);
	if (cmd->cmd)
		free(cmd->cmd);
	if (cmd->path)
		free(cmd->path);
	free(cmd);
}

void	free_for_loop(void)
{
	t_cmdlist	*cmd;
	t_cmdlist	*next;

	cmd = g_core.cmd_table;
	while (cmd)
	{
		next = cmd->next;
		free_cmd_node(cmd);
		cmd = next;
	}
}

static void	free_env_node(t_env *env)
{
	if (env->env_name)
		free(env->env_name);
	if (env->content)
		free(env->content);
	free(env);
}

void	free_core(void)
{
	t_env	*env;
	t_env	*next;

	if (g_core.cmd)
		free(g_core.cmd);
	if (g_core.metachars)
		free(g_core.metachars);
	env = g_core.env_table;
	while (env)
	{
		next = env->next;
		free_env_node(env);
		env = next;
	}
}
