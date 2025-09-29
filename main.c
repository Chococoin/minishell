/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   main.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: glugo-mu <glugo-mu@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/09/11 18:17:15 by glugo-mu          #+#    #+#             */
/*   Updated: 2025/09/29 16:55:09 by glugo-mu         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */


#include "minishell.h"

static char **split_input(char *input)
{
	char **args = malloc(64 * sizeof(char *));
	char *token;
	int i = 0;

	if (!args)
		return NULL;
	token = strtok(input, " ");
	while (token)
	{
		args[i++] = token;
		token = strtok(NULL, " ");
	}
	args[i] = NULL;
	return args;
}

int main(void)
{
	char *input;
	char **args;

	while (1)
	{
		input = readline("minishell$ ");
		if (!input) // Ctrl-D
		{
			printf("exit\n");
			break;
		}
		if (*input)
			add_history(input);

		if (strcmp(input, "exit") == 0)
		{
			free(input);
			break;
		}

		args = split_input(input);
		if (args && is_builtin(args[0]))
			exec_builtin(args);
		else
			printf("Comando no reconocido (aún): %s\n", input);

		free(args);   // Ojo: liberamos solo la lista, no los tokens (apuntan dentro de input)
		free(input);
	}
	return (0);
}