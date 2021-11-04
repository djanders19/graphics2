/**
 * David Anderson - Fall 2021
 * implementation of views.h.
 * 
 * This file includes functions that allow for the creation of view-
 * transformation matrices from View2D and View3D objects. World coordinates of
 * objects in a scene can then be multiplied by these VTMs to generate View
 * coordinates.
 */
#include "views.h"

void view2D_set(View2D *view, Point *vrp, float dx, Vector *xaxis, int screenx,\
                int screeny) {
        view->vrp = *vrp;
        view->dx = dx;
        view->x = *xaxis;
        view->screenx = screenx;
        view->screeny = screeny;
    }

void matrix_setView2D(Matrix *vtm, View2D *view) {
    double dv = (view->dx * view->screeny) / view->screenx;

    // Set the vtm to the identity matrix:
    matrix_identity(vtm);
    matrix_print(vtm, stdout);
    printf("\n");
    
    // Translate the view window to the origin:
    matrix_translate2D(vtm, -view->vrp.val[0], -view->vrp.val[1]);
    matrix_print(vtm, stdout);
    printf("\n");

    // Rotate the view window to orient it with the x-axis:
    matrix_rotateZ(vtm, view->x.val[0], -view->x.val[1]);
    matrix_print(vtm, stdout);
    printf("\n");

    // Scale the view window to the number or rows and cols and flip y-axis:
    matrix_scale2D(vtm, view->screenx / view->dx, -view->screeny / dv);
    matrix_print(vtm, stdout);
    printf("\n");

    // Shift range of y-vals back to [0, R]
    matrix_translate2D(vtm, view->screenx / 2.0, view->screeny / 2.0);
    matrix_print(vtm, stdout);
    printf("\n");

    /* We can now multiply any scene defined in world coordinates by vtm to get
    the scene in view coordinates */
}

void matrix_setView3D(Matrix *vtm, View3D *view) {
    // Begin by setting vtm to identity matrix:
    matrix_identity(vtm);
    Vector uvec, vup_prime, vpn_norm;
    Point vrp_prime;
    double b_prime, f_prime, view_plane_d;

    /* calculate uvec from VUP and VPN. This is the axis orthogonal (i.e. "to-
    the-side-of" the plane formed by VUP and VPN, which is the "way we're 
    looking") */
    vector_cross(&(view->vup), &(view->vpn), &uvec);

    /* Recalculate VUP so that view reference coordinates are orthogonal.
    This can be thought of as making sure that all of the voxels in view-space
    are cubes, rather than arbitrary parallelipipeds. We don't change anything
    in the input view structure as this is supposed to have no side effects */
    vector_cross(&(view->vpn), &uvec, &vup_prime);

    /* Translate the VRP to the origin */
    matrix_translate(vtm, -view->vrp.val[0],\
                          -view->vrp.val[1],\
                          -view->vrp.val[2]);

    // printf("After VRP translation\n");
    // matrix_print(vtm, stdout);

    /* Normalize uvec, vup_prime, and vpn and align the axes */
    vector_normalize(&uvec);
    vector_normalize(&vup_prime);
    vector_copy(&vpn_norm, &(view->vpn)); // Copy this over - no side-effects
    vector_normalize(&vpn_norm);
    matrix_rotateXYZ(vtm, &uvec, &vup_prime, &vpn_norm); // Align axes
    // printf("After Rxyz\n");
    // matrix_print(vtm, stdout);
    // printf("\n");

    /* Translate the COP to the origin. In this case, the COP is just defined by
    the projection distance d along the VPN */
    matrix_translate(vtm, 0.0, 0.0, view->d);
    // printf("After translating COP to origin\n");
    // matrix_print(vtm, stdout);
    // printf("\n");

    /* As we are assuming that the COP is along -VPN, there is no need to
    calculate DOP or shear our VTM.

    We next scale to the CVV pyramid: */
    matrix_xformPoint(vtm, &(view->vrp), &vrp_prime); // update location of vrp
    b_prime = vrp_prime.val[2] + view->b; // Update distance to back clip plane
    
    // Width and height are defined in the view struct

    /* The following gets us VTM_world->CVV - a matrix that transforms from
    world coordinates to the CVV */
    matrix_scale(vtm, (2 * vrp_prime.val[2]) / (b_prime * view->du),
                 (2 * vrp_prime.val[2]) / (b_prime * view->dv),
                  1.0 / b_prime);
    // printf("After Scaling to CVV\n");
    // matrix_print(vtm, stdout);
    // printf("\n");

    /* Update the Front clip plane */
    // f_prime = (vrp_prime.val[2] + view->f) / b_prime;
    
    view_plane_d = vrp_prime.val[2] / b_prime; // Calculate dist to view plane
    matrix_perspective(vtm, view_plane_d);
    // printf("After perspective\n");
    // matrix_print(vtm, stdout);
    // printf("\n");

    matrix_scale2D(vtm, -view->screenx / (2 * view_plane_d),
                        -view->screeny / (2 * view_plane_d));
    // printf("After scaling to image coords\n");
    // matrix_print(vtm, stdout);
    // printf("\n");
    
    matrix_translate2D(vtm, view->screenx / 2, 
                            view->screeny / 2);
    // printf("After final translation:\n");
    // matrix_print(vtm, stdout);
    // printf("\n");

    // VTM is now a linear map from WORLD -> SCREEN
}

/**
 * Create a VTM for parallel projections, treating VPN as the DOP.
 * 
 * CURRENTLY A WORK IN PROGRESS
 */
void matrix_setViewParallel(Matrix *vtm, View3D *view) {
    // Begin by setting vtm to identity matrix:
    matrix_identity(vtm);
    Vector uvec, vup_prime, vpn_norm;
    double b_prime;

    /* calculate uvec from VUP and VPN. This is the axis orthogonal (i.e. "to-
    the-side-of" the plane formed by VUP and VPN, which is the "way we're 
    looking") */
    vector_cross(&(view->vup), &(view->vpn), &uvec);

    /* Recalculate VUP so that view reference coordinates are orthogonal.
    This can be thought of as making sure that all of the voxels in view-space
    are cubes, rather than arbitrary parallelipipeds. We don't change anything
    in the input view structure as this is supposed to have no side effects */
    vector_cross(&(view->vpn), &uvec, &vup_prime);

    /* Translate the VRP to the origin */
    matrix_translate(vtm, -view->vrp.val[0],\
                          -view->vrp.val[1],\
                          -view->vrp.val[2]);

    printf("After VRP translation\n");
    matrix_print(vtm, stdout);

    /* Normalize uvec, vup_prime, and vpn and align the axes */
    vector_normalize(&uvec);
    vector_normalize(&vup_prime);
    vector_copy(&vpn_norm, &(view->vpn)); // Copy this over - no side-effects
    vector_normalize(&vpn_norm);
    matrix_rotateXYZ(vtm, &uvec, &vup_prime, &vpn_norm); // Align axes
    printf("After Rxyz\n");
    matrix_print(vtm, stdout);
    printf("\n");

    // Shear so the DOP is aligned with the positive z-axis
    matrix_shearZ(vtm, view->vpn.val[0] / view->vpn.val[2], view->vpn.val[1] / view->vpn.val[2]);
    printf("After Shear\n");
    matrix_print(vtm, stdout);
    printf("\n");

    // Translate so center of view windo and front clip plane are at origin
    matrix_translate(vtm, -((view->vrp.val[0] - view->du / 2) + (view->vrp.val[0] + view->du / 2)) / 2,
                         -((view->vrp.val[1] - view->dv / 2) + (view->vrp.val[1] + view->dv / 2)) / 2, -view->f);
    printf("After translation\n");
    matrix_print(vtm, stdout);
    printf("\n");

    // Scale to CVV:
    b_prime = view->b - view->f;
    matrix_scale(vtm, 2/view->du, 2/view->dv, 1/b_prime);
    printf("After CVV transformation:\n");
    matrix_print(vtm, stdout);
    printf("\n");

    // Drop the z-axis:
    matrix_orthogonal(vtm);
    printf("After dropping z\n");
    matrix_print(vtm, stdout);
    printf("\n");

    // Scale and translate to screen coordinates:
    matrix_scale2D(vtm, -view->screenx / 2, -view->screeny / 2);
    matrix_translate2D(vtm, view->screenx / 2, view->screeny / 2);
    printf("After Final scale and Translation\n");
    matrix_print(vtm, stdout);
    printf("\n");
    // VTM is now a linear map from WORLD -> SCREEN
}