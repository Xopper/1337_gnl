/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   get_next_line.h                                    :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ahaloua <marvin@42.fr>                     +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2019/05/29 23:30:40 by ahaloua           #+#    #+#             */
/*   Updated: 2019/06/26 19:36:06 by ahaloua          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef GET_NEXT_LINE_H
# define GET_NEXT_LINE_H

# include "libft/libft.h"
# include <sys/types.h>
# include <sys/uio.h>
# include <limits.h>

# define BUFF_SIZE 32

typedef struct		s_fd_list
{
	int				fd;
	char			*rest;
	struct s_fd_list	*next;
}					t_fd_list;

int		get_next_line(const int fd, char **line);

#endif
