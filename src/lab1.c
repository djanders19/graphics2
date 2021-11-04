/*
  This program horizontally blurs an image by averaging the r, g, and b channels of the pixels
  to the left and the right of each image. Note that it does not take into account the edges of
  the image, but rather treats the entire image as a single continuous array. Because of this,
  pixels on the left edge will be blurred with pixels on the opposite edge, one row above.

  Adapted from Bruce A. Maxwell code for red thresholding updated 2021-09-05
  David J. Anderson 2021-09-10
*/

#include <stdio.h>
#include <stdlib.h>
#include "ppmIO.h"

#define USECPP 0

int main(int argc, char *argv[]) {
  Pixel *image;
  int rows, cols, colors;
  long imagesize;
  long i;

  if(argc < 3) {
    printf("Usage: lab1 <input file> <output file>\n");
    exit(-1);
  }

  /* read in the image */
  image = readPPM(&rows, &cols, &colors, argv[1]);
  if(!image) {
    fprintf(stderr, "Unable to read %s\n", argv[1]);
    exit(-1);
  }

  /* calculate the image size */
  imagesize = (long)rows * (long)cols;

  /* mess with the image here  */
  for(i=1; i<imagesize-1; i++) {
    image[i].g = (image[i-1].g + image[i+1].g) / 2;
    image[i].r = (image[i-1].r + image[i+1].r) / 2;
    image[i].b = (image[i-1].b + image[i+1].b) / 2;
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
