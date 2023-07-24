/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   'fdf.h'                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: lbaron  <lbaron@student.42berlin.de>       :+:  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023-07-18 15:43:02 by lbaron            :+:    #+#             */
/*   Updated: 2023-07-18 15:43:02 by lbaron           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef FDF_H
# define FDF_H

# include <stdio.h>
# include <stdlib.h>
# include <stdbool.h>
# include <unistd.h>
# include <fcntl.h>
# include <math.h>
# include "MLX42/include/MLX42/MLX42.h"
# include "libft/libft.h"

# define WIDTH 1024
# define HEIGHT 812

typedef struct s_main
{
	int32_t			row;
	int32_t			col;
	char			*buf;
	int				**matrix;
	float			cosn;
	float			sino;
	mlx_t			*mlx;
	long			color;
	int32_t			start_x;
	int32_t			start_y;
	int32_t			iso_flag;
	int32_t			color_flag;
}t_main;

typedef struct s_data
{
	int		char_h_size;
	int		int_h_size;
	int		v_size;
	int		**map_ints;
	char	**map_chars;
}t_data;

typedef struct s_strm
{
	int		i;
	int		j;
	char	**split;
	char	**split2;
	int		**matrix;
}t_strm;

typedef struct s_chcl
{
	float	value;
	int32_t	new_color;
}t_chcl;

typedef struct s_chcl2
{
	float		value;
	float		ratio;
	uint32_t	color_min;
	uint32_t	color_max;
	uint8_t		r_min;
	uint8_t		g_min;
	uint8_t		b_min;
	uint8_t		a_min;
	uint8_t		r_max;
	uint8_t		g_max;
	uint8_t		b_max;
	uint8_t		a_max;
	uint8_t		r_new;
	uint8_t		g_new;
	uint8_t		b_new;
	uint8_t		a_new;
}t_chcl2;

//main.c

void	ft_put_pixel(int32_t x, int32_t y, long color);
void	ft_put_3d_matrix(void *param);
void	ft_put_2d_matrix(void *param);

//fdf_utils.c

void	print_split(char **split);
void	print_matrix(int **matrix, int *rw, int *cl);
void	free_matrix(int **matrix, int *rw);
void	free_split(char **split);

//fdf_utils2.c

int		**str_to_matrix(char *buf, int *rw, int *cl);
char	*retrieve_buf(const char *arg1);

//draw.c

void	ft_put_line(int32_t x0, int32_t y0, int32_t x1, int32_t y1, long color);

//fdf_utils3.c

void	ft_randomize(void *param);
void	ft_my_commands(void *param);
void	set_initial_coordinates(t_main *v);
void	variables_initialising(t_main *v);

//matrix.c

void	set_offset(int32_t *offset, t_main *v);
void	ft_put_3d_matrix(void *param);
void	ft_put_2d_matrix(void *param);

//color.c

void	change_color(t_main *v, int32_t rs, int32_t cs);
void	change_color2(t_main *v, int32_t rs, int32_t cs);

#endif //FDF_H
