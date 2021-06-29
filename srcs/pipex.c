/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   pipex.c                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: besellem <besellem@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2021/06/28 13:48:19 by besellem          #+#    #+#             */
/*   Updated: 2021/06/29 16:15:36 by besellem         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "pipex.h"

int	ft_check_open(char *path, int mode)
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

void	exec_cmd(t_pipex *pipex, char **cmds)
{
	const char	*executable = search_executable(cmds[0]);

	if (SYSCALL_ERROR == execve(executable, cmds, pipex->env))
	{
		ft_memdel((void **)&executable);
		perror(PROG_NAME);
		exit(EXIT_FAILURE);
	}
}

void	do_pipe(t_pipex *pipex, int __fd, char **cmds)
{
	pid_t	pid;
	int		fd[2];

	pipe(fd);
	pid = fork();
	if (pid < 0)
		perror(PROG_NAME);
	else if (0 == pid)
	{
		close(fd[0]);
		dup2(fd[1], STDOUT_FILENO);
		if (STDIN_FILENO == __fd)
			exit(EXIT_FAILURE);
		exec_cmd(pipex, cmds);
	}
	else
	{
		close(fd[1]);
		dup2(fd[0], STDIN_FILENO);
		waitpid(pid, NULL, 0);
	}
}

void	ft_pipex(t_pipex *pipex)
{
	size_t	i;

	i = 0;
	dup2(pipex->fd1, STDIN_FILENO);
	dup2(pipex->fd2, STDOUT_FILENO);
	if (BONUS)
		do_pipe(pipex, pipex->fd1, pipex->cmds[i].cmd);
	while (i < (pipex->cmds_len - 1))
	{
		if (BONUS)
			do_pipe(pipex, OTHER, pipex->cmds[i].cmd);
		else
			do_pipe(pipex, pipex->fd1, pipex->cmds[i].cmd);
		++i;
	}
	if (pipex->fd2 != SYSCALL_ERROR)
		exec_cmd(pipex, pipex->cmds[i].cmd);
}

static int	init_pipex(t_pipex *pipex, int ac, char **av, char **env)
{
	int	i;
	int	j;

	ft_memset(pipex, 0, sizeof(t_pipex));
	pipex->cmds = (t_cmds *)ft_calloc(ac - 2, sizeof(t_cmds));
	if (!pipex->cmds)
		return (EXIT_FAILURE);
	i = 0;
	j = 2;
	while (j < (ac - 1))
	{
		pipex->cmds[i].cmd = ft_strsplit(av[j++], " ");
		if (!pipex->cmds[i].cmd)
			return (EXIT_FAILURE);
		++i;
	}
	pipex->cmds_len = ac - 3;
	pipex->env = env;
	pipex->fd1 = ft_check_open(av[1], O_RDONLY);
	pipex->fd2 = open(av[ac - 1], O_WRONLY | O_CREAT | O_TRUNC, 0644);
	if (SYSCALL_ERROR == pipex->fd2)
		perror(av[ac - 1]);
	return (EXIT_SUCCESS);
}

int	main(int ac, char **av, char **env)
{
	if ((BONUS && ac < ARGS_LEN) || (!BONUS && ac != ARGS_LEN))
	{
		ft_putstr_fd(USAGE, STDERR_FILENO);
		return (EXIT_FAILURE);
	}
	if (NULL == singleton())
	{
		ft_putstr_fd("malloc error\n", STDERR_FILENO);
		return (EXIT_FAILURE);
	}
	if (EXIT_FAILURE == init_pipex(singleton(), ac, av, env))
		return (ft_free_all(EXIT_FAILURE));
	ft_pipex(singleton());
	return (ft_free_all(EXIT_SUCCESS));
}
