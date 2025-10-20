/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   main.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ahaloua <marvin@42.fr>                     +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2019/06/26 19:29:35 by ahaloua           #+#    #+#             */
/*   Updated: 2019/06/26 19:32:04 by ahaloua          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "get_next_line.h"
#include <fcntl.h>

int main(int argc, char **argv)
{
	int		fd;
	char	*line;
	int		ret;

	if (argc < 2)
		return (1);
	fd = open(argv[argc - 1], O_RDONLY);
	if (fd < 0)
		return (1);
	while ((ret = get_next_line(fd, &line)) > 0)
	{
		ft_putendl(line);
		free(line);
	}
	if (ret == 0)
		free(line);
	close(fd);
	return (0);
}
