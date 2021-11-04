#include <stdio.h>
#include <stdlib.h>
#include <math.h>
#include "views.h"

int main(int argc, char *argv[]) {
    const int rows = 180;
    const int cols = 320;
    View2D view;
    Matrix vtm;
    int i, t;
    char filename[256];
    const int nFrames = 50;
    Color WHITE;
    color_set(&WHITE, 1.0, 1.0, 1.0);
    // Matrix ltm;
    // Polygon stars[nStars];
    // Point points[nPoints];
    // float orient[nStars];
    // float freq[nStars];
    Image *src;

    src = image_create(rows, cols);
    srand48(0x01234ABCC);

    Point *vlist = malloc(sizeof(Point) * 4);
    point_set2D(&(vlist[0]), 0.5, 0.5);
    point_set2D(&(vlist[1]), 0.75, 0.5);
    point_set2D(&(vlist[2]), 0.75, 0.75);
    point_set2D(&(vlist[3]), 0.5, 0.75);
    Polygon *square = polygon_createp(4, vlist);

    // set up a view centered on (1.5, 1.5) with x pointing right
    point_set2D( &(view.vrp), 1.8, 1.8 );
    view.dx = 2.0;
    vector_set(&(view.x), 1.0, 0.0, 0.0);
    view.screenx = cols;
    view.screeny = rows;

    matrix_setView2D( &vtm, &view );
    printf("vtm:\n");
    matrix_print(&vtm, stdout);


    Polygon temp;
    for (t=0; t<nFrames; t++) {
        polygon_copy(&temp, square);
        matrix_xformPolygon(&vtm, &temp);
        polygon_drawFill(&temp, src, WHITE);

        printf("writing file\n");
        sprintf(filename, "starFrame-%04d.ppm", t );
        image_write( src, filename );

        // translate the view across the scene
        point_set2D( &(view.vrp), 1.0 - 0.5*(t+1)/nFrames, 1.0 - 0.5*(t+1)/nFrames);
        view.dx = 2.0 - 1.8*(t+1)/nFrames; // Zoom in
        matrix_setView2D(&vtm, &view);
        matrix_print(&vtm, stdout);
        image_reset(src);
    }

    image_free(src);
    free(vlist);
    polygon_free(square);
    return(0);
}