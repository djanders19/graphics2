/*
Bruce A. Maxwell
Fall 2014

Example of a 3D scene model

Draws three 3D X-xwingWings as line draxwingWings
 */

#include <stdio.h>
#include <stdlib.h>
#include "graphicslib.h"

/*
  Adds to the module a unit cylinder, aligned along the Y-axis

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

    x1 = cos( i * M_PI * 2.0 / sides );
    z1 = sin( i * M_PI * 2.0 / sides );
    x2 = cos( ( (i+1)%sides ) * M_PI * 2.0 / sides );
    z2 = sin( ( (i+1)%sides ) * M_PI * 2.0 / sides );

    point_copy( &pt[0], &xtop );
    point_set3D( &pt[1], x1, 1.0, z1 );
    point_set3D( &pt[2], x2, 1.0, z2 );

    polygon_set( &p, 3, pt );
    module_polygon( mod, &p );

    point_copy( &pt[0], &xbot );
    point_set3D( &pt[1], x1, 0.0, z1 );
    point_set3D( &pt[2], x2, 0.0, z2 );

    polygon_set( &p, 3, pt );
    module_polygon( mod, &p );

    point_set3D( &pt[0], x1, 0.0, z1 );
    point_set3D( &pt[1], x2, 0.0, z2 );
    point_set3D( &pt[2], x2, 1.0, z2 );
    point_set3D( &pt[3], x1, 1.0, z1 );
    
    polygon_set( &p, 4, pt );
    module_polygon( mod, &p );
  }

  polygon_clear( &p );
}

// adds a cube centered on the origin of width 2x2x2 to the module
void cube( Module *mod );
void cube( Module *mod ) {
  Point pt[4];
  Polygon p;

  polygon_init( &p );
  point_set3D( &pt[0], -1, -1, -1 );
  point_set3D( &pt[1], -1, -1,  1 );
  point_set3D( &pt[2], -1,  1,  1 );
  point_set3D( &pt[3], -1,  1, -1 );
  polygon_set( &p, 4, pt );
  module_polygon( mod, &p );

  point_set3D( &pt[0], 1, -1, -1 );
  point_set3D( &pt[1], 1, -1,  1 );
  point_set3D( &pt[2], 1,  1,  1 );
  point_set3D( &pt[3], 1,  1, -1 );
  polygon_set( &p, 4, pt );
  module_polygon( mod, &p );

  point_set3D( &pt[0], -1, -1, -1 );
  point_set3D( &pt[1], -1, -1,  1 );
  point_set3D( &pt[2],  1, -1,  1 );
  point_set3D( &pt[3],  1, -1, -1 );
  polygon_set( &p, 4, pt );
  module_polygon( mod, &p );

  point_set3D( &pt[0], -1, 1, -1 );
  point_set3D( &pt[1], -1, 1,  1 );
  point_set3D( &pt[2],  1, 1,  1 );
  point_set3D( &pt[3],  1, 1, -1 );
  polygon_set( &p, 4, pt );
  module_polygon( mod, &p );

  point_set3D( &pt[0], -1, -1, -1 );
  point_set3D( &pt[1], -1,  1, -1 );
  point_set3D( &pt[2],  1,  1, -1 );
  point_set3D( &pt[3],  1, -1, -1 );
  polygon_set( &p, 4, pt );
  module_polygon( mod, &p );

  point_set3D( &pt[0], -1, -1, 1 );
  point_set3D( &pt[1], -1,  1, 1 );
  point_set3D( &pt[2],  1,  1, 1 );
  point_set3D( &pt[3],  1, -1, 1 );
  polygon_set( &p, 4, pt );
  module_polygon( mod, &p );
  polygon_clear(&p);
}

// makes 3 X-xwingWing fighters in a loose formation
int main(int argc, char *argv[]) {

  Image *src;
  Module *scene;
  Module *engine;
  Module *xwingWing;
  Module *xwingWings;
  Module *laser;
  Module *xwingBody;
  Polygon p;
  Point pt[4];
  View3D view;
  Matrix vtm, gtm;
  DrawState *ds;
  Color Flame = { { 1.0, 0.7, 0.2 } };
  Color Red =  { { 1.0, 0.2, 0.1 } };
  float xwingBodyWidth = 2.0;

  // set up the view
  point_set3D( &(view.vrp), 15, 30, 100 );
  vector_set( &(view.vpn), -20, -30, -100 );
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

  // engine
  engine = module_create();
  module_scale( engine, 1.3, 6, 1.3);
  module_rotateX( engine, 0, 1 );
  cylinder( engine, 10 );
  module_scale( engine, .8, .8, 1 );
  module_color( engine, &Flame );
  cylinder( engine, 10 );

  // laser
  laser = module_create();
  module_scale( laser, 0.5, 5, 0.5 );
  module_rotateX( laser, 0, 1 );
  cylinder( laser, 6 );
  module_scale( laser, 0.4, 0.4, 1.0 );
  module_translate( laser, 0, 0, 4.5 );
  module_color( laser, &Red );
  cylinder( laser, 10 );

  // xwingWing
  xwingWing = module_create();
  polygon_init( &p );
  point_set3D( &pt[0], 0.0, 0.0, 0.0 );
  point_set3D( &pt[1], 0.0, 0.0, 5.0 );
  point_set3D( &pt[2], 15.0, 0.0, 3.0 );
  point_set3D( &pt[3], 15.0, 0.0, 0.0 );
  polygon_set( &p, 4, pt );
  module_polygon( xwingWing, &p );
  module_translate( xwingWing, 0, 0.5, 0 );
  module_polygon( xwingWing, &p );
  
  module_identity(xwingWing);
  module_translate( xwingWing, 3, 1.6, -1 );
  module_module( xwingWing, engine );

  module_identity(xwingWing);
  point_set3D( &pt[0], 15.0, 0.0, 3.0 );
  point_set3D( &pt[1], 15.0, 0.0, 0.0 );
  point_set3D( &pt[2], 15.0, 0.5, 0.0 );
  point_set3D( &pt[3], 15.0, 0.5, 3.0 );
  polygon_set( &p, 4, pt );
  module_polygon( xwingWing, &p );

  point_set3D( &pt[0], 15.0, 0.0, 0.0 );
  point_set3D( &pt[1], 0.0, 0.0, 0.0 );
  point_set3D( &pt[2], 0.0, 0.5, 0.0 );
  point_set3D( &pt[3], 15.0, 0.5, 0.0 );
  polygon_set( &p, 4, pt );
  module_polygon( xwingWing, &p );

  point_set3D( &pt[0], 15.0, 0.0, 3.0 );
  point_set3D( &pt[1], 15.0, 0.5, 3.0 );
  point_set3D( &pt[2], 0.0, 0.5, 5.0 );
  point_set3D( &pt[3], 0.0, 0.0, 5.0 );
  polygon_set( &p, 4, pt );
  module_polygon( xwingWing, &p );

  module_translate( xwingWing, 15, 0.25, 0 );
  module_module( xwingWing, laser );

  // 4 xwingWings
  xwingWings = module_create();
  module_rotateZ(xwingWings, cos( 0.3 ), sin( 0.3 ) );
  module_translate(xwingWings, xwingBodyWidth, 0, 0 );
  module_module( xwingWings, xwingWing );

  module_identity( xwingWings );
  module_scale( xwingWings, 1, -1, 1 );
  module_rotateZ(xwingWings, cos( -0.3) , sin(-0.3) );
  module_translate(xwingWings, xwingBodyWidth, 0, 0 );
  module_module( xwingWings, xwingWing );

  module_identity( xwingWings );
  module_scale( xwingWings, -1, 1, 1 );
  module_rotateZ(xwingWings, cos( -0.3) , sin(-0.3) );
  module_translate( xwingWings, -xwingBodyWidth, 0, 0 );
  module_module( xwingWings, xwingWing );

  module_identity( xwingWings );
  module_scale( xwingWings, -1, -1, 1 );
  module_rotateZ(xwingWings, cos( 0.3) , sin(0.3) );
  module_translate( xwingWings, -xwingBodyWidth, 0, 0 );
  module_module( xwingWings, xwingWing );

  // xwingBody
  xwingBody = module_create();

  module_module( xwingBody, xwingWings );

  module_scale(xwingBody, xwingBodyWidth, xwingBodyWidth, 8 );
  module_translate(xwingBody, 0, 0, 3 );
  cube( xwingBody );

  module_identity(xwingBody);
  point_set3D( &pt[0], xwingBodyWidth, xwingBodyWidth, 12 );
  point_set3D( &pt[1], xwingBodyWidth, -xwingBodyWidth, 12 );
  point_set3D( &pt[2], xwingBodyWidth*0.5, -xwingBodyWidth*0.3, 35 );
  point_set3D( &pt[3], xwingBodyWidth*0.5, xwingBodyWidth*0.3, 35 );
  polygon_set( &p, 4, pt );
  module_polygon( xwingBody, &p );

  point_set3D( &pt[0], -xwingBodyWidth, xwingBodyWidth, 12 );
  point_set3D( &pt[1], -xwingBodyWidth, -xwingBodyWidth, 12 );
  point_set3D( &pt[2], -xwingBodyWidth*0.5, -xwingBodyWidth*0.3, 35 );
  point_set3D( &pt[3], -xwingBodyWidth*0.5, xwingBodyWidth*0.3, 35 );
  polygon_set( &p, 4, pt );
  module_polygon( xwingBody, &p );

  point_set3D( &pt[0], -xwingBodyWidth, xwingBodyWidth, 12 );
  point_set3D( &pt[1],  xwingBodyWidth, xwingBodyWidth, 12 );
  point_set3D( &pt[2],  xwingBodyWidth*0.5, xwingBodyWidth*0.3, 35 );
  point_set3D( &pt[3], -xwingBodyWidth*0.5, xwingBodyWidth*0.3, 35 );
  polygon_set( &p, 4, pt );
  module_polygon( xwingBody, &p );

  point_set3D( &pt[0], -xwingBodyWidth, -xwingBodyWidth, 12 );
  point_set3D( &pt[1],  xwingBodyWidth, -xwingBodyWidth, 12 );
  point_set3D( &pt[2],  xwingBodyWidth*0.5, -xwingBodyWidth*0.3, 35 );
  point_set3D( &pt[3], -xwingBodyWidth*0.5, -xwingBodyWidth*0.3, 35 );
  polygon_set( &p, 4, pt );
  module_polygon( xwingBody, &p );

  point_set3D( &pt[0], -xwingBodyWidth*0.5,  xwingBodyWidth*0.3, 35 );
  point_set3D( &pt[1],  xwingBodyWidth*0.5,  xwingBodyWidth*0.3, 35 );
  point_set3D( &pt[2],  xwingBodyWidth*0.5, -xwingBodyWidth*0.3, 35 );
  point_set3D( &pt[3], -xwingBodyWidth*0.5, -xwingBodyWidth*0.3, 35 );
  polygon_set( &p, 4, pt );
  module_polygon( xwingBody, &p );

  // scene with three ships (xwingBody module)
  scene = module_create();
  module_scale( scene, 0.7, 0.7, 0.7 );
  module_rotateZ( scene, cos( 0.5 ), sin( 0.5 ) );
  module_rotateY( scene, cos(-0.1), sin(-0.1) );
  module_rotateX( scene, cos( 0.2), sin(0.2) );
  module_translate( scene, -15, -10, -10 );
  module_module( scene, xwingBody );

  module_identity(scene);
  module_scale( scene, 0.7, 0.7, 0.7 );
  module_rotateZ( scene, cos( 0.3 ), sin( 0.3 ) );
  module_rotateY( scene, cos(-0.05), sin(-0.05) );
  module_rotateX( scene, cos( 0.2), sin(0.2) );
  module_translate( scene, 5, -1, -50 );
  module_module( scene, xwingBody );

  module_identity(scene);
  module_scale( scene, 0.7, 0.7, 0.7 );
  module_rotateZ( scene, cos( 0.1 ), sin( 0.1 ) );
  module_rotateX( scene, cos( 0.2), sin(0.2) );
  module_translate( scene, -35, -3, -60 );
  module_module( scene, xwingBody );

  // create the image and drawstate
  src = image_create( 360, 640 );
  ds = drawstate_create();
  ds->shade = ShadeFrame;

  // draw into the scene
  module_draw( scene, &vtm, &gtm, ds, NULL, src );

	// write out the scene
  image_write( src, "xwingWings.ppm" );

	// free the polygon data
  polygon_clear( &p );
	// free the modules
  module_delete( scene );
  module_delete( xwingWing );
  module_delete( xwingWings );
  module_delete( laser );
  module_delete( xwingBody );
  module_delete( engine );

	// free the drawstate
	free(ds);

	// free the image
  image_free( src );

  return(0);
}
