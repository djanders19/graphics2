/**
 * A work in progress as of 9.24.2021. Ultimate goal is to generate 2D perlin
 * noise - but not completed for current assignments. Rather, I implemented a
 * simple sin function for lab 2.
 * 
 * David Anderson
 */

#include <stdio.h>
#include <stdlib.h>
#include <math.h>
#include "image.h"

Image *perlin(Image *im, int scale) {
    long imagesize = im->rows * im->cols;
    int i;

    // Create the perlin grid (as an image)
    Image *perlin = image_create(im->rows / scale, im->cols / scale);
    int numPerlinVectors = perlin->cols * perlin->rows;

    // Assign a random color vector to every element in the perlin grid
    for (i = 0; i < numPerlinVectors; i++) {
        perlin->data[i].rgb[0] = (rand() % 256) / 255;
        perlin->data[i].rgb[1] = (rand() % 256) / 255;
        perlin->data[i].rgb[2] = (rand() % 256) / 255;

        // Normalize the vector:
        float normFactor = 1 / sqrt(
                               perlin->data[i].rgb[0] * perlin->data[i].rgb[0] *
                               perlin->data[i].rgb[1] * perlin->data[i].rgb[1] *
                               perlin->data[i].rgb[2] * perlin->data[i].rgb[2]);
        
        perlin->data[i].rgb[0] = perlin->data[i].rgb[0] * normFactor;
        perlin->data[i].rgb[1] = perlin->data[i].rgb[1] * normFactor;
        perlin->data[i].rgb[2] = perlin->data[i].rgb[2] * normFactor;
    }
    
    for (i = 0; i < imagesize; i++) {
        // Find upper left corner of the perlin grid square that the pixel is in
        int gridRef = i / scale;
        float dotProduct1, dotProduct2, dotPoduct3, dotProduct 4;
        float interpVals[3];

        // Compute offset vectors from ith element to vectors at neighboring
        // perlin corners:
        FPixel *offset1 = (FPixel *) malloc(sizeof(FPixel));
        FPixel *offset2 = (FPixel *) malloc(sizeof(FPixel));
        FPixel *offset3 = (FPixel *) malloc(sizeof(FPixel));
        FPixel *offset4 = (FPixel *) malloc(sizeof(FPixel));

        offset1->rgb[0] = fmax(im->data[i].rgb[0] - 
                               perlin->data[gridRef].rgb[0], 0.0);
        offset1->rgb[1] = fmax(im->data[i].rgb[1] - 
                               perlin->data[gridRef].rgb[1], 0.0);
        offset1->rgb[2] = fmax(im->data[i].rgb[2] - 
                               perlin->data[gridRef].rgb[2], 0.0);

        // Compute dot product:
        interpVals[0] = offset1->rgb[0]

        if (gridRef + 1 < numPerlinVectors) {
            offset2->rgb[0] = fmax(im->data[i].rgb[0] - 
                                   perlin->data[gridRef+1].rgb[0], 0.0);
            offset2->rgb[1] = fmax(im->data[i].rgb[1] - 
                                   perlin->data[gridRef+1].rgb[1], 0.0);
            offset2->rgb[2] = fmax(im->data[i].rgb[2] - 
                                   perlin->data[gridRef+1].rgb[2], 0.0);
        }

        int belowLeft = gridRef + perlin->cols;
        if (belowLeft < numPerlinVectors) {
            offset3->rgb[0] = fmax(im->data[i].rgb[0] - 
                                   perlin->data[gridRef+belowLeft].rgb[0], 0.0);
            offset3->rgb[1] = fmax(im->data[i].rgb[1] - 
                                   perlin->data[gridRef+belowLeft].rgb[1], 0.0);
            offset3->rgb[2] = fmax(im->data[i].rgb[2] - 
                                   perlin->data[gridRef+belowLeft].rgb[2], 0.0);            
        }

        int belowRight = gridRef + perlin->cols + 1;
        if (belowRight < numPerlinVectors) {
            offset4->rgb[0] = fmax(im->data[i].rgb[0] - 
                                   perlin->data[gridRef+belowRight].rgb[0], 
                                   0.0);
            offset4->rgb[1] = fmax(im->data[i].rgb[1] - 
                                   perlin->data[gridRef+belowRight].rgb[1], 
                                   0.0);
            offset4->rgb[2] = fmax(im->data[i].rgb[2] - 
                                   perlin->data[gridRef+belowRight].rgb[2],
                                   0.0);
        }
        
        // Compute dot product of offset vectors with grid corners:


    }
}