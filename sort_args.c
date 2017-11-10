/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   sort_args.c                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: sgardner <stephenbgardner@gmail.com>       +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2017/11/09 23:03:51 by sgardner          #+#    #+#             */
/*   Updated: 2017/11/10 01:41:27 by sgardner         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "ft_ls.h"

static t_bool	valid_args(char **argv, int idx)
{
	while (argv[idx])
	{
		if (!*argv[idx++])
		{
			errno = ENOENT;
			return (FALSE);
		}
	}
	return (TRUE);
}

t_bool		sort_args(char **argv, int argc, int idx)
{
	char	*tmp;
	t_bool	change;
	int		i;

	if (!valid_args(argv, idx))
		return (FALSE);
	while (idx < argc)
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
	return (TRUE);
}

void		sort_types(t_file **args)
{
	t_file	*tmp;
	t_bool	change;
	int		i;

	while (TRUE)
	{
		change = FALSE;
		i = 0;
		while (args[++i])
		{
			if (!args[i]->children && args[i - 1]->children)
			{
				tmp = args[i];
				args[i] = args[i - 1];
				args[i - 1] = tmp;
				change = TRUE;
			}
		}
		if (!change)
			break ;
	}
}

void		split_sort(t_file **args, int flags)
{
	int	folders;
	int	files;
	int	i;

	files = 0;
	folders = 0;
	i = 0;
	while (args[i])
	{
		if (args[i++]->children)
			folders++;
		else
			files++;
	}
	if (files)
		heap_sort(args, files, flags);
	if (folders)
		heap_sort(&args[files], folders, flags);
}
