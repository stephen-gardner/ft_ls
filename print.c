/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   print.c                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: sgardner <stephenbgardner@gmail.com>       +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2017/11/01 17:23:58 by sgardner          #+#    #+#             */
/*   Updated: 2017/11/02 22:53:27 by sgardner         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "ft_ls.h"

static int	get_max_width(t_file **children, int field)
{
	int	max;
	int	len;
	int	i;

	max = 0;
	i = 0;
	while (children[i])
	{
		len = ft_strlen(children[i++]->stats[field]);
		if (len > max)
			max = len;
	}
	return (max);
}

static void	set_padding(t_file *file)
{
	file->maxlen[0] = get_max_width(file->children, 1);
	file->maxlen[1] = get_max_width(file->children, 2);
	file->maxlen[2] = get_max_width(file->children, 3);
	file->maxlen[3] = get_max_width(file->children, 4);
}

void		print_files(t_file *file, int flags)
{
	t_file	*child;
	int		i;

	if (!file->children)
		(flags & LS_L) ?
			print_long(file->stats, NULL) : ft_printf("%s\n", file->name);
	else
	{
		if (flags & LS_L)
			set_padding(file);
		i = 0;
		while ((child = file->children[i]))
		{
			if (flags & LS_L)
				print_long(file->children[i++]->stats, file->maxlen);
			else
				ft_printf("%s\n", file->children[i++]->name);
		}
	}
	free_file(file);
}

void		print_long(char **stats, int *maxlen)
{
	ft_printf("%s  ", stats[0]);
	ft_printf("%*s ", (maxlen) ? maxlen[0] : ft_strlen(stats[1]), stats[1]);
	ft_printf("%-*s  ", (maxlen) ? maxlen[1] : ft_strlen(stats[2]), stats[2]);
	ft_printf("%-*s  ", (maxlen) ? maxlen[2] : ft_strlen(stats[3]), stats[3]);
	ft_printf("%*s ", (maxlen) ? maxlen[3] : ft_strlen(stats[4]), stats[4]);
	ft_printf("%s ", stats[5]);
	ft_printf("%s\n", stats[6]);
}
