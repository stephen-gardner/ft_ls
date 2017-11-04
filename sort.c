/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   sort.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: sgardner <stephenbgardner@gmail.com>       +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2017/11/03 15:07:58 by sgardner          #+#    #+#             */
/*   Updated: 2017/11/04 14:38:28 by sgardner         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "ft_ls.h"

static void	max_heapify(t_file **array, int node, int size,
		int (*cmp)(const t_file *, const t_file *))
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
		if (left < size && cmp(array[left], array[high]) > 0)
			high = left;
		if (right < size && cmp(array[right], array[high]) > 0)
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

int			fnamecmp(const t_file *f1, const t_file *f2)
{
	return (ft_strcmp(f1->name, f2->name));
}

int			ftimecmp(const t_file *f1, const t_file *f2)
{
	struct timespec	t1;
	struct timespec t2;

	t1 = f1->timestamp;
	t2 = f2->timestamp;
	if (t2.tv_sec != t1.tv_sec)
		return (t2.tv_sec - t1.tv_sec);
	if (t2.tv_nsec != t1.tv_nsec)
		return (t2.tv_nsec - t1.tv_nsec);
	return (ft_strcmp(f1->name, f2->name));
}

void		heap_sort(t_file **children, int child_count,
		int (*cmp)(const t_file *, const t_file *))
{
	int		i;
	t_file	*tmp;

	i = (child_count / 2) - 1;
	while (i >= 0)
		max_heapify(children, i--, child_count, cmp);
	i = child_count - 1;
	while (i >= 0)
	{
		tmp = children[0];
		children[0] = children[i];
		children[i] = tmp;
		max_heapify(children, 0, i--, cmp);
	}
}
