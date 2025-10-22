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
	char	**parts;
	char	**my_env;
	char	*original;
	t_token	*tokens;
	char	*error;
	t_cmd	*cmds;
	char	*cmd_error;
	t_cmd	*first;
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
		original = ft_strdup(input);
		if (!original)
		{
			free(input);
			continue ;
		}
		parts = split_input(input);
		if (!parts)
		{
			printf("minishell: unexpected EOF while looking for matching quote\n");
			free(original);
			free(input);
			continue ;
		}
		error = NULL;
		tokens = tokenize(parts, &error);
		if (!tokens)
		{
			if (error)
				printf("minishell: syntax error near unexpected token `%s'\n",
					error);
			free(error);
			free(parts);
			free(original);
			free(input);
			continue ;
		}
		cmd_error = NULL;
		cmds = commands_from_tokens(tokens, &cmd_error);
		if (!cmds)
		{
			if (cmd_error)
				printf("minishell: %s\n", cmd_error);
			free(cmd_error);
			token_clear(&tokens);
			free(error);
			free(parts);
			free(original);
			free(input);
			continue ;
		}
		first = cmds;
		if (first->next)
			printf("Command pipeline not supported (yet)\n");
		else if (first->argv && first->argv[0])
		{
			if (ft_strcmp(first->argv[0], "env") == 0)
			{
				i = 0;
				while (my_env[i])
				{
					printf("%s\n", my_env[i]);
					i++;
				}
			}
			else if (is_builtin(first->argv[0]))
				exec_builtin(first->argv);
			else
				printf("Command not recognized (yet): %s\n", original);
		}
		else
			printf("Command not recognized (yet): %s\n", original);
		cmd_clear(&cmds);
		token_clear(&tokens);
		free(cmd_error);
		free(error);
		free(parts);
		free(original);
		free(input);
	}
	return (0);
}
