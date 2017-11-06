/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   sort.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: sgardner <stephenbgardner@gmail.com>       +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2017/11/03 15:07:58 by sgardner          #+#    #+#             */
/*   Updated: 2017/11/06 13:52:44 by sgardner         ###   ########.fr       */
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
	if (LSF(LS_MTIME))
	{
		if (f2->timestamp.tv_sec != f1->timestamp.tv_sec)
			return (f2->timestamp.tv_sec > f1->timestamp.tv_sec);
		if (f2->timestamp.tv_nsec != f1->timestamp.tv_nsec)
			return (f2->timestamp.tv_nsec > f1->timestamp.tv_nsec);
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

void		sort_args(char **argv, int argc, int idx)
{
	char	*tmp;
	t_bool	change;
	int		i;

	if (idx >= argc)
		return ;
	while (TRUE)
	{
		change = FALSE;
		i = idx;
		while (++i < argc)
		{
			if (ft_strcmp(argv[i], argv[i - 1]) < 0)
			{
				tmp = argv[i];
				argv[i] = argv[i - 1];
				argv[i - 1] = tmp;
				change = TRUE;
			}
		}
		if (!change)
			break ;
	}
}

void		sort_types(t_file **children)
{
	t_file	*tmp;
	t_bool	change;
	int		i;

	while (TRUE)
	{
		change = FALSE;
		i = 0;
		while (children[++i])
		{
			if (children[i]->stats[0][0] != 'd'
					&& children[i - 1]->stats[0][0] == 'd')
			{
				tmp = children[i];
				children[i] = children[i - 1];
				children[i - 1] = tmp;
				change = TRUE;
			}
		}
		if (!change)
			break ;
	}
}
