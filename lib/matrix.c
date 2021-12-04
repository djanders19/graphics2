/**
 * David Anderson - Fall 2021
 * matrix.c
 * 
 * This file implements the functions defined as prototypes in matrix.h. These
 * functions allow for the manipulation and transformation of various 2D and
 * 3D matrices as is necessary for defining translations from one graphical
 * coordinate system to another.
 */
#include "graphicslib.h"
#include <stdlib.h>

/* 2D & GENERIC MATRIX PROTOTYPES */

/**
 * Print out the matrix in a nice 4x4 arrangement with a blank line below.
 */
void matrix_print(Matrix *m, FILE *fp) {
    // check that inputs exist:
    if (!m) {
        printf("matrix_print(): Passed NULL matrix argument.\n");
        return;
    } else if (!fp) {
        printf("matrix_print(): Passed NULL file pointer argument.\n");
        return;
    }
    
    fprintf(fp, "[[%.3f  %.3f  %.3f  %.3f]\n"\
                " [%.3f  %.3f  %.3f  %.3f]\n"\
                " [%.3f  %.3f  %.3f  %.3f]\n"\
                " [%.3f  %.3f  %.3f  %.3f]]\n",
                m->m[0][0], m->m[0][1], m->m[0][2], m->m[0][3],
                m->m[1][0], m->m[1][1], m->m[1][2], m->m[1][3],
                m->m[2][0], m->m[2][1], m->m[2][2], m->m[2][3],
                m->m[3][0], m->m[3][1], m->m[3][2], m->m[3][3]);

}

/**
 * Set the matrix to all zeros.
 */
void matrix_clear(Matrix *m) {
    // Check that input exists
    if (!m) {
        printf("matrix_clear(): passed NULL matrix argument.\n");
        return;
    }
    // Set all the matrix values to 0.0
    m->m[0][0] = 0.0;
    m->m[0][1] = 0.0;
    m->m[0][2] = 0.0;
    m->m[0][3] = 0.0;
    m->m[1][0] = 0.0;
    m->m[1][1] = 0.0;
    m->m[1][2] = 0.0;
    m->m[1][3] = 0.0;
    m->m[2][0] = 0.0;
    m->m[2][1] = 0.0;
    m->m[2][2] = 0.0;
    m->m[2][3] = 0.0;
    m->m[3][0] = 0.0;
    m->m[3][1] = 0.0;
    m->m[3][2] = 0.0;
    m->m[3][3] = 0.0;
}

/**
 * Set the matrix to the identity matrix.
 */
void matrix_identity(Matrix *m) {
    // Check that input exists:
    if (!m) {
        printf("matrix_identity(): passed NULL matrix argument.\n");
        return;
    }

    // Set all the matrix values to 0.0 except main diagonal, which is 1.0
    m->m[0][0] = 1.0;
    m->m[0][1] = 0.0;
    m->m[0][2] = 0.0;
    m->m[0][3] = 0.0;
    m->m[1][0] = 0.0;
    m->m[1][1] = 1.0;
    m->m[1][2] = 0.0;
    m->m[1][3] = 0.0;
    m->m[2][0] = 0.0;
    m->m[2][1] = 0.0;
    m->m[2][2] = 1.0;
    m->m[2][3] = 0.0;
    m->m[3][0] = 0.0;
    m->m[3][1] = 0.0;
    m->m[3][2] = 0.0;
    m->m[3][3] = 1.0;
}

/**
 * Return the element of the matrix at (row=r, col=c).
 */
double matrix_get(Matrix *m, int r, int c) {
    // Check that input m exists:
    if (!m) {
        printf("matrix_get(): passed NULL matrix argument.\n");
        return -1.0;
    }
    
    // Check r and c to ensure in boundedness
    if (r < 0 || c < 0 || r > 3 || c > 3) {
        printf("matrix_get(): out of bounds r or c");
        return -1.0;
    }

    // get the element at (r, c)
    return m->m[r][c];
}

/**
 * Set the element of the matrix at (row=r, col=c).
 */
void matrix_set(Matrix *m, int r, int c, double v) {
    // Check that input m exists:
    if (!m) {
        printf("matrix_set(): passed NULL matrix argument.\n");
        return;
    }
    
    // Check r and c to ensure in boundedness
    if (r < 0 || c < 0 || r > 3 || c > 3) {
        printf("matrix_set(): out of bounds r or c");
        return;
    }

    // set value at (r,c)
    m->m[r][c] = v;
}

/**
 * Copy the src matrix to the dest matrix.
 */
void matrix_copy(Matrix *dest, Matrix *src) {
    if (!dest || !src) {
        printf("matrix_copy(): passed NULL dest or src\n");
        return;
    }

    // Set the values in dest to those of src
    dest->m[0][0] = src->m[0][0];
    dest->m[0][1] = src->m[0][1];
    dest->m[0][2] = src->m[0][2];
    dest->m[0][3] = src->m[0][3];
    dest->m[1][0] = src->m[1][0];
    dest->m[1][1] = src->m[1][1];
    dest->m[1][2] = src->m[1][2];
    dest->m[1][3] = src->m[1][3];
    dest->m[2][0] = src->m[2][0];
    dest->m[2][1] = src->m[2][1];
    dest->m[2][2] = src->m[2][2];
    dest->m[2][3] = src->m[2][3];
    dest->m[3][0] = src->m[3][0];
    dest->m[3][1] = src->m[3][1];
    dest->m[3][2] = src->m[3][2];
    dest->m[3][3] = src->m[3][3];
}

/**
 * Transpose the matrix m in place.
 */
void matrix_transpose(Matrix *m) {
    // Check that m exists:
    if (!m) {
        printf("matrix_transpose(): NULL matrix passed as input.\n");
        return;
    }

    /* A note: I considered doing this with the XOR swap algorithm shown in
    class - however, upon reviewing the wikipedia page
    (https://en.wikipedia.org/wiki/XOR_swap_algorithm#Reasons_for_avoidance_in_practice)
    it appears that on modern architectures using a temp variable is usually
    faster once going through the compiler (as well as being safer - swapping
    an item with itself using XOR algo zeroes the value). */

    // Swap the elements along the edges of the matrix, ignoring those on
    // the main diagonal:
    double temp = m->m[0][1];
    m->m[0][1] = m->m[1][0];
    m->m[1][0] = temp;

    temp = m->m[0][2];
    m->m[0][2] = m->m[2][0];
    m->m[2][0] = temp;

    temp = m->m[0][3];
    m->m[0][3] = m->m[3][0];
    m->m[3][0] = temp;

    temp = m->m[1][3];
    m->m[1][3] = m->m[3][1];
    m->m[3][1] = temp;    

    temp = m->m[2][3];
    m->m[2][3] = m->m[3][2];
    m->m[3][2] = temp;    

    // Swap the one interior pair that is not on the main diagonal:
    temp = m->m[2][1];
    m->m[2][1] = m->m[1][2];
    m->m[1][2] = temp;
}

/**
 * Multiply Matrix left and Matrix right and put the result in Matrix m, i.e.
 * [M] = [left][right]
 */
void matrix_multiply(Matrix *left, Matrix *right, Matrix *m) {
    // Check that inputs exist:
    if (!left || !right || !m) {
        printf("matrix_multiply(): passed NULL arguments.\n");
        return;
    }
    // Keep calculated values in a temp matrix in case right or left is m
    Matrix temp;

    // Multiply the two matrices together, storing vals in temp:
    int i;
    for (i = 0; i < 4; i++) {
            // Unroll two innermost loops, which loop over j and k from 0 to 3
            // j = 0
            temp.m[i][0] = 0.0;
            temp.m[i][0] = temp.m[i][0] + left->m[i][0] * right->m[0][0];
            temp.m[i][0] = temp.m[i][0] + left->m[i][1] * right->m[1][0];
            temp.m[i][0] = temp.m[i][0] + left->m[i][2] * right->m[2][0];
            temp.m[i][0] = temp.m[i][0] + left->m[i][3] * right->m[3][0];

            // j = 1
            temp.m[i][1] = 0.0;
            temp.m[i][1] = temp.m[i][1] + left->m[i][0] * right->m[0][1];
            temp.m[i][1] = temp.m[i][1] + left->m[i][1] * right->m[1][1];
            temp.m[i][1] = temp.m[i][1] + left->m[i][2] * right->m[2][1];
            temp.m[i][1] = temp.m[i][1] + left->m[i][3] * right->m[3][1];

            // j = 2
            temp.m[i][2] = 0.0;
            temp.m[i][2] = temp.m[i][2] + left->m[i][0] * right->m[0][2];
            temp.m[i][2] = temp.m[i][2] + left->m[i][1] * right->m[1][2];
            temp.m[i][2] = temp.m[i][2] + left->m[i][2] * right->m[2][2];
            temp.m[i][2] = temp.m[i][2] + left->m[i][3] * right->m[3][2];

            // j = 3
            temp.m[i][3] = 0.0;
            temp.m[i][3] = temp.m[i][3] + left->m[i][0] * right->m[0][3];
            temp.m[i][3] = temp.m[i][3] + left->m[i][1] * right->m[1][3];
            temp.m[i][3] = temp.m[i][3] + left->m[i][2] * right->m[2][3];
            temp.m[i][3] = temp.m[i][3] + left->m[i][3] * right->m[3][3];
    }

    // Set the values in m to those of temp
    m->m[0][0] = temp.m[0][0];
    m->m[0][1] = temp.m[0][1];
    m->m[0][2] = temp.m[0][2];
    m->m[0][3] = temp.m[0][3];
    m->m[1][0] = temp.m[1][0];
    m->m[1][1] = temp.m[1][1];
    m->m[1][2] = temp.m[1][2];
    m->m[1][3] = temp.m[1][3];
    m->m[2][0] = temp.m[2][0];
    m->m[2][1] = temp.m[2][1];
    m->m[2][2] = temp.m[2][2];
    m->m[2][3] = temp.m[2][3];
    m->m[3][0] = temp.m[3][0];
    m->m[3][1] = temp.m[3][1];
    m->m[3][2] = temp.m[3][2];
    m->m[3][3] = temp.m[3][3];
}

/**
 * Transform the Point p by the Matrix m and put the result in Point q (i.e.
 * consider the point a vector, then multiply it by the Matrix m).
 */
void matrix_xformPoint(Matrix *m, Point *p, Point *q) {
    // Check that inputs exist:
    if (!m || !p || !q) {
        printf("matrix_xformPoint(): passed NULL arguments.\n");
        return;
    }
    Point *temp = malloc(sizeof(Point));

    // Transform 0th value
    temp->val[0] = p->val[0] * m->m[0][0] + p->val[1] * m->m[0][1] + 
                p->val[2] * m->m[0][2] + p->val[3] * m->m[0][3];

    // Transform 1st value
    temp->val[1] = p->val[0] * m->m[1][0] + p->val[1] * m->m[1][1] + 
                p->val[2] * m->m[1][2] + p->val[3] * m->m[1][3];

    //Transform 2nd value
    temp->val[2] = p->val[0] * m->m[2][0] + p->val[1] * m->m[2][1] + 
                p->val[2] * m->m[2][2] + p->val[3] * m->m[2][3];

    // Transform homogenous coordinate (3rd value)
    temp->val[3] = p->val[0] * m->m[3][0] + p->val[1] * m->m[3][1] + 
                p->val[2] * m->m[3][2] + p->val[3] * m->m[3][3];

    q->val[0] = temp->val[0];
    q->val[1] = temp->val[1];
    q->val[2] = temp->val[2];
    q->val[3] = temp->val[3];

    free(temp);

}

/**
 * Transform the Vector p by the Matrix m and put the result in Vector q.
 */
void matrix_xformVector(Matrix *m, Vector *p, Vector *q) {
    if (!m || !p || !q) {
        printf("matrix_xformVector(): passed NULL arguments.\n");
        return;
    }
    Vector temp;

    // Transform 0th value
    temp.val[0] = p->val[0] * m->m[0][0] + p->val[1] * m->m[0][1] + 
                p->val[2] * m->m[0][2];

    // Transform 1st value
    temp.val[1] = p->val[0] * m->m[1][0] + p->val[1] * m->m[1][1] + 
                p->val[2] * m->m[1][2];

    //Transform 2nd value
    temp.val[2] = p->val[0] * m->m[2][0] + p->val[1] * m->m[2][1] + 
                p->val[2] * m->m[2][2];
    vector_copy(q, &temp);
}

/**
 * Transform all of the points in Polygon p by the Matrix m (this winds up
 * being the same as transforming the entire polygon). Also transforms the
 * surface normals if they exist.
 */
void matrix_xformPolygon(Matrix *m, Polygon *p) {
    if (!m || !p) {
        printf("matrix_xformPolygon(): passed NULL arguments.\n");
        return;
    }
    /* We have two cases - in the first, the surface normals don't exist, while 
    in the second they do. In the first case, we just transform the point
    coordinates. In the 2nd, we transform those coordinates and also transform
    the surface normal vector by the matrix. Not that this what's below akin to:

    for (int i = 0; i < p->nVertex; i++) {
        matrix_xformPoint(p->vertex[i]);
        matrix_xformVector(p->normal[i]);
    }

    but given that we may have scenes where we have thousands (if not millions)
    of polygons, we want to avoid wasteful function calls whenever possible. So
    I just copied over the code from there to here, with the ugly unrolled loops
    This should improve the efficiency somewhat in cases where we have lots and
    lots of polygons that need to be transformed */
    if (!p->normal) { // Surface normals don't exist, don't transform them
        for (int i = 0; i < p->nVertex; i++) {
            matrix_xformPoint(m, &(p->vertex[i]), &(p->vertex[i]));
        }
    } else { // Surface normals exist, transform them too
        for (int i = 0; i < p->nVertex; i++) {
            /* FIRST TRANSFORM THE POINT LOACATION */
            matrix_xformPoint(m, &(p->vertex[i]), &(p->vertex[i]));
            matrix_xformVector(m, &(p->normal[i]), &(p->normal[i]));
        }
    }

}

/**
 * Transform all the Points in the Polyline p by the Matrix m.
 */
void matrix_xformPolyline(Matrix *m, Polyline *p) {
    if (!m || !p) {
        printf("matrix_xformPolyline(): passed NULL arguments.\n");
        return;
    }
    double temp[4];

    /* The below is the same as calling matrix_xformPoint() on each vertex of
    the polyline. However, I just copied over the code to avoid wasteful
    function calls when rendering scenes with many polylines. */
    for (int i = 0; i < p->numVertex; i++) {
        // Transform 0th value of Point
        temp[0] = p->vertex[i].val[0] * m->m[0][0] + 
                    p->vertex[i].val[1] * m->m[0][1] + 
                    p->vertex[i].val[2] * m->m[0][2] + 
                    p->vertex[i].val[3] * m->m[0][3];

        // Transform 1st value of Point
        temp[1] = p->vertex[i].val[0] * m->m[1][0] + 
                    p->vertex[i].val[1] * m->m[1][1] + 
                    p->vertex[i].val[2] * m->m[1][2] + 
                    p->vertex[i].val[3] * m->m[1][3];

        //Transform 2nd value of Point
        temp[2] = p->vertex[i].val[0] * m->m[2][0] + 
                    p->vertex[i].val[1] * m->m[2][1] + 
                    p->vertex[i].val[2] * m->m[2][2] + 
                    p->vertex[i].val[3] * m->m[2][3];

        // Transform homogenous coordinate (3rd value) of Point
        temp[3] = p->vertex[i].val[0] * m->m[3][0] + 
                    p->vertex[i].val[1] * m->m[3][1] + 
                    p->vertex[i].val[2] * m->m[3][2] +
                    p->vertex[i].val[3] * m->m[3][3];
        
        p->vertex[i].val[0] = temp[0];
        p->vertex[i].val[1] = temp[1];
        p->vertex[i].val[2] = temp[2];
        p->vertex[i].val[3] = temp[3];
    }
}

/**
 * Transform the endpoints in the Line by Matrix m.
 */
void matrix_xformLine(Matrix *m, Line *line) {
    if (!m || !line) {
        printf("matrix_xformLine(): passed NULL arguments.\n");
        return;
    }
    // double temp[4];
    // Transform the first endpoint of the line:
    // Start with 0th value
    matrix_xformPoint(m, &(line->a), &(line->a));
    matrix_xformPoint(m, &(line->b), &(line->b));   
}

/**
 * Premultiply the Matrix m by a scale matrix parameterized by sx and sy, i.e.
 * scale m's x-component by sx and m's y-component by sy. An explicit
 * matrix representation of this looks like:
 * 
 *                   [sx 0  0  0]
 *                   [0  sy 0  0]
 * M = S(sx, sy)M == [0  0  1  0]
 *                   [0  0  0  1] M
 */
void matrix_scale2D(Matrix *m, double sx, double sy) {
    /* In practice, it's easier and more efficient to not actually carry out a
    full blown matrix multiplication (where we're mostly gonna wind up
    multiplying by 1 or zero anyway) and just apply the scale directly */
    
    // Scale x- component
    m->m[0][0] = m->m[0][0] * sx;
    m->m[0][1] = m->m[0][1] * sx;
    m->m[0][2] = m->m[0][2] * sx;
    m->m[0][3] = m->m[0][3] * sx;

    // Scale y-component
    m->m[1][0] = m->m[1][0] * sy;
    m->m[1][1] = m->m[1][1] * sy;
    m->m[1][2] = m->m[1][2] * sy;
    m->m[1][3] = m->m[1][3] * sy;

    // All other matrix values remain the same (bottom two rows)
}

/**
 * Premultiply the Matrix m by a z-axis rotation matrix parameterized by
 * cth = cos(theta) and sth = sin(theta), where theta is the angle of rotation.
 * sin and cos of theta are computed outside the function so as to give the user
 * the maximum amount of flexibility as to how to optimize the expensive trigon-
 * ometric computations. This transformation amounts to the following matrix
 * multiplication:
 * 
 * M = R_z(cth, sth)M = [cth -sth  0  0]
 *                      [sth  cth  0  0]
 *                      [0    0    1  0]
 *                      [0    0    0  1] M
 */
void matrix_rotateZ(Matrix *m, double cth, double sth) {
    if (!m) {
        printf("matrix_rotateZ(): Passed NULL argument as matrix.\n");
        return;
    }

    double temp[4];
    /* In practice, it's easier and more efficient to not actually carry out a
    full blown matrix multiplication (where we're mostly gonna wind up
    multiplying by 1 or zero anyway) and just apply the rotation directly. This
    winds up being very similar to scaling, but we have to be careful to not
    update either the 1st or 2nd rows in m until we've calculated appropriate
    values to fill in, as the resulting 1st and 2nd row each take input from 
    *both* the first and 2nd row of the input. To do this, I just store values
    in a temporary array*/

    // Calculate new values for x-component, but don't assign them yet:
    temp[0] = m->m[0][0] * cth - m->m[1][0] * sth;
    temp[1] = m->m[0][1] * cth - m->m[1][1] * sth;
    temp[2] = m->m[0][2] * cth - m->m[1][2] * sth;
    temp[3] = m->m[0][3] * cth - m->m[1][3] * sth;

    // Calculate and set y-component:
    m->m[1][0] = m->m[0][0] * sth + m->m[1][0] * cth;
    m->m[1][1] = m->m[0][1] * sth + m->m[1][1] * cth;
    m->m[1][2] = m->m[0][2] * sth + m->m[1][2] * cth;
    m->m[1][3] = m->m[0][3] * sth + m->m[1][3] * cth;

    // set x-component:
    m->m[0][0] = temp[0];
    m->m[0][1] = temp[1];
    m->m[0][2] = temp[2];
    m->m[0][3] = temp[3];
}

/**
 * Premultiply the Matrix m by a 2d translation matrix parameterized by tx and
 * ty. To do this we must take advantage of the homogenous coordinate:
 * 
 * M = T(tx, ty)M = [1  0  0  tx]
 *                  [0  1  0  ty]
 *                  [0  0  1  0 ]
 *                  [0  0  0  1 ] M
 */
void matrix_translate2D(Matrix *m, double tx, double ty) {
    if (!m) {
        printf("matrix_translate2D(): Passed NULL argument as matrix.\n");
        return;
    }
    /* Instead of carrying out a full matrix multiplication, it's easier to just
    unroll the looping and multiply the first two rows of the resulting matrix
    directly */

    // Row one: Apply x-translation
    m->m[0][0] = m->m[0][0] + m->m[3][0] * tx;
    m->m[0][1] = m->m[0][1] + m->m[3][1] * tx;
    m->m[0][2] = m->m[0][2] + m->m[3][2] * tx;
    m->m[0][3] = m->m[0][3] + m->m[3][3] * tx;

    // Row two: apply y-translation:
    m->m[1][0] = m->m[1][0] + m->m[3][0] * ty;
    m->m[1][1] = m->m[1][1] + m->m[3][1] * ty;
    m->m[1][2] = m->m[1][2] + m->m[3][2] * ty;
    m->m[1][3] = m->m[1][3] + m->m[3][3] * ty;
}

/**
 * Premultiply the matrix by a 2D shear matrix parameterized by shx and shy:
 * 
 * M = Sh(shx, shy) = [1    shx  0  0]
 *                    [shy  1    0  0]
 *                    [0    0    1  0]
 *                    [0    0    0  1] M
 */
void matrix_shear2D(Matrix *m, double shx, double shy) {
    if (!m) {
        printf("matrix_shear2D(): Passed NULL argument as matrix.\n");
        return;
    }
    double temp[2][4];
    // Shear x:
    temp[0][0] = m->m[0][0] + m->m[1][0] * shx;
    temp[0][1] = m->m[0][1] + m->m[1][1] * shx;
    temp[0][2] = m->m[0][2] + m->m[1][2] * shx;
    temp[0][3] = m->m[0][3] + m->m[1][3] * shx;

    // Shear y:
    temp[1][0] = m->m[1][0] + m->m[0][0] * shy;
    temp[1][1] = m->m[1][1] + m->m[0][1] * shy;
    temp[1][2] = m->m[1][2] + m->m[0][2] * shy;
    temp[1][3] = m->m[1][3] + m->m[0][3] * shy;

    m->m[0][0] = temp[0][0];
    m->m[0][1] = temp[0][1];
    m->m[0][2] = temp[0][2];
    m->m[0][3] = temp[0][3];

    m->m[1][0] = temp[1][0];
    m->m[1][1] = temp[1][1];
    m->m[1][2] = temp[1][2];
    m->m[1][3] = temp[1][3];
}


/* 3D MATRIX PROTOTYPES */

/**
 * Premultiply the Matrix m by a 3D translation matrix parameterized by tx, ty
 * and tz. To do this we must take advantage of the homogenous coordinate:
 * 
 * M = T(tx, ty)M = [1  0  0  tx]
 *                  [0  1  0  ty]
 *                  [0  0  1  tz]
 *                  [0  0  0  1 ] M
 */
void matrix_translate(Matrix *m, double tx, double ty, double tz) {
    if (!m) {
        printf("matrix_translate(): Passed NULL argument as matrix.\n");
        return;
    }

    // Row one: Apply x-translation
    m->m[0][0] = m->m[0][0] + m->m[3][0] * tx;
    m->m[0][1] = m->m[0][1] + m->m[3][1] * tx;
    m->m[0][2] = m->m[0][2] + m->m[3][2] * tx;
    m->m[0][3] = m->m[0][3] + m->m[3][3] * tx;

    // Row two: apply y-translation:
    m->m[1][0] = m->m[1][0] + m->m[3][0] * ty;
    m->m[1][1] = m->m[1][1] + m->m[3][1] * ty;
    m->m[1][2] = m->m[1][2] + m->m[3][2] * ty;
    m->m[1][3] = m->m[1][3] + m->m[3][3] * ty;

    // Row two: apply z-translation:
    m->m[2][0] = m->m[2][0] + m->m[3][0] * tz;
    m->m[2][1] = m->m[2][1] + m->m[3][1] * tz;
    m->m[2][2] = m->m[2][2] + m->m[3][2] * tz;
    m->m[2][3] = m->m[2][3] + m->m[3][3] * tz;
}

/**
 * Premultiply the Matrix m by a scale matrix parameterized by sx, sy, and sz,
 * i.e. scale m's x-component by sx, scale m's y-component by sy, and scale m's
 * z-component by sz. An explicit matrix representation of this looks like:
 * 
 *                   [sx 0  0  0]
 *                   [0  sy 0  0]
 * M = S(sx, sy)M == [0  0  sz 0]
 *                   [0  0  0  1] M
 */
void matrix_scale(Matrix *m, double sx, double sy, double sz) {
    if (!m) {
        printf("matrix_scale(): Passed NULL argument as matrix.\n");
        return;
    }

    // Scale x- component
    m->m[0][0] = m->m[0][0] * sx;
    m->m[0][1] = m->m[0][1] * sx;
    m->m[0][2] = m->m[0][2] * sx;
    m->m[0][3] = m->m[0][3] * sx;

    // Scale y-component
    m->m[1][0] = m->m[1][0] * sy;
    m->m[1][1] = m->m[1][1] * sy;
    m->m[1][2] = m->m[1][2] * sy;
    m->m[1][3] = m->m[1][3] * sy;

    // Scale z-component
    m->m[2][0] = m->m[2][0] * sz;
    m->m[2][1] = m->m[2][1] * sz;
    m->m[2][2] = m->m[2][2] * sz;
    m->m[2][3] = m->m[2][3] * sz;

}

/**
 * Premultiply the Matrix m by a x-axis rotation matrix parameterized by
 * cth = cos(theta) and sth = sin(theta), where theta is the angle of rotation.
 * sin and cos of theta are computed outside the function so as to give the user
 * the maximum amount of flexibility as to how to optimize the expensive trigon-
 * ometric computations. This transformation amounts to the following matrix
 * multiplication:
 * 
 * M = R_x(cth, sth)M = [1   0    0    0]
 *                      [0   cth -sth  0]
 *                      [0   sth  cth  0]
 *                      [0   0    0    1] M
 */
void matrix_rotateX(Matrix *m, double cth, double sth) {
    if (!m) {
        printf("matrix_rotateX(): passed NULL pointer as argument.\n");
        return;
    }
    // Array to keep track of calculated values
    double temp[4][4];

    temp[0][0] = m->m[1][0] * cth - m->m[2][0] * sth;
    temp[0][1] = m->m[1][1] * cth - m->m[2][1] * sth;
    temp[0][2] = m->m[1][2] * cth - m->m[2][2] * sth;
    temp[0][3] = m->m[1][3] * cth - m->m[2][3] * sth;

    temp[1][0] = m->m[1][0] * sth + m->m[2][0] * cth;
    temp[1][1] = m->m[1][1] * sth + m->m[2][1] * cth;
    temp[1][2] = m->m[1][2] * sth + m->m[2][2] * cth;
    temp[1][3] = m->m[1][3] * sth + m->m[2][3] * cth;

    m->m[1][0] = temp[0][0];
    m->m[1][1] = temp[0][1];
    m->m[1][2] = temp[0][2];
    m->m[1][3] = temp[0][3];
    m->m[2][0] = temp[1][0];
    m->m[2][1] = temp[1][1];
    m->m[2][2] = temp[1][2];
    m->m[2][3] = temp[1][3];
}

/**
 * Premultiply the Matrix m by a y-axis rotation matrix parameterized by
 * cth = cos(theta) and sth = sin(theta), where theta is the angle of rotation.
 * sin and cos of theta are computed outside the function so as to give the user
 * the maximum amount of flexibility as to how to optimize the expensive trigon-
 * ometric computations. This transformation amounts to the following matrix
 * multiplication:
 * 
 * M = R_y(cth, sth)M = [cth   0   sth  0]
 *                      [0     1   0    0]
 *                      [-sth  0   cth  0]
 *                      [0     0   0    1] M
 */
void matrix_rotateY(Matrix *m, double cth, double sth) {
    if (!m) {
        printf("matrix_rotateX(): passed NULL pointer as argument.\n");
        return;
    }
    // Array to keep track of calculated values
    double temp[4][4];

    // Calculate values:
    temp[0][0] = m->m[0][0] * cth + m->m[2][0] * sth;
    temp[0][1] = m->m[0][1] * cth + m->m[2][1] * sth;
    temp[0][2] = m->m[0][2] * cth + m->m[2][2] * sth;
    temp[0][3] = m->m[0][3] * cth + m->m[2][3] * sth;

    temp[1][0] = m->m[2][0] * cth - m->m[0][0] * sth;
    temp[1][1] = m->m[2][1] * cth - m->m[0][1] * sth;
    temp[1][2] = m->m[2][2] * cth - m->m[0][2] * sth;
    temp[1][3] = m->m[2][3] * cth - m->m[0][3] * sth;

    // Copy back calculated values:
    m->m[0][0] = temp[0][0];
    m->m[0][1] = temp[0][1];
    m->m[0][2] = temp[0][2];
    m->m[0][3] = temp[0][3];
    m->m[2][0] = temp[1][0];
    m->m[2][1] = temp[1][1];
    m->m[2][2] = temp[1][2];
    m->m[2][3] = temp[1][3];
}

/**
 * Premultiply the matrix by an XYZ-axis rotation matrix parameterized by the 
 * vectors u, v, and w,where the three vectors represent an orthonormal 3D 
 * basis. This amounts to the matrix transformation:
 * 
 * M = R_xyz(u, v, w)M = [ux uy uz 0]
 *                       [vx vy vz 0]
 *                       [wx wy wz 0]
 *                       [0  0  0  1] M
 */
void matrix_rotateXYZ(Matrix *m, Vector *u, Vector *v, Vector *w) {
    if (!m || !u || !v || !w) {
        printf("matrix_rotateXYZ(): passed NULL pointer as argument.\n");
        return;
    }

    /* We reuse the values of m in many computations, so we can't just edit them
    in place. Instead, calculated values ux, uy, ... , wy, wz are stored in a
    temp matrix and then copied over at the end. */

    Matrix temp;
    // Rotate x- component
    temp.m[0][0] = m->m[0][0] * u->val[0] + m->m[1][0] * u->val[1] + m->m[2][0] * u->val[2];
    temp.m[0][1] = m->m[0][1] * u->val[0] + m->m[1][1] * u->val[1] + m->m[2][1] * u->val[2];
    temp.m[0][2] = m->m[0][2] * u->val[0] + m->m[1][2] * u->val[1] + m->m[2][2] * u->val[2];
    temp.m[0][3] = m->m[0][3] * u->val[0] + m->m[1][3] * u->val[1] + m->m[2][3] * u->val[2];

    // Rotate y-component
    temp.m[1][0] = m->m[0][0] * v->val[0] + m->m[1][0] * v->val[1] + m->m[2][0] * v->val[2];
    temp.m[1][1] = m->m[0][1] * v->val[0] + m->m[1][1] * v->val[1] + m->m[2][1] * v->val[2];
    temp.m[1][2] = m->m[0][2] * v->val[0] + m->m[1][2] * v->val[1] + m->m[2][2] * v->val[2];
    temp.m[1][3] = m->m[0][3] * v->val[0] + m->m[1][3] * v->val[1] + m->m[2][3] * v->val[2];

    // Rotate z-component
    temp.m[2][0] = m->m[0][0] * w->val[0] + m->m[1][0] * w->val[1] + m->m[2][0] * w->val[2];
    temp.m[2][1] = m->m[0][1] * w->val[0] + m->m[1][1] * w->val[1] + m->m[2][1] * w->val[2];
    temp.m[2][2] = m->m[0][2] * w->val[0] + m->m[1][2] * w->val[1] + m->m[2][2] * w->val[2];
    temp.m[2][3] = m->m[0][3] * w->val[0] + m->m[1][3] * w->val[1] + m->m[2][3] * w->val[2];

    // Copy back:
    m->m[0][0] = temp.m[0][0];
    m->m[0][1] = temp.m[0][1];
    m->m[0][2] = temp.m[0][2];
    m->m[0][3] = temp.m[0][3];

    m->m[1][0] = temp.m[1][0];
    m->m[1][1] = temp.m[1][1];
    m->m[1][2] = temp.m[1][2];
    m->m[1][3] = temp.m[1][3];

    m->m[2][0] = temp.m[2][0];
    m->m[2][1] = temp.m[2][1];
    m->m[2][2] = temp.m[2][2];
    m->m[2][3] = temp.m[2][3];
}

/**
 * Premultiply the Matrix m by a shear z matrix parameterized by shx and shy.
 * 
 * M = R_xyz(u, v, w)M = [1  0 shx 0]
 *                       [0  1 shy 0]
 *                       [0  0  1  0]
 *                       [0  0  0  1] M
 */
void matrix_shearZ(Matrix *m, double shx, double shy) {
    if (!m) {
        printf("matrix_shearZ(): Passed NULL argument as matrix.\n");
        return;
    }
    
    // Shear x:
    m->m[0][0] = m->m[0][0] + m->m[2][0] * shx;
    m->m[0][1] = m->m[0][1] + m->m[2][1] * shx;
    m->m[0][2] = m->m[0][2] + m->m[2][2] * shx;
    m->m[0][3] = m->m[0][3] + m->m[2][3] * shx;

    // Shear y:
    m->m[1][0] = m->m[1][0] + m->m[2][0] * shy;
    m->m[1][1] = m->m[1][1] + m->m[2][1] * shy;
    m->m[1][2] = m->m[1][2] + m->m[2][2] * shy;
    m->m[1][3] = m->m[1][3] + m->m[2][3] * shy;
}

/**
 * Premultiply the matrix by a perspective matrix parameterized by d.
 * M = Persp(d)M = [1   0   0   0]
 *                 [0   1   0   0]
 *                 [0   0   1   0]
 *                 [0   0  1/d  0] M
 */
void matrix_perspective(Matrix *m, double d) {
    if (!m) {
        printf("matrix_perspectiveZ(): Passed NULL argument as matrix.\n");
        return;
    }

    // In this case we just update the last row of the input matrix:
    m->m[3][0] = m->m[2][0] / d;
    m->m[3][1] = m->m[2][1] / d;
    m->m[3][2] = m->m[2][2] / d;
    m->m[3][3] = m->m[2][3] / d;
}

/**
 * Premultiply the matrix by an orthogonal perspective matrix.
 */
void matrix_orthogonal(Matrix *m) {
    if (!m) {
        printf("matrix_perspectiveZ(): Passed NULL argument as matrix.\n");
        return;
    }
    
    // Drop the z-coordinate
    m->m[2][0] = 0.0;
    m->m[2][1] = 0.0;
    m->m[2][2] = 0.0;
    m->m[2][3] = 0.0;
}