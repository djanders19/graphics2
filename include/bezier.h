/**
 * David J Anderson - 11/4/2021
 * 
 * This file defines Data Types for Bezier Curves and Surfaces.
 * In addition, it defines headers for a variety of functions to manipulate
 * these data types.
 */
#ifndef BEZIER_H

#define BEZIER_H

typedef struct {
    Point ctrls[4]; // 4 control points for a curve
    int zBuffer;
    int subdivisions; // Subdivision cutoff. Used by module_BezierCurve
} BezierCurve;

typedef struct {
    Point ctrls[16]; // 16 control points for a surface
    int zBuffer;
    int subdivisions; // Subdivision cutoff. Used by module_BezierSurface
} BezierSurface;

/* BEZIER CURVE AND SURFACE FUNCTIONS */
void bezierCurve_init(BezierCurve *b);
void bezierSurface_init(BezierSurface *b);
void bezierCurve_set(BezierCurve *b, Point *vlist);
void bezierSurface_set(BezierSurface *b, Point *vlist);
void bezierCurve_zBuffer(BezierCurve *p, int flag);
void bezierSurface_zBuffer(BezierCurve *p, int flag);
void bezierCurve_draw(BezierCurve *b, Image *src, Color c);
void bezierCurve_draw_with_subdivisions(BezierCurve *b, int divisions, int safetyFlag, Image *src, Color c);
void bezierCurve_copy(BezierCurve *to, BezierCurve *from);
#endif