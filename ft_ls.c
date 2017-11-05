/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_ls.c                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: sgardner <stephenbgardner@gmail.com>       +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2017/10/27 19:21:45 by sgardner          #+#    #+#             */
/*   Updated: 2017/11/05 13:29:01 by sgardner         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "ft_ls.h"

time_t		g_time;
const char	*g_app;

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

int			main(int argc, char **argv)
{
	t_file	*file;
	int		flags;
	int		idx;
	t_bool	title;

	time(&g_time);
	flags = 0;
	idx = 0;
	g_app = argv[idx++] + 2;
	if (argc > 1 && !parse_flags(&flags, argv, &idx, argc))
		return (1);
	file = (idx < argc) ?
		load_parent(argv[idx++], flags) : load_parent(".", flags);
	title = ((argc - idx) > 0);
	while (file)
	{
		if (title)
			ft_printf("%s:\n", file->path);
		print_recursive(file, flags);
		file = (idx < argc) ? load_parent(argv[idx], flags) : NULL;
		if (idx++ < argc)
			ft_printf("\n");
	}
	return (0);
}

void		set_padding(t_file *file)
{
	file->maxlen[0] = get_max_width(file->children, 1);
	file->maxlen[1] = get_max_width(file->children, 2);
	file->maxlen[2] = get_max_width(file->children, 3);
	file->maxlen[3] = get_max_width(file->children, 4);
}
