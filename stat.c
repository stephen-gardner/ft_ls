/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   stat.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: sgardner <stephenbgardner@gmail.com>       +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2017/10/30 21:02:06 by sgardner          #+#    #+#             */
/*   Updated: 2017/11/10 21:03:13 by sgardner         ###   ########.fr       */
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

#define USR(x, a, b) ((st_mode & S_IRWXU) & x) ? a : b
#define GRP(x, a, b) ((st_mode & S_IRWXG) & x) ? a : b
#define OTH(x, a, b) ((st_mode & S_IRWXO) & x) ? a : b

static char		*get_perms(mode_t st_mode)
{
	char	*perms;

	if (!(perms = (char *)malloc(12)))
		return (NULL);
	perms[0] = get_type(st_mode);
	perms[1] = USR(S_IRUSR, 'r', '-');
	perms[2] = USR(S_IWUSR, 'w', '-');
	perms[3] = (st_mode & S_ISUID) ?
		USR(S_IXUSR, 's', 'S') : USR(S_IXUSR, 'x', '-');
	perms[4] = GRP(S_IRGRP, 'r', '-');
	perms[5] = GRP(S_IWGRP, 'w', '-');
	perms[6] = (st_mode & S_ISGID) ?
		GRP(S_IXGRP, 's', 'S') : GRP(S_IXGRP, 'x', '-');
	perms[7] = OTH(S_IROTH, 'r', '-');
	perms[8] = OTH(S_IWOTH, 'w', '-');
	perms[9] = (st_mode & S_ISVTX) ?
		OTH(S_IXOTH, 't', 'T') : OTH(S_IXOTH, 'x', '-');
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

t_bool			is_symdir(t_file *file, int flags)
{
	t_stat	*stats;
	t_bool	symdir;

	if (!(stats = (t_stat *)ft_memalloc(sizeof(t_stat)))
		|| stat(file->path, stats) < 0)
	{
		free(stats);
		return (FALSE);
	}
	symdir = FMT(stats->st_mode, S_IFDIR);
	return (LSF(LS_L) ? FALSE : symdir);
}

#define SET_STAT(x, y) (file->stats[x] = y)

t_bool			load_stats(t_file *file, t_stat *stats, int flags)
{
	struct passwd	*pw;
	struct group	*gr;

	file->block_size = stats->st_blocks;
	pw = getpwuid(stats->st_uid);
	gr = getgrgid(stats->st_gid);
	if (!SET_STAT(0, get_perms(stats->st_mode))
		|| !SET_STAT(1, ft_itoa(stats->st_nlink))
		|| !SET_STAT(2, (pw) ? ft_strdup(pw->pw_name) : ft_itoa(stats->st_uid))
		|| !SET_STAT(3, (gr) ? ft_strdup(gr->gr_name) : ft_itoa(stats->st_gid))
		|| !SET_STAT(4, ft_itoa(stats->st_size))
		|| !SET_STAT(5, get_time_str(file, stats, flags))
		|| !SET_STAT(6, (FMT(stats->st_mode, S_IFLNK) && LSF(LS_L))
			? build_link(file->name, file->path, flags)
			: build_dname(file, flags)))
		return (FALSE);
	if (file->stats[0][0] == 'b' || file->stats[0][0] == 'c')
	{
		if (!SET_STAT(7, ft_itoa(major(stats->st_rdev)))
			|| !SET_STAT(8, ft_itoa(minor(stats->st_rdev))))
			return (FALSE);
	}
	file->stats[0][10] = (listxattr(file->path, NULL, 0, XATTR_NOFOLLOW) > 0) ?
		'@' : ' ';
	return (TRUE);
}
