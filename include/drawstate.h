/**
 * David J Anderson - November 2021
 * Defines headers and data structures to represent the "drawing state" of a
 * scene - i.e. how a drawing program should draw that scene w/r/t colors,
 * reflection, and shading.
 */

#ifndef DRAWSTATE_H

#define DRAWSTATE_H

#include "graphicslib.h"

/* Enum naming the different shading methods */
typedef enum {
    ShadeFrame, // Draw only borders of objects
    ShadeConstant, // Draw objects using foreground color
    ShadeDepth, // Draw objects using their depth value
    ShadeFlat, // Draw objects using shading calcs, but each polygon is constant
    ShadeGouraud, // Draw using Gouraud shading
    ShadePhong // Draw using Phong shading
} ShadeMethod;

typedef struct {
    Color color; // Foreground color, used in the default drawing mode
    Color flatColor; // Flat-fill polygon color, used for shading calcs
    Color body; // body reflection color, used for shading calculations
    Color surface; // Surface reflection color, used for shading calcs
    ShadeMethod shade; // type of shading to use
    int zBufferFlag; // Whether to use the z-buffer hidden surface removal
    Point viewer; // A Point representing the view location in 3D
    float surfaceCoeff;
} DrawState;

/* DRAWSTATE FUNCTIONS */
DrawState *drawstate_create(void);
void drawstate_setColor(DrawState *s, Color c);
void drawstate_setBody(DrawState *s, Color c);
void drawstate_setSurface(DrawState *s, Color c);
void drawstate_setSurfaceCoeff(DrawState *s, float f);
void drawstate_copy(DrawState *to, DrawState *from);

#endif