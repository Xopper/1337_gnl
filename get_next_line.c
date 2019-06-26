/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   get_next_line.c                                    :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ahaloua <marvin@42.fr>                     +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2019/05/29 23:49:02 by ahaloua           #+#    #+#             */
/*   Updated: 2019/06/26 19:36:09 by ahaloua          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "get_next_line.h"

int				get_next_line(const int fd, char **line)
{
	char		buf[BUFF_SIZE + 1];
	static char	*rest[5000];
	int			ret;

	AS_RET(((BUFF_SIZE < 1) || read(fd, buf, 0) || !line || fd < 0
			|| fd > 5000), -1);
	ret = 0;
	if (!rest[fd])
		UN_RET((rest[fd] = ft_strnew(0)), -1);
	while (!ft_strchr(rest[fd], '\n') && (ret = read(fd, buf, BUFF_SIZE)))
	{
		buf[ret] = 0;
		UN_RET((rest[TMP] = ft_strjoin(rest[fd], buf)), -1);
		free(rest[fd]);
		rest[fd] = rest[TMP];
	}
	AS_RET(rest[fd][0] == '\0' && ret < 1, ret);
	UN_RET((*line = ft_strsub(rest[fd], 0, ft_strcspn(rest[fd], "\n"))), -1);
	UN_RET((rest[TMP] = ft_strdup(rest[fd][ft_strcspn(rest[fd], "\n")] == '\n'
			? &rest[fd][ft_strcspn(rest[fd], "\n") + 1] : "\0")), -1);
	free(rest[fd]);
	rest[fd] = rest[TMP];
	return (1);
}
