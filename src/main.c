/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   main.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: glugo-mu <glugo-mu@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/09/11 18:17:15 by glugo-mu          #+#    #+#             */
/*   Updated: 2025/10/08 09:37:28 by glugo-mu         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

int	main(int argc, char **argv, char **envp)
{
	char	*input;
	char	**args;
	char	**my_env;
	int		i;

	(void)argc;
	(void)argv;
	(void)envp;
	// Hardcode envp for debugging
	char *fake_env[] = {
		"USER=german",
		"PWD=/Users/german",
		"PATH=/usr/bin:/bin",
		NULL
	};
	my_env = copy_env(fake_env);
	if (!my_env)
		return (1);
	while (1)
	{
		input = readline("minishell> ");
		if (!input)
		{
			printf("exit\n");
			break ;
		}
		if (*input)
			add_history(input);
		if (ft_strcmp(input, "exit") == 0)
		{
			free(input);
			break ;
		}
		else if (ft_strcmp(input, "env") == 0)
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
				printf("Command not recognized (yet): %s\n", input);
			free(args);
		}
		free(input);
	}
	return (0);
}
