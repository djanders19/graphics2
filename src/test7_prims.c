/**
 * David J. Anderson - November 2021
 * 
 * Tests the 3D primitives (e.g. pyramid, cylinder) modeling functions defined
 * in modeling.c and modeling.h.
 */

#include <stdio.h>
#include <stdlib.h>
#include "graphicslib.h"

int main(int argc, char *argv[]) {
    Image *src;
    Module *scene;
        
    View3D view;
    Matrix vtm, gtm;
    DrawState *ds;
    
    Color White = { { 1.0, 1.0, 1.0 } };

    // set up the view
    point_set3D( &(view.vrp), 10,  2.2, 0);
    vector_set( &(view.vpn), -10, -0,  -0);
    // point_set3D( &(view.vrp), 1,  12,  1);
    // vector_set( &(view.vpn), -1, -12, -1);
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

    scene = module_create();
    module_color(scene, &White);
    // module_cylinder(scene, 10);
    //module_cone(scene, 3);
    module_teapot(scene, 2);


    src = image_create( 360, 640 );
    ds = drawstate_create();
    ds->shade = ShadeFrame;

    module_draw( scene, &vtm, &gtm, ds, NULL, src );
    image_write( src, "testing_3d_prims.ppm" );
    module_delete(scene);

    free(ds);
    image_free(src);

    return 0;
}