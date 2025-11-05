/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   cd.c                                               :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: glugo-mu <glugo-mu@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/10/08 15:32:04 by siellage          #+#    #+#             */
/*   Updated: 2025/10/31 16:28:10 by glugo-mu         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell2.h"

void	runcd(t_cmdlist *cmdnode)
{
	int		arraylen;

	arraylen = getarraylen(cmdnode->path);
	if (arraylen > 2)
	{
		g_core.exec_output = 1;
		print_error("-bash: cd: too many arguments\n", NULL, NULL);
	}
	else if (arraylen == 2)
		cddoublearg(cmdnode);
	else
		cdsinglearg();
}

void	cdsinglearg(void)
{
	t_env	*tempenv;
	char	*content;

	tempenv = g_core.env_table;
	while (tempenv)
	{
		if (str_compare("HOME", tempenv->env_name))
		{
			content = tempenv->content;
			if (!content)
				return ;
			changedir(content);
			return ;
		}
		tempenv = tempenv->next;
	}
	print_error("-bash: cd: HOME not set\n", NULL, NULL);
	g_core.exec_output |= 1;
}

void	cddoublearg(t_cmdlist *cmdnode)
{
	if (!changedir(cmdnode->path[1]))
	{
		print_error("--bash: cd: ", cmdnode->path[1],
			": No such file or directory\n");
		g_core.exec_output = 1;
		return ;
	}
}

int	changedir(char *path)
{
	char	pwd[256];
	int		ispwdaccess;
	char	*oldpwd;
	int		error;

	oldpwd = ft_strdup(getcwd(pwd, 256));
	error = chdir(path);
	if (error == -1)
	{
		if (oldpwd)
			free(oldpwd);
		return (0);
	}
	ispwdaccess = updatepwdfromexport("PWD", getcwd(pwd, 256));
	if (ispwdaccess)
		updatepwdfromexport("OLDPWD", oldpwd);
	else
		deleteenv("OLDPWD");
	if (oldpwd)
		free(oldpwd);
	changetitle();
	return (1);
}

int	updatepwdfromexport(char *pwdname, char *pwdcontent)
{
	t_env	*tempenv;
	char	*temppwd;

	if (!updateenv(pwdname, pwdcontent))
	{
		tempenv = g_core.env_table;
		temppwd = NULL;
		ownstrjoin(&temppwd, pwdname);
		straddchar(&temppwd, '=');
		ownstrjoin(&temppwd, pwdcontent);
		add_newenv(&tempenv, temppwd);
		free(temppwd);
		return (0);
	}
	return (1);
}
