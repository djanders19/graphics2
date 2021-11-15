/**
 * David J. Anderson - November 2021
 * 
 * Tests the ability to draw tetrahedrons.
 */

#include <stdio.h>
#include <stdlib.h>
#include "graphicslib.h"

int main(int argc, char *argv[]) {
    Image *src;
    Module *scene;
    int frame;
        
    View3D view;
    Matrix vtm, gtm;
    DrawState *ds;
    
    Color White = { { 1.0, 1.0, 1.0 } };

    // set up the view
    point_set3D( &(view.vrp), 20,  6, 0);
    vector_set( &(view.vpn), -20, -4,  -0);
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
    module_translate(scene, 0, 1, 6);
    module_tetrahedron(scene);

    module_identity(scene);
    module_translate(scene, 0, 1, -6);
    module_tetrahedron(scene);

    module_identity(scene);
    module_tetrahedron(scene);

    module_identity(scene);
    module_translate(scene, 4, -1, 0);
    module_tetrahedron(scene);

    module_identity(scene);
    module_translate(scene, -4, -1, 0);
    module_tetrahedron(scene);



    src = image_create( 360, 640 );
    ds = drawstate_create();
    ds->shade = ShadeFrame;

    // Create the animation by adjusting the GTM
	for(frame=0;frame<60;frame++) {
		char buffer[256];
		
		matrix_rotateY(&gtm, cos(M_PI/30.0), sin(M_PI/30.0) );
		module_draw( scene, &vtm, &gtm, ds, NULL, src );

		sprintf(buffer, "tetrahedron-frame%03d.ppm", frame);
		image_write(src, buffer);
		image_reset(src);
	}

    module_delete(scene);

    free(ds);
    image_free(src);

    return 0;
}