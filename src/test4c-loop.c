/*
  Assignment 4, polygon stress test

  C version
*/
#include <stdio.h>
#include <stdlib.h>
#include <math.h>
// #include "graphics.h"
#include "polygon.h"

/*
  Program to stress test the polygon API
*/
int main(int argc, char *argv[]) {
  Image *src;
  const int rows = 300;
  const int cols = 400;
  Polygon *pdyn;
  Polygon pstat;
  Color Red;
  Color Blue;
  Point pt[5];
  
  color_set(&Red, 8.0, 0.1, 0.2);
  color_set(&Blue, 0.2, 0.1, 0.8);

  src = image_create(rows, cols);

  // make some random polygon
  point_set2D(&(pt[0]), 30, 30);
  point_set2D(&(pt[1]), 220, 30);
  point_set2D(&(pt[2]), 180, 200);
  point_set2D(&(pt[3]), 150, 130);
  point_set2D(&(pt[4]), 60, 180);

  for(;;) {
    pdyn = polygon_createp(5, pt);

    polygon_init(&pstat);

    polygon_set(&pstat, 3, pt);

    polygon_clear(&pstat);

    polygon_copy(&pstat, pdyn);

    polygon_clear(pdyn);

    polygon_set(&pstat, 4, pt);
  
    polygon_copy(pdyn, &pstat);
  
    polygon_set(pdyn, 5, pt);

    polygon_drawFill(pdyn, src, Blue);

    polygon_set(&pstat, 3, pt);

    polygon_drawFill(&pstat, src, Red);

    polygon_free(pdyn);

    polygon_clear(&pstat);
  }

  image_free(src);

  return(0);
}
