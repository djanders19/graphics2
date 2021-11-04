/**
 * David J Anderson - November 2021
 * 
 * Implements drawstate.h, allowing for the user to define various features of a
 * "drawstate" object stipulating how a scene, as defined using the functions in
 * modeling.h, will be drawn when drawn using module_draw() in modeling.h.
 */

#include "graphicslib.h"

/* DRAWSTATE FUNCTIONS */

/**
 * Create a new DrawState and initialize the fields.
 */
DrawState *drawstate_create(void) {
    DrawState *toReturn = (DrawState *) malloc(sizeof(DrawState));
    if (!toReturn) {
        printf("drawstate_create(): malloc failed.\n");
        return NULL;
    }
    color_set(&(toReturn->body), 0.46, 0.46, 0.46); // Set to middle gray
    color_set(&(toReturn->color), 1.0, 1.0, 1.0); // Set to white
    color_set(&(toReturn->flatColor), 0.46, 0.46, 0.46); // Set to middle gray
    color_set(&(toReturn->surface), 0.46, 0.46, 0.46); // Set to middle gray
    toReturn->shade = ShadeConstant; // Default drawing mode - use fg color
    point_set3D(&(toReturn->viewer), 1.0, 1.0, 1.0); // default VRP = (1,1,1)
    toReturn->surfaceCoeff = 0.5;
    toReturn->zBufferFlag = 1;
    return toReturn;
}

/**
 * Set the color field to c.
 */
void drawstate_setColor(DrawState *s, Color c) {
    if (!s) {
        printf("drawstate_setColor(): passed NULL pointer.\n");
        return;
    }
    color_copy(&(s->color), &c);
}

/**
 * Set the body field to c.
 */
void drawstate_setBody(DrawState *s, Color c) {
    if (!s) {
        printf("drawstate_setBody(): passed NULL pointer.\n");
        return;
    }
    color_copy(&(s->body), &c);
}

/**
 * Set the surface field to c.
 */
void drawstate_setSurface(DrawState *s, Color c) {
    if (!s) {
        printf("drawstate_setSurface(): passed NULL pointer.\n");
        return;
    }
    color_copy(&(s->surface), &c);
}

/**
 * Set the surfaceCoeff field to f.
 */
void drawstate_setSurfaceCoeff(DrawState *s, float f) {
    if (!s) {
        printf("drawstate_setSurfaceCoeff(): passed NULL pointer.\n");
        return;
    }
    s->surfaceCoeff = f;
}

/**
 * Copy the DrawState data.
 */
void drawstate_copy(DrawState *to, DrawState *from) {
    if (!to || !from) {
        printf("drawstate_copy(): passed NULL pointer.\n");
        return;
    }

    color_copy(&(to->color), &(from->color));
    color_copy(&(to->flatColor), &(from->flatColor));
    color_copy(&(to->body), &(from->body));
    color_copy(&(to->surface), &(from->surface));
    to->shade = from->shade;
    to->zBufferFlag = from->zBufferFlag;
    to->surfaceCoeff = from->surfaceCoeff;
    point_copy(&(to->viewer), &(from->viewer));
}