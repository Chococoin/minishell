/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   utilits.c                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: glugo-mu <glugo-mu@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/10/08 15:32:40 by siellage          #+#    #+#             */
/*   Updated: 2025/10/31 16:55:24 by glugo-mu         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell2.h"

int	getarraylen(char **array)
{
	int	count;

	count = 0;
	while (array && *(array++))
		count++;
	return (count);
}

void	print_error(char *ptr1, char *ptr2, char *ptr3)
{
	if (ptr1)
		write(2, ptr1, ft_strlen(ptr1));
	if (ptr2)
		write(2, ptr2, ft_strlen(ptr2));
	if (ptr3)
		write(2, ptr3, ft_strlen(ptr3));
}

void	changetitle(void)
{
	char	cwd[256];

	if (g_core.title.full_title)
		free(g_core.title.full_title);
	g_core.title.full_title = NULL;
	ownstrjoin(&g_core.title.full_title, g_core.title.head);
	ownstrjoin(&g_core.title.full_title, (char *)getcwd(cwd, 256));
	ownstrjoin(&g_core.title.full_title, "$ ");
}

int	updateenv(char *envname, char *newarg)
{
	t_env	*env;

	env = g_core.env_table;
	while (env)
	{
		if (str_compare(env->env_name, envname))
		{
			if (env->content)
				free(env->content);
			env->content = NULL;
			if (!newarg)
				return (1);
			ownstrjoin(&env->content, newarg);
			return (1);
		}
		env = env->next;
	}
	return (0);
}

void	ownstrjoin(char **dst, char *src)
{
	char	*ptr;
	char	*holderptr;
	char	*holderdst;
	size_t	len;

	if (!*dst && !src)
		return ;
	len = ft_strlen(*dst) + ft_strlen(src);
	ptr = (char *)malloc(sizeof(char) * (len + 1));
	ptr[len] = 0;
	holderptr = ptr;
	holderdst = *dst;
	while (holderdst && *holderdst)
		*(holderptr++) = *(holderdst++);
	while (src && *src)
		*(holderptr++) = *(src++);
	if (*dst)
		free(*dst);
	*dst = ptr;
}
