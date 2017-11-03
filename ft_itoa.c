/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_itoa.c                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: sgardner <stephenbgardner@gmail.com>       +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2016/12/20 07:50:03 by sgardner          #+#    #+#             */
/*   Updated: 2017/09/23 19:51:34 by sgardner         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "libft.h"

/*
** ASSIGNMENT:
** Allocate (with malloc(3)) and returns a "fresh" string
**  ending with '\0' representing the integer n given as
**  argument.
** Negative numbers must be supported.
** If the allocation fails, the function returns NULL.
*/

char	*ft_itoa(int n)
{
	char	num[12];
	int		sign;
	int		digit;

	num[11] = '\0';
	sign = (n < 0) ? -1 : 1;
	digit = 10;
	if (n == 0)
		num[digit--] = '0';
	while (n != 0)
	{
		num[digit--] = (char)(((n % 10) * sign) + '0');
		n = (n - (n % 10)) / 10;
	}
	if (sign < 0)
		num[digit--] = '-';
	return (ft_strdup(&num[++digit]));
}
