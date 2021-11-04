/**
 * David Anderson - Fall 2021
 * views.h
 * 
 * This file contains structs defining two View models: View2D defines a 2D view
 * while View3D defines a 3D view. It also contains headers for functions that
 * used these Views to create a corresponding View Transformation Matrix (vtm)
 * that world coordinates can be multiplied with to get View coordinates
 * corresponding to that view.
 */
#ifndef VIEWS_H

#define VIEWS_H
#include "matrix.h"

typedef struct {
    Point vrp; // Center of the view rectangle in world coordinates
    double dx; // Width of the view rectangel in world coordinates
    double theta; // The orientation angle of the view, expressed as thetda
    Vector x; // The orientation angle of the view, expressed as x-axis
    int screenx; // Number of cols in the output image
    int screeny; // Number of rows in the output image
} View2D;

typedef struct {
    Point vrp; // 3D vector indicating the origin of the view reference coords
    Vector vpn; // 3D vector indicating the direction in which the viewer looks
    Vector vup; // 3D UP-vector - Can't be parallel to vpn
    
    /* Projection distance d - distance in the negative vpn direction at which 
    center of projection is located */
    double d; 

    /* view window extent - extent of view plane around VRP */
    double du;
    double dv;

    double f; // Front clip plane (expressed as distance along vpn - F>0 && B>F)
    double b; // Back clip plane (expressed as distance along vpn - F>0 && B>F)

    /* Screen size */
    int screenx;
    int screeny;

} View3D;

void view2D_set(View2D *view, Point *vrp, float dx, Vector *xaxis, int screenx,\
                int screeny);
void matrix_setView2D(Matrix *vtm, View2D *view);
void matrix_setView3D(Matrix *vtm, View3D *view);
void matrix_setViewParallel(Matrix *vtm, View3D *view);
#endif