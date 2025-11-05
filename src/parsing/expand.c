/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   expand.c                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: glugo-mu <glugo-mu@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/11/05 00:00:00 by glugo-mu          #+#    #+#             */
/*   Updated: 2025/11/05 00:00:00 by glugo-mu         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

/*
** Obtiene el valor de una variable de entorno
** Si es $?, devuelve el exit status como string
** Si es $VAR, busca en el entorno
*/
static char	*get_var_value(char *var_name, char **envp, int exit_status)
{
	size_t	i;
	size_t	len;

	if (!var_name)
		return (ft_strdup(""));
	if (ft_strcmp(var_name, "?") == 0)
		return (ft_itoa(exit_status));
	len = ft_strlen(var_name);
	i = 0;
	while (envp && envp[i])
	{
		if (ft_strncmp(envp[i], var_name, len) == 0
			&& envp[i][len] == '=')
			return (ft_strdup(envp[i] + len + 1));
		i++;
	}
	return (ft_strdup(""));
}

/*
** Extrae el nombre de la variable después del $
** Reglas: [a-zA-Z_][a-zA-Z0-9_]* o solo '?'
*/
static char	*extract_var_name(const char *str, size_t *len)
{
	size_t	i;
	char	*name;

	if (!str || str[0] != '$')
		return (NULL);
	if (str[1] == '?')
	{
		*len = 2;
		return (ft_strdup("?"));
	}
	i = 1;
	if (!ft_isalpha(str[i]) && str[i] != '_')
	{
		*len = 1;
		return (ft_strdup(""));
	}
	while (str[i] && (ft_isalnum(str[i]) || str[i] == '_'))
		i++;
	*len = i;
	name = malloc(i);
	if (!name)
		return (NULL);
	ft_strlcpy(name, str + 1, i);
	return (name);
}

/*
** Expande todas las variables $VAR y $? en un string
** Respeta el contexto de comillas (quote_mode)
** quote_mode: 0 = sin comillas, 1 = comillas simples, 2 = comillas dobles
*/
static char	*expand_variables(const char *str, char **envp, int exit_status,
		int quote_mode)
{
	size_t	i;
	size_t	var_len;
	char	*var_name;
	char	*var_value;
	char	*result;
	char	*temp;

	if (!str)
		return (NULL);
	result = ft_strdup("");
	if (!result)
		return (NULL);
	i = 0;
	while (str[i])
	{
		if (str[i] == '$' && quote_mode != 1)
		{
			var_name = extract_var_name(str + i, &var_len);
			if (var_name && var_name[0])
			{
				var_value = get_var_value(var_name, envp, exit_status);
				temp = result;
				result = ft_strjoin(result, var_value);
				free(temp);
				free(var_value);
			}
			else
			{
				temp = result;
				result = ft_strjoin_char(result, '$');
				free(temp);
			}
			free(var_name);
			i += var_len;
		}
		else
		{
			temp = result;
			result = ft_strjoin_char(result, str[i]);
			free(temp);
			i++;
		}
	}
	return (result);
}

/*
** Remueve las comillas de un string y determina el modo de expansión
** Retorna el string sin comillas y actualiza quote_mode si se pasó
** quote_mode: 0 = sin comillas, 1 = comillas simples, 2 = comillas dobles
*/
static char	*remove_quotes_and_get_mode(const char *str, int *quote_mode)
{
	size_t	i;
	size_t	j;
	char	*result;
	char	in_quote;

	if (!str)
		return (NULL);
	result = malloc(ft_strlen(str) + 1);
	if (!result)
		return (NULL);
	i = 0;
	j = 0;
	in_quote = 0;
	*quote_mode = 0;
	while (str[i])
	{
		if ((str[i] == '\'' || str[i] == '\"') && !in_quote)
		{
			in_quote = str[i];
			if (str[i] == '\'')
				*quote_mode = 1;
			else
				*quote_mode = 2;
		}
		else if (str[i] == in_quote)
			in_quote = 0;
		else
			result[j++] = str[i];
		i++;
	}
	result[j] = '\0';
	return (result);
}

/*
** Procesa un valor de token: remueve comillas y expande variables
** según el tipo de comillas encontrado
*/
static char	*process_token_value(const char *value, char **envp,
		int exit_status)
{
	int		quote_mode;
	char	*unquoted;
	char	*expanded;

	if (!value)
		return (NULL);
	unquoted = remove_quotes_and_get_mode(value, &quote_mode);
	if (!unquoted)
		return (NULL);
	expanded = expand_variables(unquoted, envp, exit_status, quote_mode);
	free(unquoted);
	return (expanded);
}

/*
** Expande todas las variables en una lista de tokens
** Solo procesa tokens de tipo TOKEN_WORD
** Modifica los valores de los tokens en su lugar
*/
void	expand_tokens(t_token *tokens, char **envp, int exit_status)
{
	t_token	*current;
	char	*new_value;

	current = tokens;
	while (current)
	{
		if (current->type == TOKEN_WORD && current->value)
		{
			new_value = process_token_value(current->value, envp,
					exit_status);
			if (new_value)
			{
				free(current->value);
				current->value = new_value;
			}
		}
		current = current->next;
	}
}
