/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   utilits2.c                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: glugo-mu <glugo-mu@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/11/05 00:00:00 by glugo-mu          #+#    #+#             */
/*   Updated: 2025/11/05 00:00:00 by glugo-mu         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell2.h"

static size_t	get_len(char *str)
{
	if (!str)
		return (0);
	return (ft_strlen(str));
}

void	ownstrjoin(char **dst, char *src)
{
	char	*ptr;
	char	*hp;
	char	*hd;
	size_t	len;

	if (!*dst && !src)
		return ;
	len = get_len(*dst) + get_len(src);
	ptr = (char *)malloc(sizeof(char) * (len + 1));
	ptr[len] = 0;
	hp = ptr;
	hd = *dst;
	while (hd && *hd)
		*(hp++) = *(hd++);
	while (src && *src)
		*(hp++) = *(src++);
	if (*dst)
		free(*dst);
	*dst = ptr;
}

void	straddchar(char **dst, char c)
{
	char	*ptr;
	char	*hp;
	char	*hd;
	int		len;

	len = ft_strlen(*dst) + 1;
	ptr = (char *)malloc(sizeof(char) * (len + 1));
	hp = ptr;
	hd = *dst;
	while (hd && *hd)
		*(hp++) = *(hd++);
	*(hp++) = c;
	*(hp++) = 0;
	if (*dst)
		free(*dst);
	*dst = ptr;
}
