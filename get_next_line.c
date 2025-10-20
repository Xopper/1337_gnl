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

static t_fd_list	*ft_find_or_create(t_fd_list **head, int fd)
{
	t_fd_list	*current;
	t_fd_list	*new_node;

	current = *head;
	while (current)
	{
		if (current->fd == fd)
			return (current);
		current = current->next;
	}
	new_node = (t_fd_list *)malloc(sizeof(t_fd_list));
	if (!new_node)
		return (NULL);
	new_node->fd = fd;
	new_node->rest = ft_strnew(0);
	if (!new_node->rest)
	{
		free(new_node);
		return (NULL);
	}
	new_node->next = *head;
	*head = new_node;
	return (new_node);
}

static int	ft_read_buffer(int fd, t_fd_list *node)
{
	char	*buffer;
	char	*tmp;
	int		bytes_read;

	buffer = (char *)malloc(sizeof(char) * (BUFF_SIZE + 1));
	if (!buffer)
		return (-1);
	bytes_read = 1;
	while (bytes_read > 0 && !ft_strchr(node->rest, '\n'))
	{
		bytes_read = read(fd, buffer, BUFF_SIZE);
		if (bytes_read < 0)
		{
			free(buffer);
			return (-1);
		}
		buffer[bytes_read] = '\0';
		tmp = ft_strjoin(node->rest, buffer);
		if (!tmp)
		{
			free(buffer);
			return (-1);
		}
		free(node->rest);
		node->rest = tmp;
	}
	free(buffer);
	return (0);
}

static int	ft_extract_line(t_fd_list *node, char **line)
{
	int		i;
	char	*new_rest;

	i = 0;
	while (node->rest[i] && node->rest[i] != '\n')
		i++;
	*line = ft_strsub(node->rest, 0, i);
	if (!*line)
		return (-1);
	if (!node->rest[i])
	{
		free(node->rest);
		node->rest = NULL;
		return (0);
	}
	new_rest = ft_strdup(node->rest + i + 1);
	if (!new_rest)
	{
		free(*line);
		return (-1);
	}
	free(node->rest);
	node->rest = new_rest;
	return (1);
}

static void	ft_remove_node(t_fd_list **head, int fd)
{
	t_fd_list	*current;
	t_fd_list	*prev;

	current = *head;
	prev = NULL;
	while (current)
	{
		if (current->fd == fd)
		{
			if (prev)
				prev->next = current->next;
			else
				*head = current->next;
			if (current->rest)
				free(current->rest);
			free(current);
			return ;
		}
		prev = current;
		current = current->next;
	}
}

int	get_next_line(const int fd, char **line)
{
	static t_fd_list	*head;
	t_fd_list			*node;
	int					result;

	if (fd < 0 || !line || BUFF_SIZE < 1)
		return (-1);
	node = ft_find_or_create(&head, fd);
	if (!node || !node->rest)
		return (-1);
	if (ft_read_buffer(fd, node) < 0)
		return (-1);
	result = ft_extract_line(node, line);
	if (result <= 0)
		ft_remove_node(&head, fd);
	return (result);
}
