/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   print.c                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: sgardner <stephenbgardner@gmail.com>       +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2017/11/01 17:23:58 by sgardner          #+#    #+#             */
/*   Updated: 2017/11/05 02:48:23 by sgardner         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "ft_ls.h"

static void		print_long(char **stats, int *maxlen, int flags)
{
	ft_printf("%s  ", stats[0]);
	ft_printf("%*s ", (maxlen) ? maxlen[0] : ft_strlen(stats[1]), stats[1]);
	if (!LSF(LS_GROUP))
		ft_printf("%-*s  ",
				(maxlen) ? maxlen[1] : ft_strlen(stats[2]), stats[2]);
	if (!LSF(LS_OMIT_GROUP))
		ft_printf("%-*s  ",
				(maxlen) ? maxlen[2] : ft_strlen(stats[3]), stats[3]);
	if (LSF(LS_GROUP) && LSF(LS_OMIT_GROUP))
		write(1, "  ", 2);
	ft_printf("%*s ", (maxlen) ? maxlen[3] : ft_strlen(stats[4]), stats[4]);
	ft_printf("%s ", stats[5]);
	ft_printf("%s\n", stats[6]);
}

static void		print_files(t_file *file, int flags)
{
	t_file	*child;
	int		i;

	if (!file->children)
	{
		(LSF(LS_L))
			? print_long(file->stats, NULL, flags)
			: ft_printf("%s\n", file->name);
		return ;
	}
	if (LSF(LS_L))
	{
		set_padding(file);
		if (file->child_count > 0)
			ft_printf("total %lld\n", count_blocks(file->children));
	}
	i = (LSF(LS_REV) && !LSF(LS_F)) ? file->child_count - 1 : 0;
	while ((LSF(LS_REV) && !LSF(LS_F)) ? i > -1 : i < file->child_count)
	{
		child = file->children[i];
		(LSF(LS_L))
			? print_long(child->stats, file->maxlen, flags)
			: ft_printf("%s\n", child->name);
		i += (LSF(LS_REV) && !LSF(LS_F)) ? -1 : 1;
	}
}

static t_file	*get_next_folder(t_file *file, int flags)
{
	t_file	*child;

	while (file->i < file->child_count && (child = file->children[file->i++]))
	{
		if (child->stats[0][0] != 'd'
			|| !ft_strcmp(child->name, ".")
			|| !ft_strcmp(child->name, "..")
			|| !ft_printf("\n%s:\n", child->path + (*child->path == '/'))
			|| !load_children(child, flags))
		{
			free_file(child);
			continue ;
		}
		if (child->child_count && !LSF(LS_F))
			(LSF(LS_MTIME))
				? heap_sort(child->children, child->child_count, &ftimecmp)
				: heap_sort(child->children, child->child_count, &fnamecmp);
		print_files(child, flags);
		return (child);
	}
	child = (file->parent) ? file->parent : NULL;
	free_file(file);
	return (child);
}

/*
** Except it's iterative! >:)
*/

void			print_recursive(t_file *file, int flags)
{
	if (file->child_count && !LSF(LS_F))
		(LSF(LS_MTIME))
			? heap_sort(file->children, file->child_count, &ftimecmp)
			: heap_sort(file->children, file->child_count, &fnamecmp);
	print_files(file, flags);
	if (!LSF(LS_REC) || !file->children)
	{
		free_file(file);
		return ;
	}
	while ((file = get_next_folder(file, flags)))
		;
}
