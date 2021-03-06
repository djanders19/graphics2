/**
 * David J Anderson - November 2021
 * 
 * Implements modeling.h, a suite of functions allowing for hierarchical scene
 * modeling. This allows the user to create a scene DAG describing a scene, and
 * provides the module_draw() function to traverse the graph and draw it
 * according to a user specified view.
 */
#include "modeling.h"

/* 2D AND GENERIC MODULE FUNCTIONS */

/**
 * Allocate and return an initialized but empty Element.
 */
Element *element_create(void) {
    Element *toReturn = (Element *) malloc(sizeof(Element));

    if (!toReturn) {
        printf("element_create(): malloc() failed.\n");
        return NULL;
    }

    toReturn->type = ObjNone;
    toReturn->next = NULL;

    return toReturn;
}

/**
 * Allocate an Element and store a duplicate of the data pointed to by obj in
 * the Element. Modules do not get duplicated. The function needs to handle each
 * type of object separately in a case statement.
 */
Element *element_init(ObjectType type, void *obj) {
    Element *toReturn = element_create();

    switch (type) {
    case ObjNone:
        toReturn->obj.coeff = 0.0;
        break;

    case ObjLine:
        toReturn->type = ObjLine;
        line_copy(&(toReturn->obj.line), (Line *) obj);
        break;
    
    case ObjPoint:
        toReturn->type = ObjPoint;
        toReturn->obj.point.val[0] = ((Point *) obj)->val[0];
        toReturn->obj.point.val[1] = ((Point *) obj)->val[1];
        toReturn->obj.point.val[2] = ((Point *) obj)->val[2];
        toReturn->obj.point.val[3] = ((Point *) obj)->val[3];
        break;
    
    case ObjPolyline:
        toReturn->type = ObjPolyline;
        polyline_copy(&(toReturn->obj.polyline), ((Polyline *) obj));
        break;

    case ObjPolygon:
        toReturn->type = ObjPolygon;
        polygon_copy(&(toReturn->obj.polygon), ((Polygon *) obj));
        break;
    
    case ObjIdentity:
        toReturn->type = ObjIdentity;
        matrix_identity(&(toReturn->obj.matrix));
        break;
    
    case ObjMatrix:
        toReturn->type = ObjMatrix;
        matrix_copy(&(toReturn->obj.matrix), ((Matrix *) obj));
        break;

    case ObjColor:
        toReturn->type = ObjColor;
        color_copy(&(toReturn->obj.color), ((Color *) obj));
        break;

    case ObjBodyColor:
        toReturn->type = ObjBodyColor;
        color_copy(&(toReturn->obj.color), ((Color *) obj));
        break;

    case ObjSurfaceColor:
        toReturn->type = ObjSurfaceColor;
        color_copy(&(toReturn->obj.color), ((Color *) obj));
        break;

    case ObjSurfaceCoeff:
        toReturn->type = ObjSurfaceCoeff;
        toReturn->obj.coeff = *((float *) obj);
        break;

    case ObjLight:
        toReturn->type = ObjLight;
        break;

    case ObjModule:
        toReturn->type = ObjModule;
        toReturn->obj.module = obj;
        break;
    
    case ObjBezier:
        toReturn->type = ObjBezier;
        bezierCurve_copy(&(toReturn->obj.curve), ((BezierCurve *) obj));
        break;

    default:
        printf("element_init(): passed unknown object type\n");
        free(toReturn);
        return NULL;
        break;
    }

    return toReturn;
}

/**
 * Free the element and the object it contains, as appropriate.
 */
void element_delete(Element *e) {
    if (!e) {
        printf("element_delete(): passed null pointer.\n");
        return;
    }

    switch (e->type){
    case ObjPolyline:
        polyline_clear(&(e->obj.polyline));
        free(e);
        break;

    case ObjPolygon:
        polygon_clear(&(e->obj.polygon));
        free(e);
        break;

    default:
        free(e);
        break;
    }// No need to free internals - object is represented by a union
    return;
}

/**
 * Allocate an empty module.
 */
Module *module_create(void) {
    Module *toReturn = malloc(sizeof(Module));
    toReturn->head = NULL;
    toReturn->tail = NULL;
    return toReturn;
}

/**
 * Clear the module's list of Elements, freeing memory as appropriate.
 */
void module_clear(Module *md) {
    if (!md) {
        printf("module_clear(): passed null pointer.\n");
        return;
    }
    Element *tempHead;

    while (md->head) {
        tempHead = md->head;
        md->head = md->head->next;
        tempHead->next = NULL;
        element_delete(tempHead);
    }
    md->head = NULL;
    md->tail = NULL;
}

void module_free(Module *md) {
    if (!md) {
        printf("module_clear(): passed null pointer.\n");
        return;
    }
    Element *tempHead;

    while (md->head) {
        tempHead = md->head;
        md->head = md->head->next;
        tempHead->next = NULL;
        if (tempHead->type == ObjModule) {
            printf("AA\n");
            module_free(tempHead->obj.module);
        }
        element_delete(tempHead);
    }
    md->head = NULL;
    md->tail = NULL;
}

/**
 * Clear the module's list of elements, and free submodules.
 */

/**
 * Free all of the memory associated with a module, including the memory pointed
 * to by md.
 */
void module_delete(Module *md) {
    if (!md) {
        printf("module_clear(): passed null pointer.\n");
        return;
    }
    module_clear(md); // Clears all the Elements in the Module
    free(md);
}

/**
 * Generic insert of an element into the module at the tail of the list.
 */
void module_insert(Module *md, Element *e) {
    if (!md || !e) {
        printf("module_insert(): passed null pointer.\n");
        return;
    }
    if (!md->tail) {
        md->head = e;
        md->tail = e;
    } else {
        md->tail->next = e; // Set the last element in the list to point to e
        md->tail = e; // Set the last element to be e.
    }
}

/**
 * Add a pointer to the Module sub to the tail of the module's list
 */
void module_module(Module *md, Module *sub) {
    if (!md || !sub) {
        printf("module_module(): passed null pointer.\n");
        return;
    }
    Element *e = element_init(ObjModule, sub);
    if (!md->tail) {
        md->head = e;
        md->tail = e;
    } else {
        md->tail->next = e; // Set the last element in the list to point to e
        md->tail = e; // Set the last element to be e.
    }
}

/**
 * Adds p to the tail of the module's list
 */
void module_point(Module *md, Point *p) {
    if (!md || !p) {
        printf("module_point(): passed null pointer.\n");
        return;
    }

    Element *e = element_init(ObjPoint, p);
    if (!md->tail) {
        md->head = e;
        md->tail = e;
    } else {
        md->tail->next = e; // Set the last element in the list to point to e
        md->tail = e; // Set the last element to be e.
    }
}

/**
 * Add a line to the tail of the module's list.
 */
void module_line(Module *md, Line *p) {
    if (!md || !p) {
        printf("module_line(): passed null pointer.\n");
        return;
    }

    Element *e = element_init(ObjLine, p);
    if (!md->tail) {
        md->head = e;
        md->tail = e;
    } else {
        md->tail->next = e; // Set the last element in the list to point to e
        md->tail = e; // Set the last element to be e.
    }
}

/**
 * Add the polyline p to the module's list
 */
void module_polyline(Module *md, Polyline *p) {
    if (!md || !p) {
        printf("module_polyline(): passed null pointer.\n");
        return;
    }

    Element *e = element_init(ObjPolyline, p);
    if (!md->tail) {
        md->head = e;
        md->tail = e;
    } else {
        md->tail->next = e; // Set the last element in the list to point to e
        md->tail = e; // Set the last element to be e.
    }
}

/**
 * Add the polygon p to the module's list.
 */
void module_polygon(Module *md, Polygon *p) {
    if (!md || !p) {
        printf("module_polygon(): passed null pointer.\n");
        return;
    }

    Element *e = element_init(ObjPolygon, p);
    if (!md->tail) {
        md->head = e;
        md->tail = e;
    } else {
        md->tail->next = e; // Set the last element in the list to point to e
        md->tail = e; // Set the last element to be e.
    }
}

/**
 * Set the current transform to the identity, placed at the tail end of the
 * module's list.
 */
void module_identity(Module *md) {
    if (!md) {
        printf("module_identity(): passed null pointer.\n");
        return;
    }

    Matrix *m = malloc(sizeof(Matrix));
    matrix_identity(m);

    Element *e = element_init(ObjIdentity, m);
    free(m);
    if (!md->tail) {
        md->head = e;
        md->tail = e;
    } else {
        md->tail->next = e; // Set the last element in the list to point to e
        md->tail = e; // Set the last element to be e.
    }
}

/**
 * Add a translation matrix to the tail of the module's list.
 */
void module_translate2D(Module *md, double tx, double ty) {
    if (!md) {
        printf("module_translate2D(): passed null pointer.\n");
        return;
    }

    Matrix *m = malloc(sizeof(Matrix));
    matrix_identity(m);
    matrix_translate2D(m, tx, ty);

    Element *e = element_init(ObjMatrix, m);
    free(m);
    if (!md->tail) {
        md->head = e;
        md->tail = e;
    } else {
        md->tail->next = e; // Set the last element in the list to point to e
        md->tail = e; // Set the last element to be e.
    }
}

/**
 * Add a scale matrix to the tail end of the module's list
 */
void module_scale2D(Module *md, double sx, double sy) {
    if (!md) {
        printf("module_scale2D(): passed null pointer.\n");
        return;
    }

    Matrix *m = malloc(sizeof(Matrix));
    matrix_identity(m);
    matrix_scale2D(m, sx, sy);

    Element *e = element_init(ObjMatrix, m);
    free(m);

    if (!md->tail) {
        md->head = e;
        md->tail = e;
    } else {
        md->tail->next = e; // Set the last element in the list to point to e
        md->tail = e; // Set the last element to be e.
    }
}

/**
 * Add a z-rotation matrix to the tail end of the module's list.
 */
void module_rotateZ(Module *md, double cth, double sth) {
    if (!md) {
        printf("module_rotateZ(): passed null pointer.\n");
        return;
    }

    Matrix *m = malloc(sizeof(Matrix));
    matrix_identity(m);
    matrix_rotateZ(m, cth, sth);

    Element *e = element_init(ObjMatrix, m);
    free(m);
    if (!md->tail) {
        md->head = e;
        md->tail = e;
    } else {
        md->tail->next = e; // Set the last element in the list to point to e
        md->tail = e; // Set the last element to be e.
    }
}

/**
 * Add a 2D shear matrix to the tail end of the module's list.
 */
void module_shear2D(Module *md, double shx, double shy) {
    if (!md) {
        printf("module_shear2D(): passed null pointer.\n");
        return;
    }

    Matrix *m = malloc(sizeof(Matrix));
    matrix_identity(m);
    matrix_shear2D(m, shx, shy);

    Element *e = element_init(ObjMatrix, m);
    free(m);

    if (!md->tail) {
        md->head = e;
        md->tail = e;
    } else {
        md->tail->next = e; // Set the last element in the list to point to e
        md->tail = e; // Set the last element to be e.
    }
}

/**
 * Draw the module into the image using the given VTM, Lighting, and DrawState
 * by traversing the list of Elements.
 */
void module_draw(Module *md, Matrix *VTM, Matrix *GTM,\
                 DrawState *ds, Lighting *lighting, Image *src) {
    // Set the matrix LTM to identity
    Matrix *LTM = malloc(sizeof(Matrix));
    matrix_identity(LTM);

    // For each element E in module md:
    Element *i = md->head;
    // int j = 0;
    while (i) {
        // printf("%i\n", j);
        // j++;
        // Switch on the type of E:
        switch (i->type)
        {
        case ObjColor:
            color_copy(&(ds->color), &(i->obj.color));
            break;
        
        case ObjPoint: ;
            //printf("drawing point\n");
            Point *x = malloc(sizeof(Point));
            // Copy the point data in E to X
            point_copy(x, &(i->obj.point));

            // Transform X by the LTM
            matrix_xformPoint(LTM, x, x);

            // Transform X by the GTM
            matrix_xformPoint(GTM, x, x);

            // Transform X by the VTM
            matrix_xformPoint(VTM, x, x);

            // Normalize X by the homogenous coord
            point_normalize(x);

            // Draw X using DS->color (if X is in the image)
            point_draw(x, src, ds->color);
            free(x);
            break;

        case ObjLine: ;
            Line *l = malloc(sizeof(Line));
            
            // Copy the line data in E to L
            line_copy(l, &(i->obj.line));
            Matrix *temp = malloc(sizeof(Matrix));
            matrix_multiply(GTM, LTM, temp);
            matrix_multiply(VTM, temp, temp);
            matrix_xformLine(temp, l);

            // Normalize L by the homogenous coord
            line_normalize(l);
            /*printf("drawing line (%f %f) to (%f %f) with color (%f %f %f)\n",\
            l->a.val[0], l->a.val[1], l->b.val[0], l->b.val[1],\
            ds->color.c[0], ds->color.c[1], ds->color.c[2]);*/
            // Draw L using DS->color
            line_draw(l, src, ds->color);
            free(l);
            free(temp);
            break;

        case ObjPolyline: ;
            printf("drawing polyline\n");
            // Copy polyline data to PL:
            Polyline *pl = polyline_create();
            polyline_copy(pl, &(i->obj.polyline));

            // Transform PL by the LTM:
            matrix_xformPolyline(LTM, pl);

            // Transform PL by the GTM:
            matrix_xformPolyline(GTM, pl);
            
            // Transform PL by the VTM:
            matrix_xformPolyline(VTM, pl);

            // Normalize PL:
            polyline_normalize(pl);
            
            // Draw PL using DS->color:
            polyline_draw(pl, src, ds->color);
            polyline_free(pl);
            break;
        
        case ObjPolygon: ;
            //printf("drawing polygon\n");
            // Copy the polygon data in E to P
            Polygon* p = polygon_create();
            polygon_copy(p, &(i->obj.polygon));

            // Transform P by the LTM
            matrix_xformPolygon(LTM, p);

            // Transform P by the GTM
            matrix_xformPolygon(GTM, p);

            // Transform P by the VTM
            matrix_xformPolygon(VTM, p);

            // Normalize P by the homogenous coord
            polygon_normalize(p);

            // If DS->shade is ShadeFrame -> Draw boundary lines
            if (ds->shade == ShadeFrame) {
                polygon_draw(p, src, ds->color);
            } else {
                // If DS->shade is ShadeConstant -> draw filled using DS->color
                polygon_drawFill(p, src, ds->color, ds);
                // polygon_print(p, stdout);
            }

            polygon_free(p);
            break;

        case ObjMatrix:
            //printf("drawing matrix\n");
            // Left multiply LTM by Matrix field of E (LTM = E * LTM)
            matrix_multiply(&(i->obj.matrix), LTM, LTM);
            break;
        
        case ObjIdentity:
            // printf("drawing identity\n");
            // Set LTM to the identity matrix
            matrix_identity(LTM);
            break;
        
        case ObjModule: ;
            // TM = GTM * LTM
            //printf("recursively drawing module\n");
            Matrix *tempGTM = malloc(sizeof(Matrix));
            matrix_multiply(GTM, LTM, tempGTM);
            // printf("tempGTM:\n");
            // matrix_print(tempGTM, stdout);

            // tempDS = DS
            DrawState *tempDS = malloc(sizeof(DrawState));
            drawstate_copy(tempDS, ds);

            // recursively call module_draw
            module_draw(i->obj.module, VTM, tempGTM, tempDS, lighting, src);
            // printf("Done drawing module\n");
            free(tempGTM);
            free(tempDS);
            break;

        case ObjBezier: ;
            //printf("drawing curve\n");
            // Copy the polygon data in E to P
            BezierCurve* b = malloc(sizeof(BezierCurve));
            bezierCurve_init(b);
            bezierCurve_copy(b, &(i->obj.curve));

            for (int point = 0; point < 4; point++) {
                matrix_xformPoint(LTM, &b->ctrls[point], &b->ctrls[point]);
                matrix_xformPoint(GTM, &b->ctrls[point], &b->ctrls[point]);
                matrix_xformPoint(VTM, &b->ctrls[point], &b->ctrls[point]);
                point_normalize(&b->ctrls[point]);
            }

            bezierCurve_draw_with_subdivisions(b, b->subdivisions, 0, src, ds->color);
            free(b);
            break;

        default:
            printf("module_draw(): Hit unhandled case. Passing over.\n");
            break;
        }
    i = i->next;
    }

    free(LTM);
}


/* 3D MODULE FUNCTIONS */

/**
 * Add a 3D translation matrix to the module.
 */
void module_translate(Module *md, double tx, double ty, double tz) {
    if (!md) {
        printf("module_translate(): passed null pointer.\n");
        return;
    }

    Matrix *m = malloc(sizeof(Matrix));
    matrix_identity(m);
    matrix_translate(m, tx, ty, tz);

    Element *e = element_init(ObjMatrix, m);
    free(m);

    if (!md->tail) {
        md->head = e;
        md->tail = e;
    } else {
        md->tail->next = e; // Set the last element in the list to point to e
        md->tail = e; // Set the last element to be e.
    }
}

/**
 * Add a 3D scale matrix to the Module.
 */
void module_scale(Module *md, double sx, double sy, double sz) {
    if (!md) {
        printf("module_scale(): passed null pointer.\n");
        return;
    }

    Matrix *m = malloc(sizeof(Matrix));
    matrix_identity(m);
    matrix_scale(m, sx, sy, sz);

    Element *e = element_init(ObjMatrix, m);
    free(m);

    if (!md->tail) {
        md->head = e;
        md->tail = e;
    } else {
        md->tail->next = e; // Set the last element in the list to point to e
        md->tail = e; // Set the last element to be e.
    }
}

/**
 * Add a rotation about the x-axis to the Module.
 */
void module_rotateX(Module *md, double cth, double sth) {
    if (!md) {
        printf("module_rotateX(): passed null pointer.\n");
        return;
    }

    Matrix *m = malloc(sizeof(Matrix));
    matrix_identity(m);
    matrix_rotateX(m, cth, sth);

    Element *e = element_init(ObjMatrix, m);
    free(m);

    if (!md->tail) {
        md->head = e;
        md->tail = e;
    } else {
        md->tail->next = e; // Set the last element in the list to point to e
        md->tail = e; // Set the last element to be e.
    }
}

/**
 * Add a rotation about the Y-axis to the Module.
 */
void module_rotateY(Module *md, double cth, double sth) {
    if (!md) {
        printf("module_rotateY(): passed null pointer.\n");
        return;
    }

    Matrix *m = malloc(sizeof(Matrix));
    matrix_identity(m);
    matrix_rotateY(m, cth, sth);

    Element *e = element_init(ObjMatrix, m);
    free(m);

    if (!md->tail) {
        md->head = e;
        md->tail = e;
    } else {
        md->tail->next = e; // Set the last element in the list to point to e
        md->tail = e; // Set the last element to be e.
    }
}


/**
 * Add a rotation that orients to the orthonormal axes u, v and w.
 */
void module_rotateXYZ(Module *md, Vector *u, Vector *v, Vector *w) {
    if (!md || !u || !v || !w) {
        printf("module_rotateXYZ(): passed null pointer.\n");
        return;
    }

    Matrix *m = malloc(sizeof(Matrix));
    matrix_identity(m);
    matrix_rotateXYZ(m, u, v, w);

    Element *e = element_init(ObjMatrix, m);
    free(m);

    if (!md->tail) {
        md->head = e;
        md->tail = e;
    } else {
        md->tail->next = e; // Set the last element in the list to point to e
        md->tail = e; // Set the last element to be e.
    }
}

/**
 * Add a unit cube, axis aligned and centered on zero, to the Module. If <solid>
 * is 0, add only lines. Otherwise, use polygons. Ensure each polygon has
 * surface normals defined.
 */
void module_cube(Module *md, int solid) {
    Point p[8]; // All of the corners of the cube
    Polygon side; // Sides of the cube if drawing solids
    Line edge[12]; // Edges of the cube if drawing lines
    Point tv[4]; // points to be used when actually
    int i;

    // Top of the cube:
    point_set(&(p[0]), 1, 1, 1, 1); // front right
    point_set(&(p[1]), 1, 1, -1, 1); // back right
    point_set(&(p[2]), -1, 1, -1, 1); // back left
    point_set(&(p[3]), -1, 1, 1, 1); // front left

    // Bottom of the cube:
    point_set(&(p[4]), 1, -1, 1, 1); // front right
    point_set(&(p[5]), 1, -1, -1, 1); // back right
    point_set(&(p[6]), -1, -1, -1, 1); // back left
    point_set(&(p[7]), -1, -1, 1, 1); // front left

    if (solid == 0) {
        // Front lines:
        line_set(&(edge[0]), p[0], p[3]);
        line_set(&(edge[1]), p[3], p[7]);
        line_set(&(edge[2]), p[7], p[4]);
        line_set(&(edge[3]), p[4], p[0]);

        // Back lines:
        line_set(&(edge[4]), p[1], p[2]);
        line_set(&(edge[5]), p[2], p[6]);
        line_set(&(edge[6]), p[6], p[5]);
        line_set(&(edge[7]), p[5], p[1]);

        // Joining lines:
        line_set(&(edge[8]), p[0], p[1]);
        line_set(&(edge[9]), p[3], p[2]);
        line_set(&(edge[10]), p[4], p[5]);
        line_set(&(edge[11]), p[7], p[6]);

        for (i = 0; i < 12; i++) {
            // Add the lines to the module:
            module_line(md, &(edge[i]));
        }
    } else {
        polygon_init(&side);

        // Top side:
        polygon_set(&(side), 4, &(p[0]));
        Vector normal = {{0.0, 1.0, 0.0}};
        for (i = 0; i < 4; i++) vector_copy(&side.normal[i], &normal);
        module_polygon(md, &side);
        polygon_clear(&side);

        // Bottom side:
        polygon_set(&(side), 4, &(p[4]));
        vector_set(&normal, 0.0, -1.0, 0.0);
        for (i = 0; i < 4; i++) vector_copy(&side.normal[i], &normal);
        module_polygon(md, &side);
        polygon_clear(&side);

        // front side (positive z):
        point_copy(&(tv[0]), &(p[0]));
        point_copy(&(tv[1]), &(p[3]));
        point_copy(&(tv[2]), &(p[7]));
        point_copy(&(tv[3]), &(p[4]));
        polygon_set(&(side), 4, &(tv[0]));
        vector_set(&normal, 0.0, 0.0, 1.0);
        for (i = 0; i < 4; i++) vector_copy(&side.normal[i], &normal);
        module_polygon(md, &side);
        polygon_clear(&side);

        // back side (negative z):
        point_copy(&(tv[0]), &(p[1]));
        point_copy(&(tv[1]), &(p[2]));
        point_copy(&(tv[2]), &(p[6]));
        point_copy(&(tv[3]), &(p[5]));
        polygon_set(&(side), 4, &(tv[0]));
        vector_set(&normal, 0.0, 0.0, -1.0);
        for (i = 0; i < 4; i++) vector_copy(&side.normal[i], &normal);
        module_polygon(md, &side);
        polygon_clear(&side);

        // left side (negative x)
        point_copy(&(tv[0]), &(p[3]));
        point_copy(&(tv[1]), &(p[2]));
        point_copy(&(tv[2]), &(p[6]));
        point_copy(&(tv[3]), &(p[7]));
        polygon_set(&(side), 4, &(tv[0]));
        vector_set(&normal, -1.0, 0.0, 0.0);
        for (i = 0; i < 4; i++) vector_copy(&side.normal[i], &normal);
        module_polygon(md, &side);
        polygon_clear(&side);

        // right side (positive x):
        point_copy(&(tv[0]), &(p[0]));
        point_copy(&(tv[1]), &(p[1]));
        point_copy(&(tv[2]), &(p[5]));
        point_copy(&(tv[3]), &(p[4]));
        polygon_set(&(side), 4, &(tv[0]));
        vector_set(&normal, 1.0, 0.0, 0.0);
        for (i = 0; i < 4; i++) vector_copy(&side.normal[i], &normal);
        module_polygon(md, &side);
        polygon_clear(&side);
        
        // for (i = 0; i < 2; i++) {
        //     module_polygon(md, &(side[i])); // Add the polygon to the module
        //     polygon_clear(&(side[i])); // free the polygon
        // }
    }
}

/* BEZIER CURVE AND SURFACE FUNCTIONS */
void module_bezierCurve(Module *m, BezierCurve *b, int divisions) {
    if (!m || !b) {
        printf("module_bezierCurve(): passed null pointer.\n");
        return;
    }

    b->subdivisions = divisions;
    Element *e = element_init(ObjBezier, b);
    if (!m->tail) {
        m->head = e;
        m->tail = e;
    } else {
        m->tail->next = e; // Set the last element in the list to point to e
        m->tail = e; // Set the last element to be e.
    }
}

void module_bezierSurface(Module *m, BezierSurface *b, int divisions, int solid) {
    b->subdivisions = divisions;
    Line l;
    if (divisions <= 0) {
        switch(solid) {
            case 0:
                for (int i = 0; i < 4; i++) {
                    for (int j = 1; j < 4; j++) {
                        line_set(&l, b->ctrls[i*4 + j - 1], b->ctrls[i*4 + j]);
                        module_line(m, &l);
                        line_set(&l, b->ctrls[(j-1)*4 + i], b->ctrls[j*4 + i]);                        
                        module_line(m, &l);
                    }
                }
                return;

            default:
                return;
        }
    }
    
    BezierSurface q, r;
    bezierSurface_init(&q);
    bezierSurface_copy(&q, b); //TODO: cleanup this copy
    bezierSurface_init(&r);
    bezierSurface_copy(&r, b);

    // Subdivide in x:
    for (int i = 0; i < 4; i++) {
        q.ctrls[i*4 + 1].val[0] = (b->ctrls[i*4].val[0] + b->ctrls[i*4 + 1].val[0]) / 2;
        q.ctrls[i*4 + 1].val[1] = (b->ctrls[i*4].val[1] + b->ctrls[i*4 + 1].val[1]) / 2;
        q.ctrls[i*4 + 1].val[2] = (b->ctrls[i*4].val[2] + b->ctrls[i*4 + 1].val[2]) / 2;

        q.ctrls[i*4 + 2].val[0] = (q.ctrls[i*4 + 1].val[0] / 2) + (b->ctrls[i*4 + 1].val[0] + b->ctrls[i*4 + 2].val[0]) / 4;
        q.ctrls[i*4 + 2].val[1] = (q.ctrls[i*4 + 1].val[1] / 2) + (b->ctrls[i*4 + 1].val[1] + b->ctrls[i*4 + 2].val[1]) / 4;
        q.ctrls[i*4 + 2].val[2] = (q.ctrls[i*4 + 1].val[2] / 2) + (b->ctrls[i*4 + 1].val[2] + b->ctrls[i*4 + 2].val[2]) / 4;

        r.ctrls[i*4 + 2].val[0] = (b->ctrls[i*4 + 2].val[0] + b->ctrls[i*4 + 3].val[0]) / 2;
        r.ctrls[i*4 + 2].val[1] = (b->ctrls[i*4 + 2].val[1] + b->ctrls[i*4 + 3].val[1]) / 2;
        r.ctrls[i*4 + 2].val[2] = (b->ctrls[i*4 + 2].val[2] + b->ctrls[i*4 + 3].val[2]) / 2;

        r.ctrls[i*4 + 1].val[0] = (r.ctrls[i*4 + 2].val[0] / 2) + (b->ctrls[i*4 + 1].val[0] + b->ctrls[i*4 + 2].val[0]) / 4;
        r.ctrls[i*4 + 1].val[1] = (r.ctrls[i*4 + 2].val[1] / 2) + (b->ctrls[i*4 + 1].val[1] + b->ctrls[i*4 + 2].val[1]) / 4;
        r.ctrls[i*4 + 1].val[2] = (r.ctrls[i*4 + 2].val[2] / 2) + (b->ctrls[i*4 + 1].val[2] + b->ctrls[i*4 + 2].val[2]) / 4;

        q.ctrls[i*4 + 3].val[0] = (q.ctrls[i*4 + 2].val[0] + r.ctrls[i*4 + 1].val[0]) / 2;
        q.ctrls[i*4 + 3].val[1] = (q.ctrls[i*4 + 2].val[1] + r.ctrls[i*4 + 1].val[1]) / 2;
        q.ctrls[i*4 + 3].val[2] = (q.ctrls[i*4 + 2].val[2] + r.ctrls[i*4 + 1].val[2]) / 2;

        point_copy(&r.ctrls[i*4 + 0], &q.ctrls[i*4 + 3]);
    }

    BezierSurface qUp, qDown, rUp, rDown;
    bezierSurface_init(&qUp);
    bezierSurface_copy(&qUp, &q); //TODO: cleanup this copy
    bezierSurface_init(&rUp);
    bezierSurface_copy(&rUp, &r);
    bezierSurface_init(&qDown);
    bezierSurface_copy(&qDown, &q); //TODO: cleanup this copy
    bezierSurface_init(&rDown);
    bezierSurface_copy(&rDown, &r);

    // Subdivide q in z:
    for (int i = 0; i < 4; i++) {
        qUp.ctrls[i + 1 * 4].val[0] = (q.ctrls[i].val[0] + q.ctrls[i + 1 * 4].val[0]) / 2;
        qUp.ctrls[i + 1 * 4].val[1] = (q.ctrls[i].val[1] + q.ctrls[i + 1 * 4].val[1]) / 2;
        qUp.ctrls[i + 1 * 4].val[2] = (q.ctrls[i].val[2] + q.ctrls[i + 1 * 4].val[2]) / 2;

        qUp.ctrls[i + 2 * 4].val[0] = (qUp.ctrls[i + 1 * 4].val[0] / 2) + (q.ctrls[i + 1 * 4].val[0] + q.ctrls[i + 2 * 4].val[0]) / 4;
        qUp.ctrls[i + 2 * 4].val[1] = (qUp.ctrls[i + 1 * 4].val[1] / 2) + (q.ctrls[i + 1 * 4].val[1] + q.ctrls[i + 2 * 4].val[1]) / 4;
        qUp.ctrls[i + 2 * 4].val[2] = (qUp.ctrls[i + 1 * 4].val[2] / 2) + (q.ctrls[i + 1 * 4].val[2] + q.ctrls[i + 2 * 4].val[2]) / 4;

        qDown.ctrls[i + 2 * 4].val[0] = (q.ctrls[i + 2 * 4].val[0] + q.ctrls[i + 3 * 4].val[0]) / 2;
        qDown.ctrls[i + 2 * 4].val[1] = (q.ctrls[i + 2 * 4].val[1] + q.ctrls[i + 3 * 4].val[1]) / 2;
        qDown.ctrls[i + 2 * 4].val[2] = (q.ctrls[i + 2 * 4].val[2] + q.ctrls[i + 3 * 4].val[2]) / 2;

        qDown.ctrls[i + 1 * 4].val[0] = (qDown.ctrls[i + 2 * 4].val[0] / 2) + (q.ctrls[i + 1 * 4].val[0] + q.ctrls[i + 2 * 4].val[0]) / 4;
        qDown.ctrls[i + 1 * 4].val[1] = (qDown.ctrls[i + 2 * 4].val[1] / 2) + (q.ctrls[i + 1 * 4].val[1] + q.ctrls[i + 2 * 4].val[1]) / 4;
        qDown.ctrls[i + 1 * 4].val[2] = (qDown.ctrls[i + 2 * 4].val[2] / 2) + (q.ctrls[i + 1 * 4].val[2] + q.ctrls[i + 2 * 4].val[2]) / 4;

        qUp.ctrls[i + 3 * 4].val[0] = (qUp.ctrls[i + 2 * 4].val[0] + qDown.ctrls[i + 1 * 4].val[0]) / 2;
        qUp.ctrls[i + 3 * 4].val[1] = (qUp.ctrls[i + 2 * 4].val[1] + qDown.ctrls[i + 1 * 4].val[1]) / 2;
        qUp.ctrls[i + 3 * 4].val[2] = (qUp.ctrls[i + 2 * 4].val[2] + qDown.ctrls[i + 1 * 4].val[2]) / 2;

        point_copy(&qDown.ctrls[i], &qUp.ctrls[i + 3 * 4]);
    }

    // Subdivide r in z:
    for (int i = 0; i < 4; i++) {
        rUp.ctrls[i + 1 * 4].val[0] = (r.ctrls[i].val[0] + r.ctrls[i + 1 * 4].val[0]) / 2;
        rUp.ctrls[i + 1 * 4].val[1] = (r.ctrls[i].val[1] + r.ctrls[i + 1 * 4].val[1]) / 2;
        rUp.ctrls[i + 1 * 4].val[2] = (r.ctrls[i].val[2] + r.ctrls[i + 1 * 4].val[2]) / 2;

        rUp.ctrls[i + 2 * 4].val[0] = (rUp.ctrls[i + 1 * 4].val[0] / 2) + (r.ctrls[i + 1 * 4].val[0] + r.ctrls[i + 2 * 4].val[0]) / 4;
        rUp.ctrls[i + 2 * 4].val[1] = (rUp.ctrls[i + 1 * 4].val[1] / 2) + (r.ctrls[i + 1 * 4].val[1] + r.ctrls[i + 2 * 4].val[1]) / 4;
        rUp.ctrls[i + 2 * 4].val[2] = (rUp.ctrls[i + 1 * 4].val[2] / 2) + (r.ctrls[i + 1 * 4].val[2] + r.ctrls[i + 2 * 4].val[2]) / 4;

        rDown.ctrls[i + 2 * 4].val[0] = (r.ctrls[i + 2 * 4].val[0] + r.ctrls[i + 3 * 4].val[0]) / 2;
        rDown.ctrls[i + 2 * 4].val[1] = (r.ctrls[i + 2 * 4].val[1] + r.ctrls[i + 3 * 4].val[1]) / 2;
        rDown.ctrls[i + 2 * 4].val[2] = (r.ctrls[i + 2 * 4].val[2] + r.ctrls[i + 3 * 4].val[2]) / 2;

        rDown.ctrls[i + 1 * 4].val[0] = (rDown.ctrls[i + 2 * 4].val[0] / 2) + (r.ctrls[i + 1 * 4].val[0] + r.ctrls[i + 2 * 4].val[0]) / 4;
        rDown.ctrls[i + 1 * 4].val[1] = (rDown.ctrls[i + 2 * 4].val[1] / 2) + (r.ctrls[i + 1 * 4].val[1] + r.ctrls[i + 2 * 4].val[1]) / 4;
        rDown.ctrls[i + 1 * 4].val[2] = (rDown.ctrls[i + 2 * 4].val[2] / 2) + (r.ctrls[i + 1 * 4].val[2] + r.ctrls[i + 2 * 4].val[2]) / 4;

        rUp.ctrls[i + 3 * 4].val[0] = (rUp.ctrls[i + 2 * 4].val[0] + rDown.ctrls[i + 1 * 4].val[0]) / 2;
        rUp.ctrls[i + 3 * 4].val[1] = (rUp.ctrls[i + 2 * 4].val[1] + rDown.ctrls[i + 1 * 4].val[1]) / 2;
        rUp.ctrls[i + 3 * 4].val[2] = (rUp.ctrls[i + 2 * 4].val[2] + rDown.ctrls[i + 1 * 4].val[2]) / 2;

        point_copy(&rDown.ctrls[i], &rUp.ctrls[i + 3 * 4]);
    }

    module_bezierSurface(m, &qUp, divisions-1, solid);
    module_bezierSurface(m, &qDown, divisions-1, solid);
    module_bezierSurface(m, &rUp, divisions-1, solid);
    module_bezierSurface(m, &rDown, divisions-1, solid);
}

/**
 * Makes a unit cylinder at the origin. 
 * 
 * This function is directly adapted from code provided by Bruce Maxwell in the 
 * file test6d.c.
 */
void module_cylinder(Module *md, int sides) {
    Polygon p;
    Point xtop, xbot;
    double x1, x2, z1, z2;
    int i;

    polygon_init( &p );
    point_set3D( &xtop, 0, 1.0, 0.0 );
    point_set3D( &xbot, 0, 0.0, 0.0 );

    // make a fan for the top and bottom sides
    // and quadrilaterals for the sides
    for(i=0;i<sides;i++) {
      Point pt[4];

      // Make the fan
      // Top:
      // Define x/z for the fan points on outside of cylinder
      x1 = cos( i * M_PI * 2.0 / sides );
      z1 = sin( i * M_PI * 2.0 / sides );
      x2 = cos( ( (i+1)%sides ) * M_PI * 2.0 / sides );
      z2 = sin( ( (i+1)%sides ) * M_PI * 2.0 / sides );

      // copy those points into pt[]
      point_copy( &pt[0], &xtop );
      point_set3D( &pt[1], x1, 1.0, z1 );
      point_set3D( &pt[2], x2, 1.0, z2 );

      // Set a polygon for the fan:
      polygon_set( &p, 3, pt );
      module_polygon( md, &p );

      // Do the same for the bottom:
      point_copy( &pt[0], &xbot );
      point_set3D( &pt[1], x1, 0.0, z1 );
      point_set3D( &pt[2], x2, 0.0, z2 );

      polygon_set( &p, 3, pt );
      module_polygon( md, &p );

      // Link the top and bottom with a rectangular side
      point_set3D( &pt[0], x1, 0.0, z1 );
      point_set3D( &pt[1], x2, 0.0, z2 );
      point_set3D( &pt[2], x2, 1.0, z2 );
      point_set3D( &pt[3], x1, 1.0, z1 );

      polygon_set( &p, 4, pt );
      module_polygon( md, &p );
    }

    polygon_clear( &p );
}

/**
 * Insert a unit cone with <sides> subdivisions centered at the origin. Note
 * that this does not create a smooth surface, but rather a subdivision surface.
 * So, for instance, setting sides to 4 will create a unit pyramid.
 * 
 * Adapted from Cylinder code provide by Bruce Maxwell.
 */
void module_cone(Module *md, int sides) {
    Polygon p;
    Point xtop, xbot;
    double x1, x2, z1, z2;
    int i;

    polygon_init( &p );
    point_set3D( &xtop, 0, 1.0, 0.0 );
    point_set3D( &xbot, 0, 0.0, 0.0 );

    // make a fan for the top and bottom sides
    // and triangles for the sides
    for(i=0;i<sides;i++) {
      Point pt[4];

      // Make the fan
      // bottom:
      // Define x/z for the fan points on outside of cylinder
      x1 = cos( i * M_PI * 2.0 / sides );
      z1 = sin( i * M_PI * 2.0 / sides );
      x2 = cos( ( (i+1)%sides ) * M_PI * 2.0 / sides );
      z2 = sin( ( (i+1)%sides ) * M_PI * 2.0 / sides );

      // copy those points into pt[]
    //   point_copy( &pt[0], &xbot );
      point_set3D( &pt[0], x1, 0.0, z1 );
      point_set3D( &pt[1], x2, 0.0, z2 );

      // Set a polygon for the fan:
      polygon_set( &p, 2, pt );
      module_polygon( md, &p );

      // Link the top and bottom with a triangular side
      point_set3D( &pt[0], x1, 0.0, z1 );
      point_set3D( &pt[1], x2, 0.0, z2 );
      point_copy( &pt[2], &xtop);

      polygon_set( &p, 3, pt );
      module_polygon( md, &p );
    }

    polygon_clear( &p );
}

/**
 * Inserts a tetrahedron into the module.
 */
void module_tetrahedron(Module *md) {
    const float inverseSqrt2 = 0.70710678118;
    Point pts[4] = {{{1.0, 0.0, -inverseSqrt2, 1.0}}, 
                    {{-1.0, 0.0, -inverseSqrt2, 1.0}},
                    {{0.0, 1.0, inverseSqrt2, 1.0}},
                    {{0.0, -1.0, inverseSqrt2, 1.0}}};
    Polygon p;
    polygon_init(&p);
    Point vlist[3];
    polygon_set(&p, 3, pts);
    module_polygon(md, &p);

    point_copy(&vlist[0], &pts[0]);
    point_copy(&vlist[1], &pts[2]);
    point_copy(&vlist[2], &pts[3]);
    polygon_set(&p, 3, vlist);
    module_polygon(md, &p);

    point_copy(&vlist[0], &pts[0]);
    point_copy(&vlist[1], &pts[1]);
    point_copy(&vlist[2], &pts[3]);
    polygon_set(&p, 3, vlist);
    module_polygon(md, &p);

    polygon_clear(&p);
}

/**
 * Inserts an octahedron into the module
 */
void module_octahedron(Module *md){
    Point pts[6] = {{{1.0,  0.0,  0.0, 1.0}}, 
                    {{-1.0, 0.0,  0.0, 1.0}},
                    {{0.0,  1.0,  0.0, 1.0}},
                    {{0.0, -1.0,  0.0, 1.0}},
                    {{0.0,  0.0,  1.0, 1.0}}, 
                    {{0.0,  0.0, -1.0, 1.0}}};

    Polygon p;
    polygon_init(&p);
    Point vlist[3];

    // top
    point_copy(&vlist[0], &pts[0]);
    point_copy(&vlist[1], &pts[4]);
    point_copy(&vlist[2], &pts[2]);
    polygon_set(&p, 3, vlist);
    module_polygon(md, &p);

    point_copy(&vlist[0], &pts[1]);
    polygon_set(&p, 3, vlist);
    module_polygon(md, &p);

    point_copy(&vlist[0], &pts[0]);
    point_copy(&vlist[1], &pts[5]);
    polygon_set(&p, 3, vlist);
    module_polygon(md, &p);

    point_copy(&vlist[0], &pts[1]);
    polygon_set(&p, 3, vlist);
    module_polygon(md, &p);

    // Bottom:
    point_copy(&vlist[0], &pts[0]);
    point_copy(&vlist[1], &pts[4]);
    point_copy(&vlist[2], &pts[3]);
    polygon_set(&p, 3, vlist);
    module_polygon(md, &p);

    point_copy(&vlist[0], &pts[1]);
    polygon_set(&p, 3, vlist);
    module_polygon(md, &p);

    point_copy(&vlist[0], &pts[0]);
    point_copy(&vlist[1], &pts[5]);
    polygon_set(&p, 3, vlist);
    module_polygon(md, &p);

    point_copy(&vlist[0], &pts[1]);
    polygon_set(&p, 3, vlist);
    module_polygon(md, &p);

    polygon_clear(&p);
}

/**
 * Adds the Utah Teapot to the module, defined by a bunch of Bezier Surfaces.
 * 
 * Vertices were pulled from:
 * https://www.sjbaker.org/wiki/index.php?title=The_History_of_The_Teapot
 */
void module_teapot(Module *md, int subdivisions) {
    BezierSurface s;

    // Defines the vertices to be used in the bezier surface for each component
    int rim[16] =  {102, 103, 104, 105,   4,   5,   6,   7,
                      8,   9,  10,  11,  12,  13,  14,  15};
    int body_1[16] = {12,  13,  14,  15,  16,  17,  18,  19,
                      20,  21,  22,  23,  24,  25,  26,  27};
    int body_2[16] = {24,  25,  26,  27,  29,  30,  31,  32,
                      33,  34,  35,  36,  37,  38,  39,  40};
    int lid_1[16] = {96,  96,  96,  96,  97,  98,  99, 100,
                    101, 101, 101, 101,   0,   1,   2,   3};
    int lid_2[16] = {0,   1,   2,   3, 106, 107, 108, 109,
                   110, 111, 112, 113, 114, 115, 116, 117};
    int handle_1[16] = {41,  42,  43,  44,  45,  46,  47,  48,
                        49,  50,  51,  52,  53,  54,  55,  56};
    int handle_2[16] = {53,  54,  55,  56,  57,  58,  59,  60,
                        61,  62,  63,  64,  28,  65,  66,  67};
    int spout_1[16] = {68,  69,  70,  71,  72,  73,  74,  75,
                       76,  77,  78,  79,  80,  81,  82,  83};
    int spout_2[16] = {80,  81,  82,  83,  84,  85,  86,  87,
                       88,  89,  90,  91,  92,  93,  94,  95};
    Point vlist[118] = {
    {{0.2000,  0.0000, 2.70000, 1.0 }}, {{0.2000, -0.1120, 2.70000, 1.0 }},
    {{0.1120, -0.2000, 2.70000, 1.0 }}, {{0.0000, -0.2000, 2.70000, 1.0 }},
    {{1.3375,  0.0000, 2.53125, 1.0 }}, {{1.3375, -0.7490, 2.53125, 1.0 }},
    {{0.7490, -1.3375, 2.53125, 1.0 }}, {{0.0000, -1.3375, 2.53125, 1.0 }},
    {{1.4375,  0.0000, 2.53125, 1.0 }}, {{1.4375, -0.8050, 2.53125, 1.0 }},
    {{0.8050, -1.4375, 2.53125, 1.0 }}, {{0.0000, -1.4375, 2.53125, 1.0 }},
    {{1.5000,  0.0000, 2.40000, 1.0 }}, {{1.5000, -0.8400, 2.40000, 1.0 }},
    {{0.8400, -1.5000, 2.40000, 1.0 }}, {{0.0000, -1.5000, 2.40000, 1.0 }},
    {{1.7500,  0.0000, 1.87500, 1.0 }}, {{1.7500, -0.9800, 1.87500, 1.0 }},
    {{0.9800, -1.7500, 1.87500, 1.0 }}, {{0.0000, -1.7500, 1.87500, 1.0 }},
    {{2.0000,  0.0000, 1.35000, 1.0 }}, {{2.0000, -1.1200, 1.35000, 1.0 }},
    {{1.1200, -2.0000, 1.35000, 1.0 }}, {{0.0000, -2.0000, 1.35000, 1.0 }},
    {{2.0000,  0.0000, 0.90000, 1.0 }}, {{2.0000, -1.1200, 0.90000, 1.0 }},
    {{1.1200, -2.0000, 0.90000, 1.0 }}, {{0.0000, -2.0000, 0.90000, 1.0 }},
    {{2.0000,  0.0000, 0.90000, 1.0 }}, {{2.0000,  0.0000, 0.45000, 1.0 }},
    {{2.0000, -1.1200, 0.45000, 1.0 }}, {{1.1200, -2.0000, 0.45000, 1.0 }},
    {{0.0000, -2.0000, 0.45000, 1.0 }}, {{1.5000,  0.0000, 0.22500, 1.0 }},
    {{1.5000, -0.8400, 0.22500, 1.0 }}, {{0.8400, -1.5000, 0.22500, 1.0 }},
    {{0.0000, -1.5000, 0.22500, 1.0 }}, {{1.5000,  0.0000, 0.15000, 1.0 }},
    {{1.5000, -0.8400, 0.15000, 1.0 }}, {{0.8400, -1.5000, 0.15000, 1.0 }},
    {{0.0000, -1.5000, 0.15000, 1.0 }}, {{ -1.6000,  0.0000, 2.02500, 1.0 }},
    {{-1.6000, -0.3000, 2.02500, 1.0 }}, {{-1.5000, -0.3000, 2.25000, 1.0 }},
    {{-1.5000,  0.0000, 2.25000, 1.0 }}, {{-2.3000,  0.0000, 2.02500, 1.0 }},
    {{-2.3000, -0.3000, 2.02500, 1.0 }}, {{-2.5000, -0.3000, 2.25000, 1.0 }},
    {{-2.5000,  0.0000, 2.25000, 1.0 }}, {{-2.7000,  0.0000, 2.02500, 1.0 }},
    {{-2.7000, -0.3000, 2.02500, 1.0 }}, {{-3.0000, -0.3000, 2.25000, 1.0 }},
    {{-3.0000,  0.0000, 2.25000, 1.0 }}, {{-2.7000,  0.0000, 1.80000, 1.0 }},
    {{-2.7000, -0.3000, 1.80000, 1.0 }}, {{-3.0000, -0.3000, 1.80000, 1.0 }},
    {{-3.0000,  0.0000, 1.80000, 1.0 }}, {{-2.7000,  0.0000, 1.57500, 1.0 }},
    {{-2.7000, -0.3000, 1.57500, 1.0 }}, {{-3.0000, -0.3000, 1.35000, 1.0 }},
    {{-3.0000,  0.0000, 1.35000, 1.0 }}, {{-2.5000,  0.0000, 1.12500, 1.0 }},
    {{-2.5000, -0.3000, 1.12500, 1.0 }}, {{-2.6500, -0.3000, 0.93750, 1.0 }},
    {{-2.6500,  0.0000, 0.93750, 1.0 }}, {{-2.0000, -0.3000, 0.90000, 1.0 }},
    {{-1.9000, -0.3000, 0.60000, 1.0 }}, {{-1.9000,  0.0000, 0.60000, 1.0 }},
    {{1.7000,  0.0000, 1.42500, 1.0 }}, {{1.7000, -0.6600, 1.42500, 1.0 }},
    {{1.7000, -0.6600, 0.60000, 1.0 }}, {{1.7000,  0.0000, 0.60000, 1.0 }},
    {{2.6000,  0.0000, 1.42500, 1.0 }}, {{2.6000, -0.6600, 1.42500, 1.0 }},
    {{3.1000, -0.6600, 0.82500, 1.0 }}, {{3.1000,  0.0000, 0.82500, 1.0 }},
    {{2.3000,  0.0000, 2.10000, 1.0 }}, {{2.3000, -0.2500, 2.10000, 1.0 }},
    {{2.4000, -0.2500, 2.02500, 1.0 }}, {{2.4000,  0.0000, 2.02500, 1.0 }},
    {{2.7000,  0.0000, 2.40000, 1.0 }}, {{2.7000, -0.2500, 2.40000, 1.0 }},
    {{3.3000, -0.2500, 2.40000, 1.0 }}, {{3.3000,  0.0000, 2.40000, 1.0 }},
    {{2.8000,  0.0000, 2.47500, 1.0 }}, {{2.8000, -0.2500, 2.47500, 1.0 }},
    {{3.5250, -0.2500, 2.49375, 1.0 }}, {{3.5250,  0.0000, 2.49375, 1.0 }},
    {{2.9000,  0.0000, 2.47500, 1.0 }}, {{2.9000, -0.1500, 2.47500, 1.0 }},
    {{3.4500, -0.1500, 2.51250, 1.0 }}, {{3.4500,  0.0000, 2.51250, 1.0 }},
    {{2.8000,  0.0000, 2.40000, 1.0 }}, {{2.8000, -0.1500, 2.40000, 1.0 }},
    {{3.2000, -0.1500, 2.40000, 1.0 }}, {{3.2000,  0.0000, 2.40000, 1.0 }},
    {{0.0000,  0.0000, 3.15000, 1.0 }}, {{0.8000,  0.0000, 3.15000, 1.0 }},
    {{0.8000, -0.4500, 3.15000, 1.0 }}, {{0.4500, -0.8000, 3.15000, 1.0 }},
    {{0.0000, -0.8000, 3.15000, 1.0 }}, {{0.0000,  0.0000, 2.85000, 1.0 }},
    {{1.4000,  0.0000, 2.40000, 1.0 }}, {{1.4000, -0.7840, 2.40000, 1.0 }},
    {{0.7840, -1.4000, 2.40000, 1.0 }}, {{0.0000, -1.4000, 2.40000, 1.0 }},
    {{0.4000,  0.0000, 2.55000, 1.0 }}, {{0.4000, -0.2240, 2.55000, 1.0 }},
    {{0.2240, -0.4000, 2.55000, 1.0 }}, {{0.0000, -0.4000, 2.55000, 1.0 }},
    {{1.3000,  0.0000, 2.55000, 1.0 }}, {{1.3000, -0.7280, 2.55000, 1.0 }},
    {{0.7280, -1.3000, 2.55000, 1.0 }}, {{0.0000, -1.3000, 2.55000, 1.0 }},
    {{1.3000,  0.0000, 2.40000, 1.0 }}, {{1.3000, -0.7280, 2.40000, 1.0 }},
    {{0.7280, -1.3000, 2.40000, 1.0 }}, {{0.0000, -1.3000, 2.40000, 1.0 }}};
    
    bezierSurface_init(&s);
    // Add the rim:
    for (int i = 0; i < 16; i++) {
        point_copy(&s.ctrls[i], &vlist[rim[i]]);
    }
    module_rotateX(md, 0, -1);
    module_bezierSurface(md, &s, subdivisions, 0);
    module_rotateY(md, 0, 1);
    module_bezierSurface(md, &s, subdivisions, 0);
    module_rotateY(md, 0, 1);
    module_bezierSurface(md, &s, subdivisions, 0);
    module_rotateY(md, 0, 1);
    module_bezierSurface(md, &s, subdivisions, 0);
    
    // Add the body:
    for (int i = 0; i < 16; i++) {
        point_copy(&s.ctrls[i], &vlist[body_1[i]]);
    }
    module_bezierSurface(md, &s, subdivisions, 0);
    module_rotateY(md, 0, 1);
    module_bezierSurface(md, &s, subdivisions, 0);
    module_rotateY(md, 0, 1);
    module_bezierSurface(md, &s, subdivisions, 0);
    module_rotateY(md, 0, 1);
    module_bezierSurface(md, &s, subdivisions, 0);

    for (int i = 0; i < 16; i++) {
        point_copy(&s.ctrls[i], &vlist[body_2[i]]);
    }
    module_bezierSurface(md, &s, subdivisions, 0);
    module_rotateY(md, 0, 1);
    module_bezierSurface(md, &s, subdivisions, 0);
    module_rotateY(md, 0, 1);
    module_bezierSurface(md, &s, subdivisions, 0);
    module_rotateY(md, 0, 1);
    module_bezierSurface(md, &s, subdivisions, 0);

    // Add the lid:
    for (int i = 0; i < 16; i++) {
        point_copy(&s.ctrls[i], &vlist[lid_1[i]]);
    }
    module_bezierSurface(md, &s, subdivisions, 0);
    module_rotateY(md, 0, 1);
    module_bezierSurface(md, &s, subdivisions, 0);
    module_rotateY(md, 0, 1);
    module_bezierSurface(md, &s, subdivisions, 0);
    module_rotateY(md, 0, 1);
    module_bezierSurface(md, &s, subdivisions, 0);
    
    for (int i = 0; i < 16; i++) {
        point_copy(&s.ctrls[i], &vlist[lid_2[i]]);
    }
    module_bezierSurface(md, &s, subdivisions, 0);
    module_rotateY(md, 0, 1);
    module_bezierSurface(md, &s, subdivisions, 0);
    module_rotateY(md, 0, 1);
    module_bezierSurface(md, &s, subdivisions, 0);
    module_rotateY(md, 0, 1);
    module_bezierSurface(md, &s, subdivisions, 0);

    // Add the handle:
    for (int i = 0; i < 16; i++) { // 1st patch
        point_copy(&s.ctrls[i], &vlist[handle_1[i]]);
    }
    module_bezierSurface(md, &s, subdivisions, 0);
    
    for (int i = 0; i < 16; i++) { // reflected 1st patch
        Point pt;
        point_copy(&pt, &vlist[handle_1[i]]);
        pt.val[1] = -pt.val[1];
        point_copy(&s.ctrls[i], &pt);
    }
    module_bezierSurface(md, &s, subdivisions, 0);
    
    for (int i = 0; i < 16; i++) { // 2nd patch
        point_copy(&s.ctrls[i], &vlist[handle_2[i]]);
    }
    module_bezierSurface(md, &s, subdivisions, 0);

    for (int i = 0; i < 16; i++) { // reflected 2nd patch
        Point pt;
        point_copy(&pt, &vlist[handle_2[i]]);
        pt.val[1] = -pt.val[1];
        point_copy(&s.ctrls[i], &pt);
    }
    module_bezierSurface(md, &s, subdivisions, 0);

    // Add the spout:
    for (int i = 0; i < 16; i++) { // 1/2 of base
        point_copy(&s.ctrls[i], &vlist[spout_1[i]]);
        point_print(&s.ctrls[i], stdout);
    }
    module_bezierSurface(md, &s, subdivisions, 0);
    
    for (int i = 0; i < 16; i++) { // reflected 1/2 of base
        Point pt;
        point_copy(&pt, &vlist[spout_1[i]]);
        pt.val[1] = -pt.val[1];
        point_copy(&s.ctrls[i], &pt);
    }
    module_bezierSurface(md, &s, subdivisions, 0);
    
    for (int i = 0; i < 16; i++) { // 1/2 of tip
        point_copy(&s.ctrls[i], &vlist[spout_2[i]]);
    }
    module_bezierSurface(md, &s, subdivisions, 0);

    for (int i = 0; i < 16; i++) { // reflected 1/2 of tip
        Point pt;
        point_copy(&pt, &vlist[spout_2[i]]);
        pt.val[1] = -pt.val[1];
        point_copy(&s.ctrls[i], &pt);
    }
    module_bezierSurface(md, &s, subdivisions, 0);
}

/* SHADING/COLOR MODULE FUNCTIONS */

/**
 * Add the foreground color value to the tail of the module's list.
 */
void module_color(Module *md, Color *c) {
    if (!md || !c) {
        printf("module_color(): passed null pointer.\n");
        return;
    }

    Element *e = element_init(ObjColor, c);
    if (!md->tail) {
        md->head = e;
        md->tail = e;
    } else {
        md->tail->next = e; // Set the last element in the list to point to e
        md->tail = e; // Set the last element to be e.
    }
}

/**
 * Add the body color value to the tail of the module's list.
 */
void module_bodyColor(Module  *md, Color *c) {
    if (!md || !c) {
        printf("module_bodyColor(): passed null pointer.\n");
        return;
    }

    Element *e = element_init(ObjBodyColor, c);
    if (!md->tail) {
        md->head = e;
        md->tail = e;
    } else {
        md->tail->next = e; // Set the last element in the list to point to e
        md->tail = e; // Set the last element to be e.
    }
}

/**
 * Add the surface color value to the tail of the module's list.
 */
void module_surfaceColor(Module *md, Color *c) {
    if (!md || !c) {
        printf("module_surfaceColor(): passed null pointer.\n");
        return;
    }

    Element *e = element_init(ObjSurfaceColor, c);
    if (!md->tail) {
        md->head = e;
        md->tail = e;
    } else {
        md->tail->next = e; // Set the last element in the list to point to e
        md->tail = e; // Set the last element to be e.
    }
}

/**
 * Add the specular coefficient to the tail of the module's list.
 */
void module_surfaceCoeff(Module *md, float coeff) {
    if (!md) {
        printf("module_surfaceCoeff(): passed null pointer.\n");
        return;
    }

    Element *e = element_init(ObjSurfaceCoeff, &coeff);
    if (!md->tail) {
        md->head = e;
        md->tail = e;
    } else {
        md->tail->next = e; // Set the last element in the list to point to e
        md->tail = e; // Set the last element to be e.
    }
}