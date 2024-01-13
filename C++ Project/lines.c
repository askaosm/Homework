#include <stdio.h>
#include <stdlib.h>
#include <math.h>
#include <string.h>
#include "Font.h"

#define PNG_SETJMP_NOT_SUPPORTED
#include <png.h>

#define WIDTH 256
#define HEIGHT 256
#define COLOR_DEPTH 8

typedef struct
{
	png_byte r, g, b, a;
} Pixel;

void draw_pixel(Pixel **row_pointers, int x, int y, int width, int height)
{
	for (int col = 0; col < width; ++col)
	{
		for (int row = 0; row < height; ++row)
		{
			Pixel *pixel_ptr = &row_pointers[row][col];
			if (row == y && col == x)
			{
				pixel_ptr->r = 0;	// red
				pixel_ptr->g = 255; // green
				pixel_ptr->b = 0;	// blue
				pixel_ptr->a = 255; // alpha (opacity)
			}
		}
	}
}

void draw_char(Pixel **image, int cursor, int font_arr, char *str)
{
	/* draw font */
	int image_length = strlen(str) * FONT_WIDTH;
	for (int x1 = 0; x1 < FONT_WIDTH; ++x1)
	{
		for (int y1 = 0; y1 < FONT_HEIGHT; ++y1)
		{
			if (font_array[font_arr][y1][x1] == 1)
				draw_pixel(image, cursor + x1, y1, image_length, FONT_HEIGHT);
		}
	}
}

void draw_random_graph(Pixel **row_pointers)
{
	for (int col = 0; col < WIDTH; ++col)
	{
		int bar_height = rand() % HEIGHT;
		for (int row = 0; row < HEIGHT; ++row)
		{
			/* draw bunch of lines */
			Pixel *pixel_ptr = &row_pointers[row][col];
			if (HEIGHT - row <= bar_height)
			{
				pixel_ptr->r = 0;	// red
				pixel_ptr->g = 255; // green
				pixel_ptr->b = 0;	// blue
				pixel_ptr->a = 255; // alpha (opacity)
			}
		}
	}
}

void draw_function(Pixel **row_pointers)
{
	for (int col = 0; col < WIDTH; ++col)
	{
		for (int row = 0; row < HEIGHT; ++row)
		{
			int y = col * col / 256;
			/* draw a graph */
			Pixel *pixel_ptr = &row_pointers[row][col];
			if (HEIGHT - row <= y)
			{
				pixel_ptr->r = 0;	// red
				pixel_ptr->g = 255; // green
				pixel_ptr->b = 0;	// blue
				pixel_ptr->a = 255; // alpha (opacity)
			}
		}
	}
}

void draw_rectangle(Pixel **row_pointers)
{
	int length, height;
	int init_x, init_y;

	printf("Determine left upper coordinate of rectangle (x, y) : ");
	scanf("%d %d", &init_x, &init_y);
	if (init_x > WIDTH || init_y >= HEIGHT)
	{
		printf("Left upper coordinate of rectangle is out of image!\n");
		return;
	}
	printf("Length and height of rectangle : ");
	scanf("%d %d", &length, &height);
	if (length + init_x >= WIDTH || height + init_y >= HEIGHT)
	{
		printf("Some part of rectangle is out of image!\n");
		return;
	}

	for (int col = 0; col < WIDTH; ++col)
	{
		for (int row = 0; row < HEIGHT; ++row)
		{
			/* draw a rectangle */
			Pixel *pixel_ptr = &row_pointers[row][col];
			if (init_y < row && row < init_y + height && init_x < col && col < init_x + length)
			{
				pixel_ptr->r = 0;	// red
				pixel_ptr->g = 255; // green
				pixel_ptr->b = 0;	// blue
				pixel_ptr->a = 255; // alpha (opacity)
			}
		}
	}
}

void draw_circle(Pixel **row_pointers)
{
	int dx, dy;
	int x1, y1, radius;

	printf("center of the circle(x1 y1): ");
	scanf("%d %d", &x1, &y1);
	if (x1 > WIDTH || y1 > HEIGHT)
	{
		printf("Center coordinate of the circle is out of image!\n");
		return;
	}
	printf("radius: ");
	scanf("%d", &radius);
	if (x1 + radius > WIDTH || x1 - radius < 0 || y1 + radius > HEIGHT || y1 - radius < 0)
	{
		printf("Some part of the circle is out of image!\n");
		return;
	}

	for (int y = 0; y < HEIGHT; y++)
	{
		for (int x = 0; x < WIDTH; x++)
		{
			dx = (int)x - (int)x1;
			dy = (int)y - (int)y1;
			Pixel *pixel_ptr = &row_pointers[y][x];
			/* draw a circle */
			if ((dx * dx) + (dy * dy) <= radius * radius)
			{
				pixel_ptr->r = 0;	// red
				pixel_ptr->g = 255; // green
				pixel_ptr->b = 0;	// blue
				pixel_ptr->a = 255; // alpha (opacity)
			}
		}
	}
}

void draw_triangle(Pixel **row_pointers)
{
	float length, height;
	float init_x, init_y;

	printf("Determine left upper coordinate of triangle (x, y) : ");
	scanf("%f %f", &init_x, &init_y);
	if (init_x > WIDTH || init_y > HEIGHT)
	{
		printf("Left upper coordinate of triangle is out of image!\n");
		return;
	}
	printf("Length and height of triangle : ");
	scanf("%f %f", &length, &height);
	if (length + init_x > WIDTH || height + init_y > HEIGHT)
	{
		printf("Some part of triangle is out of image!\n");
		return;
	}

	for (int y = 0; y < HEIGHT; y++)
	{
		for (int x = 0; x < WIDTH; x++)
		{
			float rate = length / height;
			Pixel *pixel_ptr = &row_pointers[y][x];
			/* draw a triangle */
			if (init_y <= y && y < init_y + height && init_x <= x && x < init_x + (rate * (y - init_y)))
			{
				pixel_ptr->r = 0;	// red
				pixel_ptr->g = 255; // green
				pixel_ptr->b = 0;	// blue
				pixel_ptr->a = 255; // alpha (opacity)
			}
		}
	}
}

void draw_string(Pixel **row_pointers)
{
	/* scan string */
	char str[10];
	printf("Input string : ");
	scanf("%s", str);

	/* draw characters */
	for (int i = 0; i < strlen(str); ++i)
	{
		int n = (int)str[i] - 'a';
		draw_char(row_pointers, i * FONT_WIDTH, n, str);
	}
}

int main(int argc, char *argv[])
{
	typedef enum
	{
		RANDOM_GRAPH = '1',
		GRAPH,
		RECT,
		CIRCLE,
		TRIANGLE,
		STRING
	} Shape;
	const Shape shape = (1 < argc) ? (Shape)*argv[1] : GRAPH;

	srand(time(NULL));

	/* open PNG file for writing */
	FILE *f = fopen("out.png", "wrb");
	if (!f)
	{
		fprintf(stderr, "could not open out.png\n");
		return 1;
	}

	/* initialize png data structures */
	png_structp png_ptr;
	png_infop info_ptr;

	png_ptr = png_create_write_struct(PNG_LIBPNG_VER_STRING, NULL, NULL, NULL);
	if (!png_ptr)
	{
		fprintf(stderr, "could not initialize png struct\n");
		return 1;
	}

	info_ptr = png_create_info_struct(png_ptr);
	if (!info_ptr)
	{
		png_destroy_write_struct(&png_ptr, (png_infopp)NULL);
		fclose(f);
		return 1;
	}

	/* begin writing PNG File */
	png_init_io(png_ptr, f);
	png_set_IHDR(png_ptr, info_ptr, WIDTH, HEIGHT, COLOR_DEPTH,
				 PNG_COLOR_TYPE_RGB_ALPHA, PNG_INTERLACE_NONE,
				 PNG_COMPRESSION_TYPE_DEFAULT, PNG_FILTER_TYPE_DEFAULT);
	png_write_info(png_ptr, info_ptr);

	/* allocate image data */
	Pixel *row_pointers[HEIGHT];
	for (int row = 0; row < HEIGHT; row++)
	{
		row_pointers[row] = calloc(WIDTH, sizeof(Pixel));
	}
	/* initialize the image with black color */
	for (int y = 0; y < HEIGHT; ++y)
	{
		for (int x = 0; x < WIDTH; ++x)
		{
			Pixel *pixel_ptr = &row_pointers[y][x];
			pixel_ptr->r = 0;	// red
			pixel_ptr->g = 0;	// green
			pixel_ptr->b = 0;	// blue
			pixel_ptr->a = 255; // alpha (opacity)
		}
	}

	/* draw something */
	switch (shape)
	{
	case RANDOM_GRAPH:
		/* draw a bunch of vertical lines */
		draw_random_graph(row_pointers);
		break;
	case GRAPH:
		// draw a graph of function
		draw_function(row_pointers);
		break;
	case RECT:
		/* draw a rectangle */
		draw_rectangle(row_pointers);
		break;
	case CIRCLE:
		/* draw a circle */
		draw_circle(row_pointers);
		break;
	case TRIANGLE:
		/* draw a triangle */
		draw_triangle(row_pointers);
		break;
	case STRING:
		/* draw string */
		draw_string(row_pointers);
		break;
	default:
		printf("functions not implemented!\n");
		break;
	}

	/* write image data to disk */
	png_write_image(png_ptr, (png_byte **)row_pointers);

	/* finish writing PNG file */
	png_write_end(png_ptr, NULL);

	/* clean up PNG-related data structures */
	png_destroy_write_struct(&png_ptr, &info_ptr);

	/* close the file */
	fclose(f);
	f = NULL;

	return 0;
}