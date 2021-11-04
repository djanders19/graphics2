#include "graphicslib.h"
#include "bezier.h"


/**
 * Set the zBuffer flag to 1 and the curve points to the x-axis between 0 and 1.
 */
void bezierCurve_init(BezierCurve *b) {
    if (!b) {
        printf("bezierCurve_init(): passed NULL pointer as argument.\n");
        return;
    }

    b->zBuffer = 1;
    point_set3D(&(b->ctrls[0]), 0.0, 0.0, 0.0);
    point_set3D(&(b->ctrls[1]), 0.33, 0.0, 0.0);
    point_set3D(&(b->ctrls[2]), 0.66, 0.0, 0.0);
    point_set3D(&(b->ctrls[3]), 1.0, 0.0, 0.0);

}

/**
 * sets the zbuffer flag to 1 and the surface to the X-Z plane between (0, 0) 
 * and (1, 1).
 */
void bezierSurface_init(BezierSurface *b);

/**
 * sets the control points of the BezierCurve to the four points in the vlist
 * array.
 */
void bezierCurve_set(BezierCurve *b, Point *vlist) {
    if (!b || !vlist) {
        printf("bezierCurve_set(): passed NULL pointer as argument.\n");
        return;
    }

    for (int i = 0; i < 4; i++) {
        point_copy(&(b->ctrls[i]), &(vlist[i]));
    }
}

/**
 * Sets the control points of the BezierSurface to the 16 points in the vlist 
 * array.
 */
void bezierSurface_set(BezierSurface *b, Point *vlist) {
    if (!b || !vlist) {
        printf("bezierSurface_set(): passed NULL pointer as argument.\n");
        return;
    }

    for (int i = 0; i < 16; i++) {
        point_copy(&(b->ctrls[i]), &(vlist[i]));
    }
}

/**
 * sets the control points of the BezierCurve to the four points in the vlist 
 * array.
 */
void bezierCurve_zBuffer(BezierCurve *b, int flag) {
    if (!b) {
        printf("bezierCurve_zBuffer(): passed NULL pointer as argument.\n");
        return;
    }

    b->zBuffer = flag;
}

/**
 * sets the z-buffer flag to the given value.
 */
void bezierSurface_zBuffer(BezierCurve *b, int flag) {
    if (!b) {
        printf("bezierSurface_zBuffer(): passed NULL pointer as argument.\n");
        return;
    }

    b->zBuffer = flag;
}

/**
 * Draws the Bezier curve, given in screen coordinates, into the image using the
 * given color. The function should be adaptive so that it uses an appropriate 
 * number of line segments to draw the curve. For example, if the bounding box 
 * of the control points is less than 10 pixels in the largest dimension, then 
 * it is reasonable to draw the lines between the control points as an 
 * approximation to the curve.
 */
void bezierCurve_draw(BezierCurve *b, Image *src, Color c) {
    Line *l = malloc(sizeof(Line));
    float distance = sqrt(
        (b->ctrls[2].val[0] - b->ctrls[1].val[0]) * (b->ctrls[2].val[0] - b->ctrls[1].val[0]) +
        (b->ctrls[2].val[1] - b->ctrls[1].val[1]) * (b->ctrls[2].val[1] - b->ctrls[1].val[1])
    );

    if (distance < 10.0) {
        line_set(l, b->ctrls[0], b->ctrls[1]);
        line_draw(l, src, c);
        line_set(l, b->ctrls[1], b->ctrls[2]);
        line_draw(l, src, c);
        line_set(l, b->ctrls[2], b->ctrls[3]);
        line_draw(l, src, c);
        free(l);
        return;
    }

    // Split the curve into two bezier curves and draw those recursively:
    BezierCurve *left = malloc(sizeof(BezierCurve));
    BezierCurve *right = malloc(sizeof(BezierCurve));

    bezierCurve_init(left);
    bezierCurve_init(right);

    // Define left curve ctl points:
    point_copy(&(left->ctrls[0]), &(b->ctrls[0])); // q0
    left->ctrls[1].val[0] = (b->ctrls[0].val[0] + b->ctrls[1].val[0]) / 2;// q1x
    left->ctrls[1].val[1] = (b->ctrls[0].val[1] + b->ctrls[1].val[1]) / 2;// q1y

    left->ctrls[2].val[0] = ((left->ctrls[1].val[0]) / 2) +
                            ((b->ctrls[1].val[0] + b->ctrls[2].val[0]) / 4);
    left->ctrls[2].val[1] = ((left->ctrls[1].val[1]) / 2) +
                            ((b->ctrls[1].val[1] + b->ctrls[2].val[1]) / 4);
    
    // Define right curve ctl points:
    point_copy(&(right->ctrls[3]), &(b->ctrls[3])); // r3
    right->ctrls[2].val[0] = (b->ctrls[2].val[0] + b->ctrls[3].val[0]) / 2;// r1x
    right->ctrls[2].val[1] = (b->ctrls[2].val[1] + b->ctrls[3].val[1]) / 2;// r1y

    right->ctrls[1].val[0] = ((right->ctrls[2].val[0]) / 2) +
                            ((b->ctrls[1].val[0] + b->ctrls[2].val[0]) / 4);
    right->ctrls[1].val[1] = ((right->ctrls[2].val[1]) / 2) +
                            ((b->ctrls[1].val[1] + b->ctrls[2].val[1]) / 4);

    // Define point where two curves meet:
    left->ctrls[3].val[0] = (left->ctrls[2].val[0] + right->ctrls[1].val[0]) / 2;
    left->ctrls[3].val[1] = (left->ctrls[2].val[1] + right->ctrls[1].val[1]) / 2;
    point_copy(&(right->ctrls[0]), &(left->ctrls[3]));

    // Recursively draw each side:
    bezierCurve_draw(left, src, c);
    bezierCurve_draw(right, src, c);

    free(l);
    free(left);
    free(right);
}
