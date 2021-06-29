/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   memory_management.c                                :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: besellem <besellem@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2021/06/28 14:04:24 by besellem          #+#    #+#             */
/*   Updated: 2021/06/29 13:43:21 by besellem         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "pipex.h"

t_pipex	*singleton(void)
{
	static t_pipex	*pipex = NULL;

	if (!pipex)
	{
		pipex = (t_pipex *)ft_calloc(1, sizeof(t_pipex));
		if (!pipex)
			return (NULL);
	}
	return (pipex);
}

int	ft_free_all(int code)
{
	size_t	i;
	size_t	len;

	if (singleton())
	{
		if (singleton()->cmds)
		{
			i = 0;
			while (singleton()->cmds[i].cmd)
			{
				len = ft_strslen(singleton()->cmds[i].cmd);
				ft_strsfree(len, singleton()->cmds[i].cmd);
				++i;
			}
			ft_memdel((void **)&singleton()->cmds);
		}
		close(singleton()->fd1);
		close(singleton()->fd2);
		free(singleton());
	}
	return (code);
}
