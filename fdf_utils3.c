/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   'fdf_utils3.c'                                     :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: lbaron  <lbaron@student.42berlin.de>       :+:  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023-07-24 22:37:16 by lbaron            :+:    #+#             */
/*   Updated: 2023-07-24 22:37:16 by lbaron           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "fdf.h"

void	ft_randomize(void *param)
{
	t_main	*v;

	v = (t_main *)param;
	mlx_put_string(v->mlx, "FDF:", 10, 10);
	mlx_put_string(v->mlx, "Arrow keys to move <- ->:", 10, 24);
	mlx_put_string(v->mlx, "Press C and G to change colors:", 10, 38);
	if (v->iso_flag == 0)
		ft_put_2d_matrix(param);
	else
		ft_put_3d_matrix(param);
}

void	ft_my_commands(void *param)
{
	t_main	*v;

	v = (t_main *)param;
	if (mlx_is_key_down(v->mlx, MLX_KEY_W))
		v->cosn += 0.1;
	if (mlx_is_key_down(v->mlx, MLX_KEY_S))
		v->cosn -= 0.1;
	if (mlx_is_key_down(v->mlx, MLX_KEY_D))
		v->sino += 0.1;
	if (mlx_is_key_down(v->mlx, MLX_KEY_A))
		v->sino -= 0.1;
	if (mlx_is_key_down(v->mlx, MLX_KEY_I))
		v->iso_flag = 1;
	if (mlx_is_key_down(v->mlx, MLX_KEY_O))
		v->iso_flag = 0;
	if (mlx_is_key_down(v->mlx, MLX_KEY_C))
		v->color_flag = 1;
	if (mlx_is_key_down(v->mlx, MLX_KEY_G))
		v->color_flag = 0;
}

void	set_initial_coordinates(t_main *v)
{
	if (v->row <= 10 && v->col <= 10)
	{
		v->start_x = 350;
		v->start_y = 0;
	}
	else if (v->row <= 20 && v->col <= 20)
	{
		v->start_x = 300;
		v->start_y = -200;
	}
	else if (v->row < 50 && v->col < 50)
	{
		v->start_x = 300;
		v->start_y = -300;
	}
	else if (v->row <= 100 && v->col <= 100)
	{
		v->start_x = 300;
		v->start_y = -250;
	}
	else if (v->row < 250 && v->col < 250)
	{
		v->start_x = 450;
		v->start_y = -50;
	}
}

void	variables_initialising(t_main *v)
{
	v->row = 0;
	v->col = 0;
	v->matrix = str_to_matrix(v->buf, &v->row, &v->col);
	v->cosn = 0.623599;
	v->sino = 0.623599;
	v->color = 0xEDEADEFF;
	v->iso_flag = 1;
	v->color_flag = 1;
	set_initial_coordinates(v);
}
