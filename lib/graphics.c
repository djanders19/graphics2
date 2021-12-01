/**
 * graphics.c - David Anderson, Sept 30 2021
 * 
 * This implements graphics.h, which defines structs for several 2D-graphics
 * primitives and functions to be used to create, edit, and draw them into an
 * image. Specifically it handles lines, polylines, circles, and ellipses. The
 * code was written by David Anderson (davidias.j.a@gmail.com) and adapted from
 * lecture notes by Bruce Maxwell at the Roux Institute. Bresenham's algorithm
 * is used to draw lines, and circle/ellipse drawing code is adapted from Hearn
 * & Baker, "Computer Graphics with OpenGL"
 */
#include <stdio.h>
#include <stdlib.h>
#include <pthread.h>
#include <math.h>
#include "graphicslib.h"


/* POINT PRIMITIVES */

/**
 * Set the first two values of the vector to x and y. Se the third value to 0.0
 * and the fourth value to 1.0.
 */
void point_set2D(Point *p, double x, double y) {
    p->val[0] = x;
    p->val[1] = y;
    p->val[2] = 0.0;
    p->val[3] = 1.0;
}


/**
 * Set the point's values to x y and z. Set the homogenous coordinate to 1.0.
 */
void point_set3D(Point *p, double x, double y, double z) {
    p->val[0] = x;
    p->val[1] = y;
    p->val[2] = z;
    p->val[3] = 1.0;
}


/**
 * Set the four values of the vector to x, y, z, and h respectively.
 */
void point_set(Point *p, double x, double y, double z, double h) {
    p->val[0] = x;
    p->val[1] = y;
    p->val[2] = z;
    p->val[3] = h;
}

/**
 * Normalize the x and y values of point by the homogenous value.
 */
void point_normalize(Point *p) {
    p->val[0] = p->val[0] / p->val[3];
    p->val[1] = p->val[1] / p->val[3];
}


/**
 * Copy the point data structure "from" to the Point data structure "to"
 */
void point_copy(Point *to, Point *from) {
    to->val[0] = from->val[0];
    to->val[1] = from->val[1];
    to->val[2] = from->val[2];
    to->val[3] = from->val[3];
}

/**
 * Draw the point into the Image src using the color c.
 */
void point_draw(Point *p, Image *src, Color c) {
    image_setColor(src, p->val[1], p->val[0], c);
}


/**
 * Draw the point p into src using the FPixel c.
 */
void point_drawf(Point *p, Image *src, FPixel c) {
        image_setf(src, p->val[1], p->val[0], c);
}


/**
 * Uses fprintf to print the contents of the point to the stream fp.
 */
void point_print(Point *p, FILE *fp) {
    fprintf(fp, "Point (%f, %f, %f) with h = %f\n", p->val[0], p->val[1],\
                                                    p->val[2], p->val[3]);
}


/* LINE PRIMITIVES */

/**
 * Initialize a 2D line stretching from (x0, y0) to (x1, y1).
 */
void line_set2D(Line *l, double x0, double y0, double x1, double y1) {
    point_set2D(&l->a, x0, y0);
    point_set2D(&l->b, x1, y1);
}

/**
 * Initialize a line stretching from Point ta to Point tb.
 */
void line_set(Line *l, Point ta, Point tb) {
    l->a = ta;
    l->b = tb;
}

/**
 * Set the z-buffer value of a line to the given flag (generally 1 or 0,
 * indicating whether to use or ignore the buffer respectively)
 */
void line_zBuffer(Line *l, int flag) {
    l->zBuffer = flag;
}

/**
 * Normalize the x and y values of the endpoints by their homogenous coordinate.
 */
void line_normalize(Line *l) {
    point_normalize(&l->a);
    point_normalize(&l->b);
}

/**
 * Copy the line data from <from> to Line <to>.
 */
void line_copy(Line *to, Line *from) {
    point_copy(&(to->a), &(from->a));
    point_copy(&(to->b), &(from->b));
    to->zBuffer = from->zBuffer;
}

/**
 * Draw the line into the src image using color c and the z-buffer, if
 * appropriate. Drawing is accomplished using Bresenham's line drawing
 * algorithm.
 */
void line_draw(Line *l, Image *src, Color c) {
    int index = 0;
    int maxIndex = src->cols * src->rows;
    int xmax = src->cols;
    int i = 0;
    int x1 = l->b.val[0];
    int y1 = l->b.val[1];
    float z1 = 1 / l->b.val[2];

    // Initial coordinates for starting point; swap if swapping points
    int x = l->a.val[0];
    int y = l->a.val[1];
    float z = 1 / l->a.val[2];
    int dx = x1 - x; // x1 - x0
    int dy = y1 - y; // y1 - y0
    float dz = z1 - z; // z1 - z0, accounting for projection

    switch (l->zBuffer) {
        case 0:
            // Special case: vertical line
            if (dx == 0) {
                if (dy >= 0) {
                    // Bottom to top:
                    for (i = 0; i < dy; i++) {
                        // Color the pixel to the right
                        index = (src->cols * y) + x;
                        if (!(index < 0) && !(index > maxIndex) && 
                        x >= 0 && x < xmax) {
                            src->data[index].rgb[0] = c.c[0];
                            src->data[index].rgb[1] = c.c[1]; 
                            src->data[index].rgb[2] = c.c[2];
                        }
                        y = y + 1;
                    }
                } else {
                    // top to bottom
                    for (i = 0; i > dy; i--) {
                        // Color the pixel to the left
                        index = (src->cols * (y - 1)) + x - 1;
                        if (!(index < 0) && !(index > maxIndex) && 
                        x >= 0 && x < xmax) {
                            src->data[index].rgb[0] = c.c[0];
                            src->data[index].rgb[1] = c.c[1]; 
                            src->data[index].rgb[2] = c.c[2];
                        }
                        y = y - 1;
                    }
                }

                return;
            }

            // special case: drawing a horizontal line.
            if (dy == 0) {
                if (dx >= 0) {
                    // Left to right:
                    for (i = 0; i < dx; i++) {
                        // Color the pixel above theoretical axis
                        index = (src->cols * (y - 1)) + x;
                        if (!(index < 0) && !(index > maxIndex) && x < xmax) {
                            src->data[index].rgb[0] = c.c[0];
                            src->data[index].rgb[1] = c.c[1]; 
                            src->data[index].rgb[2] = c.c[2];
                        } else if (index > maxIndex) {
                            return; // stop drawing
                        }
                        x = x + 1;
                    }
                } else {
                    // right to left:
                    for (i = 0; i > dx; i--) {
                        // Color the pixel below the theoretical axis
                        // don't light up rightmost pixel
                        index = (src->cols * y) + x - 1;
                        if (!(index < 0) && !(index > maxIndex) && x < xmax) {
                            src->data[index].rgb[0] = c.c[0];
                            src->data[index].rgb[1] = c.c[1]; 
                            src->data[index].rgb[2] = c.c[2];
                        } else if (index < 0) {
                            return; // stop drawing, don't waste time
                        }
                        x = x - 1;
                    }
                }
                return;
            }

            if (dy < 0) {
                // Swap the points so we're drawing from bottom to top
                x1 = l->a.val[0];
                y1 = l->a.val[1];

                x = l->b.val[0];
                y = l->b.val[1];

                dy = y1 - y; // y1 - y0
                dx = x1 - x; // x1 - x0
            }

            int e_prime = 3 * dy - 2 * dx; // initialize e_prime

            if (dx > 0 && dx >= dy) {
                // first octant
                for (i = 0; i < dx; i++) {
                    // Color the pixel
                    // image_setColor(src, y, x, c);
                    index = (src->cols * y) + x;
                    if (!(index < 0) && !(index > maxIndex) && 
                        x >= 0 && x < xmax) {
                        src->data[index].rgb[0] = c.c[0];
                        src->data[index].rgb[1] = c.c[1]; 
                        src->data[index].rgb[2] = c.c[2];
                    }

                    while (e_prime > 0) {
                        y = y + 1; // Move up
                        e_prime = e_prime - 2 * dx; // subtract 2dx
                    }

                    // We always move across, so we add dy to our error:
                    x = x + 1;
                    e_prime = e_prime + 2 * dy; // move across, add dy
                }
                return;
            }

            if (dx > 0 && dx < dy) {
                // 2nd octant
                // flip roles of x and y
                e_prime = 3 * dx - 2 * dy; // initialize e_prime
                for (i = 0; i < dy; i++) {
                    // Color the pixel
                    // image_setColor(src, y, x, c);
                    index = (src->cols * y) + x;
                    if (!(index < 0) && !(index > maxIndex) && 
                        x >= 0 && x < xmax) {
                        src->data[index].rgb[0] = c.c[0];
                        src->data[index].rgb[1] = c.c[1]; 
                        src->data[index].rgb[2] = c.c[2];
                    }

                    while (e_prime > 0) {
                        x = x + 1; // Move across
                        e_prime = e_prime - 2 * dy; // subtract 2dy
                    }

                    // We always move up, so we add dy to our error:
                    y = y + 1;
                    e_prime = e_prime + 2 * dx; // move across, add dx
                }

                return;
            } 

            if (dx < 0 && dx < -dy) {
                // 3rd octant
                // Flip roles & subtract from x
                e_prime = 3 * dy + 2 * dx; // initialize e_prime
                for (i = 0; i > dx; i--) {
                    // Color the pixel
                    // image_setColor(src, y, x-1, c);
                    index = (src->cols * y) + x;
                    if (!(index < 0) && !(index > maxIndex) && 
                        x >= 0 && x < xmax) {
                        src->data[index].rgb[0] = c.c[0];
                        src->data[index].rgb[1] = c.c[1]; 
                        src->data[index].rgb[2] = c.c[2];
                    }

                    while (e_prime > 0) {
                        y = y + 1;
                        e_prime = e_prime + 2 * dx; // subtract 2dy
                    }

                    // We always move up, so we add dy to our error:
                    x = x - 1; // Move across
                    e_prime = e_prime + 2 * dy; // move across, add dx
                }
                return;
            } 

            if (dx < 0 && dx >= -dy) {
                // 4th octant
                // subtract from x
                e_prime = 3 * dx + 2 * dy; // initialize e_prime
                for (i = 0; i < dy; i++) {
                    // Color the pixel
                    // image_setColor(src, y, x-1, c);

                    index = (src->cols * y) + x;
                    if (!(index < 0) && !(index > maxIndex) && 
                        x >= 0 && x < xmax) {
                        src->data[index].rgb[0] = c.c[0];
                        src->data[index].rgb[1] = c.c[1]; 
                        src->data[index].rgb[2] = c.c[2];
                    }

                    while (e_prime < 0) {
                        x = x - 1; // Move across
                        e_prime = e_prime + 2 * dy; // subtract 2dx
                    }

                    // We always move across, so we add dy to our error:
                    y = y + 1;
                    e_prime = e_prime + 2 * dx; // move across, add dy
                }
                return;
            }

            // Shouldn't ever hit this, but useful for debugging:
            printf("Unhandled case\n\n\n");
            break;

        default:
            // In this case, we are using the z-buffer line drawing algorithm
            // Special case: vertical line
            if (dx == 0) {
                if (dy >= 0) {
                    // Bottom to top:
                    for (i = 0; i < dy; i++) {
                        // Color the pixel to the right
                        index = (src->cols * y) + x;
                        if (!(index < 0) && !(index > maxIndex) && 
                        x >= 0 && x < xmax) {
                            src->data[index].rgb[0] = c.c[0];
                            src->data[index].rgb[1] = c.c[1]; 
                            src->data[index].rgb[2] = c.c[2];
                        }
                        y = y + 1;
                    }
                } else {
                    // top to bottom
                    for (i = 0; i > dy; i--) {
                        // Color the pixel to the left
                        index = (src->cols * (y - 1)) + x - 1;
                        if (!(index < 0) && !(index > maxIndex) && 
                        x >= 0 && x < xmax) {
                            src->data[index].rgb[0] = c.c[0];
                            src->data[index].rgb[1] = c.c[1]; 
                            src->data[index].rgb[2] = c.c[2];
                        }
                        y = y - 1;
                    }
                }

                return;
            }

            // special case: drawing a horizontal line.
            if (dy == 0) {
                if (dx >= 0) {
                    // Left to right:
                    for (i = 0; i < dx; i++) {
                        // Color the pixel above theoretical axis
                        index = (src->cols * (y - 1)) + x;
                        if (!(index < 0) && !(index > maxIndex) && x < xmax) {
                            src->data[index].rgb[0] = c.c[0];
                            src->data[index].rgb[1] = c.c[1]; 
                            src->data[index].rgb[2] = c.c[2];
                        } else if (index > maxIndex) {
                            return; // stop drawing
                        }
                        x = x + 1;
                    }
                } else {
                    // right to left:
                    for (i = 0; i > dx; i--) {
                        // Color the pixel below the theoretical axis
                        // don't light up rightmost pixel
                        index = (src->cols * y) + x - 1;
                        if (!(index < 0) && !(index > maxIndex) && x < xmax) {
                            src->data[index].rgb[0] = c.c[0];
                            src->data[index].rgb[1] = c.c[1]; 
                            src->data[index].rgb[2] = c.c[2];
                        } else if (index < 0) {
                            return; // stop drawing, don't waste time
                        }
                        x = x - 1;
                    }
                }
                return;
            }

            if (dy < 0) {
                // Swap the points so we're drawing from bottom to top
                x1 = l->a.val[0];
                y1 = l->a.val[1];
                z1 = 1 / l->a.val[2];

                x = l->b.val[0];
                y = l->b.val[1];
                z = 1 / l->b.val[2];

                dy = y1 - y; // y1 - y0
                dx = x1 - x; // x1 - x0
                dz = z1 - z;
            }

            e_prime = 3 * dy - 2 * dx; // initialize e_prime

            if (dx > 0 && dx >= dy) {
                // first octant
                // We are moving in x, so we interpolate dz in terms of dx
                dz = dz / dx;
                for (i = 0; i < dx; i++) {
                    // Color the pixel
                    // image_setColor(src, y, x, c);
                    index = (src->cols * y) + x;
                    if (!(index < 0) && !(index > maxIndex) && 
                        x >= 0 && x < xmax && z > src->depth[index]) {
                        src->depth[index] = z;
                        src->data[index].rgb[0] = c.c[0];
                        src->data[index].rgb[1] = c.c[1]; 
                        src->data[index].rgb[2] = c.c[2];
                    }

                    while (e_prime > 0) {
                        y = y + 1; // Move up
                        e_prime = e_prime - 2 * dx; // subtract 2dx
                    }

                    // We always move across, so we add dy to our error:
                    x = x + 1;
                    z = z + dz;
                    e_prime = e_prime + 2 * dy; // move across, add dy
                }
                return;
            }

            if (dx > 0 && dx < dy) {
                // 2nd octant
                // flip roles of x and y
                // We are moving in y, so we interpolate dz in terms of dy
                dz = dz / dy;
                e_prime = 3 * dx - 2 * dy; // initialize e_prime
                for (i = 0; i < dy; i++) {
                    // Color the pixel
                    // image_setColor(src, y, x, c);
                    index = (src->cols * y) + x;
                    if (!(index < 0) && !(index > maxIndex) && 
                        x >= 0 && x < xmax && z > src->depth[index]) {
                        src->depth[index] = z;
                        src->data[index].rgb[0] = c.c[0];
                        src->data[index].rgb[1] = c.c[1]; 
                        src->data[index].rgb[2] = c.c[2];
                    }

                    while (e_prime > 0) {
                        x = x + 1; // Move across
                        e_prime = e_prime - 2 * dy; // subtract 2dy
                    }

                    // We always move up, so we add dy to our error:
                    y = y + 1;
                    z = z + dz;
                    e_prime = e_prime + 2 * dx; // move across, add dx
                }

                return;
            } 

            if (dx < 0 && dx < -dy) {
                // 3rd octant
                // Flip roles & subtract from x
                // We are moving in y, so we interpolate dz in terms of dy
                dz = dz / dy;
                e_prime = 3 * dy + 2 * dx; // initialize e_prime
                for (i = 0; i > dx; i--) {
                    // Color the pixel
                    // image_setColor(src, y, x-1, c);
                    index = (src->cols * y) + x;
                    if (!(index < 0) && !(index > maxIndex) && 
                        x >= 0 && x < xmax && z > src->depth[index]) {
                        src->depth[index] = z;
                        src->data[index].rgb[0] = c.c[0];
                        src->data[index].rgb[1] = c.c[1]; 
                        src->data[index].rgb[2] = c.c[2];
                    }

                    while (e_prime > 0) {
                        y = y + 1;
                        e_prime = e_prime + 2 * dx; // subtract 2dy
                    }

                    // We always move up, so we add dy to our error:
                    x = x - 1; // Move across
                    z = z + dz;
                    e_prime = e_prime + 2 * dy; // move across, add dx
                }
                return;
            } 

            if (dx < 0 && dx >= -dy) {
                // 4th octant
                // subtract from x
                // We are moving in x, so we interpolate dz in terms of dx
                dz = dz / dx;
                e_prime = 3 * dx + 2 * dy; // initialize e_prime
                for (i = 0; i < dy; i++) {
                    // Color the pixel
                    // image_setColor(src, y, x-1, c);

                    index = (src->cols * y) + x;
                    if (!(index < 0) && !(index > maxIndex) && 
                        x >= 0 && x < xmax && z > src->depth[index]) {
                        src->depth[index] = z;
                        src->data[index].rgb[0] = c.c[0];
                        src->data[index].rgb[1] = c.c[1]; 
                        src->data[index].rgb[2] = c.c[2];
                    }

                    while (e_prime < 0) {
                        x = x - 1; // Move across
                        e_prime = e_prime + 2 * dy; // subtract 2dx
                    }

                    // We always move across, so we add dy to our error:
                    y = y + 1;
                    z = z + dz;
                    e_prime = e_prime + 2 * dx; // move across, add dy
                }
                return;
            }

            // Shouldn't ever hit this, but useful for debugging:
            printf("Unhandled case\n\n\n");
            break;
    }

}


/* CIRCLE PRIMITIVES */

/**
 * Initialize a circle with center at tc and radius tr.
 */
void circle_set(Circle *c, Point tc, double tr) {
    c->c = tc;
    c->r = tr;
}

/**
 * Draw a circle into the specified image using color p. Specifically, this
 * executes Bresenham's circle algorithm to render the circle in the sixth
 * octant, and then rotates the radius to draw the other 7 octants.
 * 
 * Code is adapted from lecture notes by Bruce Maxwell and code from Hearn
 * & Baker, "Computer Graphics with OpenGL", provided by Bruce Maxwell in Sept.
 * 2021.
 */
void circle_draw(Circle *c, Image *src, Color p) {
    // Initialize the first point
    int x = 0;
    int y = -c->r;
    int e = 1 - c->r;


    while (x >= y) {
        // Draw the points and their rotated analogs:
        image_setColor(src, c->c.val[1] + x, c->c.val[0] + y, p);
        image_setColor(src, c->c.val[1] - x - 1, c->c.val[0] + y, p);
        image_setColor(src, c->c.val[1] + x, c->c.val[0] - y - 1, p);
        image_setColor(src, c->c.val[1] - x - 1, c->c.val[0] - y - 1, p);
        image_setColor(src, c->c.val[1] + y, c->c.val[0] + x, p);
        image_setColor(src, c->c.val[1] - y - 1, c->c.val[0] + x, p);
        image_setColor(src, c->c.val[1] + y, c->c.val[0] - x - 1, p);
        image_setColor(src, c->c.val[1] - y - 1, c->c.val[0] - x - 1, p);

        x--; // Move left
        if (e < 0) {
            e = e - 2 * x + 1; // Set error
        } else {
            y++; // Move up
            e = e - 2 * (x - y) + 1; // Set error
        }
    }
}

/**
 * Draw a filled circle using Bresenhams circle algorithm.
 * Code is adapted from lecture notes by Bruce Maxwell and code from Hearn
 * & Baker, "Computer Graphics with OpenGL", provided by Bruce Maxwell in Sept.
 * 2021.
 */
void circle_drawFill(Circle *c, Image *src, Color p) {
    // Initialize the first point
    int x = 0;
    int y = -c->r;
    int e = 1 - c->r;
    Line toDraw;
    Point ta;
    Point tb;


    while (x >= y) {
        // Draw the points and their rotated analogs:
        image_setColor(src, c->c.val[1] + x, c->c.val[0] + y, p);
        image_setColor(src, c->c.val[1] - x - 1, c->c.val[0] + y, p);
        image_setColor(src, c->c.val[1] + x, c->c.val[0] - y - 1, p);
        image_setColor(src, c->c.val[1] - x - 1, c->c.val[0] - y - 1, p);
        image_setColor(src, c->c.val[1] + y, c->c.val[0] + x, p);
        image_setColor(src, c->c.val[1] - y - 1, c->c.val[0] + x, p);
        image_setColor(src, c->c.val[1] + y, c->c.val[0] - x - 1, p);
        image_setColor(src, c->c.val[1] - y - 1, c->c.val[0] - x - 1, p);

        // Draw lines linking the points:
        // 3rd octant to 2nd:
        point_set2D(&ta, c->c.val[0] - x + 1, c->c.val[1] + y + 1);
        point_set2D(&tb, c->c.val[0] + x, c->c.val[1] + y + 1);
        line_set(&toDraw, ta, tb);
        line_draw(&toDraw, src, p);

        // 4th octant to 1st
        point_set2D(&ta, c->c.val[0] - y - 1, c->c.val[1] + x);
        point_set2D(&tb, c->c.val[0] + y, c->c.val[1] + x);
        line_set(&toDraw, ta, tb);
        line_draw(&toDraw, src, p);

        // 6th octant to 7th
        point_set2D(&ta, c->c.val[0] - x - 1, c->c.val[1] - y - 1);
        point_set2D(&tb, c->c.val[0] + x, c->c.val[1] - y - 1);
        line_set(&toDraw, ta, tb);
        line_draw(&toDraw, src, p);

        // 5th octant to 8th
        point_set2D(&ta, c->c.val[0] - y - 1, c->c.val[1] - x - 1);
        point_set2D(&tb, c->c.val[0] + y, c->c.val[1] - x - 1);
        line_set(&toDraw, ta, tb);
        line_draw(&toDraw, src, p);


        x--; // Move left
        if (e < 0) {
            e = e - 2 * x + 1; // Set error
        } else {
            y++; // Move up
            e = e - 2 * (x - y) + 1; // Set error
        }
    }
    return;
}


/* ELLIPSE PRIMITIVES */

/**
 * Set an ellipse to have center at point tc, with x-radius ta and y-radius tb.
 */
void ellipse_set(Ellipse *e, Point tc, double ta, double tb) {
    e->c = tc;
    e->ra = ta;
    e->rb = tb;
    e->a = 0.0; // Aligned to major axis
}

/**
 * Draw an ellipse using Bresenham's Ellipse algorithm. Adapted from pseudocode
 * in lecture notes provided by Prof. Bruce Maxwell at the Roux Institute, Sept.
 * 2021.
 */
void ellipse_draw(Ellipse *e, Image *src, Color p) {
    // Initialize the first point
    int x = -1;
    int y = -e->rb;
    int e_x = 2 * e->rb * e->rb;
    int e_y = 2 * e->ra * e->ra * -y;
    
    // Draw the points and their reflected analogs:
    image_setColor(src, e->c.val[1] + y, e->c.val[0], p);
    image_setColor(src, e->c.val[1] - y - 1, e->c.val[0], p);
    image_setColor(src, e->c.val[1] + y, e->c.val[0] + x, p);
    image_setColor(src, e->c.val[1] - y - 1, e->c.val[0] + x, p);
    
    int err = e->rb * e->rb - e->ra * e->ra * e->rb + (e->ra * e->ra) / 4 +\
              e->rb * e->rb + e_x;

    while (e_x < e_y) {
        x--; // Move left
        e_x = e_x + 2 * e->rb * e->rb; // update x error
        if (err < 0) {
            err = err + e->rb * e->rb + e_x; // update error
        } else {
            y++; // Move up
            e_y = e_y - 2 * e->ra * e->ra; // Update y-error
            err = err + e->rb * e->rb + e_x - e_y; // update error
        }

        // Draw point and its reflections
        image_setColor(src, e->c.val[1] + y, e->c.val[0] + x, p);
        image_setColor(src, e->c.val[1] + y, e->c.val[0] - x - 1, p);
        image_setColor(src, e->c.val[1] - y - 1, e->c.val[0] + x, p);
        image_setColor(src, e->c.val[1] - y - 1, e->c.val[0] - x - 1, p);
    }

    err = e->rb * e->rb * (x*x + x) + e->ra * e->ra * (y * y - 2 * y + 1) -\
        e->ra * e->ra * e->rb * e->rb + e->ra * e->ra - e_y;
    
    while (y < 0) {
        y++; // Move up
        e_y = e_y - 2 * e->ra * e->ra; // update y error

        if (err > 0) {
            err = err + e->ra * e->ra - e_y; // update overall error
        } else {
            x--; // move left
            e_x = e_x + 2 * e->rb * e->rb; // update x-error
            err = err + e->ra * e->ra - e_y + e_x; // update overall error
        }

        // Draw point and its reflections
        image_setColor(src, e->c.val[1] + y, e->c.val[0] + x, p);
        image_setColor(src, e->c.val[1] + y, e->c.val[0] - x - 1, p);
        image_setColor(src, e->c.val[1] - y - 1, e->c.val[0] - x - 1, p);
        image_setColor(src, e->c.val[1] - y - 1, e->c.val[0] + x, p);
    }
}

/**
 * Draws a filled ellipse using the same algorithm as in ellipse_draw(), but
 * fills in the lines between points as they are drawn. Adapted from Pseudocode
 * in Prof. Bruce Maxwell's Sept 2021 lecture notes at the Roux Institute.
 */
void ellipse_drawFill(Ellipse *e, Image *src, Color p) {
    Point a;
    Point b;
    Line l;
    // Initialize the first point
    int x = -1;
    int y = -e->rb;
    int e_x = 2 * e->rb * e->rb;
    int e_y = 2 * e->ra * e->ra * -y;
    
    // Draw the initial points and their reflected analogs:
    image_setColor(src, e->c.val[1] + y, e->c.val[0], p);
    image_setColor(src, e->c.val[1] - y - 1, e->c.val[0], p);
    image_setColor(src, e->c.val[1] + y, e->c.val[0] + x, p);
    image_setColor(src, e->c.val[1] - y - 1, e->c.val[0] + x, p);
    
    int err = e->rb * e->rb - e->ra * e->ra * e->rb + (e->ra * e->ra) / 4 +\
              e->rb * e->rb + e_x; // set error

    while (e_x < e_y) {
        x--; // Move left
        e_x = e_x + 2 * e->rb * e->rb; // update x error
        if (err < 0) {
            err = err + e->rb * e->rb + e_x; // update error
        } else {
            y++; // Move up
            e_y = e_y - 2 * e->ra * e->ra; // Update y-error
            err = err + e->rb * e->rb + e_x - e_y; // update error
        }

        // Draw point and its reflections
        image_setColor(src, e->c.val[1] + y, e->c.val[0] + x, p);
        image_setColor(src, e->c.val[1] + y, e->c.val[0] - x - 1, p);
        image_setColor(src, e->c.val[1] - y - 1, e->c.val[0] + x, p);
        image_setColor(src, e->c.val[1] - y - 1, e->c.val[0] - x - 1, p);

        // Draw lines linking points we just drew:
        point_set2D(&a, e->c.val[0] - x - 1, e->c.val[1] + y);
        point_set2D(&b, e->c.val[0] + x, e->c.val[1] + y);
        line_set(&l, a, b);
        line_draw(&l, src, p);

        point_set2D(&a, e->c.val[0] - x - 1, e->c.val[1] - y - 1);
        point_set2D(&b, e->c.val[0] + x, e->c.val[1] - y - 1);
        line_set(&l, a, b);
        line_draw(&l, src, p);    
    }

    err = e->rb * e->rb * (x*x + x) + e->ra * e->ra * (y * y - 2 * y + 1) -\
        e->ra * e->ra * e->rb * e->rb + e->ra * e->ra - e_y;
    
    while (y < 0) {
        y++; // Move up
        e_y = e_y - 2 * e->ra * e->ra; // update y error

        if (err > 0) {
            err = err + e->ra * e->ra - e_y; // update overall error
        } else {
            x--; // move left
            e_x = e_x + 2 * e->rb * e->rb; // update x-error
            err = err + e->ra * e->ra - e_y + e_x; // update overall error
        }

        // Draw point and its reflections
        image_setColor(src, e->c.val[1] + y, e->c.val[0] + x, p);
        image_setColor(src, e->c.val[1] + y, e->c.val[0] - x - 1, p);
        image_setColor(src, e->c.val[1] - y - 1, e->c.val[0] - x - 1, p);
        image_setColor(src, e->c.val[1] - y - 1, e->c.val[0] + x, p);
        
        // Draw lines linking points we just drew:
        point_set2D(&a, e->c.val[0] - x - 1, e->c.val[1] + y);
        point_set2D(&b, e->c.val[0] + x, e->c.val[1] + y);
        line_set(&l, a, b);
        line_draw(&l, src, p);

        point_set2D(&a, e->c.val[0] - x - 1, e->c.val[1] - y - 1);
        point_set2D(&b, e->c.val[0] + x, e->c.val[1] - y - 1);
        line_set(&l, a, b);
        line_draw(&l, src, p);    
    }
}

/* POLYLINE PRIMITIVES */

/**
 * Allocate and return the pointer to a Polyline. The returned polyline has
 * garbage values for numV and vertex list.
 */
Polyline *polyline_create() {
    Polyline *toReturn = malloc(sizeof(Polyline));
    if (!toReturn) {
        printf("Malloc error in polyline_create(). Returning NULL\n");
        return NULL;
    }

    return toReturn;
}

/**
 * Allocate and return a Polyline with the specified numV and a copy of the
 * vertices in the vlist passed as an argument.
 */
Polyline *polyline_createp(int numV, Point *vlist) {
    Polyline *toReturn = (Polyline *) malloc(sizeof(Polyline));
    if (!toReturn) {
        printf("Malloc error in polyline_createp(). Returning NULL\n");
        return NULL;
    }

    toReturn->numVertex = numV;
    toReturn->vertex = (Point *) malloc(sizeof(Point) * numV);
    if (!toReturn->vertex) {
        free(toReturn);
        printf("Error mallocing vlist in polyline_createp(). Returning NULL");
        return NULL;
    }

    for (int i = 0; i < numV; i++) {
        toReturn->vertex[i] = vlist[i];
    }

    return toReturn;
}

/**
 * Free the polyline and associated vertex list passed as an argument.
 */
void polyline_free(Polyline *p) {
    free(p->vertex);
    free(p);
}

/* The functions polyline init, polyline set, and polyline clear work on a pre-
existing Polyline data structure and manage only the memory required for the 
vertex list. */
 
/**
 * Initializes the pre-existing Polyline to an empty Polyline. Sets vertex list
 * to null. This assumes that the input polyline has garbage internal values and
 * should be used after polyline_create() is called to initialize the values.
 */
void polyline_init(Polyline *p) {
    p->numVertex = 0;
    p->zBuffer = 1;
    p->vertex = NULL;
}

/** 
 * Initializes the vertex list to the points in vlist. De-allocates/allocates
 * the vertex list for p, as necessary. This performs a deep copy - editing
 * points in the vlist passed as an argument will not change the polyline - its
 * copy of the vlist must be accessed directly.
 */
void polyline_set(Polyline *p, int numV, Point *vlist) {
    if (p->vertex) {
        free(p->vertex); // If there's an existing vlist, free it
        p->vertex = NULL; // Initialize vertex to null
    }
    
    p->numVertex = numV; // Set number of vertices
    p->vertex = (Point *) malloc(sizeof(Point) * numV);
    if (!p->vertex) {
        printf("Malloc error in polyline_set(). Returning.\n");
        p->vertex = NULL; // Initialize vertex to null
        return;
    }
    
    // Copy the points - mutation of vlist shouldn't change state of polyline
    for (int i = 0; i < numV; i++) {
        p-> vertex[i] = vlist[i];
    }
}

/**
 * Frees the internal data for a Polyline, if necessary, and sets numVertex to\
 * 0 and vertex to NULL. Assumes p has already been initialized.
 */
void polyline_clear(Polyline *p) {
    if (p->numVertex != 0) {
        p->numVertex = 0;
    }

    if (p->zBuffer != 1) {
        p->zBuffer = 1;
    }

    if (p->vertex) {
        free(p->vertex);
        p->vertex = NULL;
    }
}

/**
 * Sets the z-buffer flag to the given value.
 */
void polyline_zBuffer(Polyline *p, int flag) {
    p->zBuffer = flag;
}

/**
 * De-allocates/allocates space as necessary in the destination Polyline data 
 * structure and copies the vertex data from the source polyline (from) to the
 * destination (to).
 */
void polyline_copy(Polyline *to, Polyline *from) {
    if (to->vertex) {
        free(to->vertex);
        to->vertex = NULL;
    }

    to->numVertex = from->numVertex;
    to->zBuffer = from->zBuffer;

    to->vertex = (Point *) malloc(sizeof(Point) * to->numVertex);
    if (!to->vertex) {
        printf("Error mallocing in polyline_copy,\n");
        return;
    }

    for (int i = 0; i < to->numVertex; i++) {
        to->vertex[i] = from->vertex[i];
    }
}

/**
 * Prints Polyline data to the stream designated by the FILE pointer.
 */
void polyline_print(Polyline *p, FILE *fp) {
    fprintf(fp, "Number of vertices: %d\n", p->numVertex);
    for (int i = 0; i < p->numVertex; i++) {
        fprintf(fp, "Vertex %d: (%f %f)\n", i, p->vertex[i].val[0],\
                                               p->vertex[i].val[1]);
    }
}

/**
 * Normalize the x and y values of each vertex by the homogeneous coordinate.
 */
void polyline_normalize( Polyline *p ) {
    for (int i = 0; i < p->numVertex; i++) {
        point_normalize(&p->vertex[i]);
    }
}

/**
 * Draw the polyline using color c and the z-buffer, if appropriate.
 */
void polyline_draw(Polyline *p, Image *src, Color c) {
    Line l;
    // Case where no vertices
    if (p->numVertex == 0) {
        return;
    }

    // case where one vertex:
    if (p->numVertex == 1) {
        point_draw(&p->vertex[0], src, c);
        return;
    }
    
    // Case where more than one vertex: draw the lines between pairs of points
    for (int i = 1; i < p->numVertex; i++) {
        line_set(&l, p->vertex[i-1], p->vertex[i]);
        line_draw(&l, src, c);
    }
}


/* VECTOR FUNCTIONS */

/**
 * Set the vector to x, y, z, 0.0.
 */
void vector_set(Vector *v, double x, double y, double z) {
    v->val[0] = x;
    v->val[1] = y;
    v->val[2] = z;
}

/**
 * Print out the Vector v to stream fp.
 */
void vector_print(Vector *v, FILE *fp) {
    fprintf(fp, "Vector (x, y z) = (%f, %f, %f)\n", v->val[0], 
                                                    v->val[1], 
                                                    v->val[2]);
}

/**
 * Copy the Vector src to the Vector dest.
 */
void vector_copy(Vector *dest, Vector *src) {
    dest->val[0] = src->val[0];
    dest->val[1] = src->val[1];
    dest->val[2] = src->val[2];
}

/**
 * Return the Euclidean length of the vector, assuming the homogenous
 * coordinate is 1.0.
 * 
 * Specifically, L = ||v|| = sqrt(v_x^2 + v_y^2 + v_z^2).
 */
double vector_length(Vector *v) {
    return (double) sqrt(v->val[0] * v->val[0] + 
                v->val[1] * v->val[1] + 
                v->val[2] * v->val[2]);
}

/**
 * Normalize the vector to unit length. Do not modify the homogenous coordinate.
 * This is achieved by normalizing each of the vectors components by the vectors
 * Euclidean length
 */
void vector_normalize(Vector *v) {
    // Don't call vector length, as that's an unnecessary function call.
    double length = sqrt(v->val[0] * v->val[0] + 
                         v->val[1] * v->val[1] + 
                         v->val[2] * v->val[2]);
    
    v->val[0] = v->val[0] / length;
    v->val[1] = v->val[1] / length;
    v->val[2] = v->val[2] / length;
}

/**
 * Return the scalar product/dot product/inner product of a and b.
 * Shake your fist at mathematicians and physicists for having so many different
 * names for the same thing.
 */
double vector_dot(Vector *a, Vector *b) {
    return a->val[0]*b->val[0] + a->val[1]*b->val[1] + a->val[2]*b->val[2];
}

/**
 * Calculate the cross product (vector product) of a and b and put the result in
 * c.
 */
void vector_cross(Vector *a, Vector *b, Vector *c) {
    c->val[0] = a->val[1] * b->val[2] - a->val[2] * b->val[1];
    c->val[1] = a->val[2] * b->val[0] - a->val[0] * b->val[2];
    c->val[2] = a->val[0] * b->val[1] - a->val[1] * b->val[0];
}