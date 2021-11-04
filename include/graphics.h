/**
 * Graphics.h - david Anderson, Sept 30th 2021.
 * 
 * Headers for a simple 2d graphics library implementing lines, polylines,
 * circles, and ellipses. Also includes function prototypes. Adapted from spec
 * provided by Bruce Maxwell at the Roux Institute, Sept 2021.
 */
#ifndef GRAPHICS_H
#define GRAPHICS_H
#include "color.h"
#include "image.h"
#include "fpixel.h"


typedef struct {
    double val[4]; // four element vector of doubles
} Point;

typedef struct {
    double val[3];
} Vector; // Two or three element representing a direction in space

typedef struct {
    int zBuffer; // Whether to use zBuffer - default to true (1)
    Point a; // Start
    Point b; // end
} Line;


typedef struct {
    double r; // radius
    Point c; // center
} Circle;

typedef struct {
    double ra; // major radius axis
    double rb; // minor radius axis
    Point c; // center
    double a; // Optional angle of major axis relative to the x-axis
} Ellipse;


typedef struct {
    int zBuffer; // whether to use the z-buffer; should default to true (1)
    int numVertex; // number of vertices
    Point *vertex; // array of vertices and their info
} Polyline;

/**
 * Struct for passing arguments of line_draw to threads when pawning off
 * work onto them.
 */
struct args {
    Line *l;
    Image *src;
    Color c;
};


/* PRIMITIVE FUNCTIONS */
/* POINT PRIMITIVES */
void point_set2D(Point *p, double x, double y); 
void point_set3D(Point *p, double x, double y, double z);
void point_set(Point *p, double x, double y, double z, double h);
void point_normalize(Point *p);
void point_copy(Point *to, Point *from);
void point_draw(Point *p, Image *src, Color c);
void point_drawf(Point *p, Image *src, FPixel c);
void point_print(Point *p, FILE *fp);

/* LINE PRIMITIVES */
void line_set2D(Line *l, double x0, double y0, double x1, double y1);
void line_set(Line *l, Point ta, Point tb);
void line_zBuffer(Line *l, int flag);
void line_normalize(Line *l);
void line_copy(Line *to, Line *from);
void line_draw(Line *l, Image *src, Color c);

/* CIRCLE PRIMITIVES */
void circle_set(Circle *c, Point tc, double tr);
void circle_draw(Circle *c, Image *src, Color p);
void circle_drawFill(Circle *c, Image *src, Color p);

/* ELLIPSE PRIMITIVES */
void ellipse_set(Ellipse *e, Point tc, double ta, double tb);
void ellipse_draw(Ellipse *e, Image *src, Color p);
void ellipse_drawFill(Ellipse *e, Image *src, Color p);

/* POLYLINE PRIMITIVES */
Polyline *polyline_create(void);
Polyline *polyline_createp(int numV, Point *vlist);
void polyline_free(Polyline *p);
void polyline_init(Polyline *p);
void polyline_set(Polyline *p, int numV, Point *vlist);
void polyline_clear(Polyline *p);
void polyline_zBuffer(Polyline *p, int flag);
void polyline_copy(Polyline *to, Polyline *from);
void polyline_print(Polyline *p, FILE *fp);
void polyline_normalize( Polyline *p );
void polyline_draw(Polyline *p, Image *src, Color c);

/* VECTOR FUNCTIONS */
void vector_set(Vector *v, double x, double y, double z);
void vector_print(Vector *v, FILE *fp);
void vector_copy(Vector *dest, Vector *src);
double vector_length(Vector *v);
void vector_normalize(Vector *v);
double vector_dot(Vector *a, Vector *b);
void vector_cross(Vector *a, Vector *b, Vector *c);

#endif