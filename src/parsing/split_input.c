/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   split_input.c                                      :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: glugo-mu <glugo-mu@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/10/08 16:42:11 by glugo-mu          #+#    #+#             */
/*   Updated: 2025/10/08 17:18:41 by glugo-mu         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

static int	is_blank(char c)
{
	return (c == ' ' || c == '\t');
}

static int	ensure_capacity(t_split_ctx *ctx)
{
	char	**new_args;
	size_t	new_capacity;
	size_t	i;

	if (ctx->count < ctx->capacity)
		return (1);
	new_capacity = ctx->capacity * 2;
	new_args = malloc(sizeof(char *) * (new_capacity + 1));
	if (!new_args)
		return (0);
	i = 0;
	while (i < ctx->count)
	{
		new_args[i] = ctx->tokens[i];
		i++;
	}
	free(ctx->tokens);
	ctx->tokens = new_args;
	ctx->capacity = new_capacity;
	return (1);
}

static int	add_token(t_split_ctx *ctx, char *input, size_t *idx)
{
	size_t	i;
	char	quote;

	if (!ensure_capacity(ctx))
		return (0);
	ctx->tokens[ctx->count++] = &input[*idx];
	i = *idx;
	while (input[i] && !is_blank(input[i]))
	{
		if (input[i] == '\'' || input[i] == '\"')
		{
			quote = input[i++];
			while (input[i] && input[i] != quote)
				i++;
			if (!input[i])
				return (0);
		}
		if (input[i])
			i++;
	}
	if (input[i])
		input[i++] = '\0';
	*idx = i;
	return (1);
}

static int	fill_tokens(t_split_ctx *ctx, char *input)
{
	size_t	i;

	i = 0;
	while (input[i])
	{
		while (input[i] && is_blank(input[i]))
			i++;
		if (!input[i])
			break ;
		if (!add_token(ctx, input, &i))
			return (0);
	}
	return (1);
}

char	**split_input(char *input)
{
	t_split_ctx	ctx;

	if (!input)
		return (NULL);
	ctx.capacity = 8;
	ctx.tokens = malloc(sizeof(char *) * (ctx.capacity + 1));
	if (!ctx.tokens)
		return (NULL);
	ctx.count = 0;
	if (!fill_tokens(&ctx, input))
	{
		free(ctx.tokens);
		return (NULL);
	}
	ctx.tokens[ctx.count] = NULL;
	return (ctx.tokens);
}
