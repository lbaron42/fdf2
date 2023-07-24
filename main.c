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

static mlx_image_t	*g_image;

void	ft_put_pixel(int32_t x, int32_t y, long color)
{
	mlx_put_pixel(g_image, x, y, color);
}

void	ft_hook(void *param)
{
	t_main	*v;

	v = (t_main *)param;
	if (mlx_is_key_down(v->mlx, MLX_KEY_ESCAPE))
		mlx_close_window(v->mlx);
	if (mlx_is_key_down(v->mlx, MLX_KEY_UP))
		g_image->instances[0].y -= 5;
	if (mlx_is_key_down(v->mlx, MLX_KEY_DOWN))
		g_image->instances[0].y += 5;
	if (mlx_is_key_down(v->mlx, MLX_KEY_LEFT))
		g_image->instances[0].x -= 5;
	if (mlx_is_key_down(v->mlx, MLX_KEY_RIGHT))
		g_image->instances[0].x += 5;
}

int32_t	init_mlx(mlx_t **mlx, t_main *v)
{
	if (!(*mlx = mlx_init(WIDTH, HEIGHT, "MLX42", true)))
	{
		puts(mlx_strerror(mlx_errno));
		return (EXIT_FAILURE);
	}
	if (!(g_image = mlx_new_image(*mlx, 1024, 812)))
	{
		mlx_close_window(*mlx);
		puts(mlx_strerror(mlx_errno));
		return (EXIT_FAILURE);
	}
	if (mlx_image_to_window(*mlx, g_image, 0, 0) == -1)
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
