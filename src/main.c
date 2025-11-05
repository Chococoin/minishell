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

static void	execute_builtin(t_cmd *cmd)
{
	t_cmdlist	*cmdlist;
	int			builtin_type;

	cmdlist = cmd_to_cmdlist(cmd);
	if (!cmdlist)
		return ;
	builtin_type = isbuiltin(cmd->argv[0]);
	if (builtin_type)
		runbuiltin(cmdlist, builtin_type, NULL, -1);
	free_cmdlist_adapter(cmdlist);
}

static void	execute_cmd(t_cmd *first, char **my_env, char *original)
{
	(void)my_env;
	if (first->next)
		printf("Command pipeline not supported (yet)\n");
	else if (first->argv && first->argv[0])
	{
		if (isbuiltin(first->argv[0]))
			execute_builtin(first);
		else
			printf("Command not recognized (yet): %s\n", original);
	}
	else
		printf("Command not recognized (yet): %s\n", original);
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
			(execute_cmd(cmds, my_env, original), cmd_clear(&cmds));
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
	(void)envp;
	exit_status = 0;
	my_env = init_env();
	if (!my_env)
		return (1);
	init_global_env(my_env);
	while (process_loop(my_env, &exit_status))
		;
	free_env(my_env);
	return (0);
}
