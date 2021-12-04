/**
 * Tests the ability to do Gouroud shading using scanline fill on a bespoke
 * polygon, with all the vertices and normals carefully defined by hand.
 */

#include <stdio.h>
#include <stdlib.h>
#include <math.h>
#include "graphicslib.h"


int main(int argc, char *argv[]) {
    Image *src;
    Lighting *l = lighting_create();
    DrawState *ds = drawstate_create();
    Polygon *p = polygon_create();
    Color BlueGrey;
    Color Sun;
    Color Cb;
    Color Cs;
    Color c;
    Point V;
    Point lp;
    int rows = 500, cols = 500;

	color_set(&BlueGrey, 0.2, 0.25, 0.3);
	color_set(&Sun, 0.9, 0.85, 0.8);
	color_set(&Cb, 0.7, 0.2, 0.1);
	color_set(&Cs, 0.3, 0.3, 0.3);

	point_set(&V, 250.0, 250.0, 1.0, 1.0);
	point_set(&lp, 450.0, 450.0, 4.0, 1.0);


    // Set up ds:
    drawstate_setBody(ds, Cb);
    drawstate_setSurface(ds, Cs);
    drawstate_setSurfaceCoeff(ds, 10);
    drawstate_setColor(ds, Sun);
    ds->zBufferFlag = 1;
    ds->shade = ShadeGouraud;
    point_copy(&ds->viewer, &V);

    src = image_create(rows, cols);

    // add an ambient light and a point light, slightly below and to the right of the viewer
    l = lighting_create();
    lighting_add(l, LightAmbient, &BlueGrey, NULL, NULL, 0.0, 0.0);
    lighting_add(l, LightPoint, &Sun, NULL, &lp, 0.0, 0.0);

    // Define our polygon to draw:
    Point pts[4];
    point_set(&pts[0], 0.0, 0.0, 0.25, 1.0);
    point_set(&pts[1], 0.0, 200.0, 0.25, 1.0);
    point_set(&pts[2], 400.0, 300.0, 0.5, 1.0);
    point_set(&pts[3], 400.0, 100.0, 0.5, 1.0);

    polygon_set(p, 4, pts);
    vector_set(&p->normal[0], 0.0, 0.0, 1.0);
    vector_set(&p->normal[1], 0.0, 0.0, 1.0);
    vector_set(&p->normal[2], 0.0, 0.0, 1.0);
    vector_set(&p->normal[3], 0.0, 0.0, 1.0);
    p->oneSided = 0;
    printf("test(): Polygon is %d sided\n", p->oneSided);
    // Draw the polygon:
    polygon_drawShade(p, src, ds, l);

    image_write( src, "test_drawShade.ppm");

    polygon_free(p);
    free(ds);
    free(l);
    image_free(src);

    return(0);
}