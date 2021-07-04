/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   pipex.c                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: besellem <besellem@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2021/06/28 13:48:19 by besellem          #+#    #+#             */
/*   Updated: 2021/07/04 21:41:58 by besellem         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "pipex.h"

void	exec_cmd(t_pipex *pipex, t_cmds *cmds)
{
	const char	*executable = search_executable(cmds->cmd[0]);

	if (SYSCALL_ERROR == execve(executable, cmds->cmd, pipex->env))
	{
		ft_dprintf(STDERR_FILENO, PROG_NAME": %s: command not found\n",
			cmds->cmd[0]);
		ft_free_all(EXIT_FAILURE);
		exit(EXIT_FAILURE);
	}
}

void	do_pipe(t_pipex *pipex, int __fd, t_cmds *cmds)
{
	pid_t	pid;
	int		fd[2];

	pipe(fd);
	pid = fork();
	if (pid < 0)
		perror(NULL);
	else if (0 == pid)
	{
		close(fd[0]);
		dup2(fd[1], STDOUT_FILENO);
		if (STDIN_FILENO == __fd)
		{
			ft_free_all(EXIT_SUCCESS);
			exit(EXIT_FAILURE);
		}
		else
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
	do_pipe(pipex, pipex->fd1, &pipex->cmds[i++]);
	while (BONUS && i < (pipex->cmds_len - 1))
	{
		do_pipe(pipex, OTHER, &pipex->cmds[i]);
		++i;
	}
	if (pipex->fd2 != SYSCALL_ERROR)
		exec_cmd(pipex, &pipex->cmds[i]);
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
