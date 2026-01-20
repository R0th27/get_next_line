/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   get_next_line.c                                    :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: htoe <htoe@student.42bangkok.com>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/01/19 08:24:04 by htoe              #+#    #+#             */
/*   Updated: 2026/01/21 01:06:12 by htoe             ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "get_next_line.h"

static int	append_gnode(char *content, ssize_t byte, t_gnode **head)
{
	t_gnode	*node;
	t_gnode	*current;

	node = (t_gnode *)malloc(sizeof(t_gnode));
	if (!node)
		return (0);
	node -> str = content;
	node -> len = byte;
	node -> next = NULL;
	if (!*head)
		*head = node;
	else
	{
		current = *head;
		while (current -> next)
			current = current -> next;
		current -> next = node;
	}
	return (1);
}

static void	read_to_list(int fd, t_gnode **head)
{
	char	*buffer;
	ssize_t	byte;

	buffer = NULL;
	byte = 1;
	while (byte && !find_nl(buffer))
	{
		buffer = (char *)malloc(sizeof(char) * (BUFFER_SIZE + 1));
		if (!buffer)
			return ;
		byte = read(fd, buffer, BUFFER_SIZE);
		if (byte <= 0)
			return (free(buffer));
		buffer[byte] = '\0';
		if (!append_gnode(buffer, byte, head))
			return (free(buffer));
	}
}

static char	*extract_line(t_gnode *head)
{
	char	*str;
	ssize_t	len;
	ssize_t	i;
	ssize_t	j;

	len = line_length(head);
	if (!len)
		return (NULL);
	str = (char *)malloc(sizeof(char) * (len + 1));
	if (!str)
		return (NULL);
	i = 0;
	while (head)
	{
		j = 0;
		while (i < len && head -> str[j])
			str[i++] = head -> str[j++];
		head = head -> next;
	}
	str[i] = '\0';
	return (str);
}

static t_gnode	*update_cache(t_gnode **head)
{
	t_gnode	*node;
	t_gnode	*current;
	ssize_t	i;
	ssize_t	ni;

	current = *head;
	while (current -> next)
		current = current -> next;
	ni = line_length(current);
	if (ni == current -> len)
		return (free_gnode(head), NULL);
	node = (t_gnode *)malloc(sizeof(t_gnode));
	if (!node)
		return (free_gnode(head), NULL);
	node -> len = current -> len - ni;
	node -> str = (char *)malloc(sizeof(char) * (node -> len + 1));
	if (!node -> str)
		return (free_gnode(head), free(node), NULL);
	i = 0;
	while (ni < current -> len)
		node -> str[i++] = current -> str[ni++];
	node -> str[i] = '\0';
	node -> next = NULL;
	return (free_gnode(head), node);
}

char	*get_next_line(int fd)
{
	static t_gnode	*cache;
	char			*str;

	if (!BUFFER_SIZE)
		return (NULL);
	if (!cache || !find_nl(cache -> str))
		read_to_list(fd, &cache);
	if (!cache)
		return (NULL);
	str = extract_line(cache);
	cache = update_cache(&cache);
	return (str);
}
