/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   parser.c                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: sgardner <stephenbgardner@gmail.com>       +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2017/11/04 15:33:34 by sgardner          #+#    #+#             */
/*   Updated: 2017/11/06 17:03:15 by sgardner         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "ft_ls.h"

const t_lsflag	g_lsflags[] = {
	{ 'R', LS_REC, "" },
	{ 'T', LS_CT, "" },
	{ 'U', LS_CTIME, "tu" },
	{ 'a', LS_A, "" },
	{ 'f', LS_F, "" },
	{ 'g', LS_GROUP, "" },
	{ 'l', LS_L, "1" },
	{ 'o', LS_OMIT_GROUP, "" },
	{ 'r', LS_REV, "" },
	{ 't', LS_MTIME, "Uu" },
	{ 'u', LS_ATIME, "Ut" },
	{ '1', LS_1, "l" },
};

const int		g_lsflag_count = sizeof(g_lsflags) / sizeof(t_lsflag);

static const t_lsflag	*get_flag(char c)
{
	int	i;

	i = 0;
	while (i < g_lsflag_count)
	{
		if (c == g_lsflags[i].c)
			return (&g_lsflags[i]);
		i++;
	}
	return (NULL);
}

static void				clear_conflicts(const t_lsflag *flag, int *flags)
{
	int	i;
	int	conflict;

	i = 0;
	while (flag->conflicts[i])
	{
		conflict = get_flag(flag->conflicts[i++])->flag;
		if (*flags & conflict)
			*flags ^= conflict;
	}
}

static t_bool			print_usage(char c)
{
	int		i;

	write(2, g_app, ft_strlen(g_app));
	write(2, ": illegal option -- ", 20);
	write(2, &c, 1);
	write(2, "\nusage: ", 8);
	write(2, g_app, ft_strlen(g_app));
	write(2, " [-", 3);
	i = 0;
	while (i < g_lsflag_count)
		write(2, &g_lsflags[i++].c, 1);
	write(2, "] [file ...]\n", 13);
	return (FALSE);
}

t_bool					parse_flags(char **argv, int argc, int *idx, int *flags)
{
	char			*arg;
	const t_lsflag	*f;

	while (*idx < argc && (arg = argv[*idx]))
	{
		if (*arg != '-' || !*(arg + 1))
			break ;
		while (*(++arg))
		{
			if ((f = get_flag(*arg)))
			{
				clear_conflicts(f, flags);
				if (f->flag & (LS_GROUP | LS_OMIT_GROUP))
					*flags |= LS_L;
				*flags |= f->flag;
			}
			else
				return (print_usage(*arg));
		}
		(*idx)++;
	}
	return (TRUE);
}
