#include <stdio.h>
#include <unistd.h>
#include <stdlib.h>
#include <math.h>

typedef struct s_border
{
	int width;
	int height;
	char color;
} t_border;

typedef struct s_circle
{
	char type;
	float x;
	float y;
	float radius;
	char color;
} t_circle;

int ft_strlen(char *str)
{
	int i = 0;

	while (str[i] != 0)
		i++;
	return (i);
}

void ft_putstr(char *str)
{
	write(1, str, ft_strlen(str));
}

int get_border(FILE *file, t_border *border)
{
	int scan_res = fscanf(file, "%d %d %c\n", &border->width, &border->height,
					  &border->color);
	if (scan_res != 3)
		return (0);
	if (border->width < 0 || border->width >= 300 || border->height < 0 ||
	border->height >= 300)
		return (0);
	return (1);
}

char *get_border_to_planshet(t_border *border)
{
	int len = border->width * border->height;
	char *planshet = malloc(sizeof(char) * (len + 1));
	if (planshet == NULL)
		return (NULL);
	int i = 0;
	while (i < len)
	{
		planshet[i] = border->color;
		i++;
	}
	planshet[i] = 0;
	return (planshet);
}

int in_circle(float x, float y, t_circle *circle)
{
	float dst = sqrtf(powf(x - circle->x, 2) + powf(y - circle->y, 2));
	//out circle
	if (dst > circle->radius)
		return (0);
	//in circle
	if (circle->radius - dst < 1)
		return (1);
	return (2);
}

void draw_circle_to_planshet(t_circle *circle, t_border *border, char *planshet)
{
	int x;
	int y = 0;
	while (y < border->height)
	{
		x = 0;
		while (x < border->width)
		{
			if (in_circle(x, y, circle) != 0)
				planshet[y * border->width + x] = circle->color;
			x++;
		}
		y++;
	}
}

int	is_correct_figure(t_circle *circle)
{
	if (!(circle->type == 'C' && circle->radius > 0))
		return (0);
	return (1);
}

int get_figure_to_planshet(FILE *file, t_border *border, char *planshet)
{
	t_circle circle;
	int fscanfres = fscanf(file, "%c %f %f %f %c\n", &circle.type, &circle.x,
		&circle.y, &circle.radius, &circle.color);
	while (fscanfres == 5)
	{
		if (!is_correct_figure(&circle))
			return (0);
		draw_circle_to_planshet(&circle, border, planshet);
		fscanfres = fscanf(file, "%c %f %f %f %c\n", &circle.type, &circle.x,
							   &circle.y, &circle.radius, &circle.color);
	}
	if (fscanfres == -1)
		return (1);
	return (0);
}

void print_planshet_to_terminal(t_border *border, char *planshet)
{
	int i = 0;
	while (i < border->width * border->height)
	{
		write(1, &(planshet[i]), 1);
		if ((i + 1) % border->width == 0)
			write(1, "\n", 1);
		i++;
	}
}

int main(int argc, char **argv)
{
	FILE *file;
	t_border border;
	char *planshet;


	if (argc != 2)
	{
		ft_putstr("Error: argument\n");
		return (1);
	}
	file = fopen(argv[1], "r");
	if (file == NULL)
	{
		ft_putstr("Error: Operation file corrupted\n");
		return (1);
	}
	if (!get_border(file, &border))
	{
		ft_putstr("Operation file corrupted\n");
		fclose(file);
		return (1);
	}
	planshet = get_border_to_planshet(&border);
	if (planshet == NULL)
	{
		ft_putstr("Error: malloc\n");
		fclose(file);
		return (1);
	}
	if (!(get_figure_to_planshet(file, &border, planshet)))
	{
		ft_putstr("Operation file corrupted\n");
		fclose(file);
		free(planshet);
		return (1);
	}
	print_planshet_to_terminal(&border, planshet);

	free(planshet);
	fclose(file);
	return 0;
}
