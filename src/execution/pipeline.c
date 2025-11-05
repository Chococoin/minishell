/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   pipeline.c                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: glugo-mu <glugo-mu@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/11/05 21:00:00 by glugo-mu          #+#    #+#             */
/*   Updated: 2025/11/05 21:00:00 by glugo-mu         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"
#include "minishell2.h"
#include <sys/wait.h>

int		count_cmds(t_cmd *cmd);
void	close_pipes(int **pipes, int n_pipes);
int		**create_pipes(int n_pipes);
void	close_all_pipes(int **pipes, int n_pipes);

static void	setup_pipe_fds(int i, int n_cmds, int **pipes)
{
	if (i > 0)
		dup2(pipes[i - 1][0], STDIN_FILENO);
	if (i < n_cmds - 1)
		dup2(pipes[i][1], STDOUT_FILENO);
	close_all_pipes(pipes, n_cmds - 1);
}

static void	exec_pipe_cmd(t_pipe_ctx *ctx)
{
	char	*path;

	setup_child_signals();
	setup_pipe_fds(ctx->cmd_i, ctx->n_cmds, ctx->pipes);
	if (ctx->cmd->redirs && apply_redirections(ctx->cmd->redirs) < 0)
		exit(1);
	if (isbuiltin(ctx->cmd->argv[0]))
	{
		execute_builtin_simple(ctx->cmd);
		exit(0);
	}
	path = find_in_path(ctx->cmd->argv[0], ctx->envp);
	if (!path)
	{
		printf("minishell: %s: command not found\n", ctx->cmd->argv[0]);
		exit(127);
	}
	execve(path, ctx->cmd->argv, ctx->envp);
	perror("minishell");
	exit(127);
}

int	execute_pipeline(t_cmd *first, char **envp)
{
	t_pipe_ctx	ctx;
	pid_t		pid;

	ctx.n_cmds = count_cmds(first);
	ctx.pipes = create_pipes(ctx.n_cmds - 1);
	if (!ctx.pipes && ctx.n_cmds > 1)
		return (1);
	ctx.cmd = first;
	ctx.envp = envp;
	ctx.cmd_i = 0;
	while (ctx.cmd)
	{
		pid = fork();
		if (pid == 0)
			exec_pipe_cmd(&ctx);
		ctx.cmd = ctx.cmd->next;
		ctx.cmd_i++;
	}
	close_pipes(ctx.pipes, ctx.n_cmds - 1);
	while (--ctx.n_cmds >= 0)
		wait(NULL);
	return (0);
}
