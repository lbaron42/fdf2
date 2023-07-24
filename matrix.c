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
	if(v->row < 10 && v->col < 10)
		*offset = (int32_t)40;
	else if(v->row < 20 && v->col < 20)
		*offset = (int32_t)25;
	else if(v->row < 50 && v->col < 50)
		*offset = (int32_t)12;
	else if(v->row < 100 && v->col < 100)
		*offset = (int32_t)11;
	else if(v->row < 250 && v->col < 250)
		*offset = (int32_t)4;
}

void	ft_put_3d_matrix(void *param)
{
	t_main *v = (t_main *)param;
	int32_t offset;
	int32_t rs = 0;
	int32_t cs;
	int32_t isoX1, isoY1, isoX2, isoY2;
	int32_t z1, z2, z2_vertical;

	set_offset(&offset, v);
	while (rs < v->row)
	{
		cs = 0;
		while (cs < v->col)
		{
			z1 = v->matrix[rs][cs];
			if(v->color_flag == 0)
				change_color(v, rs, cs);
			else
				change_color2(v, rs, cs);
			isoX1 = (v->start_x + cs * offset - v->start_y - rs * offset) * cos(v->cosn);
			isoY1 = -z1 + (v->start_x + cs * offset + v->start_y + rs * offset) * sin(v->sino);
			if (cs < v->col - 1)
			{
				z2 = v->matrix[rs][cs + 1];
				isoX2 = (v->start_x + (cs + 1) * offset - v->start_y - rs * offset) * cos(v->cosn);
				isoY2 = -z2 + (v->start_x + (cs + 1) * offset + v->start_y + rs * offset) * sin(v->sino);
				ft_put_line(isoX1, isoY1, isoX2, isoY2, v->color);
			}
			if (rs < v->row - 1)
			{
				z2_vertical = v->matrix[rs + 1][cs];
				isoX2 = (v->start_x + cs * offset - v->start_y - (rs + 1) * offset) * cos(v->cosn);
				isoY2 = -z2_vertical + (v->start_x + cs * offset + v->start_y + (rs + 1) * offset) * sin(v->sino);
				ft_put_line(isoX1, isoY1, isoX2, isoY2, v->color);
			}

			cs++;
		}
		rs++;
	}
}

void	ft_put_2d_matrix(void *param)
{
	t_main *v = (t_main *)param;
	int32_t offset;
	int32_t rs = 0;
	int32_t cs;
	int32_t startX = 150;
	int32_t startY = 150;
	int32_t X1, Y1, X2, Y2;

	set_offset(&offset, v);
	while (rs < v->row)
	{
		cs = 0;
		while (cs < v->col)
		{
			if(v->color_flag == 0)
				change_color(v, rs, cs);
			else
				change_color2(v, rs, cs);
			X1 = startX + cs * offset;
			Y1 = startY + rs * offset;
			if (cs < v->col - 1)
			{
				X2 = startX + (cs + 1) * offset;
				Y2 = Y1;
				ft_put_line(X1, Y1, X2, Y2, v->color);
			}
			if (rs < v->row - 1)
			{
				X2 = X1;
				Y2 = startY + (rs + 1) * offset;
				ft_put_line(X1, Y1, X2, Y2, v->color);
			}
			cs++;
		}
		rs++;
	}
}
