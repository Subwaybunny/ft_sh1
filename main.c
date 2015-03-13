/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   main.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: jragot <marvin@42.fr>                      +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2015/01/31 05:11:52 by jragot            #+#    #+#             */
/*   Updated: 2015/03/01 16:49:28 by jragot           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

char	*get_field_value(char *str)
{
	while (*str && *str != '=')
		++str;
	if (*str == '=')
		return (++str);
	return (NULL);
}

int		builtin(char **args)
{
	if (ft_strcmp(args[0], "cd") == 0)
		return (ft_cd(args, getcwd(NULL, 0)));
	if (ft_strcmp(args[0], "env") == 0)
		return (ft_puttab(g_env));
	else if (ft_strcmp(args[0], "setenv") == 0)
	{
		if (ft_tablen(args) == 4)
			(ft_strlen(ft_strjoin(args[1], args[3])) > 1
			&& ft_strcmp(args[2], "=") == 0) ? env_set(args[1], args[3])
				: ft_putendl(SET_R);
		else
			ft_putendl(SET_R);
		return (1);
	}
	else if (ft_strcmp(args[0], "unsetenv") == 0)
	{
		if (ft_tablen(args) == 2)
			(ft_strlen(args[1]) && env_find(args[1]) != -1) ? env_unset(args[1])
				: ft_putendl(ft_strjoin("unsetenv: error unsetting ", args[1]));
		else
			ft_putendl("unsetenv: Please check your syntax: (unsetenv var)");
		return (1);
	}
	else
		return (0);
}

void	minishell(char **dirs, char **args)
{
	unsigned int	i;
	char			*cmd;

	i = 0;
	cmd = args[0];
	while (i < ft_tablen(dirs))
	{
		dirs[i] = ft_strjoin(dirs[i], "/");
		if (access(ft_strjoin(dirs[i], cmd), F_OK | X_OK) == 0)
		{
			if (args[0])
				execve(ft_strjoin(dirs[i], cmd), args, g_env);
			else
				execve(ft_strjoin(dirs[i], cmd), NULL, g_env);
		}
		++i;
	}
	(access(cmd, X_OK & R_OK) == 0) ? execve(cmd, args, NULL) : 0;
	ft_putendl(ft_strjoin("command not found: ", cmd));
	free(dirs);
	exit(0);
}

char	**handle_path(void)
{
	char		**path;
	int			i;

	i = env_find("PATH");
	if (i != -1)
	{
		path = ft_strsplit(g_env[i], ':');
		path[0] = get_field_value(path[0]);
		return (path);
	}
	ft_putstr("\033[33m");
	ft_putendl("Warning: Your PATH is not set. This shell may become useless.");
	ft_putendl("You can set your PATH using 'setenv PATH = /example:/example'");
	ft_putstr("\033[0m");
	return (g_env);
}

int		main(void)
{
	extern char	**environ;
	pid_t		father;
	char		buffer[BUFFER_SIZE];
	char		**args;

	g_env = ft_tabdup(environ);
	while (1)
	{
		ft_putstr(PROMPT);
		read(0, buffer, BUFFER_SIZE);
		buffer[ft_strlen(buffer)] = 0;
		args = ft_strsplit(buffer, ' ');
		(ft_strcmp(buffer, "exit") == 0) ? exit(0) : 0;
		if (args[1] && args[1][0] == '~')
			(env_find("HOME") != -1) ? args[1] = ft_strdup(ft_strjoin
(get_field_value(g_env[env_find("HOME")]), ++args[1])) : 0;
		if (args[0] && !builtin(args))
		{
			father = fork();
			(father > 0) ? wait(0) : 0;
			(father == 0) ? minishell(handle_path(), args) : 0;
		}
		free(args);
	}
	return (0);
}
