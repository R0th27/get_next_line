/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   get_next_line.h                                    :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: htoe <htoe@student.42bangkok.com>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/01/19 08:21:00 by htoe              #+#    #+#             */
/*   Updated: 2026/01/21 00:57:42 by htoe             ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef GET_NEXT_LINE_H
# define GET_NEXT_LINE_H

# ifndef BUFFER_SIZE
#  define BUFFER_SIZE 3
# endif

# include <unistd.h>
# include <stdlib.h>
# include <stddef.h>

typedef struct s_gnode
{
	char			*str;
	ssize_t			len;
	struct s_gnode	*next;
}	t_gnode;

char	*get_next_line(int fd);
int		find_nl(char const *str);
void	free_gnode(t_gnode **head);
ssize_t	line_length(t_gnode *head);

#endif
