/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   search_executable.c                                :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: besellem <besellem@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2021/06/28 16:08:07 by besellem          #+#    #+#             */
/*   Updated: 2021/06/28 16:08:40 by besellem         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "pipex.h"

static int	ft_is_openable(char *path, int mode)
{
	int	fd;

	fd = open(path, mode);
	if (-1 == fd)
		return (0);
	close(fd);
	return (1);
}

static char	*find_exec(char **exectbl, char *command)
{
	char	*cmd;
	size_t	i;

	if (ft_strchr(command, '/'))
	{
		ft_asprintf(&cmd, "%s", command);
		if (ft_is_openable(cmd, O_RDONLY))
			return (cmd);
	}
	else
	{
		i = 0;
		while (exectbl[i])
		{
			ft_asprintf(&cmd, "%s/%s", exectbl[i++], command);
			if (!cmd)
				return (NULL);
			if (ft_is_openable(cmd, O_RDONLY))
				return (cmd);
			ft_memdel((void **)&cmd);
		}
	}
	return (NULL);
}

static char	*ft_getenv(const char *name)
{
	char	**env;
	size_t	i;
	int		idx;
	char	*ptr;

	if (!name)
		return (NULL);
	i = 0;
	env = singleton()->env;
	while (env[i])
	{
		ptr = ft_strchr(env[i], '=');
		if (ptr)
		{
			idx = ft_stridx(env[i], "=");
			if (idx != -1 && 0 == ft_strncmp(name, env[i], idx))
				return (ft_strdup(ptr + 1));
		}
		else
			if (0 == ft_strcmp(name, env[i]))
				return (ft_strdup(""));
		++i;
	}
	return (NULL);
}

char	*search_executable(char *command)
{
	const char	*path = ft_getenv("PATH");
	char		**exectbl;
	char		*cmd;

	if (!path)
		return (command);
	exectbl = ft_split(path, ':');
	ft_memdel((void **)&path);
	if (!exectbl)
		return (NULL);
	cmd = find_exec(exectbl, command);
	if (!cmd)
		cmd = command;
	ft_strsfree(ft_strslen(exectbl), exectbl);
	return (cmd);
}
