/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   print.c                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: sgardner <stephenbgardner@gmail.com>       +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2017/11/01 17:23:58 by sgardner          #+#    #+#             */
/*   Updated: 2017/11/10 21:00:33 by sgardner         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "ft_ls.h"

#define VWF(x, y) (max) ? max[x] : ft_strlen(stats[y]), stats[y]

static long long	count_blocks(t_file **children)
{
	long long	blocks;
	int			i;

	blocks = 0;
	i = 0;
	while (children[i])
		blocks += children[i++]->block_size;
	return (blocks);
}

static void			print_long(char **stats, int *max, int flags)
{
	ft_printf("%s ", stats[0]);
	ft_printf("%*s ", VWF(0, 1));
	if (!LSF(LS_GROUP))
		ft_printf("%-*s  ", VWF(1, 2));
	if (!LSF(LS_OMIT_GROUP))
		ft_printf("%-*s  ", VWF(2, 3));
	if (LSF(LS_GROUP) && LSF(LS_OMIT_GROUP))
		write(1, "  ", 2);
	if (stats[0][0] == 'b' || stats[0][0] == 'c')
		ft_printf(" %*s, %*s ", VWF(4, 7), VWF(5, 8));
	else
		ft_printf("%*s ", VWF(3, 4));
	ft_printf("%s ", stats[5]);
	ft_printf("%s\n", stats[6]);
}

static void			print_files(t_file *file, int flags)
{
	t_file	*child;
	int		i;

	if (!file->children)
	{
		(LSF(LS_L))
			? print_long(file->stats, NULL, flags)
			: ft_printf("%s\n", file->stats[6]);
		return ;
	}
	if (LSF(LS_L))
	{
		set_padding(file);
		if (file->child_count)
			ft_printf("total %lld\n", count_blocks(file->children));
	}
	i = 0;
	while ((child = file->children[i++]))
	{
		(LSF(LS_L))
			? print_long(child->stats, file->maxlen, flags)
			: ft_printf("%s\n", child->stats[6]);
	}
}

static t_file		*get_next_folder(t_file *file, int flags)
{
	t_file	*child;

	while (file->i < file->child_count && (child = file->children[file->i++]))
	{
		if (child->stats[0][0] != 'd'
			|| !ft_strcmp(child->name, ".")
			|| !ft_strcmp(child->name, "..")
			|| !ft_printf("\n%s:\n", child->path)
			|| !load_children(child, flags))
		{
			free_file(&child);
			continue ;
		}
		if (file->child_count && !LSF(LS_F))
			heap_sort(child->children, child->child_count, flags);
		print_files(child, flags);
		return (child);
	}
	child = (file->parent) ? file->parent : NULL;
	free_file(&file);
	return (child);
}

/*
** Except it's iterative! >:)
*/

void				print_recursive(t_file *file, int flags)
{
	int	i;

	if (file->child_count && !LSF(LS_F))
		heap_sort(file->children, file->child_count, flags);
	print_files(file, flags);
	if (!LSF(LS_REC) || !file->child_count)
	{
		i = 0;
		while (i < file->child_count)
			free_file(&file->children[i++]);
		free_file(&file);
		return ;
	}
	while ((file = get_next_folder(file, flags)))
		;
}
