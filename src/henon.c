/**
 * henon.c
 * 
 * David Anderson, fall 2021
 * 
 * Work in progress. This is a simple program to render the Henon
 * attractor that I wrote while reading up on chaotic systems. it is
 * not currently designed to be flexible enough for generalized use.
 */

#include <math.h>
#include <stdio.h>
#include "../include/image.h"

int main(int argc, char *argv[]) {
    int rows = 250;
    int cols = 600;
    double a, b, x, y, xPrime, yPrime;
    int t;
    Image *image = image_create(rows, cols);

    a = 7.0/5.0;
    b = 3.0/10.0;

    x = 0.0;
    y = 0.0;


    for (t = 0; t < 100000; t++) {
        printf("t = %d, x = %f, y = %f\n", t, x, y);
        image_setc(image, (125 - (int) (y * 100)), ((int) (x * 100)) + 300, 0, 
            image_getc(image, (125 - (int) (y * 100)), ((int) (x * 100)) + 300, 0) + 0.005);
        xPrime = y - a * x * x + 1;
        yPrime = b * x;
        x = xPrime;
        y = yPrime;
    }

    image_write(image, "henon.ppm");
    image_free(image);
    return 1;
}