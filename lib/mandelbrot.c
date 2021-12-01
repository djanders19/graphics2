/**
 * mandelbrot.c
 * 
 * David Anderson, September 2021
 * 
 * Provides two functions for drawing Mandelbrot sets. The first function takes
 * arguments from the user and outputs an Image object with appropriate values,
 * while the second takes fewer arguments from the user and an input Image* for
 * the function to write to. The combination of these two functions allows for
 * maximum flexibility when drawing the set.
 * 
 * NB that the constants defined below set the number of iterations to compute
 * (i.e. the depth) when drawing the set. Increasing the number of iterations
 * allows for the rendering of higher-resolution images, but takes n^3 more
 * processing time.
 */ 

#include <math.h>
#include <stdio.h>
#include "graphicslib.h"

#define ITERATIONS 1000

/**
 * Easy to use version of mandelbrot. This function takes two points in the 
 * complex plane (x0, y0) and (x1, y1) which define a rectangle. It then creates
 * an image of that size, with the number of rows specified and scaling the
 * number of columns based on the other inputs. Then, it runs the mandelbrot
 * algorithm for a set number of iterations for every pixel in the image,
 * coloring each based on which iteration it diverges at. This allows the user
 * to easily specify what area and resolution is to be rendered of the
 * mandelbrot set.
 */
Image *image_mandelbrot(float x0, float y0, float x1, float y1, int rows) {
    Image *im;
    int cols;
    float sCols, sRows, zx, zy, zx_temp, zy_temp, x, y;
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
            // this corresponds to cx and cy in the Mandelbrot equation
            x = sCols * j + x0;
            y = -sRows * i + y1;
            
            // set zx and zy to (0, 0)
            zx = 0;
            zy = 0;

            // for some number of iterations up to N (e.g. 100)
            for (n = 0; n < ITERATIONS; n++) {
                // iterate the Mandelbrot equation
                zx_temp = zx * zx - zy * zy - x;
                zy_temp = 2 * zx * zy - y;
                zx = zx_temp;
                zy = zy_temp;

                // if the length of z is greater than 2.0,
                if (sqrt(zx * zx + zy * zy) > 2) {
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

    // return the image
    return(im);
}


/**
 * Function called by user to produce a mandelbrot image. It is somewhat more
 * obtuse to use than image_mandelbrot(), as it takes an image input with a
 * pre-existing resolution and scales the pixelsize to the complex plane based
 * on the inputs. It takes three arguments: (x0, y0) specify the lower left hand
 * corner of the image in the complex plane, while dx specifies the width of the
 * image in the complex plane. Height in the complex plane is computed within
 * the function.
 * 
 * This internal computation of height can make it a cumbersome tool to use
 * interactively, but would be useful if implementing a program where we wished
 * to pan about or zoom the set interactively within a window (say, a screen) of
 * a fixed size.
 */
void mandelbrot(Image *im, float x0, float y0, float dx) {
    image_reset(im);
    int cols = im->cols;
    int rows = im->rows;
    
    float zx, zy, zx_temp, zy_temp, x, y;
    int i, j, n;
    int numIters;

    // Compute pixel width
    float pixelwidth = dx / cols;

    // compute height of the complex rectangle
    float height = pixelwidth * rows;

    // Compute x1 and y1
    float x1 = x0 + dx;
    float y1 = y0 + height;

    // for each pixel in the image (i, j)
    for (i = 0; i < rows; i++) {
        for (j = 0; j < cols; j++) {
            // calculate (x, y) given (i, j)
            // this corresponds to cx and cy in the Mandelbrot equation
            x = pixelwidth * j + x0;
            y = -pixelwidth * i + y1;
            
            // set zx and zy to (0, 0)
            zx = 0;
            zy = 0;

            // for some number of iterations up to N (e.g. 100)
            for (n = 0; n < ITERATIONS; n++) {
                // iterate the Mandelbrot equation
                zx_temp = (zx * zx) - (zy * zy) - x;
                zy_temp = 2 * zx * zy - y;
                zx = zx_temp;
                zy = zy_temp;

                // if the length of z is greater than 2.0,
                if (sqrt(zx * zx + zy * zy) > 2) {
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
}


