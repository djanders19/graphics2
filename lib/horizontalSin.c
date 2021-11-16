#include <stdio.h>
#include <stdlib.h>
#include <math.h>
#include "graphicslib.h"

/**
 * Apply
 */
void horizontalSin(Image *im) {
    Image *sinMask = image_create(im->rows, im->cols);

    for (int i = 0; i < sinMask->rows; i++) {
        for (int j = 0; j < sinMask->cols; j++) {
            image_setc(sinMask, i, j, 0, fabs(sin((double) i)));
            image_setc(sinMask, i, j, 1, fabs(sin((double) i)));
            image_setc(sinMask, i, j, 2, fabs(sin((double) i)));
        }
    }

    for (int i = 0; i < im->rows; i++) {
        for (int j = 0; j < im->cols; j++) {
            printf("SinMask val is %f, input val is %f\n\n", image_getc(sinMask, i, j, 0), image_getc(im, i, j, 0));
            image_setc(im, i, j, 0, image_getc(im, i, j, 0) * image_getc(sinMask, i, j, 0));
            image_setc(im, i, j, 1, image_getc(im, i, j, 1) * image_getc(sinMask, i, j, 1));
            image_setc(im, i, j, 2, image_getc(im, i, j, 2) * image_getc(sinMask, i, j, 2));
        }
    }
    image_free(sinMask);

    return;
}