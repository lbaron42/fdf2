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
		*offset = (int32_t)25;
	else if(v->row < 200 && v->col < 200)
		*offset = (int32_t)5;
	else if(v->row < 250 && v->col < 250)
		*offset = (int32_t)5;
}


void ft_put_2d_matrix(long color, void *param)
{
	t_main *v = (t_main *)param;
	int32_t offset;
	int32_t rs = 0;
	int32_t cs;
	int32_t startX = 500;
	int32_t startY = 150;
	int32_t isoX1, isoY1, isoX2, isoY2;
	int32_t z1, z2;

	set_offset(&offset, v);
	while (rs < v->row - 1)
	{
		cs = 0;
		while (cs < v->col - 1)
		{
			z1 = v->matrix[rs][cs];
			z2 = v->matrix[rs][cs + 1];
			isoX1 = (startX + cs * offset - startY - rs * offset) * cos(0.523599);
			isoY1 = -z1 + (startX + cs * offset + startY + rs * offset) * sin(0.523599);
			isoX2 = (startX + (cs + 1) * offset - startY - rs * offset) * cos(0.523599);
			isoY2 = -z2 + (startX + (cs + 1) * offset + startY + rs * offset) * sin(0.523599);
			ft_put_line(isoX1, isoY1, isoX2, isoY2, color);

			z2 = v->matrix[rs + 1][cs];
			isoX2 = (startX + cs * offset - startY - (rs + 1) * offset) * cos(0.523599);
			isoY2 = -z2 + (startX + cs * offset + startY + (rs + 1) * offset) * sin(0.523599);
			ft_put_line(isoX1, isoY1, isoX2, isoY2, color);

			cs++;
		}
		z2 = v->matrix[rs + 1][cs];
		isoX1 = (startX + cs * offset - startY - rs * offset) * cos(0.523599);
		isoY1 = -z1 + (startX + cs * offset + startY + rs * offset) * sin(0.523599);
		isoX2 = (startX + cs * offset - startY - (rs + 1) * offset) * cos(0.523599);
		isoY2 = -z2 + (startX + cs * offset + startY + (rs + 1) * offset) * sin(0.523599);
		ft_put_line(isoX1, isoY1, isoX2, isoY2, color);
		rs++;
	}

	cs = 0;
	while (cs < v->col - 1)
	{
		z1 = v->matrix[rs][cs];
		z2 = v->matrix[rs][cs + 1];
		isoX1 = (startX + cs * offset - startY - rs * offset) * cos(0.523599);
		isoY1 = -z1 + (startX + cs * offset + startY + rs * offset) * sin(0.523599);
		isoX2 = (startX + (cs + 1) * offset - startY - rs * offset) * cos(0.523599);
		isoY2 = -z2 + (startX + (cs + 1) * offset + startY + rs * offset) * sin(0.523599);
		ft_put_line(isoX1, isoY1, isoX2, isoY2, color);
		cs++;
	}
}

void	ft_randomize(void *param)
{
	t_main *v = param;
	long color = 0xEDEADEFF;
	ft_put_2d_matrix(color, param);
}

void	ft_hook(void *param)
{
	mlx_t	*mlx;

	mlx = param;
	if (mlx_is_key_down(mlx, MLX_KEY_ESCAPE))
		mlx_close_window(mlx);
	if (mlx_is_key_down(mlx, MLX_KEY_UP))
		image->instances[0].y -= 5;
	if (mlx_is_key_down(mlx, MLX_KEY_DOWN))
		image->instances[0].y += 5;
	if (mlx_is_key_down(mlx, MLX_KEY_LEFT))
		image->instances[0].x -= 5;
	if (mlx_is_key_down(mlx, MLX_KEY_RIGHT))
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
	mlx_loop_hook(*mlx, ft_hook, *mlx);
	return (EXIT_SUCCESS);
}

int32_t	main(int32_t argc, const char *argv[])
{
	mlx_t	*mlx;
	t_main	v;

	if (argc != 2)
	{
		write(2, "error: you need a valid map name\n", 33);
		return (0);
	}
	v.row = 0;
	v.col = 0;
	v.buf = retrieve_buf(argv[1]);
	v.matrix = str_to_matrix(v.buf, &v.row, &v.col);
	//print_matrix(v.matrix, &v.row, &v.col);
	if (init_mlx(&mlx, &v) != EXIT_SUCCESS)
	{
		return (EXIT_FAILURE);
	}
	mlx_loop(mlx);
	mlx_terminate(mlx);
	free_matrix(v.matrix, &v.row);
	free(v.buf);
	return (EXIT_SUCCESS);
}
