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

static char	*ft_read_buffer(int fd, char *rest)
{
	char	*buffer;
	char	*tmp;
	int		bytes_read;

	buffer = (char *)malloc(sizeof(char) * (BUFF_SIZE + 1));
	if (!buffer)
		return (NULL);
	bytes_read = 1;
	while (bytes_read > 0 && !ft_strchr(rest, '\n'))
	{
		bytes_read = read(fd, buffer, BUFF_SIZE);
		if (bytes_read < 0)
		{
			free(buffer);
			return (NULL);
		}
		buffer[bytes_read] = '\0';
		tmp = ft_strjoin(rest, buffer);
		free(rest);
		rest = tmp;
	}
	free(buffer);
	return (rest);
}

static char	*ft_extract_line(char *rest)
{
	int		i;
	char	*line;

	i = 0;
	if (!rest)
		return (NULL);
	while (rest[i] && rest[i] != '\n')
		i++;
	line = ft_strsub(rest, 0, i);
	return (line);
}

static char	*ft_update_rest(char *rest)
{
	int		i;
	char	*new_rest;

	i = 0;
	if (!rest)
		return (NULL);
	while (rest[i] && rest[i] != '\n')
		i++;
	if (!rest[i])
	{
		free(rest);
		return (NULL);
	}
	new_rest = ft_strdup(rest + i + 1);
	free(rest);
	return (new_rest);
}

int	get_next_line(const int fd, char **line)
{
	static char	*rest[OPEN_MAX];

	if (fd < 0 || !line || BUFF_SIZE < 1 || fd >= OPEN_MAX)
		return (-1);
	if (!rest[fd])
		rest[fd] = ft_strnew(0);
	if (!rest[fd])
		return (-1);
	rest[fd] = ft_read_buffer(fd, rest[fd]);
	if (!rest[fd])
		return (-1);
	*line = ft_extract_line(rest[fd]);
	if (!*line)
		return (-1);
	if (ft_strlen(*line) == 0 && ft_strlen(rest[fd]) == 0)
	{
		free(rest[fd]);
		rest[fd] = NULL;
		return (0);
	}
	rest[fd] = ft_update_rest(rest[fd]);
	return (1);
}
