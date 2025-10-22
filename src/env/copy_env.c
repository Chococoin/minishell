/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   copy_env.c                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: glugo-mu <glugo-mu@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/10/08 17:37:11 by glugo-mu          #+#    #+#             */
/*   Updated: 2025/10/08 17:37:11 by glugo-mu         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

static int	env_count(char **envp)
{
	int	count;

	count = 0;
	if (!envp)
		return (0);
	while (envp[count])
		count++;
	return (count);
}

static void	free_env_partial(char **env, int last)
{
	while (last >= 0)
	{
		free(env[last]);
		last--;
	}
	free(env);
}

char	**copy_env(char **envp)
{
	char	**my_env;
	int		count;
	int		i;

	if (!envp)
		return (NULL);
	count = env_count(envp);
	my_env = malloc(sizeof(char *) * (count + 1));
	if (!my_env)
		return (NULL);
	i = 0;
	while (i < count)
	{
		my_env[i] = ft_strdup(envp[i]);
		if (!my_env[i])
		{
			free_env_partial(my_env, i - 1);
			return (NULL);
		}
		i++;
	}
	my_env[count] = NULL;
	return (my_env);
}
