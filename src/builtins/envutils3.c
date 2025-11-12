/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   envutils3.c                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: glugo-mu <glugo-mu@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/11/12 12:00:45 by glugo-mu          #+#    #+#             */
/*   Updated: 2025/11/12 12:07:02 by glugo-mu         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell2.h"

void	sync_my_env(void)
{
	if (g_core.my_env)
		freeenvcpy(g_core.my_env);
	g_core.my_env = getenvcpy();
}
