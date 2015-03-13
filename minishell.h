/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   minishell.h                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: jragot <marvin@42.fr>                      +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2015/02/06 23:57:06 by jragot            #+#    #+#             */
/*   Updated: 2015/02/10 03:10:37 by jragot           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef MINISHELL_H
# define MINISHELL_H

# include "libft/libft.h"
# define PROMPT "\33[0;36mMinishell>>\33[0m "
# define SET_R "setenv: Please check your syntax (setenv var = value)"
# define USET_R "unsetenv: there was a problem unsetting "
# define CD_ERR "cd: this file doesn't exist or you have unsufficient rights."
# define BUFFER_SIZE 4096

char			**g_env;
int				ft_cd(char **args, char *oldpwd);
int				builtin(char **args);
char			**handle_path(void);
void			minishell(char **dirs, char **args);
int				env_find(char *str);
void			env_set(char *target, char *str);
int				env_unset(char *target);
char			**env_add(char *str);
char			*get_field_value(char *str);

#endif
