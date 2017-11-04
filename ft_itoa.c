/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_itoa.c                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: sgardner <stephenbgardner@gmail.com>       +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2016/12/20 07:50:03 by sgardner          #+#    #+#             */
/*   Updated: 2017/11/03 20:11:12 by sgardner         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include <stdlib.h>
#include "libft.h"

/*
** ASSIGNMENT:
** Allocate (with malloc(3)) and returns a "fresh" string
**  ending with '\0' representing the integer n given as
**  argument.
** Negative numbers must be supported.
** If the allocation fails, the function returns NULL.
*/

char	*ft_itoa(intmax_t n)
{
	char		*num;
	int			sign;
	int			len;
	intmax_t	tmp;

	sign = (n < 0) ? -1 : 1;
	len = (n == 0) + (sign < 0);
	tmp = n;
	while (tmp != 0 && ++len)
		tmp /= 10;
	if (!(num = (char *)malloc(len + 1)))
		return (NULL);
	num += len;
	*num-- = '\0';
	if (n == 0)
		*num-- = '0';
	while (n != 0)
	{
		*num-- = (char)(((n % 10) * sign) + '0');
		n /= 10;
	}
	if (sign < 0)
		*num-- = '-';
	return (++num);
}
