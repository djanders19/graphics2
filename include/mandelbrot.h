#ifndef MANDELBROT_H

#define MANDELBROT_H
#include "image.h"

Image *image_mandelbrot(float x0, float y0, float x1, float y1, int rows);
void mandelbrot(Image *im, float x0, float y0, float dx);

#endif