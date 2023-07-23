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


// works by putting either vertical or horizontal lines only. Commented part of the code only prints out the diagonal line.
// Have to use https://en.wikipedia.org/wiki/Bresenham%27s_line_algorithm to print out lines in any angle


void ft_put_line(int32_t x, int32_t y, int32_t x_end, int32_t y_end, long color)
{
	float len_x;
	float len_y;
	int32_t big_size;
	float dev;
	float store_dev;
	int flag;

	len_x = x_end - x;
	len_y = y_end - y;
	if (len_x > len_y)
	{
		big_size = len_x;
		dev = len_y / len_x;
		store_dev = dev;
		dev += y;
		flag = 1;
	}
	else
	{
		big_size = len_y;
		dev = len_x / len_y;
		store_dev = dev;
		dev += x;
		flag = 0;
	}

	while (big_size--)
	{
		if (flag)
		{
			if (y >= 0 && y < HEIGHT && x >= 0 && x < WIDTH)
				ft_put_pixel(x, y, color);
			dev += store_dev;
			x++;
			y = round(dev);
		}
		else
		{
			if (y >= 0 && y < HEIGHT && x >= 0 && x < WIDTH)
				ft_put_pixel(x, y, color);
			dev += store_dev;
			x = round(dev);
			y++;
		}
	}
}

void ft_put_line_any(int32_t x, int32_t y, int32_t x_end, int32_t y_end, long color)
{
	if(x_end > x && y_end > y)
		ft_put_line(x_end, y_end, x, y, color);
	else
		ft_put_line(x, y, x_end, y_end, color);
}

void set_offset(int32_t *offset, t_main *v)
{
	if(v->row < 10 && v->col < 10)
		*offset = (int32_t)40;
	else if(v->row < 100 && v->col < 100)
		*offset = (int32_t)25;
	else if(v->row < 200 && v->col < 200)
		*offset = (int32_t)20;
	else if(v->row < 500 && v->col < 500)
		*offset = (int32_t)5;
}

void ft_put_2d_matrix(long color, void *param)
{
	t_main *v = (t_main *)param;
	int32_t offset;
	int32_t rs = 0;
	int32_t cs;
	int32_t startX = 150;
	int32_t startY = 150;

	set_offset(&offset, v);
	while (rs < v->row - 1)
	{
		cs = 0;
		while (cs < v->col - 1)
		{
			ft_put_line_any(startX + cs * offset, startY + rs * offset, startX + (cs + 1) * offset, startY + rs * offset, color);
			ft_put_line_any(startX + cs * offset, startY + rs * offset, startX + cs * offset, startY + (rs + 1) * offset, color);
			cs++;
		}
		ft_put_line_any(startX + cs * offset, startY + rs * offset, startX + cs * offset, startY + (rs + 1) * offset, color);
		rs++;
	}

	cs = 0;
	while (cs < v->col - 1)
	{
		ft_put_line_any(startX + cs * offset, startY + rs * offset, startX + (cs + 1) * offset, startY + rs * offset, color);
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
	if (!(image = mlx_new_image(*mlx, 1024, 512)))
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
