/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   pipex.h                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: besellem <besellem@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2021/06/28 13:10:40 by besellem          #+#    #+#             */
/*   Updated: 2021/07/04 22:22:09 by besellem         ###   ########.fr       */
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

# define __TMP_HEREDOC_FILE__ "/tmp/.tmp_pipex_heredoc"

# define ARGS_LEN      5
# define OTHER         1
# define SYSCALL_ERROR (-1)
# define CMD_NOT_FOUND 127 /* command not found code */

# if !defined(BONUS)
#  define BONUS        0
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
	int		is_heredoc;
	char	**env;
}	t_pipex;

/*
** -- PROTOYPES --
*/
/* Memory Management */
t_pipex		*singleton(void);
int			ft_free_all(int code);

/* Utils */
int			init_pipex(t_pipex *pipex, int ac, char **av, char **env);
char		*search_executable(char *command);

#endif
