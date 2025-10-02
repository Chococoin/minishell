/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   main.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: glugo-mu <glugo-mu@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/09/11 18:17:15 by glugo-mu          #+#    #+#             */
/*   Updated: 2025/10/01 16:17:06 by glugo-mu         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"
#include <string.h>

static char **copy_env(char **envp)
{
	int i = 0;
	int count = 0;
	char **my_env;

	while (envp[count])
		count++;
	my_env = malloc((count + 1) * sizeof(char *));
	if (!my_env)
		return (NULL);
	while (i < count)
	{
		my_env[i] = strdup(envp[i]);
		i++;
	}
	my_env[count] = NULL;
	return (my_env);
}

int	main(int argc, char **argv, char **envp)
{
	char	*input;
	char	**args;
	char	**my_env;
	int		i;

	(void)argc;
	(void)argv;
	my_env = copy_env(envp);
	if (!my_env)
		return (1);

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
		else if (strcmp(input, "env") == 0)
		{
			i = 0;
			while (my_env[i])
			{
				printf("%s\n", my_env[i]);
				i++;
			}
		}
		else
		{
			args = split_input(input);
			if (args && is_builtin(args[0]))
				exec_builtin(args);
			else
				printf("Comando no reconocido (aún): %s\n", input);
			free(args); // libera solo la lista, no los tokens (apuntan dentro de input)
		}
		free(input);
	}
	return (0);
}