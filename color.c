/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   'color.c'                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: lbaron  <lbaron@student.42berlin.de>       :+:  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023-07-24 22:52:55 by lbaron            :+:    #+#             */
/*   Updated: 2023-07-24 22:52:55 by lbaron           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "fdf.h"

void	change_color(t_main *v, int32_t rs, int32_t cs)
{
	t_chcl	n;

	n.value = v->matrix[rs][cs];
	if (n.value <= -25)
	{
		v->color = 0xFF0000FF;
	}
	else if (n.value <= 0)
	{
		n.new_color = (int32_t)(((n.value + 25) / 25.0) * 0xFF);
		v->color = (0xFF << 24) | (0xFF << 16) | (n.new_color << 8) | 0xFF;
	}
	else if (n.value <= 25)
	{
		n.new_color = 0xFF - (int32_t)(((n.value) / 25.0) * 0xFF);
		v->color = (0xFF << 24) | (n.new_color << 16)
			| (n.new_color << 8) | 0xFF;
	}
	else
	{
		v->color = 0x0000FFFF;
	}
}

void	change_color2(t_main *v, int32_t rs, int32_t cs)
{
	t_chcl2	n;

	n.value = v->matrix[rs][cs];
	n.ratio = (n.value + 25) / 50.0;
	n.color_min = 0x0000FFFF;
	n.color_max = 0xFF0000FF;
	n.r_min = (n.color_min >> 24) & 0xFF;
	n.g_min = (n.color_min >> 16) & 0xFF;
	n.b_min = (n.color_min >> 8) & 0xFF;
	n.a_min = n.color_min & 0xFF;
	n.r_max = (n.color_max >> 24) & 0xFF;
	n.g_max = (n.color_max >> 16) & 0xFF;
	n.b_max = (n.color_max >> 8) & 0xFF;
	n.a_max = n.color_max & 0xFF;
	n.r_new = n.r_min + n.ratio * (n.r_max - n.r_min);
	n.g_new = n.g_min + n.ratio * (n.g_max - n.g_min);
	n.b_new = n.b_min + n.ratio * (n.b_max - n.b_min);
	n.a_new = n.a_min + n.ratio * (n.a_max - n.a_min);
	v->color = (n.r_new << 24) | (n.g_new << 16) | (n.b_new << 8) | n.a_new;
}
