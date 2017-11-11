/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_ls.h                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: sgardner <stephenbgardner@gmail.com>       +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2017/10/27 19:21:24 by sgardner          #+#    #+#             */
/*   Updated: 2017/11/10 23:30:08 by sgardner         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef FT_LS_H
# define FT_LS_H
# include <dirent.h>
# include <errno.h>
# include <grp.h>
# include <pwd.h>
# include <stdio.h>
# include <sys/stat.h>
# include <sys/xattr.h>
# include <time.h>
# include <unistd.h>
# include "ft_printf.h"

# define LSF(x) (flags & x)
# define FMT(x, y) ((x & S_IFMT) == y)

typedef struct dirent	t_dirent;
typedef struct stat		t_stat;

typedef struct			s_file
{
	char			*name;
	char			*path;
	long long		block_size;
	long long		byte_size;
	char			*stats[10];
	int				maxlen[6];
	struct timespec	timestamp;
	struct s_file	*parent;
	struct s_file	**children;
	int				child_count;
	int				i;
}						t_file;

typedef struct			s_lsflag
{
	char	c;
	int		flag;
	char	*prereqs;
	char	*conflicts;
}						t_lsflag;

enum	e_lsflags
{
	LS_ALL = 1,
	LS_FTYPE = 1 << 1,
	LS_REC = 1 << 2,
	LS_SIZE = 1 << 3,
	LS_CT = 1 << 4,
	LS_BTIME = 1 << 5,
	LS_A = 1 << 6,
	LS_CTIME = 1 << 7,
	LS_D = 1 << 8,
	LS_F = 1 << 9,
	LS_GROUP = 1 << 10,
	LS_L = 1 << 11,
	LS_OMIT_GROUP = 1 << 12,
	LS_P = 1 << 13,
	LS_REV = 1 << 14,
	LS_MTIME = 1 << 15,
	LS_ATIME = 1 << 16,
	LS_1 = 1 << 17
};

/*
** ft_ls.c
*/

void					set_padding(t_file *file);

/*
** load.c
*/

t_file					*build_file(char *path, char *name, int flags);

void					*free_file(t_file **file);

t_bool					load_children(t_file *file, int flags);

t_file					*load_parent(char *path, int flags);

t_bool					skip_file(char *fname, int flags);

/*
** parser.c
*/

t_bool					parse_flags(char **argv, int argc, int *idx,
		int *flags);

/*
** print.c
*/

void					print_recursive(t_file *file, int flags);

/*
** sort.c
*/

void					heap_sort(t_file **children, int child_count,
		int flags);

/*
** sort_args.c
*/

t_bool					sort_args(char **argv, int argc, int idx);

void					sort_types(t_file **children);

void					split_sort(t_file **args, int flags);

/*
** stat.c
*/

t_bool					is_symdir(t_file *file, int flags);
t_bool					load_stats(t_file *file, t_stat *stats, int flags);

/*
** util.c
*/

char					*build_dname(t_file *file, int flags);

char					*build_link(char *link, char *target, int flags);

char					*build_path(char *parent, char *child);

int						dir_len(char *path, int flags);

void					*ls_error(char *path);

/*
** wrappers.c
*/

int						ls_close(DIR *dir, char *path);

DIR						*ls_open(char *path);

t_dirent				*ls_read(DIR *dir, char *path);

/*
** CONSTANTS
** =========
**  ft_ls.c:
**   g_time, *g_app
**  parser.c:
**   g_lsflags[], g_lsflag_count
*/

extern time_t			g_time;
extern const char		*g_app;
extern const t_lsflag	g_lsflags[];
extern const int		g_lsflag_count;
#endif
