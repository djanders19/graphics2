#ifndef JULIA_H

#define JULIA_H
#include "image.h"
#include <math.h>

Image *image_julia(float x0, float y0, float x1, float y1, int rows);
void julia(Image *im, float x0, float y0, float dx);

#endif