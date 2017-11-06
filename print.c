/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   print.c                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: sgardner <stephenbgardner@gmail.com>       +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2017/11/01 17:23:58 by sgardner          #+#    #+#             */
/*   Updated: 2017/11/06 15:43:33 by sgardner         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "ft_ls.h"

static void		print_long(t_file *file, int *maxlen, int flags)
{
	char	**stats;
	char	xattr;

	stats = file->stats;
	if (listxattr(file->path, NULL, 0, XATTR_NOFOLLOW) > 0)
		xattr = '@';
	else
		xattr = ' ';
	ft_printf("%s%c ", stats[0], xattr);
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
			? print_long(file, NULL, flags)
			: ft_printf("%s\n", file->name);
		return ;
	}
	if (LSF(LS_L))
	{
		set_padding(file);
		if (file->child_count > 0)
			ft_printf("total %lld\n", count_blocks(file->children));
	}
	i = 0;
	while (i < file->child_count)
	{
		child = file->children[i++];
		(LSF(LS_L))
			? print_long(child, file->maxlen, flags)
			: ft_printf("%s\n", child->name);
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
			|| !ft_printf("\n%s:\n", child->path)
			|| !load_children(child, flags))
		{
			free_file(child);
			continue ;
		}
		if (child->child_count && !LSF(LS_F))
			heap_sort(child->children, child->child_count, flags);
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
	int	i;

	if (file->child_count && !LSF(LS_F))
		heap_sort(file->children, file->child_count, flags);
	print_files(file, flags);
	if (!LSF(LS_REC) || !file->child_count)
	{
		i = 0;
		while (i < file->child_count)
			free_file(file->children[i++]);
		free_file(file);
		return ;
	}
	while ((file = get_next_folder(file, flags)))
		;
}
