/**
 * David J Anderson - November 2021
 * 
 * Defines headers and Object Types allowing for hierarchical modeling of
 * graphical scenes as a traversable DAG.
 */
#ifndef MODELING_H

#define MODELING_H
#include "graphicslib.h"

/* The following code is borrowed from the system specification defined by
Professor Bruce Maxwell, Roux Institute Fall 2021 */

typedef enum { // enum of different types of objects that can be in modules
    ObjNone,
    ObjLine,
    ObjPoint,
    ObjPolyline,
    ObjPolygon,
    ObjIdentity,
    ObjMatrix,
    ObjColor,
    ObjBodyColor,
    ObjSurfaceColor,
    ObjSurfaceCoeff,
    ObjLight,
    ObjModule,
    ObjBezier
} ObjectType;

/* Union defining possible contents of Element objects */
typedef union {
    Point point;
    Line line;
    Polyline polyline;
    Polygon polygon;
    Matrix matrix;
    Color color;
    float coeff;
    BezierCurve curve;
    void *module;
} Object;

// Module structure
/* Define the Elements of the Module */
typedef struct {
    ObjectType type;
    Object obj;
    void *next;
} Element;

/* Modules themselves are simply double endedlinked lists */
typedef struct {
    Element *head; // pointer to the head of the linked list
    Element *tail; // keep around a pointer to the last object
} Module;

/* 2D AND GENERIC MODULE FUNCTIONS */
Element *element_create(void);
Element *element_init(ObjectType type, void *obj);
void element_delete(Element *e);
Module *module_create(void);
void module_clear(Module *md);
void module_delete(Module *md);
void module_insert(Module *md, Element *e);
void module_module(Module *md, Module *sub);
void module_point(Module *md, Point *p);
void module_line(Module *md, Line *p);
void module_polyline(Module *md, Polyline *p);
void module_polygon(Module *md, Polygon *p);
void module_identity(Module *md);
void module_translate2D(Module *md, double tx, double ty);
void module_scale2D(Module *md, double sx, double sy);
void module_rotateZ(Module *md, double cth, double sth);
void module_shear2D(Module *md, double shx, double shy);
void module_draw(Module *md, Matrix *VTM, Matrix *GTM, DrawState *ds, Lighting *lighting, Image *src);

/* 3D MODULE FUNCTIONS */
void module_translate(Module *md, double tx, double ty, double tz);
void module_scale(Module *md, double sx, double sy, double sz);
void module_rotateX(Module *md, double cth, double sth);
void module_rotateY(Module *md, double cth, double sth);
void module_rotateXYZ(Module *md, Vector *u, Vector *v, Vector *w);
void module_cube(Module *md, int solid);

/* BEZIER CURVE AND SURFACE FUNCTIONS */
void module_bezierCurve(Module *m, BezierCurve *b, int divisions);
void module_bezierSurface(Module *m, BezierSurface *b, int divisions, int solid);
void module_cylinder(Module *md, int sides);
void module_pyramid(Module *md, int solid);
void module_sphere(Module *md, int solid);

/* SHADING/COLOR MODULE FUNCTIONS */
void module_color(Module *md, Color *c);
void module_bodyColor(Module  *md, Color *c);
void module_surfaceColor(Module *md, Color *c);
void module_surfaceCoeff(Module *md, float coeff);
#endif