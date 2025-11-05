/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   validate.c                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: glugo-mu <glugo-mu@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/11/05 00:00:00 by glugo-mu          #+#    #+#             */
/*   Updated: 2025/11/05 00:00:00 by glugo-mu         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

/*
** Verifica si un carácter es un metacarácter no soportado
** según el subject de minishell
*/
static int	is_unsupported_char(char c)
{
	return (c == ';' || c == '\\' || c == '&');
}

/*
** Valida que no haya caracteres especiales no soportados
** fuera de comillas en el input
** Retorna 1 si es válido, 0 si hay caracteres no soportados
*/
int	validate_input(const char *input)
{
	size_t	i;
	char	quote;

	if (!input)
		return (1);
	i = 0;
	quote = 0;
	while (input[i])
	{
		if ((input[i] == '\'' || input[i] == '\"') && !quote)
			quote = input[i];
		else if (input[i] == quote)
			quote = 0;
		else if (!quote && is_unsupported_char(input[i]))
		{
			printf("minishell: syntax error near unexpected token `%c'\n",
				input[i]);
			return (0);
		}
		i++;
	}
	return (1);
}
