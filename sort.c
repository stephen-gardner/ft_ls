/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   sort.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: sgardner <stephenbgardner@gmail.com>       +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2017/11/03 15:07:58 by sgardner          #+#    #+#             */
/*   Updated: 2017/11/10 23:27:24 by sgardner         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "ft_ls.h"

static int	cmp(const t_file *f1, const t_file *f2, int flags)
{
	const t_file	*tmp;

	if (LSF(LS_REV))
	{
		tmp = f1;
		f1 = f2;
		f2 = tmp;
	}
	if (LSF(LS_SIZE) && f1->byte_size != f2->byte_size)
		return (f1->byte_size < f2->byte_size);
	if (LSF(LS_MTIME) || (LSF(LS_MTIME) && (LSF(LS_ATIME) || LSF(LS_CTIME))))
	{
		if (f1->timestamp.tv_sec != f2->timestamp.tv_sec)
			return (f1->timestamp.tv_sec < f2->timestamp.tv_sec);
		if (f1->timestamp.tv_nsec != f2->timestamp.tv_nsec)
			return (f1->timestamp.tv_nsec < f2->timestamp.tv_nsec);
	}
	return (ft_strcmp(f1->name, f2->name));
}

static void	max_heapify(t_file **array, int node, int size, int flags)
{
	int		high;
	int		left;
	int		right;
	t_file	*tmp;

	while (TRUE)
	{
		high = node;
		left = (2 * node) + 1;
		right = (2 * node) + 2;
		if (left < size && cmp(array[left], array[high], flags) > 0)
			high = left;
		if (right < size && cmp(array[right], array[high], flags) > 0)
			high = right;
		if (high != node)
		{
			tmp = array[node];
			array[node] = array[high];
			array[high] = tmp;
			node = high;
		}
		else
			break ;
	}
}

void		heap_sort(t_file **children, int child_count, int flags)
{
	int		i;
	t_file	*tmp;

	i = (child_count / 2) - 1;
	while (i >= 0)
		max_heapify(children, i--, child_count, flags);
	i = child_count - 1;
	while (i >= 0)
	{
		tmp = children[0];
		children[0] = children[i];
		children[i] = tmp;
		max_heapify(children, 0, i--, flags);
	}
}
