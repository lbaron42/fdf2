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

static void	put_line_loop(t_putli *v, int32_t *x1, int32_t *y1)
{
	v->e2 = 2 * v->err;
	if (v->e2 >= v->dy)
	{
		v->err += v->dy;
		*x1 += v->sx;
	}
	if (v->e2 <= v->dx)
	{
		v->err += v->dx;
		*y1 += v->sy;
	}
}

void	ft_put_line(int32_t x1, int32_t y1, int32_t x2, t_norm *c)
{
	t_putli	v;

	v.dx = abs(x2 - x1);
	v.dy = -abs(c->y2 - y1);
	if (x1 < x2)
		v.sx = 1;
	else
		v.sx = -1;
	if (y1 < c->y2)
		v.sy = 1;
	else
		v.sy = -1;
	v.err = v.dx + v.dy;
	while (1)
	{
		if (y1 >= 0 && y1 < HEIGHT && x2 >= 0 && x2 < WIDTH)
			ft_put_pixel(x1, y1, c->color);
		if (x1 == x2 && y1 == c->y2)
			break ;
		put_line_loop(&v, &x1, &y1);
	}
}
