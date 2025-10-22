/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   minishell.h                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: glugo-mu <glugo-mu@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/09/11 17:59:21 by glugo-mu          #+#    #+#             */
/*   Updated: 2025/10/02 16:45:22 by glugo-mu         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef MINISHELL_H
# define MINISHELL_H

# include <stdio.h>
# include <stdlib.h>
# include <string.h>
# include <unistd.h>
# include <readline/readline.h>
# include <readline/history.h>

typedef struct s_split_ctx
{
	char	**tokens;
	size_t	count;
	size_t	capacity;
}	t_split_ctx;

typedef enum e_token_type
{
	TOKEN_WORD,
	TOKEN_PIPE,
	TOKEN_REDIR_IN,
	TOKEN_REDIR_OUT,
	TOKEN_REDIR_APPEND,
	TOKEN_HEREDOC
}	t_token_type;

typedef struct s_token
{
	char			*value;
	t_token_type	type;
	struct s_token	*next;
}	t_token;

typedef struct s_token_iter
{
	char		**parts;
	size_t		index;
	t_token_type	prev;
}	t_token_iter;

typedef struct s_redir
{
	t_token_type	type;
	char			*target;
	struct s_redir	*next;
}	t_redir;

typedef struct s_cmd
{
	char		**argv;
	size_t		argc;
	t_redir		*redirs;
	struct s_cmd	*next;
}	t_cmd;

int		is_builtin(char *cmd);
int		exec_builtin(char **args);
char	*ft_strdup(const char *s);
int		ft_strcmp(const char *s1, const char *s2);
char	**copy_env(char **envp);
char	**split_input(char *input);
int		ft_strlen(const char *s);
t_token	*tokenize(char **parts, char **error);
void	token_clear(t_token **tokens);
int		token_add_new(t_token **tokens, char *value, t_token_type type);
int		token_is_redirect(t_token_type type);
int		token_set_error(char **error, const char *token);
t_cmd	*commands_from_tokens(t_token *tokens, char **error);
void	cmd_clear(t_cmd **cmds);

#endif
