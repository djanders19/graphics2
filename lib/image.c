#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "image.h"
#include "color.h"
#include "ppmIO.h"

#define USECPP 0



/* Constructors/destructors */

/**
 * Allocates an Image structure and initializes the top level fields to 
 * appropriate values. Allocates space for an image of the specified size, 
 * unless either rows or cols is 0. Returns a pointer to the allocated Image 
 * structure. Returns a NULL pointer if the operation fails.
 */
Image *image_create(int rows, int cols) {
    // Malloc our image:
    Image *image = malloc(sizeof(Image));
    
    // Initialize our image:
    image_init(image);

    // Allocate image internals:
    if (image_alloc(image, rows, cols) == 0) {
        return image;
    }
    
    // In this case, we've failed to allocate the image and we exit:
    printf("Failed to create image.\n");
    return(NULL);
}


/**
 * de-allocates image data and frees the Image structure.
 */ 
void image_free(Image *src) {
    free(src->depth);
    free(src->alpha);
    free(src->data);
    free(src);
    return;
}


/**
 * Given an uninitialized Image structure, sets the rows and cols fields to zero
 * and the data, alpha and depth fields to NULL. Sets maxval to 1.0.
 */ 
void image_init(Image *src) {
    src->rows = 0;
    src->cols = 0;
    src->maxval = 1.0;
    src->alpha = NULL;
    src->depth = NULL;
    src->data = NULL;
    return;
}


/**
 * Allocates space for an image with rows/cols as specified, initializing rgb
 * values to 0.0, maxval to 1.0, and alpha/depth to 1.0 for every pixel.
 * 
 * returns 0 if the operation is successful. Otherwise, returns -1.
 */
int image_alloc(Image *src, int rows, int cols) {
    // If rows or cols are zero or negative, return -1
    // printf("Allocating image with Rows: %d Cols: %d\n", rows, cols);
    if (rows < 0 || cols < 0) {
        printf("Attempted to allocate image with bad number of rows or cols\n");
        return(-1);
    }

    // Set image internals:
    src->maxval = 1.0;
    src->rows = rows;
    src->cols = cols;

    // Malloc image pixels and associated alpha/depth arrays:
    src->depth = (float *) malloc(rows * cols * sizeof(float));
    src->alpha = (float *) malloc(rows * cols * sizeof(float));
    src->data = (FPixel *) malloc(rows * cols * sizeof(FPixel));

    // Ensure malloc succeeded:
    if (src->depth && src->alpha && src->data) {
        // set default depths, alphas, and make the image black (rgb all 0.0)
        for (int i = 0; i < rows*cols; i++) {
            src->depth[i] = 1.0;
            src->alpha[i] = 1.0;

            // Initialize rgb values to 0.0:
            for (int j = 0; j < 3; j++) {
                src->data[i].rgb[j] = 0.0;
            }
        }

        return(0);
    }
    
    // Malloc failed - return -1:
    printf("Failed to malloc depth, alpha, or data arrays in image.\n");
    return(-1);
}


/**
 * De-allocates image data and resets the Image structure fields. 
 * This function does not free the Image structure
 */
void image_dealloc(Image *src) {
    free(src->depth);
    free(src->data);
    free(src->alpha);

    src->depth = NULL;
    src->alpha = NULL;
    src->data = NULL;

    src->maxval = 1.0;
    src->rows = 0;
    src->cols = 0;
    return;
}



/* I/O Functions */
Image *image_read(char *filename) {
    Pixel *image;
    Image *toReturn;
    int rows, cols, colors;
    long i, imageSize;

    // Read the ppm:
    image = readPPM(&rows, &cols, &colors, filename);
    if (!image) {
        fprintf(stderr, "Unable to read %s\n", filename);
        exit(-1);    
    }

    // Calculate image size:
    imageSize = rows * cols;

    // Create an image object:
    toReturn = image_create(rows, cols);
    if (!toReturn) {
        fprintf(stderr, "Unable to create image for %s\n", filename);
    }

    // Set the RGB for each Image pixel:
    for (i = 0; i < imageSize; i++) {
        // We need to convert char digits in ppm to floats.
        // write the data directly instead of using function calls for #speed
        toReturn->data[i].rgb[0] = ((float) image[i].r) / 255.0;
        toReturn->data[i].rgb[1] = ((float) image[i].g) / 255.0;
        toReturn->data[i].rgb[2] = ((float) image[i].b) / 255.0;
    }

    free(image);
    return toReturn;
}


int image_write(Image *src, char *filename) {
    Pixel *image;
    long i, imageSize;

    imageSize = src->rows * src->cols;

    // Malloc our output Pixel array:
    image = (Pixel *) malloc(sizeof(Pixel) * imageSize);
    if (!image) {
        printf("Failed to malloc Pixel array.\n");
        return -1;
    }

    // fill our pixel array:
    for (i = 0; i < imageSize; i++) {
        image[i].r = ((char) ((int) (src->data[i].rgb[0] * 255)));
        image[i].g = ((char) ((int) (src->data[i].rgb[1] * 255)));
        image[i].b = ((char) ((int) (src->data[i].rgb[2] * 255)));
    }

    writePPM(image, src->rows, src->cols, 255, filename);
    free(image);
    return 0;
}



/* Getters/Setters */

/**
 * Get the FPixel at row r, column c and return it. If the specified row/col is
 * outside the data range, the function prints an error and returns the first
 * FPixel.
 */
FPixel image_getf(Image *src, int r, int c) {
    int index = (src->cols * r) + c;
    if (index > (src->cols * src->rows) || index < 0) {
        printf("Attempted to get pixel (%d %d), which is outside the image.\n",\
            r, c);
        return src->data[0];
    }

    return src->data[index];
}


/**
 * Get the value of band b for pixel at row r column c. If the specified row/col
 * is outside the data range, return -1.0.
 */
float image_getc(Image *src, int r, int c, int b) {
    int index = (src->cols * r) + c;
    if (index > (src->cols * src->rows) || index < 0) {
        printf("Attempted to get pixel (%d, %d) which is outside the image.\n",\
            r, c);
        return -1.0;
    }

    // ensure we're grabbing a valid color band:
    if (b > 2 || b < 0) {
        printf("Attempted to access non-existent band in Pixel (%d, %d)", r, c);
        return -1.0;
    }

    return src->data[index].rgb[b];
}


/**
 * Get the value of alpha for pixel at row r column c. If the specified row/col
 * is outside the data range, return -1.0.
 */
float image_geta(Image *src, int r, int c) {
    long index = (src->cols * r) + c;
    if (index > (src->cols * src->rows) || index < 0) {
        printf("Attempted to get pixel (%d, %d) which is outside the image.\n",\
            r, c);
        return -1.0;
    }

    return src->alpha[index];
}


/**
 * Get the value of Z (depth) for pixel at row r column c. If the specified 
 * row/col is outside the data range, return -1.0.
 */
float image_getz(Image *src, int r, int c) {
    int index = (src->cols * r) + c;
    if (index > (src->cols * src->rows) || index < 0) {
        printf("Attempted to get pixel (%d, %d) which is outside the image.\n",\
            r, c);
        return -1.0;
    }

    return src->depth[index];
}


/**
 * Set the FPixel at row r, column c. If the specified row/col is
 * outside the data range, the function prints an error.
 */
void image_setf(Image *src, int r, int c, FPixel val) {
    int index = (src->cols * r) + c;
    if (index > (src->cols * src->rows) || index < 0) {
        printf("Attempted to get pixel (%d %d), which is outside the image.\n",\
            r, c);
        return;
    }
    
    src->data[index] = val;
}


/**
 * Set the value of band b for pixel at row r column c. If the specified row/col
 * is outside the data range, print an error and return.
 */
void image_setc(Image *src, int r, int c, int b, float val) {
    long index = (src->cols * r) + c;
    if (index > (src->cols * src->rows) || index < 0) {
        printf("Attempted to get pixel (%d %d), which is outside the image.\n",\
            r, c);
        return;
    }
    
    src->data[index].rgb[b] = val;
}


/**
 * Set the value of alpha for pixel at row r column c. If the specified 
 * row/col is outside the data range print an error. Prints an error if the
 * specified alpha value is outside the range [0.0, 1.0].
 */
void image_seta(Image *src, int r, int c, float val) {
    int index = (src->cols * r) + c;
    if (index > (src->cols * src->rows) || index < 0) {
        printf("Attempted to get pixel (%d %d), which is outside the image.\n",\
            r, c);
        return;
    }

    // Ensure val is in correct range:
    if (val < 0.0 || val > 1.0) {
        printf("Attempted to set invalid alpha in pixel (%d,%d)\n", r, c);
        return;
    }
    
    src->alpha[index] = val;    
}


/**
 * Set the value of Z (depth) for pixel at row r column c. If the specified 
 * row/col is outside the data range print an error
 */
void image_setz(Image *src, int r, int c, float val) {
    int index = (src->cols * r) + c;
    if (index > (src->cols * src->rows) || index < 0) {
        printf("Attempted to get pixel (%d %d), which is outside the image.\n",\
            r, c);
        return;
    }
    
    src->depth[index] = val;    
}



/* Image Utilities */

/**
 * Resets every pixel to a default value (e.g. Black, alpha value of 1.0, z 
 * value of 1.0).
 */
void image_reset(Image *src) {
    for (int i = 0; i < src->rows * src->cols; i++) {
            src->depth[i] = 1.0;
            src->alpha[i] = 1.0;

            // Initialize rgb values to 0.0:
            for (int j = 0; j < 3; j++) {
                src->data[i].rgb[j] = 0.0;
            }
        }
    return;
}


/**
 * Sets every FPixel to the given value.
 */
void image_fill(Image *src, FPixel val) {
    for (int i = 0; i < src->rows * src->cols; i++) {
        // Set data for every pixel to val
        for (int j = 0; j < 3; j++) {
            src->data[i] = val;
        }
    }
    return;
}


/**
 * Sets the (r, g, b) values of each pixel to the given color.
 */
void image_fillrgb(Image *src, float r, float g, float b) {
    // Ensure valid rgb values
    if (r > 1.0 || g > 1.0 || b > 1.0 || r < 0.0 || g < 0.0 || b < 0.0) {
        printf("attempted to fill image with invalid color\n");
        return;
    }

    // Set color channels for each pixel:
    for (int i = 0; i < src->rows * src->cols; i++) {
        src->data[i].rgb[0] = r;
        src->data[i].rgb[1] = g;
        src->data[i].rgb[2] = b;
    }

    return;    
}


/**
 * Sets the alpha value of each pixel to the given value.
 */
void image_filla(Image *src, float a) {
    // Ensure valid alpha value:
    if (a > 1.0 || a < 0.0) {
        printf("attempted to set invalid alpha for entire image.\n");
        return;
    }

    // Set alpha channels for each pixel:
    for (int i = 0; i < src->rows * src->cols; i++) {
        src->alpha[i] = a;
    }

    return;
}


/**
 * Sets the z value of each pixel to the given value.
 */
void imagefillz(Image *src, float z) {
    for (int i = 0; i < src->rows * src->cols; i++) {
        // Set color channels for each pixel:
        src->depth[i] = z;
    }

    return;
}



/* Coloring functions */

/**
 * Set the color of the pixel at (r, c) to the values from the Color object val.
 */
void image_setColor(Image *src, int r, int c, Color val) {
    int index = (src->cols * r) + c;

    // Ensure index in range:
    if (index > (src->cols * src->rows) || index < 0) {
        printf("Attempted to get pixel (%d %d), which is outside the image.\n",\
            r, c);
        return;
    }

    src->data[index].rgb[0] = val.c[0];
    src->data[index].rgb[1] = val.c[1]; 
    src->data[index].rgb[2] = val.c[2];

    return;
}


/**
 * Get the color of a pixel (r,c) as a Color object.
 */
Color image_getColor(Image *src, int r, int c) {
    int index = (src->cols * r) + c;
    Color *color = (Color *) malloc(sizeof(Color));

    // Ensure index in range:
    if (index > (src->cols * src->rows) || index < 0) {
        printf("Attempted to get pixel (%d %d), which is outside the image.\n",\
            r, c);
        exit(-1);
    }

    // Set color values:
    color_set(color, src->data[index].rgb[0],
                     src->data[index].rgb[1],
                     src->data[index].rgb[2]);

    return *color;         
}