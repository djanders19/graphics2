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
    b->subdivisions = 0;
}

/**
 * sets the zbuffer flag to 1 and the surface to the X-Z plane between (0, 0) 
 * and (1, 1).
 */
void bezierSurface_init(BezierSurface *b) {
    b->subdivisions = 0; // Zero unless set by bezierSurface_draw_with_subdivisions
    b->zBuffer = 1;
    
    // X-axis points
    point_set3D(&(b->ctrls[0]), 0.0, 0.0, 0.0);
    point_set3D(&(b->ctrls[1]), 0.33, 0.0, 0.0);
    point_set3D(&(b->ctrls[2]), 0.66, 0.0, 0.0);
    point_set3D(&(b->ctrls[3]), 1.0, 0.0, 0.0);

    // 1st set up z-axis
    point_set3D(&(b->ctrls[4]), 0.0, 0.0, 0.33);
    point_set3D(&(b->ctrls[5]), 0.33, 0.0, 0.33);
    point_set3D(&(b->ctrls[6]), 0.66, 0.0, 0.33);
    point_set3D(&(b->ctrls[7]), 1.0, 0.0, 0.33);

    // 2nd set up z-axis
    point_set3D(&(b->ctrls[8]), 0.0, 0.0, 0.66);
    point_set3D(&(b->ctrls[9]), 0.33, 0.0, 0.66);
    point_set3D(&(b->ctrls[10]), 0.66, 0.0, 0.66);
    point_set3D(&(b->ctrls[11]), 1.0, 0.0, 0.66);

    // 3rd set up z-axis
    point_set3D(&(b->ctrls[12]), 0.0, 0.0, 1.0);
    point_set3D(&(b->ctrls[13]), 0.33, 0.0, 1.0);
    point_set3D(&(b->ctrls[14]), 0.66, 0.0, 1.0);
    point_set3D(&(b->ctrls[15]), 1.0, 0.0, 1.0);
}

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
 * Copy the information in curve from to curve to.
 */
void bezierCurve_copy(BezierCurve *to, BezierCurve *from) {
    if (!to || !from) {
        printf("bezierCurve_copy(): passed NULL pointer as argument.\n");
        return;
    }

    for (int i = 0; i < 4; i++) {
        point_copy(&(to->ctrls[i]), &(from->ctrls[i]));
    }
    to->subdivisions = from->subdivisions;
    to->zBuffer = from->zBuffer;
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

void bezierSurface_copy(BezierSurface *to, BezierSurface *from) {
    if (!to || !from) {
        printf("bezierSurface_copy(): passed NULL pointer as argument.\n");
        return;
    }

    for (int i = 0; i < 16; i++) {
        point_copy(&(to->ctrls[i]), &(from->ctrls[i]));
    }
    to->subdivisions = from->subdivisions;
    to->zBuffer = from->zBuffer;
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
 * given color. The function is adaptive so that it uses an appropriate 
 * number of line segments to draw the curve. Specifically, if the diagonal of
 * the bounding box defined by the first and last line on the curve is shorter
 * than 10 pixels, the function ceases to subdivide.
 */
void bezierCurve_draw(BezierCurve *b, Image *src, Color c) {
    Line *l = malloc(sizeof(Line));

    // Calculate diagonal of bounding box:
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
    left->ctrls[1].val[2] = (b->ctrls[0].val[2] + b->ctrls[1].val[2]) / 2;// q1z
    
    left->ctrls[2].val[0] = ((left->ctrls[1].val[0]) / 2) +
                            ((b->ctrls[1].val[0] + b->ctrls[2].val[0]) / 4);
    left->ctrls[2].val[1] = ((left->ctrls[1].val[1]) / 2) +
                            ((b->ctrls[1].val[1] + b->ctrls[2].val[1]) / 4);
    left->ctrls[2].val[2] = ((left->ctrls[1].val[2]) / 2) +
                            ((b->ctrls[1].val[2] + b->ctrls[2].val[2]) / 4);

    // Define right curve ctl points:
    point_copy(&(right->ctrls[3]), &(b->ctrls[3])); // r3
    right->ctrls[2].val[0] = (b->ctrls[2].val[0] + b->ctrls[3].val[0]) / 2;// r1x
    right->ctrls[2].val[1] = (b->ctrls[2].val[1] + b->ctrls[3].val[1]) / 2;// r1y
    right->ctrls[2].val[2] = (b->ctrls[2].val[2] + b->ctrls[3].val[2]) / 2;// r1y

    right->ctrls[1].val[0] = ((right->ctrls[2].val[0]) / 2) +
                            ((b->ctrls[1].val[0] + b->ctrls[2].val[0]) / 4);
    right->ctrls[1].val[1] = ((right->ctrls[2].val[1]) / 2) +
                            ((b->ctrls[1].val[1] + b->ctrls[2].val[1]) / 4);
    right->ctrls[1].val[2] = ((right->ctrls[2].val[2]) / 2) +
                            ((b->ctrls[1].val[2] + b->ctrls[2].val[2]) / 4);

    // Define point where two curves meet:
    left->ctrls[3].val[0] = (left->ctrls[2].val[0] + right->ctrls[1].val[0]) / 2;
    left->ctrls[3].val[1] = (left->ctrls[2].val[1] + right->ctrls[1].val[1]) / 2;
    left->ctrls[3].val[2] = (left->ctrls[2].val[2] + right->ctrls[1].val[2]) / 2;
    point_copy(&(right->ctrls[0]), &(left->ctrls[3]));

    // Recursively draw each side:
    bezierCurve_draw(left, src, c);
    bezierCurve_draw(right, src, c);

    free(l);
    free(left);
    free(right);
}

/**
 * Draw a bezier curve with the specified number of subdivisions. Note that
 * setting subdivisions too high can cause breaks in the curve when rendered due
 * to rounding errors in the point values as the subdivisions become very fine. 
 * This is preventable by setting the safetyFlag to 1 - this causes the curve to
 * be drawn if the points are closer than 10 pixels to each other, as in the 
 * naive bezierCurve_draw function.
 */
void bezierCurve_draw_with_subdivisions(BezierCurve *b, int divisions,
                                        int safetyFlag, Image *src, Color c) {
    Line *l = malloc(sizeof(Line));
    float distance = sqrt(
        (b->ctrls[2].val[0] - b->ctrls[1].val[0]) * (b->ctrls[2].val[0] - b->ctrls[1].val[0]) +
        (b->ctrls[2].val[1] - b->ctrls[1].val[1]) * (b->ctrls[2].val[1] - b->ctrls[1].val[1])
    );

    // If using in safe mode, draw when points are less than 10.0 units from
    // each other to prevent tearing:
    if (safetyFlag && distance < 10.0) {
        line_set(l, b->ctrls[0], b->ctrls[1]);
        line_draw(l, src, c);
        line_set(l, b->ctrls[1], b->ctrls[2]);
        line_draw(l, src, c);
        line_set(l, b->ctrls[2], b->ctrls[3]);
        line_draw(l, src, c);
        free(l);
        return;
    }

    // Otherwise, subdivide all the way down to zero:
    if (divisions == 0) {
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
    bezierCurve_draw_with_subdivisions(left, divisions - 1, safetyFlag, src, c);
    bezierCurve_draw_with_subdivisions(right, divisions - 1, safetyFlag, src, c);

    free(l);
    free(left);
    free(right);
}

void subdivide(Point *vals, Point *toReturn) {
    // Define left curve ctl points:
    point_copy(&(toReturn[0]), &(vals[0])); // q0
    toReturn[1].val[0] = (vals[0].val[0] + vals[1].val[0]) / 2;// q1x
    toReturn[1].val[1] = (vals[0].val[1] + vals[1].val[1]) / 2;// q1y
    toReturn[1].val[2] = (vals[0].val[2] + vals[1].val[2]) / 2;// q1z
    
    toReturn[2].val[0] = ((toReturn[1].val[0]) / 2) +
                            ((vals[1].val[0] + vals[2].val[0]) / 4);
    toReturn[2].val[1] = ((toReturn[1].val[1]) / 2) +
                            ((vals[1].val[1] + vals[2].val[1]) / 4);
    toReturn[2].val[2] = ((toReturn[1].val[2]) / 2) +
                            ((vals[1].val[2] + vals[2].val[2]) / 4);

    // Define right curve ctl points:
    point_copy(&(toReturn[7]), &(vals[3])); // r3
    toReturn[6].val[0] = (vals[2].val[0] + vals[3].val[0]) / 2;// r1x
    toReturn[6].val[1] = (vals[2].val[1] + vals[3].val[1]) / 2;// r1y
    toReturn[6].val[2] = (vals[2].val[2] + vals[3].val[2]) / 2;// r1y

    toReturn[5].val[0] = ((toReturn[2].val[0]) / 2) +
                            ((vals[1].val[0] + vals[2].val[0]) / 4);
    toReturn[5].val[1] = ((toReturn[2].val[1]) / 2) +
                            ((vals[1].val[1] + vals[2].val[1]) / 4);
    toReturn[5].val[2] = ((toReturn[2].val[2]) / 2) +
                            ((vals[1].val[2] + vals[2].val[2]) / 4);

    // Define point where two curves meet:
    toReturn[3].val[0] = (toReturn[2].val[0] + toReturn[5].val[0]) / 2;
    toReturn[3].val[1] = (toReturn[2].val[1] + toReturn[5].val[1]) / 2;
    toReturn[3].val[2] = (toReturn[2].val[2] + toReturn[5].val[2]) / 2;
    point_copy(&(toReturn[4]), &(toReturn[3]));
}