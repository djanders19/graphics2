/**
 * This program allows the user to overlay a horizontal sin mask across an image
 * Briefly, the function takes the abolute value of the sin of each row number
 * and scales pixel values in that row accordingly. It also outputs a mask of
 * the same dimensions as the input image on a white background to clearly show
 * the function.
 *
 * Usage: horizontalSinTest <inputimage.ppm> <outputimage.ppm>
 */

#include <stdio.h>
#include <stdlib.h>
#include "horizontalSin.h"
#include "image.h"

int main(int argc, char *argv[]) {
    Image *src;

    if(argc < 3) {
        printf("Usage: sinNoise <input file> <output file>\n");
        exit(-1);
    }

    src = image_read(argv[1]);
    horizontalSin(src);
    image_write(src, argv[2]);

    FPixel *white = malloc(sizeof(FPixel));
    white->rgb[0] = 1.0;
    white->rgb[1] = 1.0;
    white->rgb[2] = 1.0;

    image_fill(src, *white);
    horizontalSin(src);
    image_write(src, "demo_horizontal_sin.ppm");
    
    image_free(src);
    free(white);
    return 1;
}