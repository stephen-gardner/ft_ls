/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   load.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: sgardner <stephenbgardner@gmail.com>       +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2017/10/27 22:17:59 by sgardner          #+#    #+#             */
/*   Updated: 2017/11/07 20:58:11 by sgardner         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "ft_ls.h"

t_file		*build_file(char *path, t_dirent *dp, int flags)
{
	t_file		*file;
	t_stat		*stats;
	int			res;

	stats = NULL;
	if (!(file = (t_file *)ft_memalloc(sizeof(t_file)))
		|| !(stats = (t_stat *)ft_memalloc(sizeof(t_stat)))
		|| !(file->name = ft_strdup(dp->d_name))
		|| !(file->path = ft_strdup(path)))
	{
		free(stats);
		return (free_file(&file));
	}
	res = lstat(path, stats);
	if (res < 0 || !load_stats(file, stats, flags))
	{
		ls_error(path);
		free_file(&file);
	}
	free(stats);
	return (file);
}

void		*free_file(t_file **file)
{
	int	i;

	free((*file)->name);
	free((*file)->path);
	i = 0;
	while ((*file)->stats[i])
		free((*file)->stats[i++]);
	free((*file)->children);
	free(*file);
	*file = NULL;
	return (NULL);
}

t_bool		load_children(t_file *file, int flags)
{
	DIR			*dir;
	t_dirent	*dp;
	char		*cpath;
	int			i;

	if ((file->child_count = dir_len(file->path, flags)) < 0
		|| !(file->children =
			(t_file **)ft_memalloc(sizeof(t_file *) * (file->child_count + 1)))
		|| !(dir = ls_open(file->path)))
		return (FALSE);
	i = 0;
	while ((dp = ls_read(dir, file->path)))
	{
		if (!LSF(LS_A) && *dp->d_name == '.')
			continue ;
		if (!(cpath = build_path(file->path, dp->d_name))
			|| !(file->children[i] = build_file(cpath, dp, flags))
			|| !(file->children[i++]->parent = file))
		{
			free(cpath);
			return (FALSE);
		}
		free(cpath);
	}
	return (!errno && ls_close(dir, file->path) > -1);
}

t_file		*load_parent(char *path, int flags)
{
	t_file	*file;
	t_stat	*stats;

	stats = NULL;
	if (!(file = (t_file *)ft_memalloc(sizeof(t_file)))
		|| !(file->name = ft_strdup(path))
		|| !(file->path = ft_strdup(path))
		|| !(stats = (t_stat *)ft_memalloc(sizeof(t_stat)))
		|| lstat(path, stats) < 0
		|| !load_stats(file, stats, flags))
	{
		ls_error(path);
		free_file(&file);
	}
	if (FMT(stats->st_mode, S_IFDIR)
		|| FMT(stats->st_mode, S_IFLNK && is_symdir(file)))
	{
		if (!load_children(file, flags))
			free_file(&file);
	}
	free(stats);
	return (file);
}
