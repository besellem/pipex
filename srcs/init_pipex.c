/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   init_pipex.c                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: besellem <besellem@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2021/07/04 21:40:44 by besellem          #+#    #+#             */
/*   Updated: 2021/07/13 18:09:23 by besellem         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "pipex.h"

static int	ft_check_open(char *path, int mode)
{
	int	fd;

	fd = open(path, mode);
	if (SYSCALL_ERROR == fd)
	{
		ft_dprintf(STDERR_FILENO, PROG_NAME": %s: %s\n", path, strerror(errno));
		return (STDIN_FILENO);
	}
	return (fd);
}

static void	__read_heredoc__(t_pipex *pipex, char *limiter)
{
	char	*ret;
	int		r;

	pipex->fd1 = open(__TMP_HEREDOC_FILE__, O_WRONLY | O_CREAT | O_TRUNC, 0644);
	while (TRUE)
	{
		ft_putstr_fd("> ", STDERR_FILENO);
		r = get_next_line(STDIN_FILENO, &ret);
		if (0 == r || 0 == ft_strcmp(ret, limiter))
		{
			ft_memdel((void **)&ret);
			break ;
		}
		ft_putendl_fd(ret, pipex->fd1);
		ft_memdel((void **)&ret);
	}
	close(pipex->fd1);
}

static void	__open_fds__(t_pipex *pipex, int ac, char **av)
{
	if (BONUS && 0 == ft_strcmp(av[1], "here_doc"))
	{
		__read_heredoc__(pipex, av[2]);
		pipex->fd1 = open(__TMP_HEREDOC_FILE__, O_RDONLY);
		pipex->fd2 = open(av[ac - 1], O_WRONLY | O_CREAT | O_APPEND, 0644);
	}
	else
	{
		pipex->is_heredoc = FALSE;
		pipex->fd1 = ft_check_open(av[1], O_RDONLY);
		pipex->fd2 = open(av[ac - 1], O_WRONLY | O_CREAT | O_TRUNC, 0644);
	}
	if (SYSCALL_ERROR == pipex->fd2)
		perror(av[ac - 1]);
}

int	init_pipex(t_pipex *pipex, int ac, char **av, char **env)
{
	const int	here_doc_check = (BONUS && 0 == ft_strcmp(av[1], "here_doc"));
	int			i;
	int			j;

	pipex->is_heredoc = here_doc_check;
	pipex->cmds_len = ac - 3 - here_doc_check;
	pipex->cmds = (t_cmds *)ft_calloc(pipex->cmds_len + 1, sizeof(t_cmds));
	if (!pipex->cmds)
		return (EXIT_FAILURE);
	i = 0;
	j = 2 + (TRUE == pipex->is_heredoc);
	while (j < (ac - 1))
	{
		pipex->cmds[i].cmd = ft_strsplit(av[j++], " ");
		if (0 == ft_strslen(pipex->cmds[i].cmd))
		{
			ft_putstr_fd(USAGE, STDERR_FILENO);
			return (EXIT_FAILURE);
		}
		if (!pipex->cmds[i++].cmd)
			return (EXIT_FAILURE);
	}
	__open_fds__(pipex, ac, av);
	pipex->env = env;
	return (EXIT_SUCCESS);
}
