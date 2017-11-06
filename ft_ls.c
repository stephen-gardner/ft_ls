/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_ls.c                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: sgardner <stephenbgardner@gmail.com>       +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2017/10/27 19:21:45 by sgardner          #+#    #+#             */
/*   Updated: 2017/11/05 20:01:17 by sgardner         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "ft_ls.h"

time_t		g_time;
const char	*g_app;

static void		*free_args(t_file **args)
{
	int	i;

	i = 0;
	while (args[i])
		free(args[i++]);
	free(args);
	return (NULL);
}

static t_file	**build_args(int idx, int argc, char **argv, int flags)
{
	t_file	**array;
	int		len;
	int		i;

	len = (idx >= argc) ? 1 : (argc - idx);
	if (!(array = (t_file **)ft_memalloc(sizeof(t_file *) * (len + 1))))
		return (NULL);
	if (idx >= argc)
	{
		if (!(array[0] = load_parent(".", flags)))
			return (free_args(array));
		return (array);
	}
	i = 0;
	while (i < len)
	{
		if (!(array[i++] = load_parent(argv[idx++], flags)))
			return (free_args(array));
	}
	if (!LSF(LS_F))
		(LSF(LS_MTIME))
			? heap_sort(array, len, &ftimecmp)
			: heap_sort(array, len, &fnamecmp);
	return (array);
}

static int		get_max_width(t_file **children, int field)
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

int				main(int argc, char **argv)
{
	t_file	**args;
	int		flags;
	int		idx;
	t_bool	title;
	int		i;

	time(&g_time);
	flags = 0;
	idx = 0;
	g_app = argv[idx++] + 2;
	if ((argc > 1 && !parse_flags(&flags, argv, &idx, argc))
		|| !(args = build_args(idx, argc, argv, flags)))
		return (1);
	title = ((argc - idx) > 0);
	i = 0;
	while (args[i])
	{
		if (title)
			ft_printf("%s:\n", (args[i])->path);
		print_recursive(args[i++], flags);
		if (args[i])
			ft_printf("\n");
	}
	free(args);
	return (0);
}

void			set_padding(t_file *file)
{
	file->maxlen[0] = get_max_width(file->children, 1);
	file->maxlen[1] = get_max_width(file->children, 2);
	file->maxlen[2] = get_max_width(file->children, 3);
	file->maxlen[3] = get_max_width(file->children, 4);
}
