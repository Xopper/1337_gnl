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
	int fd = open(argv[argc - 1], O_RDONLY);
	char *line;
	while (get_next_line(fd, &line))
	{
		ft_putendl(line);
		free(line);
	}
	return (0);
}
