#include <stdio.h>
#include <stdlib.h>
#include <math.h>
#include <string.h>
#include <stdarg.h>

#define PNG_SETJMP_NOT_SUPPORTED
#include <png.h>

#define HEIGHT 128
#define WIDTH 128
#define COLOR_DEPTH 8

typedef struct
{
    png_byte r, g, b, a;
} Pixel;

void abort_(const char *s, ...)
{
    va_list args;
    va_start(args, s);
    vfprintf(stderr, s, args);
    fprintf(stderr, "\n");
    va_end(args);
    abort();
}

int width, height, rowbytes;
png_byte color_type;
png_byte bit_depth;

png_structp png_ptr;
png_infop info_ptr;
int number_of_passes;
Pixel **row_pointers;

void read_png_file(char *file_name)
{
    unsigned char header[8]; // 8 is the maximum size that can be checked

    /* open file and test for it being a png */
    FILE *fp = fopen(file_name, "rb");
    if (!fp)
        abort_("[read_png_file] File %s could not be opened for reading", file_name);
    fread(header, 1, 8, fp);
    if (png_sig_cmp(header, 0, 8))
        abort_("[read_png_file] File %s is not recognized as a PNG file", file_name);

    /* initialize stuff */
    png_ptr = png_create_read_struct(PNG_LIBPNG_VER_STRING, NULL, NULL, NULL);

    if (!png_ptr)
        abort_("[read_png_file] png_create_read_struct failed");

    info_ptr = png_create_info_struct(png_ptr);
    if (!info_ptr)
        abort_("[read_png_file] png_create_info_struct failed");

    if (setjmp(png_jmpbuf(png_ptr)))
        abort_("[read_png_file] Error during init_io");

    png_init_io(png_ptr, fp);
    png_set_sig_bytes(png_ptr, 8);

    png_read_info(png_ptr, info_ptr);

    width = png_get_image_width(png_ptr, info_ptr);
    height = png_get_image_height(png_ptr, info_ptr);
    color_type = png_get_color_type(png_ptr, info_ptr);
    bit_depth = png_get_bit_depth(png_ptr, info_ptr);

    number_of_passes = png_set_interlace_handling(png_ptr);
    png_read_update_info(png_ptr, info_ptr);

    /* read file */
    if (setjmp(png_jmpbuf(png_ptr)))
        abort_("[read_png_file] Error during read_image");

    row_pointers = (Pixel **)malloc(sizeof(Pixel *) * height);
    rowbytes = png_get_rowbytes(png_ptr, info_ptr);
    for (int y = 0; y < height; ++y)
        row_pointers[y] = (Pixel *)malloc(rowbytes);

    png_read_image(png_ptr, (png_bytepp)row_pointers);

    fclose(fp);
}

void write_png_file(char *file_name)
{
    /* create file */
    FILE *fp = fopen(file_name, "wb");
    if (!fp)
        abort_("[write_png_file] File %s could not be opened for writing", file_name);

    /* initialize stuff */
    png_ptr = png_create_write_struct(PNG_LIBPNG_VER_STRING, NULL, NULL, NULL);

    if (!png_ptr)
        abort_("[write_png_file] png_create_write_struct failed");

    info_ptr = png_create_info_struct(png_ptr);
    if (!info_ptr)
        abort_("[write_png_file] png_create_info_struct failed");

    if (setjmp(png_jmpbuf(png_ptr)))
        abort_("[write_png_file] Error during init_io");

    png_init_io(png_ptr, fp);

    /* write header */
    if (setjmp(png_jmpbuf(png_ptr)))
        abort_("[write_png_file] Error during writing header");

    png_set_IHDR(png_ptr, info_ptr, width, height,
                 bit_depth, color_type, PNG_INTERLACE_NONE,
                 PNG_COMPRESSION_TYPE_BASE, PNG_FILTER_TYPE_BASE);

    png_write_info(png_ptr, info_ptr);

    /* write bytes */
    if (setjmp(png_jmpbuf(png_ptr)))
        abort_("[write_png_file] Error during writing bytes");

    /* 사진 좌우 뒤집기 */
    for (int y = 0; y < height; ++y)
    {
        for (int x = 0; x < width / 2; ++x)
        {
            Pixel tmp = row_pointers[y][width - 1 - x];
            row_pointers[y][width - 1 - x] = row_pointers[y][x];
            row_pointers[y][x] = tmp;
        }
    }
    png_write_image(png_ptr, (png_bytepp)row_pointers);

    /* end write */
    if (setjmp(png_jmpbuf(png_ptr)))
        abort_("[write_png_file] Error during end of write");

    png_write_end(png_ptr, NULL);

    /* cleanup heap allocation */
    for (int y = 0; y < height; ++y)
        free(row_pointers[y]);
    free(row_pointers);
    fclose(fp);
}

int main(int argc, char **argv)
{
    if (argc != 3)
        abort_("Usage: program_name <file_in> <file_out>");

    read_png_file(argv[1]);
    process_file();
    write_png_file(argv[2]);

    return 0;
}