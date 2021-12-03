/**
 * polygon.h
 * 
 * David J. Anderson, adapted from Lecture notes/spec provided by Bruce Maxwell
 * at the Roux Institute, F'2021.
 * 
 * Provides data struct for polygon primitives in a graphics library, as well as
 * definitions of functions to create, dispose of, edit, and draw polygons.
 * 
 * Last updated 10/11/2021
 */

#ifndef POLYGON_H

#define POLYGON_H

#include "graphicslib.h"


/**
 * Polygons are somewhat more complex than other primitives, so for the time
 * being I'm breaking them out into their own header. Basically, these are
 * similar to polylines except they can be filled with a color. It's always
 * assumed that the last vertex (last element of whatever is pointed to by 
 * *vertex) connects to the first.
 */
typedef struct {
    /* Whether to convert the polygon to one sided (1) or two-sided (0) for
       shading */
    int oneSided;
    int nVertex; // number of vertices
    Point *vertex; // vertex information/list
    Color *color; // Color information for each vertex
    Vector *normal; // Surface normal information for each vertex
    int zBuffer; // Whether to use the z-buffer - should default to true (1)
} Polygon;

Polygon *polygon_create(void);
Polygon *polygon_createp(int numV, Point *vlist);
void polygon_free(Polygon *p);
void polygon_init(Polygon *p);
void polygon_set(Polygon *p, int numV, Point *vlist);
void polygon_clear(Polygon *p);
void polygon_setSided(Polygon *p, int oneSided);
void polygon_copy(Polygon *to, Polygon *from);
void polygon_print(Polygon *p, FILE *fp);
void polygon_normalize(Polygon *p);
void polygon_draw(Polygon *p, Image *src, Color c);
void polygon_drawFill(Polygon *p, Image *src, Color c, DrawState* ds);
void polygon_drawShade(Polygon *p, Image *src, DrawState *ds, Lighting *light);
void polygon_drawFill_SuperSampled(Polygon *p, Image *src, Color c, DrawState* ds);
void polygon_drawFillB(Polygon *p, Image *src, Color c);

/* Utility functions */
void polygon_shade(Polygon *p, Lighting *lighting, DrawState *ds);

#endif