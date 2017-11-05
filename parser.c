/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   parser.c                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: sgardner <stephenbgardner@gmail.com>       +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2017/11/04 15:33:34 by sgardner          #+#    #+#             */
/*   Updated: 2017/11/05 02:11:57 by sgardner         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "ft_ls.h"

const t_lsflag	g_lsflags[] = {
	{ 'a', LS_A },
	{ 'f', LS_F },
	{ 'g', LS_GROUP },
	{ 'l', LS_L },
	{ 'o', LS_OMIT_GROUP },
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
