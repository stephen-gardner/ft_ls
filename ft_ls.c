/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_ls.c                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: sgardner <stephenbgardner@gmail.com>       +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2017/10/27 19:21:45 by sgardner          #+#    #+#             */
/*   Updated: 2017/11/08 00:15:11 by sgardner         ###   ########.fr       */
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

static t_file	**build_args(char **argv, int argc, int idx, int flags)
{
	t_file	**array;
	int		len;
	int		i;

	sort_args(argv, argc, idx);
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
	while (idx < argc)
	{
		if (!(array[i] = load_parent(argv[idx++], flags)))
			continue ;
		i++;
	}
	sort_types(array);
//	if (!LSF(LS_F))
//		heap_sort(array, i, flags);
	return (array);
}

static int		get_max_width(t_file **children, int field)
{
	t_file	*ch;
	int		max;
	int		len;
	int		i;
	int		tmp;

	max = 0;
	i = 0;
	while ((ch = children[i++]))
	{
		len = 0;
		if (field == 4 && (ch->stats[0][0] == 'b' || ch->stats[0][0] == 'c'))
		{
			if ((len = ft_strlen(ch->stats[7])) > ch->parent->maxlen[4])
				ch->parent->maxlen[4] = len;
			if ((len = ft_strlen(ch->stats[8])) > ch->parent->maxlen[5])
				ch->parent->maxlen[5] = len;
			len = ch->parent->maxlen[4] + ch->parent->maxlen[5] + 3;
		}
		if ((tmp = ft_strlen(ch->stats[field])) > len)
			len = tmp;
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
	int		i;

	time(&g_time);
	flags = 0;
	idx = 0;
	g_app = ft_strrchr(argv[idx++], '/') + 1;
	if ((argc > 1 && !parse_flags(argv, argc, &idx, &flags))
		|| !(args = build_args(argv, argc, idx, flags)))
		return (1);
	i = 0;
	while (args[i])
	{
		if ((argc - idx) > 1 && args[i]->stats[0][0] == 'd')
		{
			if (i > 0)
				write(1, "\n", 1);
			ft_printf("%s:\n", (args[i])->path);
		}
		print_recursive(args[i++], flags);
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
