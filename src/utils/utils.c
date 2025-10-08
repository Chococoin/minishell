/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   utils.c                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: glugo-mu <glugo-mu@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/10/02 14:33:32 by glugo-mu          #+#    #+#             */
/*   Updated: 2025/10/08 09:04:48 by glugo-mu         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

int	ft_strlen(const char *s)
{
	int	i;

	i = 0;
	while (s[i])
		i++;
	return (i);
}

char	*ft_strdup(const char *s)
{
	char	*ptr;
	int		len;
	int		i;

	len = ft_strlen(s);
	ptr = (char *)malloc(sizeof(char) * (len + 1));
	if (!ptr)
		return (NULL);
	i = 0;
	while (i < len)
	{
		ptr[i] = s[i];
		i++;
	}
	ptr[i] = '\0';
	return (ptr);
}

int	ft_strcmp(const char *s1, const char *s2)
{
	size_t	i;

	i = 0;
	while (s1[i] || s2[i])
	{
		if (s1[i] != s2[i])
			return ((unsigned char)s1[i] - (unsigned char)s2[i]);
		i++;
	}
	return (0);
}

char **copy_env(char **envp)
{
	int		count;
	char	**my_env;
	int		i;

	count = 0;
	while(envp[count])
		count++;
	my_env = malloc((count + 1) * sizeof(*my_env));
	if (!my_env)
		return (NULL);
	i = 0;
	while (i <= count)
	{
		my_env[i] = ft_strdup(envp[i]);
		if (!my_env[i])
		{
			while (--i >= 0)
				free(my_env[i]);
			free(my_env);
			return (NULL);
		}
		i++;
	}
	my_env[i] = NULL;
	return (my_env);
}

char **split_input(char *input)
{
	char	**args;
	char	*token;
	int		i;

	args = malloc(64 * sizeof(char *));
	if (!args)
		return (NULL);
	token = strtok(input, " ");
	i = 0;
	while (token != NULL)
	{
		args[i++] = token;
		token = strtok(NULL, " ");
	}
	args[i] = NULL;
	return (args);
}

int is_builtin(char *cmd)
{
	if (!cmd)
		return (0);
	if (strcmp(cmd, "echo") == 0)
		return (1);
	if (strcmp(cmd, "env") == 0)
		return (1);
	return (0);
}

// Mock: prints which built-in was detected
int exec_builtin(char **args)
{
	if (!args || !args[0])
		return (1);
	if (strcmp(args[0], "echo") == 0)
	{
		printf("[MOCK] Built-in echo detected\n");
		return (0);
	}
	if (strcmp(args[0], "env") == 0)
	{
		printf("[MOCK] Built-in env detected\n");
		return (0);
	}
	return (1);
}
