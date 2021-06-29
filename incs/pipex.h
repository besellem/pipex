/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   pipex.h                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: besellem <besellem@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2021/06/28 13:10:40 by besellem          #+#    #+#             */
/*   Updated: 2021/06/29 17:30:58 by besellem         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef PIPEX_H
# define PIPEX_H

/*
** -- INCLUDES --
*/
# include <unistd.h>
# include <fcntl.h>
# include <stdlib.h>
# include <sys/wait.h>
# include <stdio.h>
# include <errno.h>

# include "libft.h"

/*
** -- DEFINES --
*/
# define PROG_NAME  "pipex"
# define USAGE      "usage: ./pipex file1 cmd1 cmd2 file2\n"

# define ARGS_LEN      5
# define OTHER         1
# define SYSCALL_ERROR (-1)
# define CMD_NOT_FOUND 127 /* command not found code */

# if !defined(BONUS)
#  define BONUS    0
# endif /* !defined(BONUS) */

/*
** -- DATA STRUCTURES --
*/
typedef struct s_cmds
{
	char	**cmd;
}	t_cmds;

typedef struct s_pipex
{
	t_cmds	*cmds;
	size_t	cmds_len;
	int		fd1;
	int		fd2;
	char	**env;
}	t_pipex;

/*
** -- PROTOYPES --
*/
/* Memory Management */
t_pipex		*singleton(void);
int			ft_free_all(int code);

/* Utils */
int			ft_check_open(char *path, int mode);
char		*search_executable(char *command);

#endif
