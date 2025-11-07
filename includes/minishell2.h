/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   minishell2.h                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: glugo-mu <glugo-mu@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/10/07 12:11:44 by siellage          #+#    #+#             */
/*   Updated: 2025/11/07 14:10:40 by glugo-mu         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef MINISHELL2_H
# define MINISHELL2_H

# include <stdio.h>
# include <stdlib.h>
# include <string.h>
# include <unistd.h>
# include <readline/readline.h>
# include <readline/history.h>
# include "libft.h"

#define ECHO 1
#define CD 2
#define PWD 3
#define EXPORT 4
#define UNSET 5
#define ENV 6
#define EXIT 7
#define HERADOC -2
#define SSTDERR 2

typedef struct s_filelist
{
	char				*metachar;
	char				*filename;
	int					fd;
	struct s_filelist	*next;
}	t_filelist;

typedef struct s_lexlist
{
	char				type;
	char				*content;
	struct s_lexlist	*next;
}	t_lexlist;


typedef struct s_title
{
	char	*head;
	char	*full_title;
}	t_title;

typedef struct s_env
{
	char			*env_name;
	char			*content;
	struct s_env	*next;
}	t_env;

typedef struct s_cmdlist
{
	int					infile;
	int					outfile;
	int					pid;
	char				*heradoc_values;
	char				*cmd;
	char				**path;
	t_filelist			*files;
	struct s_cmdlist	*next;
}	t_cmdlist;

typedef struct s_core
{
	int			main_pid;
	int			exec_output;
	int			old_exec_output;
	int			heradoc_fd;
	int			is_read_arg;
	char		*cmd;
	char		**metachars;
	char		**my_env;
	t_env		*env_table;
	t_lexlist	*lex_table;
	t_cmdlist	*cmd_table;
	t_title		title;
}	t_core;

t_core g_core;


int	isbuiltin(char *cmd);
void	runbuiltin(t_cmdlist *cmdnode, int builtin, int *fd, int fd_index);
void	runecho(t_cmdlist *cmdnode);
void	runenv(t_cmdlist *cmdnode);
int	updateenv(char *envname, char *newarg);
void	runpwd(t_cmdlist *cmdnode);

//cd
void	runcd(t_cmdlist *cmdnode);
void	cdsinglearg(void);
void	cddoublearg(t_cmdlist *cmd_node);
int	changedir(char *path);
int	updatepwdfromexport(char *pwd_name, char *pwd_content);

// unset
void	rununset(t_cmdlist *cmdnode);
void	deleteenv(char *name);


void	createdup(t_cmdlist *cmd_list, int *fd, int fdindex);

//util
int	getarraylen(char **array);
void	print_error(char *ptr1, char *ptr2, char *ptr3);
void	changetitle(void);
int	updateenv(char *envname, char *newarg);
void	ownstrjoin(char **dst, char *src);
void	straddchar(char **dst, char c);
int	str_compare(char *s1, char *s2);
int	compare_metachars(char *str);


//exit
int	isallnumeric(char *text);
void	runexit(t_cmdlist *cmdnode);

// export
int	runexport(t_cmdlist *cmdnode);
void	singleexportarg(t_cmdlist *cmdnode);
void	doubleexportarg(char *envcmd);
int	changeenv(char *envname, char *arg, int isequal);

// envutils
int	getenvlen(void);
char	**getenvcpy(void);
void	freeenvcpy(char **envlist);
void	fillenvs(char **env);
t_env	*addnewenv(t_env **envtable, char *env);
void	sync_my_env(void);

//envutils2

char	*getenvname(char *content);
int	getenvnamecount(char *envarg);
char	*validenv(char *env);
char	*envnamecontrol(char *env);
int	envargcontrol(char *env);

//execve

void	runexecve(t_cmdlist *cmdnode, int *fd, int fdindex);

//execcommand 
void	execcommand(t_cmdlist *cmdnode, int *fd, int fdindex);
char	*getcmd(char *cmd);


// pipe
int	*createpipe(void);
void	clearpipe(int *fd);
void	switchpipe(int **fd);

//executer
void	runmultiplecommand(t_cmdlist *cmdlist);
void	runsinglecommand(t_cmdlist *cmdlist, int *fd);
void	executer(void);

// process

void	runprocess(t_cmdlist *cmdlist, int *fd, int fdindex);
void	waitall(void);

#endif
