/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   get_next_line_utils.c                              :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: htoe <htoe@student.42bangkok.com>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/01/19 09:30:59 by htoe              #+#    #+#             */
/*   Updated: 2026/01/21 00:58:08 by htoe             ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "get_next_line.h"

int	find_nl(char const *str)
{
	if (!str)
		return (0);
	while (*str)
	{
		if (*str == '\n')
			return (1);
		str++;
	}
	return (0);
}

void	free_gnode(t_gnode **head)
{
	t_gnode	*temp;
	t_gnode	*current;

	if (!head || !*head)
		return ;
	current = *head;
	while (current)
	{
		temp = current -> next;
		free(current -> str);
		free(current);
		current = temp;
	}
	*head = NULL;
}

ssize_t	line_length(t_gnode *head)
{
	ssize_t	len;
	char	*ptr;

	len = 0;
	while (head -> next)
	{
		len += head -> len;
		head = head -> next;
	}
	ptr = head -> str;
	while (*ptr && *ptr != '\n')
	{
		len++;
		ptr++;
	}
	len += (*ptr == '\n');
	return (len);
}
