/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_putnbr_fd.c                                     :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: aulopez <marvin@42.fr>                     +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2018/11/19 09:22:20 by aulopez           #+#    #+#             */
/*   Updated: 2019/04/12 13:19:16 by aulopez          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include <unistd.h>

static int	puissance10(int pow)
{
	int	i;

	i = 1;
	pow++;
	while (--pow)
		i = i * 10;
	return (i);
}

void		ft_putnbr_fd(int n, int fd)
{
	size_t			len;
	int				buf;
	unsigned int	positif;
	char			c;
	int				ret;

	len = 0;
	buf = n;
	while ((buf /= 10))
		++len;
	if (n < 0)
		ret = write(fd, "-", 1);
	positif = n * (1 - 2 * (n < 0));
	buf = puissance10(len);
	len++;
	while (len--)
	{
		c = positif / buf + '0';
		ret = write(fd, &c, 1);
		positif %= buf;
		buf /= 10;
	}
	(void)ret;
}
