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
	int32_t dx = abs(x1 - x0), sx = x0 < x1 ? 1 : -1;
	int32_t dy = -abs(y1 - y0), sy = y0 < y1 ? 1 : -1;
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
	else if(v->row < 100 && v->col < 100)
		*offset = (int32_t)20;
	else if(v->row < 200 && v->col < 200)
		*offset = (int32_t)10;
	else if(v->row < 250 && v->col < 250)
		*offset = (int32_t)5;
}

void change_color(t_main *v, int32_t rs, int32_t cs)
{
	if(v->matrix[rs][cs] < 0)
		v->color = 0xFF0000FF;
	if(v->matrix[rs][cs] < 5)
		v->color = 0xEDEADEFF;
	if(v->matrix[rs][cs] > 5)
		v->color = 0x0000FFFF;
	if(v->matrix[rs][cs] > 15)
		v->color = 0x00FF00FF;
	if(v->matrix[rs][cs] > 25)
		v->color = 0x800000FF;

}

void ft_put_3d_matrix(void *param)
{
	t_main *v = (t_main *)param;
	int32_t offset;
	int32_t rs = 0;
	int32_t cs;
	int32_t startX = 550;
	int32_t startY = 50;
	int32_t isoX1, isoY1, isoX2, isoY2;
	int32_t z1, z2, z2_vertical;


	set_offset(&offset, v);
	while (rs < v->row)
	{
		cs = 0;
		while (cs < v->col)
		{
			z1 = v->matrix[rs][cs];
			change_color(v, rs, cs);
			isoX1 = (startX + cs * offset - startY - rs * offset) * cos(v->cosn);
			isoY1 = -z1 + (startX + cs * offset + startY + rs * offset) * sin(v->sino);
			if (cs < v->col - 1)
			{
				z2 = v->matrix[rs][cs + 1];
				isoX2 = (startX + (cs + 1) * offset - startY - rs * offset) * cos(v->cosn);
				isoY2 = -z2 + (startX + (cs + 1) * offset + startY + rs * offset) * sin(v->sino);
				ft_put_line(isoX1, isoY1, isoX2, isoY2, v->color);
			}
			if (rs < v->row - 1)
			{
				z2_vertical = v->matrix[rs + 1][cs];
				isoX2 = (startX + cs * offset - startY - (rs + 1) * offset) * cos(v->cosn);
				isoY2 = -z2_vertical + (startX + cs * offset + startY + (rs + 1) * offset) * sin(v->sino);
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
	int32_t startX = 550;
	int32_t startY = 50;
	int32_t X1, Y1, X2, Y2;

	set_offset(&offset, v);
	while (rs < v->row)
	{
		cs = 0;
		while (cs < v->col)
		{
			change_color(v, rs, cs);
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
	//ft_put_2d_matrix(param);
	ft_put_3d_matrix(param);
}

void ft_mycommands(void *param)
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
	mlx_loop_hook(*mlx, ft_mycommands, v);
	mlx_loop_hook(*mlx, ft_hook, v);
	return (EXIT_SUCCESS);
}

void variables_declaring(t_main *v)
{
	v->row = 0;
	v->col = 0;
	v->matrix = str_to_matrix(v->buf, &v->row, &v->col);
	v->cosn = 0.523599;
	v->sino = 0.523599;
	v->color  = 0xEDEADEFF;
}

int32_t	main(int32_t argc, const char *argv[])
{
	t_main	v;

	if (argc != 2)
	{
		write(2, "error: you need a valid map name\n", 33);
		return (0);
	}
	v.buf = retrieve_buf(argv[1]);
	variables_declaring(&v);
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
