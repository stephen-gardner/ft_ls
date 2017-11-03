/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   parser.c                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: sgardner <stephenbgardner@gmail.com>       +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2017/11/01 20:12:04 by sgardner          #+#    #+#             */
/*   Updated: 2017/11/01 23:19:20 by sgardner         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "ft_ls.h"

const t_lsflag	g_lsflags[] = {
	{ 'a', LS_A },
	{ 'l', LS_L },
	{ 'R', LS_REC },
	{ 'r', LS_REV },
	{ 'T', LS_CT },
	{ 't', LS_MTIME },
	{ 'U', LS_CTIME },
	{ 'u', LS_ATIME },
};

const int		g_lsflag_count = sizeof(g_lsflags) / sizeof(t_lsflag);

static int		get_flag(char c)
{
	int	i;

	i = 0;
	while (i < g_lsflag_count)
	{
		if (c == g_lsflags[i].c)
			return (g_lsflags[i].flag);
		i++;
	}
	return (0);
}

t_bool			parse_flags(int *flags, char **argv, int *idx, int argc)
{
	char	*arg;
	int		f;

	while (*idx < argc && (arg = argv[*idx]))
	{
		if (*arg != '-' || !*(arg + 1))
			break ;
		while (*(++arg))
		{
			if ((f = get_flag(*arg)))
				*flags |= f;
			else
			{
				return (FALSE);
			}
		}
		(*idx)++;
	}
	return (TRUE);
}
