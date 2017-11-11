/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   util.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: sgardner <stephenbgardner@gmail.com>       +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2017/10/31 11:28:35 by sgardner          #+#    #+#             */
/*   Updated: 2017/11/10 21:11:10 by sgardner         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "ft_ls.h"

#define STAT(x, y) file->stats[x][y]

char		*build_dname(t_file *file, int flags)
{
	char	*dname;
	char	*tmp;
	t_bool	x;

	if (!LSF(LS_FTYPE) && !LSF(LS_P))
		return (ft_strdup(file->name));
	if (!(dname = (char *)malloc(ft_strlen(file->name) + 2)))
		return (NULL);
	tmp = ft_stpcpy(dname, file->name);
	if (STAT(0, 0) == 'd')
		*tmp++ = '/';
	if (!LSF(LS_P))
	{
		x = (STAT(0, 3) != '-' || STAT(0, 6) != '-' || STAT(0, 9) != '-');
		if (STAT(0, 0) == '-' && x)
			*tmp++ = '*';
		else if (STAT(0, 0) == 'l')
			*tmp++ = '@';
		else if (STAT(0, 0) == 's')
			*tmp++ = '=';
		else if (STAT(0, 0) == 'p')
			*tmp++ = '|';
	}
	*tmp = '\0';
	return (dname);
}

char		*build_link(char *link, char *path, int flags)
{
	char	target[256];
	char	*label;
	char	*tmp;
	int		len;

	if ((len = readlink(path, target, 255)) < 0)
		return (ls_error(path));
	target[len] = '\0';
	len = ft_strlen(link) + ft_strlen(target) + LSF(LS_FTYPE) + 5;
	if (!(label = (char *)malloc(len)))
		return (NULL);
	tmp = ft_stpcpy(label, link);
	if (LSF(LS_FTYPE))
		*tmp++ = '@';
	tmp = ft_stpcpy(tmp, " -> ");
	tmp = ft_stpcpy(tmp, target);
	return (label);
}

char		*build_path(char *parent, char *child)
{
	char	*path;
	char	*tmp;

	if (!(path = (char *)malloc(ft_strlen(parent) + ft_strlen(child) + 2)))
		return (NULL);
	tmp = ft_stpcpy(path, parent);
	if (*(tmp - 1) != '/')
		*tmp++ = '/';
	tmp = ft_stpcpy(tmp, child);
	return (path);
}

int			dir_len(char *path, int flags)
{
	DIR			*dir;
	int			dlen;
	t_dirent	*dp;

	if (!(dir = ls_open(path)))
		return (-1);
	dlen = 0;
	while ((dp = ls_read(dir, path)))
	{
		if (skip_file(dp->d_name, flags))
			continue ;
		dlen++;
	}
	if (errno || ls_close(dir, path) < 0)
		return (-1);
	return (dlen);
}

void		*ls_error(char *path)
{
	char	*tmp;

	write(2, g_app, ft_strlen(g_app));
	write(2, ": ", 2);
	if (path)
	{
		if (!*path)
			path = "fts_open";
		if (errno != ENOENT && (tmp = ft_strrchr(path, '/')))
			path = tmp + 1;
		write(2, path, ft_strlen(path));
		write(2, ": ", 2);
	}
	write(2, strerror(errno), ft_strlen(strerror(errno)));
	write(2, "\n", 1);
	return (NULL);
}
