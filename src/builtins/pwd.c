/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   pwd.c                                              :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: glugo-mu <glugo-mu@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/10/08 15:34:06 by siellage          #+#    #+#             */
/*   Updated: 2025/10/31 16:24:29 by glugo-mu         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell2.h"

int	runpwd(t_cmdlist *cmdnode)
{
	char	cwd[256];
	char	*pwd;

	pwd = getcwd(cwd, 256);
	if (!pwd)
		return (1);
	write(cmdnode->outfile, pwd, ft_strlen(pwd));
	write(cmdnode->outfile, "\n", 1);
	return (0);
}
