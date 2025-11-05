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

void	straddchar(char **dst, char c)
{
	char	*ptr;
	char	*holderptr;
	char	*holderdst;
	int		len;

	len = ft_strlen(*dst) + 1;
	ptr = (char *)malloc(sizeof(char) * (len + 1));
	holderptr = ptr;
	holderdst = *dst;
	while (holderdst && *holderdst)
		*(holderptr++) = *(holderdst++);
	*(holderptr++) = c;
	*(holderptr++) = 0;
	if (*dst)
		free(*dst);
	*dst = ptr;
}
