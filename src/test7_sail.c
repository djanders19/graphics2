/**
 * David J. Anderson - November 2021
 * 
 * Tests the ability to draw some "cool" Bezier surfaces by drawing a hillscape.
 */

#include <stdio.h>
#include <stdlib.h>
#include "graphicslib.h"

int main(int argc, char *argv[]) {
    Image *src;
    Module *scene;
    int frame;
    const int subdivisions = 0;
        
    View3D view;
    Matrix vtm, gtm;
    DrawState *ds;
    
    Color Green = { { 0.0, 1.0, 0.0 } };

    // set up the view
    point_set3D( &(view.vrp), 10,  3, 0);
    vector_set( &(view.vpn), -10, -3,  -0);
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
    Module *mast = module_create();
    module_color(scene, &Green);
    BezierSurface sail;
    bezierSurface_init(&sail);
    // Bottom edge:
    point_set3D(&sail.ctrls[0], 1.0, 0, 0);
    point_set3D(&sail.ctrls[1], 1.0, 0.0, 0.75);
    point_set3D(&sail.ctrls[2], 0.25, 0.0, 1.0);
    point_set3D(&sail.ctrls[3], 0.0, 0.0, 1.0);

    // Lower belly of the spinnaker:
    point_set3D(&sail.ctrls[4], 1.0, 0.33, 0.3);
    point_set3D(&sail.ctrls[5], 0.66, 0.33, 0.33);
    point_set3D(&sail.ctrls[6], 0.33, 0.33, 0.66);
    point_set3D(&sail.ctrls[7], 0.0, 0.33, 1.0);

    // upper belly of the spinnaker:
    point_set3D(&sail.ctrls[8], 1.0, 0.66, 0.3);
    point_set3D(&sail.ctrls[9], 0.66, 0.66, 0.33);
    point_set3D(&sail.ctrls[10], 0.33, 0.66, 0.66);
    point_set3D(&sail.ctrls[11], 0.0, 0.66, 1.0);

    // Top of the spinnaker:
    point_set3D(&sail.ctrls[12], 0.0, 1.0, 0.0);
    point_set3D(&sail.ctrls[13], 0.0, 1.0, 0.0);
    point_set3D(&sail.ctrls[14], 0.0, 1.0, 0.0);
    point_set3D(&sail.ctrls[15], 0.0, 1.0, 0.0);

    module_bezierSurface(mast, &sail, subdivisions, 0);

    // Add the mast:
    module_scale(mast, 0.05, 2.0, 0.05);
    module_translate(mast, 0, -0.7, 0);
    module_cylinder(mast, 10);
    module_module(scene, mast);

    Module *boat = module_create();
    // prow
    point_set3D(&sail.ctrls[0], 2.0, 1.0, 0);
    point_set3D(&sail.ctrls[1], 2.0, 1.0, 0);
    point_set3D(&sail.ctrls[2], 2.0, 1.0, 0);
    point_set3D(&sail.ctrls[3], 2.0, 1.0, 0);

    // Lower belly of the spinnaker:
    point_set3D(&sail.ctrls[4], 0.5, 1.0, 1.0);
    point_set3D(&sail.ctrls[5], 0.5, -1.0, 0.25);
    point_set3D(&sail.ctrls[6], 0.5, -1.0, -0.25);
    point_set3D(&sail.ctrls[7], 0.5, 1.0, -1.0);

    // upper belly of the spinnaker:
    point_set3D(&sail.ctrls[8],  -3.0, 1.0, 1.0);
    point_set3D(&sail.ctrls[9],  -3.0, -1.0, 0.50);
    point_set3D(&sail.ctrls[10], -3.0, -1.0, -0.50);
    point_set3D(&sail.ctrls[11], -3.0, 1.0, -1.0);

    // stern
    point_set3D(&sail.ctrls[12], -6.0,  1.0, 0.25);
    point_set3D(&sail.ctrls[13], -6.0,  1.0, 0.25);
    point_set3D(&sail.ctrls[14], -6.0,  1.0, -0.25);
    point_set3D(&sail.ctrls[15], -6.0,  1.0, -0.25);
    
    module_bezierSurface(boat, &sail, subdivisions, 0);
    module_identity(scene);
    module_scale(scene, 0.5, 0.5, 0.5);
    module_translate(scene, 0.6, -0.8, 0);
    module_module(scene, boat);
    module_identity(scene);

    src = image_create( 360, 640 );
    ds = drawstate_create();
    ds->shade = ShadeFrame;

    // Create the animation by adjusting the GTM
	for(frame=0;frame<60;frame++) {
		char buffer[256];
		
		matrix_rotateY(&gtm, cos(M_PI/30.0), sin(M_PI/30.0) );
		module_draw( scene, &vtm, &gtm, ds, NULL, src );

		sprintf(buffer, "sail-frame%03d.ppm", frame);
		image_write(src, buffer);
		image_reset(src);
	}

    module_delete(boat);
    module_delete(mast);
    module_delete(scene);

    free(ds);
    image_free(src);

    return 0;
}