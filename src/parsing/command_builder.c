/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   command_builder.c                                  :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: glugo-mu <glugo-mu@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/10/09 12:25:41 by glugo-mu          #+#    #+#             */
/*   Updated: 2025/10/15 19:39:59 by glugo-mu         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

static t_cmd	*cmd_new(void)
{
	t_cmd	*cmd;

	cmd = malloc(sizeof(*cmd));
	if (!cmd)
		return (NULL);
	cmd->argv = NULL;
	cmd->argc = 0;
	cmd->redirs = NULL;
	cmd->next = NULL;
	return (cmd);
}

static int	append_arg(t_cmd *cmd, const char *value, char **error)
{
	char	**argv;
	size_t	i;

	argv = malloc(sizeof(char *) * (cmd->argc + 2));
	if (!argv)
	{
		token_set_error(error, "allocation");
		return (0);
	}
	i = 0;
	while (i < cmd->argc)
	{
		argv[i] = cmd->argv[i];
		i++;
	}
	argv[i] = ft_strdup(value);
	if (!argv[i])
	{
		free(argv);
		token_set_error(error, "allocation");
		return (0);
	}
	argv[i + 1] = NULL;
	free(cmd->argv);
	cmd->argv = argv;
	cmd->argc++;
	return (1);
}

static int	append_redir(t_cmd *cmd, t_token_type type, const char *target,
		char **error)
{
	t_redir	*node;
	t_redir	*last;

	node = malloc(sizeof(*node));
	if (!node)
	{
		token_set_error(error, "allocation");
		return (0);
	}
	node->target = ft_strdup(target);
	if (!node->target)
	{
		free(node);
		token_set_error(error, "allocation");
		return (0);
	}
	node->type = type;
	node->next = NULL;
	if (!cmd->redirs)
		cmd->redirs = node;
	else
	{
		last = cmd->redirs;
		while (last->next)
			last = last->next;
		last->next = node;
	}
	return (1);
}

static int	fill_command(t_cmd *cmd, t_token **cursor, char **error)
{
	t_token		*token;
	t_token		*target;

	while (*cursor && (*cursor)->type != TOKEN_PIPE)
	{
		token = *cursor;
		if (token->type == TOKEN_WORD)
		{
			if (!append_arg(cmd, token->value, error))
				return (0);
			*cursor = token->next;
			continue;
		}
		target = token->next;
		if (!target)
		{
			token_set_error(error, "newline");
			return (0);
		}
		if (!append_redir(cmd, token->type, target->value, error))
			return (0);
		*cursor = target->next;
	}
	return (1);
}

t_cmd	*commands_from_tokens(t_token *tokens, char **error)
{
	t_cmd	*head;
	t_cmd	*tail;
	t_cmd	*cmd;
	t_token	*cursor;

	cursor = tokens;
	head = NULL;
	tail = NULL;
	if (error)
		*error = NULL;
	while (cursor)
	{
		cmd = cmd_new();
		if (!cmd)
		{
			token_set_error(error, "allocation");
			cmd_clear(&head);
			return (NULL);
		}
		if (!fill_command(cmd, &cursor, error))
		{
			cmd_clear(&cmd);
			cmd_clear(&head);
			return (NULL);
		}
		if (!head)
			head = cmd;
		else
			tail->next = cmd;
		tail = cmd;
		if (cursor && cursor->type == TOKEN_PIPE)
			cursor = cursor->next;
	}
	return (head);
}
