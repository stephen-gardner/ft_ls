/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   sort.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: sgardner <stephenbgardner@gmail.com>       +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2017/11/03 15:07:58 by sgardner          #+#    #+#             */
/*   Updated: 2017/11/03 17:11:08 by sgardner         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "ft_ls.h"

static void	heapify(t_file **array, int root, int size,
		int (*cmp)(const t_file *, const t_file *))
{
	int 	largest;
	int		left;
	int 	right;
	t_file	*tmp;

	largest = root;
	left = (2 * root) + 1;
	right = (2 * root) + 2;
	if (left < size && cmp(array[left], array[largest]) > 0)
		largest = left;
	if (right < size && cmp(array[right], array[largest]) > 0)
		largest = right;
	if (largest != root)
	{
		tmp = array[root];
		array[root] = array[largest];
		array[largest] = tmp;
		heapify(array, largest, size, cmp);
	}
}

int			fnamecmp(const t_file *f1, const t_file *f2)
{
	return (ft_strcmp(f1->name, f2->name));
}

void		heap_sort(t_file **children, int child_count,
		int (*cmp)(const t_file *, const t_file *))
{
	int		i;
	t_file	*tmp;

	i = (child_count / 2) - 1;
	while (i >= 0)
		heapify(children, i--, child_count, cmp);
	i = child_count - 1;
	while (i >= 0)
	{
		tmp = children[0];
		children[0] = children[i];
		children[i] = tmp;
		heapify(children, 0, i--, cmp);
	}
}
