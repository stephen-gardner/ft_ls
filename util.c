/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   util.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: sgardner <stephenbgardner@gmail.com>       +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2017/10/31 11:28:35 by sgardner          #+#    #+#             */
/*   Updated: 2017/11/03 20:36:00 by sgardner         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "ft_ls.h"

char	*build_path(char *parent, char *child)
{
	char	*path;
	char	*tmp;

	if (!(path = (char *)malloc(ft_strlen(parent) + ft_strlen(child) + 2)))
		return (NULL);
	tmp = path;
	tmp = ft_stpcpy(tmp, parent);
	*tmp++ = '/';
	tmp = ft_stpcpy(tmp, child);
	return (path);
}

char	*build_link(char *link, char *path)
{
	char	target[256];
	char	*label;
	char	*tmp;
	int		len;

	if ((len = readlink(path, target, 255)) < 0)
		return (ls_error(path));
	target[len] = '\0';
	if (!(label = (char *)malloc(ft_strlen(link) + ft_strlen(target) + 5)))
		return (NULL);
	tmp = label;
	tmp = ft_stpcpy(tmp, link);
	tmp = ft_stpcpy(tmp, " -> ");
	tmp = ft_stpcpy(tmp, target);
	return (label);
}

long long	count_blocks(t_file **children)
{
	long long	blocks;
	int			i;

	blocks = 0;
	i = 0;
	while (children[i])
		blocks += children[i++]->block_size;
	return (blocks);
}

int		dir_len(char *path, int flags)
{
	DIR			*dir;
	int			dlen;
	t_dirent	*dp;

	if (!(dir = ls_open(path)))
		return (-1);
	dlen = 0;
	while ((dp = ls_read(dir, path)))
	{
		if (!(flags & LS_A) && *dp->d_name == '.')
			continue ;
		dlen++;
	}
	if (errno || ls_close(dir, path) < 0)
		return (-1);
	return (dlen);
}

void	*ls_error(char *path)
{
	write(2, g_app, ft_strlen(g_app));
	write(2, ": ", 2);
	if (path)
	{
		write(2, path, ft_strlen(path));
		write(2, ": ", 2);
	}
	write(2, strerror(errno), ft_strlen(strerror(errno)));
	write(2, "\n", 1);
	return (NULL);
}
