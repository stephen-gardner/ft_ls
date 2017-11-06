/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_strrchr.c                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: sgardner <stephenbgardner@gmail.com>       +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2016/12/16 01:23:17 by sgardner          #+#    #+#             */
/*   Updated: 2016/12/22 18:23:01 by sgardner         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "libft.h"

/*
** ASSIGNMENT:
** Re-code similarly named libc function.
*/

char	*ft_strrchr(const char *s, int c)
{
	char	*res;
	char	b;

	if (!c)
		return ((char *)(s + ft_strlen(s)));
	res = NULL;
	b = (char)c;
	while (*s)
	{
		if (*s == b)
			res = (char *)s;
		s++;
	}
	return (res);
}
