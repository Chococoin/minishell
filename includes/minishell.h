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

int		is_builtin(char *cmd);
int		exec_builtin(char **args);
char	*ft_strdup(const char *s);
int		ft_strcmp(const char *s1, const char *s2);
char	**copy_env(char **envp);
char	**split_input(char *input);
int		ft_strlen(const char *s);

#endif
