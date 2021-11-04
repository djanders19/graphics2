/**
 * David J. Anderson - November 2021
 * 
 * Draws a blue cube outline at the center of an image.
 */

#include <stdio.h>
#include <stdlib.h>
#include "graphicslib.h"

int main(int argc, char *argv[]) {

    Image *src;
    Module *scene;
    Module *cube;
    View3D view;
    Matrix vtm, gtm;
    DrawState *ds;
    Color Blue = { { 0.0, 0.0, 1.0 } };

    // set up the view
    point_set3D( &(view.vrp), 5, 5, 10);
    vector_set( &(view.vpn), -5, -5, -10 );
    vector_set( &(view.vup), 0, 1, 0 );
    view.d = 2.2;
    view.du = 1.6;
    view.dv = 0.9;
    view.f = 1;
    view.b = 50;
    view.screenx = 640;
    view.screeny = 360;

    matrix_setView3D( &vtm, &view );
    matrix_identity( &gtm );

    cube = module_create();
    module_color(cube, &Blue);
    module_cube(cube, 0);

    scene = module_create();
    module_module(scene, cube);

    src = image_create( 360, 640 );
    ds = drawstate_create();
    ds->shade = ShadeFrame;

    module_draw( scene, &vtm, &gtm, ds, NULL, src );

	// write out the scene
    image_write( src, "cube.ppm" );

    module_delete(cube);
    module_delete(scene);

    free(ds);
    image_free(src);

    return 0;
}