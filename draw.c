/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   'draw.c'                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: lbaron  <lbaron@student.42berlin.de>       :+:  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023-07-24 21:44:57 by lbaron            :+:    #+#             */
/*   Updated: 2023-07-24 21:44:57 by lbaron           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "fdf.h"

typedef struct s_putli
{
	int32_t	dx;
	int32_t	dy;
	int32_t	sx;
	int32_t	sy;
	int32_t	err;
	int32_t	e2;
}t_putli;

static void	put_line_loop(t_putli *v, int32_t *x0, int32_t *y0)
{
	v->e2 = 2 * v->err;
	if (v->e2 >= v->dy)
	{
		v->err += v->dy;
		*x0 += v->sx;
	}
	if (v->e2 <= v->dx)
	{
		v->err += v->dx;
		*y0 += v->sy;
	}
}

void	ft_put_line(int32_t x0, int32_t y0, int32_t x1, int32_t y1, long color)
{
	t_putli	v;

	v.dx = abs(x1 - x0);
	v.dy = -abs(y1 - y0);
	if (x0 < x1)
		v.sx = 1;
	else
		v.sx = -1;
	if (y0 < y1)
		v.sy = 1;
	else
		v.sy = -1;
	v.err = v.dx + v.dy;
	while (1)
	{
		if (y0 >= 0 && y0 < HEIGHT && x0 >= 0 && x0 < WIDTH)
			ft_put_pixel(x0, y0, color);
		if (x0 == x1 && y0 == y1)
			break ;
		put_line_loop(&v, &x0, &y0);
	}
}
