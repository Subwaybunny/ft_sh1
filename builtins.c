/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   builtins.c                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: jragot <marvin@42.fr>                      +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2015/02/07 00:17:24 by jragot            #+#    #+#             */
/*   Updated: 2015/02/10 03:14:01 by jragot           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

int		ft_cd(char **args, char *oldpwd)
{
	if (ft_tablen(args) <= 2)
	{
		if (!args[1] && env_find("HOME") != -1)
		{
			env_set("OLDPWD", oldpwd);
			chdir(get_field_value(g_env[env_find("HOME")]));
			env_set("PWD", getcwd(NULL, 0));
		}
		else if (args[1] && args[1][0] == '-' && env_find("OLDPWD") != -1)
		{
			chdir(get_field_value(g_env[env_find("OLDPWD")]));
			env_set("OLDPWD", oldpwd);
			env_set("PWD", getcwd(NULL, 0));
		}
		else
		{
			env_set("OLDPWD", oldpwd);
			(chdir(args[1]) != 0) ? ft_putendl(CD_ERR) : 0;
			env_set("PWD", getcwd(NULL, 0));
		}
		return (1);
	}
	else
		ft_putendl("cd: Too many arguments");
	return (0);
}

int		env_find(char *str)
{
	char			**array;
	unsigned int	i;

	array = NULL;
	i = 0;
	while (g_env[i])
	{
		array = ft_strsplit(g_env[i], '=');
		if (ft_strcmp(array[0], str) == 0)
		{
			free(array);
			return (i);
		}
		free(array);
		++i;
	}
	return (-1);
}

void	env_set(char *target, char *str)
{
	int		position;
	char	*temp;

	position = env_find(target);
	temp = ft_strdup(ft_strjoin(ft_strjoin(target, "="), str));
	if (position != -1)
		g_env[position] = ft_strdup(temp);
	else
		g_env = env_add(temp);
	free(temp);
}

int		env_unset(char *target)
{
	int		i;

	i = 0;
	if (target)
	{
		while (ft_strcmp(g_env[i], target) != '=')
		{
			++i;
			if (g_env[i] == NULL)
				return (0);
		}
		while (g_env[i + 1])
		{
			g_env[i] = g_env[i + 1];
			++i;
		}
		g_env[i] = NULL;
		return (1);
	}
	else
		return (-1);
}

char	**env_add(char *str)
{
	char			**temp;
	unsigned int	i;

	i = 0;
	if ((temp = (char **)malloc(sizeof(char *) * (ft_tablen(g_env) + 2))))
	{
		while (g_env[i])
		{
			temp[i] = ft_strdup(g_env[i]);
			free(g_env[i]);
			++i;
		}
		temp[i] = ft_strdup(str);
		temp[i + 1] = 0;
	}
	return (temp);
}
