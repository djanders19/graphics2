/**
 * David J. Anderson - November 2021
 * 
 * Draws a few formations of tie-fighters. Tie-fighters are defined using a
 * hierarchical modeling (as defined in modeling.c); each tie fighter is made up
 * of a body module with two wing sub-modules. Formations are in turn made up of
 * tie-fighter submodules.
 */
#include <stdio.h>
#include <stdlib.h>
#include <math.h>
#include <string.h>
#include "graphicslib.h"

int main(int argc, char *argv[]) {
    Image *src;
    Matrix VTM;
    Matrix GTM;
    Module *body;
    Module *wing;
    Module *formation;
    int rows = 360;
    int cols = 640;

    Color Grey = {{0.6, 0.62, 0.64}};
    Color Red = {{1.0, 0.0, 0.0}};
    Color Green = {{0.0, 0.0, 1.0}};

    DrawState *ds;
    View3D view;

    // initialize the image
    src = image_create(rows, cols);

    // initialize matrices
    matrix_identity(&GTM);
    matrix_identity(&VTM);

    // set the View parameters
    point_set3D(&(view.vrp), 7,  5, -7);
    vector_set(&(view.vpn), -7, -5,  7);
    vector_set(&(view.vup), 0.0, 1.0, 0.0);
    view.d = 1;
    view.du = 1.6;
    view.dv = .9;
    view.f = 0.0;
    view.b = 15;
    view.screenx = cols;
    view.screeny = rows;
    matrix_setView3D(&VTM, &view);

    // print out VTM
    printf("Final VTM: \n");
    matrix_print(&VTM, stdout);

    // make a simple body module
    body = module_create();
    module_color( body, &Grey );
    module_cube( body, 1);
    module_rotateX(body, 0, 1);
    module_translate(body, 0, 0,-1.5);
    module_scale(body, 0.5, 0.5, 1.5);
    module_cylinder(body, 6);
    module_rotateY(body, 0, 1);
    module_rotateY(body, 0, 1);
    module_cylinder(body, 6);

    // Define a wing:
    // Corners:
    module_identity(body);
    module_translate(body, 0, 0, -2.2);
    wing = module_create();
    module_color(wing, &Grey);
    module_scale(wing, 2, 0.1, 2.5);
    module_rotateX(wing, 0, 1);
    module_cylinder(wing, 6);
    module_module(body, wing);
    module_rotateY(body, 0, 1);
    module_rotateY(body, 0, 1);
    module_module(body, wing);

    // Define a formation:
    formation = module_create();
    module_module(formation, body);
    module_translate(formation, -4, 2, 4);
    module_color(formation, &Red);
    module_module(formation, body);
    module_translate(formation, 9, 0, -8);
    module_color(formation, &Green);
    module_module(formation, body);

    ds = drawstate_create();
    ds->shade = ShadeDepth;

    matrix_identity(&GTM);
    module_draw(formation, &VTM, &GTM, ds, NULL, src);

    for(int i=0;i<36;i++) {
        char buffer[256];

        image_reset( src );

        matrix_identity(&GTM);
        matrix_rotateY(&GTM, cos(i*2*M_PI/36.0), sin(i*2*M_PI/36.0));
        module_draw(formation, &VTM, &GTM, ds, NULL, src);

        // write out the image
        sprintf(buffer, "tie-frame-%03d.ppm", i);
        image_write(src, buffer);
    }

    // write out the image
    // printf("Writing image.\n");
    // image_write(src, "tie-volume.ppm");

    // free stuff here
    module_delete( body );
    free(ds);
    image_free( src );
    

    return(0);
}