#ifndef IMAGE_H

#define IMAGE_H
#include "graphicslib.h"

typedef struct {
    FPixel *data;
    int rows; // num rows in the image
    int cols; // num cols in the image
    float *depth; // z-values (depth) for each pixel
    float *alpha; // alpha (transparency) for each pixel
    float maxval; // maximum value a pixel can have
} Image;


/* Constructors/destructors */
Image *image_create(int rows, int cols);
void image_free(Image *src);
void image_init(Image *src);
int image_alloc(Image *src, int rows, int cols);
void image_dealloc(Image *src);

/* I/O Functions */
Image *image_read(char *filename);
int image_write(Image *src, char *filename);

/* Getters/Setters */
FPixel image_getf(Image *src, int r, int c);
float image_getc(Image *src, int r, int c, int b);
float image_geta(Image *src, int r, int c);
float image_getz(Image *src, int r, int c);

void image_setf(Image *src, int r, int c, FPixel val);
void image_setc(Image *src, int r, int c, int b, float val);
void image_seta(Image *src, int r, int c, float val);
void image_setz(Image *src, int r, int c, float val);

/* Image Utilities */
void image_reset(Image *src);
void image_fill(Image *src, FPixel val);
void image_fillrgb(Image *src, float r, float g, float b);
void image_filla(Image *src, float a);
void imagefillz(Image *src, float z);

/* Coloring functions */
void image_setColor(Image *src, int r, int c, Color val);
Color image_getColor(Image *src, int r, int c);
#endif