/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   echo.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: glugo-mu <glugo-mu@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/10/08 15:33:10 by siellage          #+#    #+#             */
/*   Updated: 2025/11/07 10:57:38 by glugo-mu         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell2.h"

void	runecho(t_cmdlist *cmdnode)
{
	char	**path;
	int		isn;

	isn = 0;
	path = &cmdnode->path[1];
	while (*path && str_compare(*path, "-n"))
	{
		isn = 1;
		path++;
	}
	while (*path)
	{
		if (*path)
			write(cmdnode->outfile, *path, ft_strlen(*path));
		if (*(++path))
			write(cmdnode->outfile, " ", 1);
	}
	if (!isn)
		write(cmdnode->outfile, "\n", 1);
}
