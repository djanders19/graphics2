/**
 * Composites a masked blue/green screen image with another image, either placing the
 * other image into the green part of the screen image and outputting an image that is
 * the same size as the screen image (fg mode) or inserting the non-blue/green part of
 * the screen image into the background image and outputting an image with the same
 * dimensions as the background.
 * 
 * Usage: compositeGreenBlue <foreground file> <background file> <mask file> 
 *                           <output file> <mode> <dx> <dy>
 * 
 * Parameters:
 * <foreground file> : the blue/green screen image file, as a ppm
 * <background file> : the image which is to be inserted into the blue/green section of
 *                     the foreground file (fg) or the image in which to insert the
 *                     non-blue/green part of the foreground file into (bg)
 * <mask file> : A mask of the foreground image generated with maskGreenBlue
 * <output file> : the name of the ppm to output to
 * <mode> : either fg or bg. In fg mode, the output image is the size of the foreground
 *          image and dy/dx move the background image. In bg mosde, the output image is
 *          the size of the background and dy/dx move the foreground image.
 * <dx> : x-offset for the foreground/background image, depending on mode. Positive
 *        values move the image right, negative move the image left.
 * <dy> : y-offset for the foreground/background image, depending on mode. Positive
 *        values move the image down, negative move the image up.
 * 
 * Created by David Anderson for Computer Graphics at the Roux Institute
 * Professor: Bruce Maxwell, Roux Institute.
 * Last updated 9/16/2021
 */

#include <stdio.h>
#include <stdlib.h>
#include "ppmIO.h"
#include <string.h>

#define USECPP 0

int main(int argc, char *argv[]) {
  Pixel *image;
  Pixel *mask;
  Pixel *background;
  int imageRows, maskRows, imageCols, maskCols, imageColors, maskColors;
  int backgroundRows, backgroundCols, backgroundColors, dx, dy, iPlusDy, jPlusDx;
  long imageSize, maskSize, backgroundSize;
  long i, j, imageIndex, backgroundIndex;
  float alpha;
  char* mode;

  if(argc < 8) {
    printf("Usage: compositeGreenBlue <foreground file> <background file> <mask file> "\
    "<output file> <mode> <dx> <dy>.\n Mode is one of fg or bg. This specifies which "\
    "input file the resulting image should get its dimensions from.\n dx/dy specifies "\
    "the x/y offset of the image specified as \n");
    exit(-1);
  }

  /* read in the image */
  image = readPPM(&imageRows, &imageCols, &imageColors, argv[1]);
  if(!image) {
    fprintf(stderr, "Unable to read %s\n", argv[1]);
    exit(-1);
  }

  /* read in the background image */
  background = readPPM(&backgroundRows, &backgroundCols, &backgroundColors, argv[2]);
  if(!background) {
    fprintf(stderr, "Unable to read %s\n", argv[2]);
    exit(-1);
  }

  /* read in the mask */
  mask = readPPM(&maskRows, &maskCols, &maskColors, argv[3]);
  if(!mask) {
    fprintf(stderr, "Unable to read %s\n", argv[3]);
    exit(-1);
  }

  // set Mode:
  mode = argv[5];

  /* Get the offsets */
  dx = atoi(argv[6]);
  dy = atoi(argv[7]);

  /* calculate the image size */
  imageSize = (long)imageRows * (long)imageCols;
  maskSize = (long) maskRows * (long) maskCols;
  backgroundSize = (long) backgroundRows * (long) backgroundCols;
  
  printf("Foreground image size: %ld\n", imageSize);
  printf("Mask image size: %ld\n", maskSize);
  printf("Background image size: %ld\n", backgroundSize);

  /* ensure mask and image are the same size */
  if (maskSize != imageSize) {
    printf("Error: mask and image different sizes\n");
    exit(-1);
  }
  
  printf("Mode: %s\n", mode);

  if (strcmp(mode, "fg") == 0) {
    /* In this case, we are outputting an image with the same dimensions as the foreground file.
     * That is, we are essentially inserting the background into the masked part of the foreground.
     */
    for (i = 0; i < imageRows; i++) {
      for (j = 0; j < imageCols; j++) {
        // Index of pixel to modify in the foreground image:
        imageIndex = (i * imageCols) + j;
        alpha = ((float) mask[imageIndex].g) / 255; // Calculate alpha (scaled from [0, 255] to [0, 1])

        // Handle offset
        iPlusDy = i - dy;
        jPlusDx = j - dx;
        if (iPlusDy > backgroundRows || jPlusDx > backgroundCols || iPlusDy < 0 || jPlusDx < 0) {
          /* Handles case where bg image is not large enough to fill entire bg of foreground.
           * Empty space will be black.
           */
          image[imageIndex].r = alpha * image[imageIndex].r; 
          image[imageIndex].g = alpha * image[imageIndex].g;
          image[imageIndex].b = alpha * image[imageIndex].b; 
        } else {
          // Index of pixel to grab from the background image
	        backgroundIndex = (iPlusDy * backgroundCols) + jPlusDx;
          image[imageIndex].r =  alpha * image[imageIndex].r + (1.0 - alpha) * background[backgroundIndex].r;
          image[imageIndex].g =  alpha * image[imageIndex].g + (1.0 - alpha) * background[backgroundIndex].g;
          image[imageIndex].b =  alpha * image[imageIndex].b + (1.0 - alpha) * background[backgroundIndex].b;
        }
      } 
    }
    // Write resulting image:
    writePPM(image, imageRows, imageCols, imageColors /* s/b 255 */, argv[4]);
  } else if (strcmp(mode, "bg") == 0) {
    /* Handles case where we're outputting an image with the same dimensions as the background.
     * That is, we are essentially inserting the unmasked parts of the foreground into the background.
     */
    for (i = 0; i < imageRows; i++) {
      for (j = 0; j < imageCols; j++) {
        // Index of pixel in background image that we may modify
        backgroundIndex = ((i + dy) * backgroundCols) + (j + dx);

        if (i + dy > backgroundRows || i + dy <= 0 || j + dx >= backgroundCols || j + dx <= 0) {
          // Case where we're outside the bounds of the background image that's being inserted
          // Do nothing - just display the background image
        } else {
          // We're in bounds of image to be overlaid - calculate which pixel to grab from foreground:
          imageIndex = (i * imageCols) + j;
          alpha = ((float) mask[imageIndex].g) / 255; // Calculate alpha (scaled from [0, 255] to [0, 1])

          // Calculate what the pixel of the background image should be:
          background[backgroundIndex].r =  alpha * image[imageIndex].r + (1.0 - alpha) * background[backgroundIndex].r;
          background[backgroundIndex].g =  alpha * image[imageIndex].g + (1.0 - alpha) * background[backgroundIndex].g;
          background[backgroundIndex].b =  alpha * image[imageIndex].b + (1.0 - alpha) * background[backgroundIndex].b;
        }
      } 
    }

    // Write resulting image:
    writePPM(background, backgroundRows, backgroundCols, backgroundColors /* s/b 255 */, argv[4]);
  } else {
    printf("Not a valid mode. Please enter either fg or bg as the 5th argument\n");
    exit(-1);
  }

  /* free the image memory */
#if USECPP
  delete[] image;
  delete[] background;
  delete[] mask;
#else
  free(mask);
  free(background);
  // This results in a core dump if dx is in range [396, 418]
  // No idea why, but it only happens in bg mode
  free(image); 
#endif

  return(0);
}
