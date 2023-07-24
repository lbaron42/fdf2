/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   'main.c'                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: lbaron  <lbaron@student.42berlin.de>       :+:  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023-07-18 15:24:19 by lbaron            :+:    #+#             */
/*   Updated: 2023-07-18 15:24:19 by lbaron           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "fdf.h"

#define WIDTH 1024
#define HEIGHT 812

static	mlx_image_t	*image;


// -----------------------------------------------------------------------------
int32_t	ft_pixel(int32_t r, int32_t g, int32_t b, int32_t a)
{
	return (r << 24 | g << 16 | b << 8 | a);
}

void	ft_put_pixel(int32_t x, int32_t y, long color)
{
	mlx_put_pixel(image, x, y, color);
}

void ft_put_line(int32_t x0, int32_t y0, int32_t x1, int32_t y1, long color)
{
	int32_t dx = abs(x1 - x0);
	int32_t dy = -abs(y1 - y0);
	int32_t sx, sy;

	if (x0 < x1) {
		sx = 1;
	} else {
		sx = -1;
	}
	if (y0 < y1) {
		sy = 1;
	} else {
		sy = -1;
	}
	int32_t err = dx + dy, e2;
	while(1)
	{
		if (y0 >= 0 && y0 < HEIGHT && x0 >= 0 && x0 < WIDTH)
			ft_put_pixel(x0, y0, color);
		if (x0 == x1 && y0 == y1)
			break;
		e2 = 2 * err;
		if (e2 >= dy)
		{
			err += dy;
			x0 += sx;
		}
		if (e2 <= dx)
		{
			err += dx;
			y0 += sy;
		}
	}
}

void set_offset(int32_t *offset, t_main *v)
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

void erase_2d_matrix(t_main *v)
{
	int32_t offset;
	set_offset(&offset, v);
	int32_t max_x = 150 + (v->col * offset);
	int32_t max_y = 150 + (v->row * offset);

	for(int y = 150; y <= max_y; y++)
	{
		for(int x = 150; x <= max_x; x++)
		{
			mlx_put_pixel(image, x, y, 0x000000FF);
		}
	}
}

void erase_3d_matrix(t_main *v)
{
	int32_t offset;
	set_offset(&offset, v);
	int32_t max_x = v->startX + ((v->col > v->row ? v->col : v->row) * offset);
	int32_t max_y = v->startY + ((v->col > v->row ? v->col : v->row) * offset);

	for(int y = v->startY; y <= max_y; y++)
	{
		for(int x = v->startX; x <= max_x; x++)
		{
			mlx_put_pixel(image, x, y, 0x000000FF);
		}
	}
}

void change_color(t_main *v, int32_t rs, int32_t cs)
{
	float value = v->matrix[rs][cs];

	if (value <= -25) {
		v->color = 0xFF0000FF;
	}
	else if (value <= 0) {
		int32_t new_color = (int32_t)(((value + 25) / 25.0) * 0xFF);
		v->color = (0xFF << 24) | (0xFF << 16) | (new_color << 8) | 0xFF;
	}
	else if (value <= 25) {
		int32_t new_color = 0xFF - (int32_t)(((value) / 25.0) * 0xFF);
		v->color = (0xFF << 24) | (new_color << 16) | (new_color << 8) | 0xFF;
	}
	else {
		v->color = 0x0000FFFF;
	}
}

void change_color2(t_main *v, int32_t rs, int32_t cs)
{
	float value = v->matrix[rs][cs];
	float ratio = (value + 25) / 50.0;

	uint32_t color_min = 0x0000FFFF;
	uint32_t color_max = 0xFF0000FF;
	uint8_t r_min = (color_min >> 24) & 0xFF;
	uint8_t g_min = (color_min >> 16) & 0xFF;
	uint8_t b_min = (color_min >> 8) & 0xFF;
	uint8_t a_min = color_min & 0xFF;
	uint8_t r_max = (color_max >> 24) & 0xFF;
	uint8_t g_max = (color_max >> 16) & 0xFF;
	uint8_t b_max = (color_max >> 8) & 0xFF;
	uint8_t a_max = color_max & 0xFF;
	uint8_t r_new = r_min + ratio * (r_max - r_min);
	uint8_t g_new = g_min + ratio * (g_max - g_min);
	uint8_t b_new = b_min + ratio * (b_max - b_min);
	uint8_t a_new = a_min + ratio * (a_max - a_min);
	v->color = (r_new << 24) | (g_new << 16) | (b_new << 8) | a_new;
}

void ft_put_3d_matrix(void *param)
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
			isoX1 = (v->startX + cs * offset - v->startY - rs * offset) * cos(v->cosn);
			isoY1 = -z1 + (v->startX + cs * offset + v->startY + rs * offset) * sin(v->sino);
			if (cs < v->col - 1)
			{
				z2 = v->matrix[rs][cs + 1];
				isoX2 = (v->startX + (cs + 1) * offset - v->startY - rs * offset) * cos(v->cosn);
				isoY2 = -z2 + (v->startX + (cs + 1) * offset + v->startY + rs * offset) * sin(v->sino);
				ft_put_line(isoX1, isoY1, isoX2, isoY2, v->color);
			}
			if (rs < v->row - 1)
			{
				z2_vertical = v->matrix[rs + 1][cs];
				isoX2 = (v->startX + cs * offset - v->startY - (rs + 1) * offset) * cos(v->cosn);
				isoY2 = -z2_vertical + (v->startX + cs * offset + v->startY + (rs + 1) * offset) * sin(v->sino);
				ft_put_line(isoX1, isoY1, isoX2, isoY2, v->color);
			}

			cs++;
		}
		rs++;
	}
}

void ft_put_2d_matrix(void *param)
{
	t_main *v = (t_main *)param;
	int32_t offset;
	int32_t rs = 0;
	int32_t cs;
	int32_t startX = 150;
	int32_t startY = 150;
	int32_t X1, Y1, X2, Y2;

	erase_3d_matrix(v);
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

void	ft_randomize(void *param)
{
	t_main *v = (t_main *)param;

	mlx_put_string(v->mlx,"FDF:",10,10);
	mlx_put_string(v->mlx,"Arrow keys to move <- ->:",10,24);
	mlx_put_string(v->mlx,"Press C and G to change colors:",10,38);
	if(v->iso_flag == 0)
		ft_put_2d_matrix(param);
	else
		ft_put_3d_matrix(param);
}

void ft_my_commands(void *param)
{
	t_main *v = (t_main *)param;

	if(mlx_is_key_down(v->mlx, MLX_KEY_W))
		v->cosn += 0.1;
	if(mlx_is_key_down(v->mlx, MLX_KEY_S))
		v->cosn -= 0.1;
	if(mlx_is_key_down(v->mlx, MLX_KEY_D))
		v->sino += 0.1;
	if(mlx_is_key_down(v->mlx, MLX_KEY_A))
		v->sino -= 0.1;
	if(mlx_is_key_down(v->mlx, MLX_KEY_I))
		v->iso_flag = 1;
	if(mlx_is_key_down(v->mlx, MLX_KEY_O))
		v->iso_flag = 0;
	if(mlx_is_key_down(v->mlx, MLX_KEY_C))
		v->color_flag = 1;
	if(mlx_is_key_down(v->mlx, MLX_KEY_G))
		v->color_flag = 0;
}

void	ft_hook(void *param)
{
	t_main *v = (t_main *)param;

	if (mlx_is_key_down(v->mlx, MLX_KEY_ESCAPE))
		mlx_close_window(v->mlx);
	if (mlx_is_key_down(v->mlx, MLX_KEY_UP))
		image->instances[0].y -= 5;
	if (mlx_is_key_down(v->mlx, MLX_KEY_DOWN))
		image->instances[0].y += 5;
	if (mlx_is_key_down(v->mlx, MLX_KEY_LEFT))
		image->instances[0].x -= 5;
	if (mlx_is_key_down(v->mlx, MLX_KEY_RIGHT))
		image->instances[0].x += 5;
}

//test

int32_t	init_mlx(mlx_t **mlx, t_main *v)
{
	if (!(*mlx = mlx_init(WIDTH, HEIGHT, "MLX42", true)))
	{
		puts(mlx_strerror(mlx_errno));
		return (EXIT_FAILURE);
	}
	if (!(image = mlx_new_image(*mlx, 1024, 812)))
	{
		mlx_close_window(*mlx);
		puts(mlx_strerror(mlx_errno));
		return (EXIT_FAILURE);
	}
	if (mlx_image_to_window(*mlx, image, 0, 0) == -1)
	{
		mlx_close_window(*mlx);
		puts(mlx_strerror(mlx_errno));
		return (EXIT_FAILURE);
	}
	mlx_loop_hook(*mlx, ft_randomize, v);
	mlx_loop_hook(*mlx, ft_my_commands, v);
	mlx_loop_hook(*mlx, ft_hook, v);
	return (EXIT_SUCCESS);
}

void set_initial_coordinates(t_main *v)
{
	if(v->row < 10 && v->col < 10)
	{
		v->startX = 550;
		v->startY = 50;
	}
	else if(v->row < 50 && v->col < 50)
	{
		v->startX = 300;
		v->startY = -300;
	}
	else if(v->row < 100 && v->col < 100)
	{
		v->startX = 550;
		v->startY = 50;
	}
	else if(v->row < 250 && v->col < 250)
	{
		v->startX = 450;
		v->startY = 50;
	}
}

void variables_initialising(t_main *v)
{
	v->row = 0;
	v->col = 0;
	v->matrix = str_to_matrix(v->buf, &v->row, &v->col);
	v->cosn = 0.523599;
	v->sino = 0.523599;
	v->color  = 0xEDEADEFF;
	v->iso_flag = 1;
	v->color_flag = 1;
	set_initial_coordinates(v);
}

int32_t	main(int32_t argc, const char *argv[])
{
	t_main	v;

	if (argc != 2)
	{
		write(2, "Name of file missing or to many arguments.\n", 43);
		return (0);
	}
	v.buf = retrieve_buf(argv[1]);
	variables_initialising(&v);
	//print_matrix(v.matrix, &v.row, &v.col);
	if (init_mlx(&v.mlx, &v) != EXIT_SUCCESS)
	{
		return (EXIT_FAILURE);
	}
	mlx_loop(v.mlx);
	mlx_terminate(v.mlx);
	free_matrix(v.matrix, &v.row);
	free(v.buf);
	return (EXIT_SUCCESS);
}
