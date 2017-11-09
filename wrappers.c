/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   wrappers.c                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: sgardner <stephenbgardner@gmail.com>       +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2017/10/27 01:17:00 by sgardner          #+#    #+#             */
/*   Updated: 2017/11/08 19:57:50 by sgardner         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "ft_ls.h"

int				ls_close(DIR *dir, char *path)
{
	if (closedir(dir) < 0)
	{
		ls_error(path);
		return (-1);
	}
	return (0);
}

DIR				*ls_open(char *path)
{
	DIR	*dir;

	if (!(dir = opendir(path)))
		ls_error(path);
	return (dir);
}

struct dirent	*ls_read(DIR *dir, char *path)
{
	struct dirent	*dp;

	errno = 0;
	dp = readdir(dir);
	if (errno)
		ls_error(path);
	return (dp);
}
