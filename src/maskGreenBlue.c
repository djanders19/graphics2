/*
  This program reads in an image and outputs a mask image with the same 
  dimensions as the input image, with green or blue pixels masked, depending 
  on the mode it is run in. In the output image, pixels will be a shade of gray
  between black/rgb(0,0,0) where the image is green/blue and white/rgb(255,255,255)
  elsewhere. How dark a pixel is is determined by creating a harsd mask of the
  image where pixels are made white or black depending on the angle of their color
  vector to that of the pure green vector rgb(0,255,0) as well as their luminance.
  The user specifies thresholds for both these processing steps. A 3x3 box blur
  kernel is then convolved across the resultant hard mask to blur the edges,
  essentially setting each pixels brightness to the average of itself and its 8
  neighbors. The resulting soft mask is output to be used with the 
  compositeGreenBlue program.

  Usage: maskGreenBlue <input file> <output file> <threshold> <luminanceThreshold> 
                       <mode (g/b)>

  Parameters:
  <input file> : The file to be masked, must be a ppm
  <output file> : The name of the output ppm
  <threshold> : max absolute value of angle between a pixels color vector and the 
                pure green color vector for a pixel to be considered green, i.e.
                how not-green a pixel can be while still being thought of as green.
                Recommended starting value is 40. Measured in degrees, not radians.
  <luminanceThreshold> : How bright a pixel must be to be masked out. Keeping this
                low ensures dark, noisy greens that appear blackish remain in the 
                photo while brighter greens are masked out. Increasing it will mask
                more of the image. Recommended starting value: 0.02.
  <mode> : either g or b. In g mode, green will be masked from the image. In b mode,
           blue pixels will be masked from the image.

  Created by David Anderson for Computer Graphics at the Roux Institute
  Professor: Bruce Maxwell, Roux Institute.
  Last updated 9/16/2021
*/

#include <stdio.h>
#include <stdlib.h>
#include <math.h>
#include "ppmIO.h"
#include "maskGreenBlue.h"

#define USECPP 0
/**
 * Convert a number in [0,255] sRGB to it's equivalent value in Linear RGB.
 * This is useful as we need RGB values to be in Linear RGB
 * This transformation is pulled from https://en.wikipedia.org/wiki/SRGB.
 * 
 * This returns RGB as an array, so must be freed when the caller is done.
 */
double *sRGBtoLinearRGB(int r, int g, int b) {
  double *ret = malloc(3 * sizeof(double));
  // Convert rgb [0, 1]
  ret[0] = (double)r / 255.0;
  ret[1] = (double)g / 255.0;
  ret[2] = (double)b / 255.0;

  // Apply transformation to gamma expanded rgb:
  for (int i = 0; i < 3; i++) {
    if (ret[i] <= 0.04045) {
      ret[i] = ret[i] / 12.92;
    } else {
      ret[i] = pow((ret[i] + 0.055) / 1.055, 2.4);
    }
  }

  // Transform values to CIE XYZ i.e. linear RGB
  double x = ret[0] * 0.4124 + ret[1] * 0.3576 + ret[2] * 0.1805;
  double y = ret[0] * 0.2126 + ret[1] * 0.7152 + ret[2] * 0.0722;
  double z = ret[0] * 0.0193 + ret[1] * 0.1192 + ret[2] * 0.9505;
  ret[0] = x;
  ret[1] = y;
  ret[2] = z;

  return ret;
}

/**
 * Calculate relative luminance of a pixel according to the formula here:
 * https://en.wikipedia.org/wiki/Relative_luminance
 */
double calculateLuminance(int r, int g, int b) {
  double *xyzColorSpace = sRGBtoLinearRGB(r, g, b);
  double luminance = 0.2126*xyzColorSpace[0] + 
                     0.7152*xyzColorSpace[1] + 
                     0.0722*xyzColorSpace[2];
  free(xyzColorSpace);

  return luminance;
}

int main(int argc, char *argv[]) {
  Pixel *image;
  int rows, cols, colors;
  long imagesize;
  int i, j;
  double threshold, luminanceThreshold;
  char mode;
  double angle;

  if(argc < 6) {
    printf("Usage: maskGreenBlue <input file> <output file> <threshold> " \
    "<luminanceThreshold> <mode (g/b)> \n The threshold " \
    "establishes what blue/green value pixels must be above to be " \
    "masked\nThe luminance threshold sets a minimum luminance. Pixels " \
    "must have a higher luminance than this threshold to be masked (this " \
    "allows users to avoid masking dark, near black greens.");
    exit(-1);
  }
  /* set threshold and mode variables */
  threshold = atof(argv[3]);
  luminanceThreshold = atof(argv[4]);
  mode = *argv[5];



  /* read in the image */
  image = readPPM(&rows, &cols, &colors, argv[1]);
  if(!image) {
    fprintf(stderr, "Unable to read %s\n", argv[1]);
    exit(-1);
  }

  /* calculate the image size */
  imagesize = (long)rows * (long)cols;

  /* Mask the image here  */
  if (mode == 'g') {
    printf("Masking green from image\n");

    for (i = 0; i < imagesize; i++) {
      /* Calculate how green the pixel is by taking finding the angle between
       * the rgb vector for the pixel and the pure green vector r(0,255,0).
       * We convert this to degrees for eas of use
       */
      angle = acos((255.0*image[i].g) / 
      (sqrt(pow(255.0, 2)) * sqrt(pow(image[i].r, 2) + 
                          pow(image[i].g, 2) + 
                          pow(image[i].b, 2)))) * 
                          (180.0 / 3.14159265358979323846); // degrees conversion
      
      double luminance = calculateLuminance(image[i].r, image[i].g, image[i].b);
      // If the angle is below our set threshold, pixel is green.
      // If it's also bright enough, it gets masked.                   
      if (angle < threshold && luminance > luminanceThreshold) {
        image[i].r = 0; 
        image[i].g = 0; 
        image[i].b = 0; 
      } else {
          image[i].r = 255;
          image[i].g = 255;
          image[i].b = 255;
      }
    }
  } else if (mode == 'b') {
    printf("Masking blue from image\n");

    for (i = 0; i < imagesize; i++) {
      /* Calculate how blue the pixel is by taking finding the angle between
       * the rgb vector for the pixel and the pure green vector r(0,255,0).
       */
      angle = acos((255.0*image[i].b) / 
      (sqrt(pow(255.0, 2)) * sqrt(pow(image[i].r, 2) + 
                          pow(image[i].g, 2) + 
                          pow(image[i].b, 2))));
      
      double luminance = calculateLuminance(image[i].r, image[i].g, image[i].b);
      // If the angle is below our set threshold, pixel is blue, gets masked                 
      if (angle < threshold && luminance > luminanceThreshold) {
        image[i].r = 0; 
        image[i].g = 0; 
        image[i].b = 0; 
      } else {
          image[i].r = 255;
          image[i].g = 255;
          image[i].b = 255;
      }
    }
  } else {
    printf("Error: unsupported mode. Specify 'b' or 'g' as the fourth argument.");
    exit(-1);
  }
  printf("cols: %d rows: %d\n", cols, rows);

  /**
   * The code below softens the edge of the mask by convolving a 3x3 ones-matrix
   * kernel across the image. This essentially applies a "box blur" to the image,
   * making pixels that are on the border between white and black pixels varying
   * shades of gray, and softens the transition between the masked image and a
   * background image when later compositing
   */

  printf("Softening mask\n");
  // keep track of what to update rgb values to in a separate array
  int tempArr[imagesize]; 

  // Iterate across the mask 
  for (i = 0; i < rows; i++) {
    for (j = 0; j < cols; j++) {
      int imageIndex = (i*cols) + j;
      // If we're on the edges, don't change anything
      if ((i == 0) || (j == 0) || (i == (rows - 1)) || (j == (cols - 1))) {
        tempArr[imageIndex] = image[imageIndex].r;
      } else {
        // Otherwise average the pixel with all of it's neighbors
        tempArr[imageIndex] = (image[imageIndex + 1].r +
                      image[imageIndex - 1].r +
                      image[imageIndex - cols].r+
                      image[imageIndex + cols].r+ 
                      image[imageIndex - cols - 1].r+
                      image[imageIndex + cols + 1].r+
                      image[imageIndex - cols + 1].r+
                      image[imageIndex + cols - 1].r+
                      image[imageIndex].r) / 9;     
      }  
    }
  }
  
  /** 
   * Now we've stored what we want the color channels to be in tempArr. We loop
   * back through and update the channel values
   */
  for (i = 1; i < rows - 1; i++) { 
    for (j = 1; j < cols - 1; j++) { 
      int imageIndex = (i*cols) + j;

      /* Set RGB in the image */
      image[imageIndex].r = tempArr[imageIndex];
      image[imageIndex].g = tempArr[imageIndex];
      image[imageIndex].b = tempArr[imageIndex];
    }
  }

  /* write out the resulting image */
  writePPM(image, rows, cols, colors /* s/b 255 */, argv[2]);

  /* free the image memory */
#if USECPP
  delete[] image;
#else
  free(image);
#endif

  return(0);
}
