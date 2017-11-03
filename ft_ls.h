/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_ls.h                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: sgardner <stephenbgardner@gmail.com>       +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2017/10/27 19:21:24 by sgardner          #+#    #+#             */
/*   Updated: 2017/11/02 22:50:29 by sgardner         ###   ########.fr       */
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
# include <time.h>
# include <unistd.h>
# include "ft_printf.h"

# define FMT(x, y) ((x & S_IFMT) == y)
# define USR(x, y) ((x & S_IRWXU) & y)
# define GRP(x, y) ((x & S_IRWXG) & y)
# define OTH(x, y) ((x & S_IRWXO) & y)

typedef struct dirent	t_dirent;
typedef struct stat		t_stat;
typedef struct			s_file
{
	char			*name;
	char			*path;
	char			*stats[7];
	int				maxlen[4];
	struct s_file	*parent;
	struct s_file	**children;
}						t_file;

typedef struct			s_lsflag
{
	char	c;
	int		flag;
}						t_lsflag;

enum	e_lsflags
{
	LS_A = 1,
	LS_L = 1 << 1,
	LS_REC = 1 << 2,
	LS_REV = 1 << 3,
	LS_MTIME = 1 << 4,
	LS_ATIME = 1 << 5,
	LS_CTIME = 1 << 6,
	LS_CT = 1 << 7
};


/*
** load.c
*/

t_file					*build_file(char *path, t_dirent *dp, int flags);
void					*free_file(t_file *file);
t_bool					load_children(t_file *file, int flags);
t_file					*load_parent(char *path, int flags);

/*
** parser.c
*/

t_bool					parse_flags(int *flags, char **argv, int *idx,
		int argc);

/*
** print.c
*/

void					print_files(t_file *file, int flags);
void					print_long(char **stats, int *maxlen);

/*
** stat.c
*/

t_bool					load_stats(t_file *file, t_stat *stats, int flags);

/*
** util.c
*/

char					*build_path(char *parent, char *child);
char					*build_link(char *link, char *target);
int						dir_len(char *path, int flags);
void					*ls_error(char *path);

/*
** wrappers.c
*/

int						ls_close(DIR *dir, char *path);
DIR						*ls_open(char *path);
t_dirent				*ls_read(DIR *dir, char *path);

extern const char		*g_app;
extern const t_lsflag	g_lsflags[];
extern const int		g_lsflag_count;
#endif
