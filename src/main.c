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
#include "minishell2.h"

t_core		g_core;
t_cmdlist	*cmd_to_cmdlist(t_cmd *cmd);
void		free_cmdlist_adapter(t_cmdlist *cmdlist);

static t_token	*process_input(char *input, char ***parts_out, char **orig_out)
{
	char	**parts;
	char	*error;
	t_token	*tokens;

	*orig_out = ft_strdup(input);
	if (!*orig_out || !validate_input(input))
		return (NULL);
	parts = split_input(input);
	*parts_out = parts;
	if (!parts)
		return (NULL);
	error = NULL;
	tokens = tokenize(parts, &error);
	if (!tokens && error)
	{
		printf("minishell: syntax error near unexpected token `%s'\n", error);
		free(error);
	}
	return (tokens);
}

static int	execute_builtin(t_cmd *cmd)
{
	return (execute_builtin_simple(cmd));
}

static int	execute_cmd(t_cmd *first, char **my_env, char *original)
{
	int	status;

	(void)original;
	status = 0;
	if (first->next)
		status = execute_pipeline(first, my_env);
	else if (first->argv && first->argv[0])
	{
		if (isbuiltin(first->argv[0]))
			status = execute_builtin(first);
		else
			status = execute_external(first, my_env);
	}
	return (status);
}

static int	process_loop(char **my_env, int *exit_status)
{
	char	*input;
	char	**parts;
	char	*original;
	t_token	*tokens;
	t_cmd	*cmds;

	input = readline("minishell> ");
	if (!input || ft_strcmp(input, "exit") == 0)
		return (free_resources(input, NULL, NULL), 0);
	if (*input)
		add_history(input);
	tokens = process_input(input, &parts, &original);
	if (tokens)
	{
		expand_tokens(tokens, my_env, *exit_status);
		cmds = commands_from_tokens(tokens, NULL);
		if (cmds)
		{
			*exit_status = execute_cmd(cmds, my_env, original);
			cmd_clear(&cmds);
		}
		token_clear(&tokens);
	}
	return (free_resources(input, parts, original), 1);
}

int	main(int argc, char **argv, char **envp)
{
	char	**my_env;
	int		exit_status;

	(void)argc;
	(void)argv;
	exit_status = 0;
	if (!envp || !envp[0])
		my_env = init_env();
	else
		my_env = copy_env(envp);
	if (!my_env)
		return (1);
	init_global_env(my_env);
	g_core.my_env = my_env;
	setup_signals();
	while (process_loop(g_core.my_env, &exit_status))
		;
	free_env(g_core.my_env);
	return (0);
}
