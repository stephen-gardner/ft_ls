/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_memmove.c                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: sgardner <stephenbgardner@gmail.com>       +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2016/12/16 01:15:21 by sgardner          #+#    #+#             */
/*   Updated: 2017/09/19 21:16:13 by sgardner         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "libft.h"

/*
** ASSIGNMENT:
** Re-code similarly named libc function.
** This function lacks optimizations such as word copying and
**  vectorization.
*/

void	*ft_memmove(void *s1, const void *s2, size_t n)
{
	const t_byte	*src;
	t_byte			*dst;

	if (s1 == s2)
		return (s1);
	if (s1 < s2)
		return (ft_memcpy(s1, s2, n));
	src = (const t_byte *)s2;
	dst = (t_byte *)s1;
	while (n--)
		dst[n] = src[n];
	return (s1);
}
