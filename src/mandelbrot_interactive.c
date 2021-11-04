/**
 * mandelbrot_interactive.c
 * 
 * This allows for the creation of various images from the mandelbrot set at the
 * command line. The user simply runs the program and then is prompted to input
 * (x0, y0) and (x1, y1) points in complex space defining the rectangle to 
 * render, as well as the number of rows (a proxy for resolution) to render. It
 * then calls the functions defined in mandelbrot.h to output the image to the
 * file "mandelbrot_main_output.ppm" in whichever directory the program is being
 * run.
 */


#include <stdio.h>
#include "image.h"
#include "mandelbrot.h"

int main(int argc, char *argv[]) {
    /* For storing rows and dimensions of the complex rect, the user input */
    int rows;
    float x0, y0, x1, y1;
    
    /* startup message */
    printf("Now running mandelbrot. Press Ctl-C to quit.\n");
    
    while (1) {
        printf("Enter the rows to render, the x0 and y0 coordinates of the"\
        "lower left corner of the rectangle to render, and the coordinates of"\
        "upper left corner of the rectangle to render (e.g. 1000 -2.0 -1.0"\
        " 0.6 1.0):\n");
        
        /* Read in a line of user input */
        int result = scanf("%d%f%f%f%f", &rows, &x0, &y0, &x1, &y1);
        
        /* Check case where input is not valid*/
        if (rows <= 0 || result < 5 || (x1 - x0) <= 0.0 || (y1 - y0) <= 0) {
            rows = 0; // Reset inputs
            x0 = 0.0;
            y0 = 0.0;
            x1 = 1.0;
            y1 = 1.0;
            printf("Invalid input. Please enter inputs in the form <rows> <x0>"\
            "<y0> <x1> <y1> with rows > 0 and (x1, y1) - (x0, y0) > 0.\n");

            while (fgetc(stdin) != '\n') {
                ; // Read until newline
            }
        } else {
            /* Create an image with the specs given by the user */
            Image *toReturn = image_mandelbrot(x0, y0, x1, y1, rows);
            image_write(toReturn, "mandelbrot_main_output.ppm");
            image_free(toReturn);
        }
    }
    return 0;
}