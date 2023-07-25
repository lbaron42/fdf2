/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   'matix.c'                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: lbaron  <lbaron@student.42berlin.de>       :+:  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023-07-24 22:49:41 by lbaron            :+:    #+#             */
/*   Updated: 2023-07-24 22:49:41 by lbaron           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "fdf.h"

void	set_offset(int32_t *offset, t_main *v)
{
	if (v->row < 10 && v->col < 10)
		*offset = (int32_t)40;
	else if (v->row < 20 && v->col < 20)
		*offset = (int32_t)25;
	else if (v->row < 50 && v->col < 50)
		*offset = (int32_t)12;
	else if (v->row < 100 && v->col < 100)
		*offset = (int32_t)11;
	else if (v->row < 250 && v->col < 250)
		*offset = (int32_t)4;
}

typedef struct s_3d
{
	int32_t	offset;
	int32_t	rs;
	int32_t	cs;
	int32_t	x1;
	int32_t	y1;
	int32_t	x2;
	int32_t	y2;
	int32_t	z1;
	int32_t	z2;
	int32_t	z2_vertical;
}t_3d;

static void	ft_3d_loop(t_3d *n, t_main *v, t_norm *c)
{
	n->x1 = (v->start_x + n->cs * n->offset
			- v->start_y - n->rs * n->offset) * cos(v->cosn);
	n->y1 = -n->z1 + (v->start_x + n->cs * n->offset
			+ v->start_y + n->rs * n->offset) * sin(v->sino);
	if (n->cs < v->col - 1)
	{
		n->z2 = v->matrix[n->rs][n->cs + 1];
		n->x2 = (v->start_x + (n->cs + 1) * n->offset
				- v->start_y - n->rs * n->offset) * cos(v->cosn);
		c->y2 = -n->z2 + (v->start_x + (n->cs + 1) * n->offset
				+ v->start_y + n->rs * n->offset) * sin(v->sino);
		c->color = v->color;
		ft_put_line(n->x1, n->y1, n->x2, c);
	}
	if (n->rs < v->row - 1)
	{
		n->z2_vertical = v->matrix[n->rs + 1][n->cs];
		n->x2 = (v->start_x + n->cs * n->offset
				- v->start_y - (n->rs + 1) * n->offset) * cos(v->cosn);
		c->y2 = -n->z2_vertical + (v->start_x + n->cs * n->offset
				+ v->start_y + (n->rs + 1) * n->offset) * sin(v->sino);
		c->color = v->color;
		ft_put_line(n->x1, n->y1, n->x2, c);
	}
}

void	ft_put_3d_matrix(void *param)
{
	t_main	*v;
	t_3d	n;
	t_norm	c;

	v = (t_main *)param;
	n.rs = 0;
	set_offset(&n.offset, v);
	while (n.rs < v->row)
	{
		n.cs = 0;
		while (n.cs < v->col)
		{
			n.z1 = v->matrix[n.rs][n.cs];
			if (v->color_flag == 0)
				change_color(v, n.rs, n.cs);
			else
				change_color2(v, n.rs, n.cs);
			ft_3d_loop(&n, v, &c);
			n.cs++;
		}
		n.rs++;
	}
}

typedef struct s_2d
{
	int32_t	offset;
	int32_t	rs;
	int32_t	cs;
	int32_t	srx;
	int32_t	sry;
	int32_t	x1;
	int32_t	y1;
	int32_t	x2;
	int32_t	y2;
}t_2d;

static void	ft_2d_loop(t_2d *n, t_main *v, t_norm *c)
{
	if (v->color_flag == 0)
		change_color(v, n->rs, n->cs);
	else
		change_color2(v, n->rs, n->cs);
	n->x1 = n->srx + n->cs * n->offset;
	n->y1 = n->sry + n->rs * n->offset;
	if (n->cs < v->col - 1)
	{
		n->x2 = n->srx + (n->cs + 1) * n->offset;
		c->y2 = n->y1;
		c->color = v->color;
		ft_put_line(n->x1, n->y1, n->x2, c);
	}
	if (n->rs < v->row - 1)
	{
		n->x2 = n->x1;
		c->y2 = n->sry + (n->rs + 1) * n->offset;
		c->color = v->color;
		ft_put_line(n->x1, n->y1, n->x2, c);
	}
}

void	ft_put_2d_matrix(void *param)
{
	t_main	*v;
	t_2d	n;
	t_norm	c;

	v = (t_main *)param;
	n.rs = 0;
	n.srx = 150;
	n.sry = 150;
	set_offset(&n.offset, v);
	while (n.rs < v->row)
	{
		n.cs = 0;
		while (n.cs < v->col)
		{
			ft_2d_loop(&n, v, &c);
			n.cs++;
		}
		n.rs++;
	}
}
