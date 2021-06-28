/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   pipex.c                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: besellem <besellem@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2021/06/28 13:48:19 by besellem          #+#    #+#             */
/*   Updated: 2021/06/28 18:18:55 by besellem         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "pipex.h"

void	first_part(t_pipex *pipex, const char *executable, int fd[2])
{
	int		file1_fd;
	pid_t	pid;

	file1_fd = open(pipex->file1, O_RDONLY);
	if (-1 == file1_fd)
	{
		ft_dprintf(STDERR_FILENO, PROG_NAME ": %s: %s\n",
			pipex->file1, strerror(errno));
		return ;
	}
	pipe(fd);
	pid = fork();
	if (pid < 0)
		perror(PROG_NAME);
	else if (0 == pid)
	{
		dup2(file1_fd, STDIN_FILENO);
		// close(fd[0]);
		dup2(fd[1], STDOUT_FILENO);
		if (-1 == execve(executable, pipex->cmds[0].cmd, pipex->env))
			perror(PROG_NAME);
	}
	else
	{
		waitpid(pid, NULL, 0);
		close(fd[1]);
		close(file1_fd);
	}
}

void	second_part(t_pipex *pipex, size_t index, int fd[], int fd_index)
{
	const char	*executable = search_executable(pipex->cmds[index].cmd[0]);
	pid_t		pid;
	int			fd_tmp[2];

	if (!executable || !pipex->cmds[index].cmd)
		return ;
	pipe(fd_tmp);
	pid = fork();
	if (pid < 0)
		perror(PROG_NAME);
	else if (0 == pid)
	{
		dup2(fd_tmp[1], STDIN_FILENO);
		if (-1 == execve(executable, pipex->cmds[index].cmd, pipex->env))
			perror(PROG_NAME);
	}
	else
	{
		waitpid(pid, NULL, 0);
		close(fd_tmp[1]);
		ft_printf("fd[0]: [%d] fd[1]: [%d]\n", fd[0], fd[1]);
		
		char	buf[2000] = {0};
		read(fd[0], buf, 1999);
		ft_putendl_fd(buf, 1);
		
		fd[fd_index] = fd_tmp[0];
		fd[fd_index + 1] = fd_tmp[1];
		
		if ((index + 1) < pipex->cmds_len)
			second_part(pipex, index + 1, fd, fd_index + 2);
	}
	ft_memdel((void **)&executable);
}

void	ft_pipex(t_pipex *pipex)
{
	const char	*ex = search_executable(pipex->cmds[0].cmd[0]);
	int			fd[2];
	int			fd2[20];

	first_part(pipex, ex, fd);
	ft_memdel((void **)&ex);
	second_part(pipex, 0, fd2, 0);

	// const char	*ex = search_executable(pipex->cmds[0].cmd[0]);
	// const pid_t	pid = fork();
	// pid_t		wpid;
	// int			status;
	// int			fd[2];
	
	// pipe(fd);
	// if (pid < 0)
	// 	perror(PROG_NAME);
	// else if (0 == pid)
	// {
	// 	dup2(fd[1], STDOUT_FILENO);
	// 	if (-1 == execve(ex, pipex->cmds[0].cmd, pipex->env))
	// 	{
	// 		ft_dprintf(STDERR_FILENO, PROG_NAME ": %s: %s\n", ex, strerror(errno));
	// 	}
	// }
	// else
	// {
	// 	wpid = waitpid(pid, &status, WUNTRACED);
	// 	printf("pid[%d] status[%d]\n", pid, status);
	// 	close(fd[1]);
	// 	free((char *)ex);
	// }
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
	pipex->file1 = av[1];
	pipex->file2 = av[ac - 1];
	pipex->env = env;
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
	ft_free_all(EMPTY);
	return (EXIT_SUCCESS);
}
