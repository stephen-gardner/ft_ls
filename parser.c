/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   parser.c                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: sgardner <stephenbgardner@gmail.com>       +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2017/11/04 15:33:34 by sgardner          #+#    #+#             */
/*   Updated: 2017/11/09 21:51:36 by sgardner         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "ft_ls.h"

/*
**  { 'l', LS_L, "", "1" }
**     |    |    |    |
**     |    |    |    Conflicting flags
**     |    |   Prerequisite flags
**     |   Flag bit value
**    Specifier
*/

const t_lsflag	g_lsflags[] = {
	{ 'R', LS_REC, "", "" },
	{ 'T', LS_CT, "", "" },
	{ 'U', LS_CTIME, "", "tu" },
	{ 'a', LS_A, "", "" },
	{ 'd', LS_D, "", "" },
	{ 'f', LS_F, "a", "" },
	{ 'g', LS_GROUP, "l", "" },
	{ 'l', LS_L, "", "1" },
	{ 'o', LS_OMIT_GROUP, "l", "" },
	{ 'r', LS_REV, "", "" },
	{ 't', LS_MTIME, "", "Uu" },
	{ 'u', LS_ATIME, "", "Ut" },
	{ '1', LS_1, "", "l" },
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

static t_bool			is_hyphenf(char *arg)
{
	DIR			*dir;
	t_dirent	*dp;
	int			n;
	int			i;
	t_bool		res;

	n = 0;
	while (arg[n] == '-')
		n++;
	if (!(dir = opendir(".")))
		return (FALSE);
	res = FALSE;
	while ((dp = readdir(dir)))
	{
		i = 0;
		while (i < n && dp->d_name[i] == '-')
			i++;
		if (i == n - 1 && !dp->d_name[i])
			res = TRUE;
	}
	if (closedir(dir) < 0)
		return (FALSE);
	return (res);
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

static void				set_flag(const t_lsflag *f, int *flags)
{
	int	i;
	int	c;

	i = 0;
	while (f->conflicts[i])
	{
		c = get_flag(f->conflicts[i++])->flag;
		if (*flags & c)
			*flags ^= c;
	}
	i = 0;
	while (f->prereqs[i])
		*flags |= get_flag(f->prereqs[i++])->flag;
	*flags |= f->flag;
}

t_bool					parse_flags(char **argv, int argc, int *idx, int *flags)
{
	char			*arg;
	const t_lsflag	*f;

	while (*idx < argc && (arg = argv[*idx]))
	{
		if (*arg != '-' || !*(arg + 1))
			break ;
		if (*(++arg) == '-')
		{
			if ((*idx > 2 && *(arg + 1) == '-' && !is_hyphenf(arg))
				|| (*(arg + 1) == '-'))
				return (print_usage(*arg));
			else
				(*idx)++;
			break ;
		}
		while (*arg)
		{
			if (!(f = get_flag(*arg++)))
				return (print_usage(*(arg - 1)));
			set_flag(f, flags);
		}
		(*idx)++;
	}
	return (TRUE);
}
