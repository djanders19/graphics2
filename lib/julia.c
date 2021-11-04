/**
 * julia.c
 * 
 * David Anderson, September 2021
 * 
 * Provides two functions for rendering Julia sets. The first function takes
 * arguments from the user and outputs an Image object with appropriate values,
 * while the second takes fewer arguments from the user and an input Image* for
 * the function to write to. The combination of these two functions allows for
 * maximum flexibility when writing out the Julia set.
 * 
 * I chose to color the set by setting the red channel to the log of the number
 * of iterations the algorithm takes to diverge and the blue channel to the
 * inverse of the log(iterations). Since these are both undefined at 0, areas
 * which immediately diverge are colored black. Otherwise, the bluer a rendered
 * area is the earlier it diverges, and the redder the area the later it
 * diverges. Areas that 
 * 
 * NB that the constants defined below set the number of iterations to compute
 * (i.e. the depth) when drawing the set while CX and CY define the components
 * of the complex number c that is subtracted from z_n at each iteration when
 * computing z_{n+1}. Modifying these constants will produce a different image.
 * Increasing ITERATIONS will allow for the rendering of higher resolution
 * images, but increases the rendering time dramatically.
 */

#include <math.h>
#include <stdio.h>
#include "image.h"
#include "julia.h"

#define ITERATIONS 10000 // Num of iterations to run of the dynamic system
#define CX 0.7454054; // constant component of c in the julia set equation
#define CY 0.1130063; // i-component of c in the julia set equation

/**
 * "Simple" version of julia(). Allows the user to specify the coordinates
 * (x0, y0) (x1, y1) defining a rectangle in the complex plane, and a number of
 * rows to render in the output image. Then, it creates and returns an Image
 * object with the Julia set rendered.
 */
Image *image_julia(float x0, float y0, float x1, float y1, int rows) {
    Image *im;
    int cols;
    float sCols, sRows, x, y, x_temp, y_temp;
    int i, j, n;
    int numIters;

    // calculate the number of columns 
    cols = ((x1 - x0) * rows) / (y1 - y0);
    
    // allocate an image that is rows by cols
    im = image_create(rows, cols);

    // scale cols and rows:
    sCols = (x1 - x0) / cols;
    sRows = (y1 - y0) / rows;

    // for each pixel in the image (i, j)
    for (i = 0; i < rows; i++) {
        for (j = 0; j < cols; j++) {
            // calculate (x, y) given (i, j)
            // this corresponds to the initial start value in the Julia equation
            x = sCols * j + x0;
            y = -sRows * i + y1;

            // for some number of iterations up to N (e.g. 100)
            for (n = 0; n < ITERATIONS; n++) {
                // iterate the julia equation
                // First, square z:
                // rule for multiplying n in C: (a+bi)(c+di) = (ac-bd)+(ad+bc)i
                // We have (x+yi)(x+yi) = (xx-yy)+(xy+yx)i
                x_temp = (x * x) - (y * y);
                y_temp = (x * y) + (y * x);
                x = x_temp;
                y = y_temp;

                // Then, subtract c:
                x = x - CX;
                y = y - CY;

                // if the length of z is greater than 2.0,
                if (sqrt(x * x + y * y) > 2) {
                    numIters = n;
                    break;
                }

                numIters = n;
            }
            // color pixel (i, j)
            image_setc(im, i, j, 0, log((double) numIters));
            image_setc(im, i, j, 1, 1.0 / log(((double) numIters)));
        }
    }
    // return the image
    return(im);
}

/**
 * Allows the user to specify the coordinates (x0, y0) and a width dx defining 
 * a rectangle in the complex plane, and an Image object in which to render. 
 * Then, it creates and returns an Image object with the Julia set rendered in
 * for the corresponding rectangle at the depth specified in the ITERATIONS
 * variable defined at the top of this file.
 */
void julia(Image *im, float x0, float y0, float dx) {
    image_reset(im);
    // Grab rows and cols from the image:
    int cols = im->cols;
    int rows = im->rows;
    
    float x, y, cx, cy, x_temp, y_temp;
    int i, j, n;
    int numIters;

    // Compute pixel width
    float pixelwidth = dx / cols;

    // compute height of the complex rectangle
    float height = pixelwidth * rows;

    // Compute y1
    float y1 = y0 + height;

    // for each pixel in the image (i, j)
    for (i = 0; i < rows; i++) {
        for (j = 0; j < cols; j++) {
            // calculate (x, y) given (i, j)
            // this corresponds to the initial start value in the Julia equation
            x = pixelwidth * j + x0;
            y = -pixelwidth * i + y1;

            // Set C:
            cx = 0.7454054; 
            cy = 0.1130063;

            // for some number of iterations up to N (e.g. 100)
            for (n = 0; n < ITERATIONS; n++) {
                // iterate the julia equation
                // First, square z:
                // rule for multiplying n in C: (a+bi)(c+di) = (ac-bd)+(ad+bc)i
                // We have (x+yi)(x+yi) = (xx-yy)+(xy+yx)i
                x_temp = (x * x) - (y * y);
                y_temp = (x * y) + (y * x);
                x = x_temp;
                y = y_temp;

                // Then, subtract c:
                x = x - cx;
                y = y - cy;

                // if the length of z is greater than 2.0,
                if (sqrt(x * x + y * y) > 2) {
                    numIters = n;
                    break;
                }

                numIters = n;
            }
            // color pixel (i, j)
            image_setc(im, i, j, 0, log((double) numIters));
            image_setc(im, i, j, 2, 1.0 / log(((double) numIters)));
        }
    }
    return;
}
