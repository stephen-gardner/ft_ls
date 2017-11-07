/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   stat.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: sgardner <stephenbgardner@gmail.com>       +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2017/10/30 21:02:06 by sgardner          #+#    #+#             */
/*   Updated: 2017/11/06 20:18:36 by sgardner         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "ft_ls.h"

static char		get_type(mode_t st_mode)
{
	if (FMT(st_mode, S_IFREG))
		return ('-');
	else if (FMT(st_mode, S_IFDIR))
		return ('d');
	else if (FMT(st_mode, S_IFLNK))
		return ('l');
	else if (FMT(st_mode, S_IFCHR))
		return ('c');
	else if (FMT(st_mode, S_IFSOCK))
		return ('s');
	else if (FMT(st_mode, S_IFBLK))
		return ('b');
	else if (FMT(st_mode, S_IFIFO))
		return ('p');
	return ('-');
}

static char		*get_perms(mode_t st_mode)
{
	char	*perms;

	if (!(perms = (char *)malloc(12)))
		return (NULL);
	perms[0] = get_type(st_mode);
	perms[1] = (USR(st_mode, S_IRUSR)) ? 'r' : '-';
	perms[2] = (USR(st_mode, S_IWUSR)) ? 'w' : '-';
	if (st_mode & S_ISUID)
		perms[3] = (USR(st_mode, S_IXUSR)) ? 's' : 'S';
	else
		perms[3] = (USR(st_mode, S_IXUSR)) ? 'x' : '-';
	perms[4] = (GRP(st_mode, S_IRGRP)) ? 'r' : '-';
	perms[5] = (GRP(st_mode, S_IWGRP)) ? 'w' : '-';
	if (st_mode & S_ISGID)
		perms[6] = (GRP(st_mode, S_IXGRP)) ? 's' : 'S';
	else
		perms[6] = (GRP(st_mode, S_IXGRP)) ? 'x' : '-';
	perms[7] = (OTH(st_mode, S_IROTH)) ? 'r' : '-';
	perms[8] = (OTH(st_mode, S_IWOTH)) ? 'w' : '-';
	if (st_mode & S_ISVTX)
		perms[9] = (OTH(st_mode, S_IXOTH)) ? 't' : 'T';
	else
		perms[9] = (OTH(st_mode, S_IXOTH)) ? 'x' : '-';
	perms[11] = '\0';
	return (perms);
}

/*
** Check for timestamp older than 6 months is approximate:
**  6 months = (365 / 2) * 1 day
**           = (  182.5) * 86400
**           =          15768000
*/

static char		*get_time_str(t_file *file, t_stat *stats, int flags)
{
	struct timespec	spec;
	char			*ftime;

	if (LSF(LS_ATIME))
		spec = stats->st_atimespec;
	else if (LSF(LS_CTIME))
		spec = stats->st_ctimespec;
	else
		spec = stats->st_mtimespec;
	file->timestamp = spec;
	ftime = ctime(&spec.tv_sec);
	if (!(ftime = ft_strsub(ftime, 4, 20)))
		return (NULL);
	if (!LSF(LS_CT))
	{
		if (spec.tv_sec <= g_time && g_time - spec.tv_sec <= 15768000)
			*(ftime + 12) = '\0';
		else
			ft_memmove(ftime + 7, ftime + 15, 6);
	}
	return (ftime);
}

t_bool			load_stats(t_file *file, t_stat *stats, int flags)
{
	struct passwd	*pw;
	struct group	*gr;

	file->block_size = stats->st_blocks;
	if (!(pw = getpwuid(stats->st_uid))
		|| !(gr = getgrgid(stats->st_gid)))
		return ((int)ls_error(file->path));
	if (!(file->stats[0] = get_perms(stats->st_mode))
		|| !(file->stats[1] = ft_itoa(stats->st_nlink))
		|| !(file->stats[2] = ft_strdup(pw->pw_name))
		|| !(file->stats[3] = ft_strdup(gr->gr_name))
		|| !(file->stats[4] = ft_itoa(stats->st_size))
		|| !(file->stats[5] = get_time_str(file, stats, flags)))
		return (FALSE);
	file->stats[0][10] = (listxattr(file->path, NULL, 0, XATTR_NOFOLLOW) > 0)
		? '@' : ' ';
	file->stats[6] = (FMT(stats->st_mode, S_IFLNK)) ?
		build_link(file->name, file->path) : ft_strdup(file->name);
	return ((file->stats[6]) ? TRUE : FALSE);
}
