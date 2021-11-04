#include <stdio.h>
#include <stdlib.h>
#include <math.h>
#include "views.h"

int main(int argc, char *argv[]) {
    const int rows = 180;
    const int cols = 320;
    const int nSurfaces = 16;
    const int nFrames = 50;
    char filename[256];
    View3D view;
    Matrix vtm;
    Polygon side[nSurfaces]; // Drawing a hyper cube
    Polygon tpoly;
    Point  tv[4];
    Point  v[16];
    Color  color[6];
    Image *src;
    int i;
    
    // set some colors
    color_set( &color[0], 0, 0, 1 );
    color_set( &color[1], 0, 1, 0 );
    color_set( &color[2], 1, 0, 0 );
    color_set( &color[3], 1, 0, 1 );
    color_set( &color[4], 0, 1, 1 );
    color_set( &color[5], 1, 1, 0 );

    for(i=0;i<nSurfaces;i++) {
        polygon_init(&side[i]);
    }
    
    // Corners of a cube, centered at (0, 0, 0)
    point_set( &v[0], -1, -1, -1, 1 );
    point_set( &v[1],  1, -1, -1, 1 );
    point_set( &v[2],  1,  1, -1, 1 );
    point_set( &v[3], -1,  1, -1, 1 );
    point_set( &v[4], -1, -1,  1, 1 );
    point_set( &v[5],  1, -1,  1, 1 );
    point_set( &v[6],  1,  1,  1, 1 );
    point_set( &v[7], -1,  1,  1, 1 );

    // Corners of surrounding cube, centered at (0,0,0)
    point_set( &v[8],  -2, -2, -2, 1);
    point_set( &v[9],   2, -2, -2, 1);
    point_set( &v[10],  2,  2, -2, 1);
    point_set( &v[11], -2,  2, -2, 1);
    point_set( &v[12], -2, -2,  2, 1);
    point_set( &v[13],  2, -2,  2, 1);
    point_set( &v[14],  2,  2,  2, 1);
    point_set( &v[15], -2,  2,  2, 1);

    // Draw internal cube:
    // front side
    polygon_set( &side[0], 4, &(v[0]) );

    // back side
    polygon_set( &side[1], 4, &(v[4]) );

    // top side
    point_copy( &tv[0], &v[2] );
    point_copy( &tv[1], &v[3] );
    point_copy( &tv[2], &v[7] );
    point_copy( &tv[3], &v[6] );

    polygon_set( &side[2], 4, tv );

    // bottom side
    point_copy( &tv[0], &v[0] );
    point_copy( &tv[1], &v[1] );
    point_copy( &tv[2], &v[5] );
    point_copy( &tv[3], &v[4] );

    polygon_set( &side[3], 4, tv );

    // left side
    point_copy( &tv[0], &v[0] );
    point_copy( &tv[1], &v[3] );
    point_copy( &tv[2], &v[7] );
    point_copy( &tv[3], &v[4] );

    polygon_set( &side[4], 4, tv );

    // right side
    point_copy( &tv[0], &v[1] );
    point_copy( &tv[1], &v[2] );
    point_copy( &tv[2], &v[6] );
    point_copy( &tv[3], &v[5] );

    polygon_set( &side[5], 4, tv );

    // Draw external cube
    // front side
    polygon_set( &side[6], 4, &(v[8]) );

    // back side
    polygon_set( &side[7], 4, &(v[12]) );

    // top side
    point_copy( &tv[0], &v[10] );
    point_copy( &tv[1], &v[11] );
    point_copy( &tv[2], &v[15] );
    point_copy( &tv[3], &v[14] );

    polygon_set(&side[8], 4, tv);

    // bottom side
    point_copy( &tv[0], &v[8] );
    point_copy( &tv[1], &v[9] );
    point_copy( &tv[2], &v[13] );
    point_copy( &tv[3], &v[12] );

    polygon_set( &side[9], 4, tv );

    // left side
    point_copy( &tv[0], &v[8] );
    point_copy( &tv[1], &v[11] );
    point_copy( &tv[2], &v[15] );
    point_copy( &tv[3], &v[12] );

    polygon_set( &side[10], 4, tv );

    // right side
    point_copy( &tv[0], &v[9] );
    point_copy( &tv[1], &v[10] );
    point_copy( &tv[2], &v[14] );
    point_copy( &tv[3], &v[13] );

    polygon_set( &side[11], 4, tv );

    // Right back tesseract link:
    point_copy( &tv[0], &v[5] );
    point_copy( &tv[1], &v[6] );
    point_copy( &tv[2], &v[14] );
    point_copy( &tv[3], &v[13] );

    polygon_set( &side[12], 4, tv );

    // left back tesseract link:
    point_copy( &tv[0], &v[7] );
    point_copy( &tv[1], &v[4] );
    point_copy( &tv[2], &v[12] );
    point_copy( &tv[3], &v[15] );

    polygon_set( &side[13], 4, tv );

    // right front tesseract link:
    point_copy( &tv[0], &v[1] );
    point_copy( &tv[1], &v[2] );
    point_copy( &tv[2], &v[10] );
    point_copy( &tv[3], &v[9] );

    polygon_set( &side[14], 4, tv );

    // left front tesseract link:
    point_copy( &tv[0], &v[0] );
    point_copy( &tv[1], &v[3] );
    point_copy( &tv[2], &v[11] );
    point_copy( &tv[3], &v[8] );

    polygon_set( &side[15], 4, tv );

    // set up the view structure
    view.d = 0.5;  // focal length
    view.du = 2;
    view.dv = view.du * (float)rows / cols;
    view.f = 0; // front clip plane
    view.b = 4; // back clip plane
    view.screenx = cols;
    view.screeny = rows;
    double alpha = -1.0;

    for (int t=0; t<nFrames; t++) {
        printf("Alpha = %f\n", alpha);
        // Set the changeable parts of the view:
        point_set(&(view.vrp), 3*alpha, 2*alpha, -2*alpha - (1.0-alpha)*3, 1.0);
        vector_set(&(view.vup), 0, 1, 0);
        vector_set(&(view.vpn), -view.vrp.val[0], -view.vrp.val[1], -view.vrp.val[2]);
        
        // Create VTM:
        matrix_setView3D(&vtm, &view);
        matrix_print(&vtm, stdout);
        
        // create image
        src = image_create(rows, cols);

        // use a temprary polygon to transform stuff
        polygon_init(&tpoly);

        for(i=0;i<nSurfaces;i++) {
            polygon_copy( &tpoly, &side[i] );
            matrix_xformPolygon( &vtm, &tpoly );

            // normalize by homogeneous coordinate before drawing
            polygon_normalize( &tpoly );

            polygon_draw( &tpoly, src, color[i%6] );
        }

        printf("Writing image\n");
        sprintf(filename, "hypercube-%04d.ppm", t );
        image_write(src, filename);
        alpha = alpha + 3.0 / nFrames;
        image_free(src);
    }

  return(0);
}