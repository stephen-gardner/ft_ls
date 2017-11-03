/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_ls.c                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: sgardner <stephenbgardner@gmail.com>       +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2017/10/27 19:21:45 by sgardner          #+#    #+#             */
/*   Updated: 2017/11/02 22:53:53 by sgardner         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "ft_ls.h"

const char	*g_app;

int		main(int argc, char **argv)
{
	t_file	*file;
	int		flags;
	int		idx;

	flags = 0;
	idx = 0;
	g_app = argv[idx++] + 2;
	if (argc > 1 && !parse_flags(&flags, argv, &idx, argc))
		return (1);
	if (idx >= argc)
		file = load_parent(".", flags);
	else
		file = load_parent(argv[idx], flags);
	if (!file)
		return (1);
	print_files(file, flags);
	return (0);
}
