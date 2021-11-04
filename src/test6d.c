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
#include "graphicslib.h"

/*
    This function was provided by Bruce Maxwell, Roux institute, Fall 2021.
  Adds to the module a unit cylinder, aligned along the Y-axis.

  Takes in the number of subdivisions to use when creating the cylinder
 */
void cylinder( Module *mod, int sides );
void cylinder( Module *mod, int sides ) {
  Polygon p;
  Point xtop, xbot;
  double x1, x2, z1, z2;
  int i;

  polygon_init( &p );
  point_set3D( &xtop, 0, 1.0, 0.0 );
  point_set3D( &xbot, 0, 0.0, 0.0 );

  // make a fan for the top and bottom sides
  // and quadrilaterals for the sides
  for(i=0;i<sides;i++) {
    Point pt[4];

    // Make the fan
    // Top:
    // Define x/z for the fan points on outside of cylinder
    x1 = cos( i * M_PI * 2.0 / sides );
    z1 = sin( i * M_PI * 2.0 / sides );
    x2 = cos( ( (i+1)%sides ) * M_PI * 2.0 / sides );
    z2 = sin( ( (i+1)%sides ) * M_PI * 2.0 / sides );

    // copy those points into pt[]
    point_copy( &pt[0], &xtop );
    point_set3D( &pt[1], x1, 1.0, z1 );
    point_set3D( &pt[2], x2, 1.0, z2 );

    // Set a polygon for the fan:
    polygon_set( &p, 3, pt );
    module_polygon( mod, &p );

    // Do the same for the bottom:
    point_copy( &pt[0], &xbot );
    point_set3D( &pt[1], x1, 0.0, z1 );
    point_set3D( &pt[2], x2, 0.0, z2 );

    polygon_set( &p, 3, pt );
    module_polygon( mod, &p );

    // Link the top and bottom with a rectangular side
    point_set3D( &pt[0], x1, 0.0, z1 );
    point_set3D( &pt[1], x2, 0.0, z2 );
    point_set3D( &pt[2], x2, 1.0, z2 );
    point_set3D( &pt[3], x1, 1.0, z1 );
    
    polygon_set( &p, 4, pt );
    module_polygon( mod, &p );
  }

  polygon_clear( &p );
}

int main(int argc, char *argv[]) {
    // Pieces of Tie-Fighter:
    Image *src;
    Module *scene;
    Module *body;
    Module *wing;
    Module *wings;
    Module *formation;
    Polygon tpoly;
    polygon_init(&tpoly);
    Line l;
    Point  v[22];
    
    View3D view;
    Matrix vtm, gtm;
    DrawState *ds;
    
    Color White = { { 1.0, 1.0, 1.0 } };

    // set up the view
    point_set3D( &(view.vrp), 20,  20,  40);
    vector_set( &(view.vpn), -20, -20, -40 );
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

    body = module_create();
    module_color(body, &White);
    module_cube(body, 0);

    // Define a wing:
    // Corners:
    point_set(&(v[0]), 2, 2, 2, 1); // Top front
    point_set(&(v[1]), 2, 2, -2, 1); // Top back
    point_set(&(v[2]), 2, 0, -2.5, 1); // middle back
    point_set(&(v[3]), 2, -2, -2, 1); // Bottom Back
    point_set(&(v[4]), 2, -2, 2, 1); // Bottom front
    point_set(&(v[5]), 2, 0, 2.5, 1); // Middle front
    point_set(&(v[6]), 2, 0, 0, 1);
    wing = module_create();
    polygon_set(&tpoly, 6, &(v[0]));
    module_polygon(wing, &tpoly);

    // Draw wing scaffolding:
    line_set(&l, v[0], v[6]);
    module_line(wing, &l);
    line_set(&l, v[1], v[6]);
    module_line(wing, &l);
    line_set(&l, v[2], v[6]);
    module_line(wing, &l);
    line_set(&l, v[3], v[6]);
    module_line(wing, &l);
    line_set(&l, v[4], v[6]);
    module_line(wing, &l);
    line_set(&l, v[5], v[6]);
    module_line(wing, &l);

    // Draw strut linking wing to body:
    module_rotateZ(wing, 0, -1);
    module_translate(wing, 1, 0, 0);
    module_scale(wing, 1.0, 0.3, 0.3);
    cylinder(wing, 10);

    // Add wing to wings module:
    wings = module_create();
    module_module(wings, wing);
    module_rotateZ(wings, 0, 1);
    module_rotateZ(wings, 0, 1);
    module_module(wings, wing);

    // Add wings to body:
    module_module(body, wings);

    // Add cockpit to body:
    module_rotateX(body, 0, 1);
    module_translate(body, 0, 0, 10);
    module_scale(body, 0.8, 0.8, 0.1);
    cylinder(body, 6);
    
    // Add tie fighter 1 to formation:
    formation = module_create();
    module_module(formation, body);

    // Add tie fighter 2 to formation
    module_translate(formation, -10, 2, -12);
    module_module(formation, body);

    // Add tie fighter 3 to formation
    module_translate(formation, 20, 0, -3);
    module_module(formation, body);

    // Add the formation of tie-fighters:
    scene = module_create();
    module_module(scene, formation);

    // Add another formation of tie fighters behind the first at an angle:
    module_rotateY(scene, cos(0.8), sin(0.8));
    module_rotateZ(scene, cos(0.8), sin(0.8));
    module_translate(scene, -33, -19, -32);
    module_module(scene, formation);

    // Add a third formation of tie-fighters right behind the first:
    module_identity(scene);
    module_translate(scene, 1.0, -1.5, -20.0);
    module_module(scene, formation);

    src = image_create( 360, 640 );
    ds = drawstate_create();
    ds->shade = ShadeFrame;

    module_draw( scene, &vtm, &gtm, ds, NULL, src );

	// write out the scene
    image_write( src, "tie-formation.ppm" );

    // Clean up:
    module_delete(wing);
    module_delete(wings);
    module_delete(formation);
    module_delete(body);
    module_delete(scene);
    polygon_clear(&tpoly);

    free(ds);
    image_free(src);


    return 0;
}
