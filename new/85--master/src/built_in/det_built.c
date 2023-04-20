/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   det_built.c                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: imac21 <imac21@student.42.fr>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/04/04 12:03:06 by dojannin          #+#    #+#             */
/*   Updated: 2023/04/19 20:15:47 by imac21           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../include/include.h"

int	help_builtin()
{
	
	if (strcmp(g_data.token->value, "echo") == 0 || strcmp(g_data.token->value, "/bin/echo") == 0)
	{
		if (g_data.token->next)
			ft_echo(1);
		else
			ft_echo(0);
		return (1);
	}
	return (0);
}

int	det_builtin()
{
	if (strcmp(g_data.token->value, "pwd") == 0 || strcmp(g_data.token->value, "/bin/pwd") == 0)
	{
		ft_pwd(1);
		return (1);
	}
	if (ft_strcmp(g_data.token->value, "exit") == 0)
	{
		g_data.exit = 1;
		if (g_data.token->next)
			ft_exit(ft_atoi(g_data.token->next->value));
		else 
			ft_exit(0);
	}
	if (strcmp(g_data.token->value, "cd") == 0 || strcmp(g_data.token->value, "/usr/bin/cd") == 0){
		if (g_data.token->next)
			ft_cd(g_data.token->next->value);
		else
		{	
			printf("avant\n");
			ft_cd("$HOME");
		}
		return (1);
	}
	if (strcmp(g_data.token->value, "unset") == 0)
	{
		if (g_data.token->next)
		{
			printf("g_data-> = %s\n", g_data.token->next->value);
			ft_unset(g_data.token->next->value);
			printf("merci\n");
		}
		else
			ft_unset(NULL);
		return (1);
	}
	if (strcmp(g_data.token->value , "export") == 0)
	{
		ft_export(1);
		return (1);
	}
	if (strcmp(g_data.token->value, "env") == 0 || strcmp(g_data.token->value, "/usr/bin/env") == 0)
	{
		if (!g_data.token->next)
			ft_env(NULL);
		else
			ft_env(g_data.token->next->value);
		return (1);
	}
	return (help_builtin());
}