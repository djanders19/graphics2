/**
 * Polygon.c - David J Anderson, 10/11/2021
 * Adapted from skeleton code/spec by Bruce Maxwell, Roux institute, F'2021
 * 
 * This file implements polygon.h, providing various methods for creating,
 * editing, and drawing filled an unfilled polygons.
 * 
 * For drawing, it provides three methods. The first draws the outline of a 
 * polygon and behaves identically to the polyline draw method.
 * 
 * The second two draw filled polygons. polygon_drawFill() draws a filled
 * polygon with a specified color using the scanline fill algorithm, while
 * polygon_drawfillB draws triangles with the barycentric fill algorithm. In the
 * future, I will extend these algorithms to enable the drawing of polygons in
 * 3d.
 */

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <math.h>
#include "graphicslib.h"


/* The functions polygon create and polygon free manage both the Polygon data 
structure and the memory required for the vertex list. */

/**
 * Returns an allocated Polygon pointer initialized so that numVertex is 0 and
 * vertex is NULL.
 */
Polygon *polygon_create() {
    // Allocate polygon pointer
    Polygon *toReturn = (Polygon *) malloc(sizeof(Polygon));
    if (!toReturn) {
        printf("Malloc error in polygon_create(). Returning NULL.\n");
        return NULL;
    }

    // Initialize numVertex to 0, vertex to null.
    polygon_init(toReturn);

    // Return the pointer:
    return toReturn;
}

/**
 * Returns an allocated Polygon pointer with the vertex list initialized to a 
 * copy of the points in vlist.
 */
Polygon *polygon_createp(int numV, Point *vlist) {
    Polygon *toReturn = (Polygon *) malloc(sizeof(Polygon));
    if (!toReturn) {
        printf("polygon_createp(): Failed to allocate polygon memory\n");
        return NULL;
    }

    toReturn->nVertex = numV;
    toReturn->oneSided = 1;
    toReturn->zBuffer = 1;
    toReturn->vertex = (Point *) malloc(sizeof(Point) * toReturn->nVertex);
    toReturn->color = (Color *) malloc(sizeof (Color) * toReturn->nVertex);
    toReturn->normal = (Vector *) malloc(sizeof(Vector) * toReturn->nVertex);
    for (int i = 0; i < toReturn->nVertex; i++) {
        toReturn->vertex[i].val[0] = vlist[i].val[0];
        toReturn->vertex[i].val[1] = vlist[i].val[1];
        toReturn->vertex[i].val[2] = vlist[i].val[2];
        toReturn->vertex[i].val[3] = vlist[i].val[3];
        color_set(&toReturn->color[i], 1.0, 1.0, 1.0);
        toReturn->normal[i].val[0] = 0.0;
        toReturn->normal[i].val[1] = 0.0;
        toReturn->normal[i].val[2] = 1.0;
    }

    return(toReturn);
}

/**
 * Frees the internal data for a Polygon and the Polygon pointer.
 */
void polygon_free(Polygon *p) {
    if (p->vertex) {
        free(p->vertex);
    }

    if (p->color) {
        free(p->color);
    }

    if (p->normal) {
        free(p->normal);
    }
    free(p);
}


/* The functions polygon init, polygon set, and polygon clear work on a 
pre-existing Polygon data structure and manage only the memory required for the
vertex list */

/**
 * Initializes the existing Polygon to an empty Polygon.
 */
void polygon_init(Polygon *p) {
    p->color = NULL;
    p->normal = NULL;
    p->vertex = NULL;
    p->nVertex = 0;
    p->zBuffer = 1;
    p->oneSided = 1;
}

/**
 * Initializes the vertex array to the points in vlist.
 */
void polygon_set(Polygon *p, int numV, Point *vlist) {
    int i;

    // Free any existing data in the polygon:
    polygon_clear(p);

    // Check number of input vectors
    if (numV < 0) {
        printf("Error in polygon_set(): cannot create a polygon with "\
        "negative vertices. Returning.\n");
        return;
    }

    // Set nVertex to the numV value and malloc the internal space for vlist:
    p->nVertex = numV;

    p->vertex = (Point *) malloc(sizeof(Point) * p->nVertex);
    if (!p->vertex) {
        printf("Error allocating internal vlist in polygon_set. "\
        "Returning.\n");
        free(p);
        return;
    } 

    // Malloc memory for point colors:
    p->color = (Color *) malloc(sizeof(Color) * numV);
    if (!p->color) {
        printf("Error allocating internal colors in polygon_set. "\
        "Returning.\n");
        free(p);
        return;
    }

    // Malloc memory for normal vectors
    p->normal = (Vector *) malloc(sizeof(Vector) * numV);
    if (!p->normal) {
        printf("Error allocating internal normals in polygon_set. "\
        "Returning.\n");
        free(p);
        return;
    }

    for (i = 0; i < numV; i++) {
        // Copy over the Points in vlist
        // p->vertex[i] = vlist[i];
        p->vertex[i].val[0] = vlist[i].val[0];
        p->vertex[i].val[1] = vlist[i].val[1];
        p->vertex[i].val[2] = vlist[i].val[2];
        p->vertex[i].val[3] = vlist[i].val[3];        
        
        // Set the color of every point to white:
        p->color[i].c[0] = 1.0;
        p->color[i].c[1] = 1.0;
        p->color[i].c[2] = 1.0;

        // Set the normal vector to the default (z-unit vector)
        p->normal[i].val[0] = 0.0;
        p->normal[i].val[1] = 0.0;
        p->normal[i].val[2] = 1.0;
    }
}

/**
 * Frees the internal data and resets the fields.
 */
void polygon_clear(Polygon *p) {
    if (p->normal) {
        free(p->normal);
        p->normal = NULL;
    }

    if (p->vertex) {
        free(p->vertex);
        p->vertex = NULL;
    }

    if (p->color) {
        free(p->color);
        p->color = NULL;
    }

    p->nVertex = 0;
    p->oneSided = 1;
    p->zBuffer = 1;
}

/**
 * Sets the oneSided field to the value. Argument <oneSided> can be either 0
 * (two-sided) or 1 (one-sided). If it is any value other than 0, it defaults
 * to setting the drawing style to one-sided (1).
 */
void polygon_setSided(Polygon *p, int oneSided) {
    if (oneSided == 0) {
        p->oneSided = 0;
        return;
    }

    // If the argument isn't 0, default to one-sided (1)
    p->oneSided = 1;
}

/**
 *  De-allocates/allocates space and copies the vertex and color data from one 
 *  polygon to the other.
 */
void polygon_copy(Polygon *to, Polygon *from) {
    polygon_init(to);

    // Copy over primitive fields:
    to->nVertex = from->nVertex;
    to->oneSided = from->oneSided;
    to->zBuffer = from->zBuffer;

    // Malloc the internal space for vlist:
    to->vertex = (Point *) malloc(sizeof(Point) * to->nVertex);
    if (!to->vertex) {
        printf("polygon_copy(): Error allocating internal vlist. "\
        "Returning.\n");
        free(to);
        return;
    }

    // Malloc memory for point colors:
    to->color = (Color *) malloc(sizeof(Color) * to->nVertex);
    if (!to->color) {
        printf("polygon_copy(): Error allocating internal color. "\
        "Returning.\n");
        free(to);
        return;
    }

    // Malloc memory for normal vectors
    to->normal = (Vector *) malloc(sizeof(Vector) * to->nVertex);
    if (!to->normal) {
        printf("polygon_copy(): Error allocating internal normals. "\
        "Returning.\n");
        free(to);
        return;
    }

    for (int i = 0; i < to->nVertex; i++) {
        // Copy over the Points in vlist
        to->vertex[i].val[0] = from->vertex[i].val[0];
        to->vertex[i].val[1] = from->vertex[i].val[1];
        to->vertex[i].val[2] = from->vertex[i].val[2];
        to->vertex[i].val[3] = from->vertex[i].val[3];        
        
        // Set the color of every point to white:
        to->color[i].c[0] = from->color[i].c[0];
        to->color[i].c[1] = from->color[i].c[1];
        to->color[i].c[2] = from->color[i].c[2];

        // Set the normal vector to the default (z-unit vector)
        to->normal[i].val[0] = from->normal[i].val[0];
        to->normal[i].val[1] = from->normal[i].val[1];
        to->normal[i].val[2] = from->normal[i].val[2];
    }
    
}

/**
 * Prints polygon data to the stream designated by the FILE pointer.
 */
void polygon_print(Polygon *p, FILE *fp) {
    fprintf(fp, "Polygon is one-sided and comprised of %d vertices. Z-buffer"\
    " is set to %d\nPolygon is comprised of points:\n", p->nVertex, p->zBuffer);

    for (int i = 0; i < p->nVertex; i++) {
        fprintf(fp, "(%.2f,%.2f,%.2f), color = (%.2f,%.2f,%.2f), normal = (%.2f,%.2f,%.2f)\n",
        p->vertex[i].val[0], p->vertex[i].val[1], p->vertex[i].val[2],
        p->color[i].c[0], p->color[i].c[1], p->color[i].c[2],
        p->normal[i].val[0], p->normal[i].val[1], p->normal[i].val[2]);
    }
}

/**
 * Normalize the x and y values of each vertex by the homogeneous coord.
 */
void polygon_normalize(Polygon *p) {
    for (int i = 0; i < p->nVertex; i++) {
        point_normalize(&p->vertex[i]);
    }
}

/**
 * Draw the outline of the polygon using color c
 */
void polygon_draw(Polygon *p, Image *src, Color c) {
    Line l;

    // Case where there are no vertices
    if (p->nVertex == 0) {
        return;
    }

    // Case where there is one vertex:
    if (p->nVertex == 1) {
        point_draw(&p->vertex[0], src, c);
        return;
    }

    // Case where there is more than one vertex: draw the lines between pairs of
    // vertices:
    for (int i = 1; i < p->nVertex; i++) {
        line_set(&l, p->vertex[i-1], p->vertex[i]);
        line_draw(&l, src, c);
    }
    line_set(&l, p->vertex[p->nVertex - 1], p->vertex[0]);
    line_draw(&l, src, c);
}

/**
 * Draw the filled polygon using color c with the Barycentric coordinates 
 * algorithm. This only works for triangles (i.e. polygons with 3 vertices). If
 * the passed polygon is *not* a triangle, the function does nothing.
 */
void polygon_drawFillB(Polygon *p, Image *src, Color col) {
    // Ensure that the polygon has three and only three vertices:
    if (p->nVertex != 3) {
        return;
    }
    Point a = p->vertex[0];
    Point b = p->vertex[1];
    Point c = p->vertex[2];

    // Find the bounding box for our polygon so we don't test
    // pixels outside the bounding box.
    int min_x = floor(fmin(fmin(a.val[0], b.val[0]), c.val[0]));
    int max_x = floor(fmax(fmax(a.val[0], b.val[0]), c.val[0]));
    int min_y = floor(fmin(fmin(a.val[1], b.val[1]), c.val[1]));
    int max_y = floor(fmax(fmax(a.val[1], b.val[1]), c.val[1]));
    
    float beta = 0.0;
    float gamma = 0.0;
    float alpha = 0.0;

    for (int y = min_y; y < max_y; y++) {
        for (int x = min_x; x < max_x; x++) {
            // Compute barycentric coordinates of x, y
            // We could do this functionally but I believe it would negatively
            // impact draw time, as we'd have to define f_ca() and f_ab() every
            // time we called the fill
            beta = ((c.val[1] - a.val[1]) * x + 
                    (a.val[0] - c.val[0]) * y + 
                     c.val[0] * a.val[1] - 
                     a.val[0] * c.val[1]) / // Above these lines is f_ca(x, y)
                    ((c.val[1] - a.val[1]) * b.val[0] + 
                     (a.val[0] - c.val[0]) * b.val[1] + 
                      c.val[0] * a.val[1] - 
                      a.val[0] * c.val[1]); // This denominator is f_ca(x_b, yb)

            gamma = ((a.val[1] - b.val[1]) * x +
                     (b.val[0] - a.val[0]) * y +
                      a.val[0] * b.val[1] -
                      b.val[0] * a.val[1]) / // above these lines is f_ab(x,y)
                    ((a.val[1] - b.val[1]) * c.val[0] +
                     (b.val[0] - a.val[0]) * c.val[1] +
                      a.val[0] * b.val[1] -
                      b.val[0] * a.val[1]); // This denominator is f_ab(x_g,y_g)
            
            // Barycentric coordinates add to 1, so:
            alpha = 1.0 - beta - gamma;

            // If all coordinates are in range [0,1], we're in the triangle
            if (alpha > -0.00001 && beta > -0.00001 && gamma > -0.00001 &&\
                alpha <= 1.0 && beta <= 1.0 && gamma <= 1.0) {
                    image_setColor(src, y, x, col);
            }

            // Otherwise, we're outside the triangle so we proceed to the next
            // point in the bounding box
        }
    }
}


/********************
Scanline Fill Algorithm
********************/

// define the struct here, because it is local to only this file
typedef struct tEdge {
	float x0, y0, z0;            /* start point for the edge */
	float x1, y1, z1;            /* end point for the edge */
	int yStart, yEnd;            /* start row and end row */
    float xIntersect, dxPerScan; /* where the edge intersects the current scanline and how it changes in x */
	float zIntersect, dzPerScan; /* Where the edge intersects the current scanline and how it changes in z */
    struct tEdge *next;
} Edge;


/*
	This is a comparison function that returns a value < 0 if a < b, a
	value > 0 if a > b, and 0 if a = b.  It uses the yStart field of the
	Edge structure.  It is used to sort the overall edge list.
 */
static int compYStart( const void *a, const void *b ) {
	Edge *ea = (Edge *)a;
	Edge *eb = (Edge *)b;

	return(ea->yStart - eb->yStart);
}


/*
	This is a comparison function that returns a value < 0 if a < b, a
	value > 0 if a > b, and 0 if a = b.  It uses the xIntersect field of the
	Edge structure.  It is used to sort the active edge list.
 */
static int compXIntersect( const void *a, const void *b ) {
	Edge *ea = (Edge *)a;
	Edge *eb = (Edge *)b;

	if( ea->xIntersect < eb->xIntersect )
		return(-1);
	else if(ea->xIntersect > eb->xIntersect )
		return(1);

	return(0);
}

/*
	Allocates, creates, fills out, and returns an Edge structure given
	the inputs.

	Current inputs are just the start and end location in image space.
	Eventually, the points will be 3D and we'll add color and texture
	coordinates.
 */
static Edge *makeEdgeRec( Point start, Point end, Image *src) {
	// float dscan = end.val[1] - start.val[1];
	Point temp;

	/******
				 Your code starts here
	******/
	// Check that start/end are properly ordered (start should have lower y):
	if (start.val[1] > end.val[1]) {
		temp = start;
		start = end;
		end = temp;
	}

	// Check if the starting row is below the image or the end row is
	// above the image and skip the edge if either is true
	if (start.val[1] > src->rows || end.val[1] < 0) {
		return NULL;
	}

	// allocate an edge structure and set the x0, y0, x1, y1 values
	Edge *edge = (Edge *) malloc(sizeof(Edge));
	if (!edge) {
		printf("makeEdgeRec: failed to allocate edge memory. Returning NULL\n");
		return NULL;
	}

	edge->x0 = start.val[0];
	edge->y0 = start.val[1];
    edge->z0 = start.val[2];
	edge->x1 = end.val[0];
	edge->y1 = end.val[1];
    edge->z1 = end.val[2];

	// turn on an edge only if the edge starts in the top half of it or
	// the lower half of the pixel above it.  In other words, round the
	// start y value to the nearest integer and assign it to
	// edge->yStart.
	edge->yStart = round(edge->y0);

	// Turn off the edge if it starts in the lower half or the top half
	// of the next pixel.  In other words, round the end y value to the
	// nearest integer and subtract 1, assigning the result to
	// edge->yEnd.
	edge->yEnd = round(edge->y1) - 1;

	// Clip yEnd to the number of rows-1.
	edge->yEnd = fmin(edge->yEnd, (src->rows) - 1);

	// Calculate the slope, dxPerScan
	edge->dxPerScan = (end.val[0] - start.val[0]) / (edge->y1 - edge->y0);
    edge->dzPerScan = ((1/end.val[2]) - (1/start.val[2])) / (edge->y1 - edge->y0);

	// Calculate xIntersect, adjusting for the fraction of the point in the pixel.
	// Scanlines go through the middle of pixels
	edge->xIntersect = edge->x0 + 
					   edge->dxPerScan * ((edge->yStart + 0.5) - edge->y0);
    edge->zIntersect = (1 / edge->z0) +
                       edge->dzPerScan * ((edge->yStart + 0.5) - edge->y0);

	// adjust if the edge starts above the image
	// move the intersections down to scanline zero
	// if edge->y0 < 0
	//   update xIntersect
	//   update y0
  	//   update x0
	//   update yStart
	if (edge->y0 < 0) {
		/* Since y0 is negative, -y0 is the difference (or "rise", "dy") between
		y0 and 0. The 0th scanline crosses at y = 0.5 because the pixel grid is
		aligned to the tops/bottoms of pixels, so we adjust accordingly */
		edge->xIntersect = edge->x0 + (edge->dxPerScan) * (-(edge->y0) + 0.5);
        edge->zIntersect = 1 / edge->z0 + (edge->dzPerScan) * (-(edge->y0) + 0.5);
		edge->x0 = edge->x0 + (edge->dxPerScan) * (-(edge->y0));
        edge->z0 = edge->z0 + (edge->dzPerScan) * (-(edge->y0));
		edge->y0 = 0; // y0 is 0
		edge->yStart = 0; // Starting scanline is 0 - does not line up with y0!
    }

	// check for really bad cases with steep slopes where xIntersect has gone 
	// beyond the end of the edge
    if (edge->x0 < edge->x1) {
        if (edge->xIntersect > edge->x1) {
            edge->xIntersect = edge->x1;
	    }
    } else {
        if (edge->xIntersect < edge->x1) {
            edge->xIntersect = edge->x1;
        }
    }

	// return the newly created edge data structure
	return(edge);
}


/*
	Returns a list of all the edges in the polygon in sorted order by
	smallest row.
*/
static LinkedList *setupEdgeList( Polygon *p, Image *src) {
	LinkedList *edges = NULL;
	Point v1, v2;
	int i;

	// create a linked list
	edges = ll_new();

	// walk around the polygon, starting with the last point
	v1 = p->vertex[p->nVertex-1];

	for(i=0;i<p->nVertex;i++) {
		
		// the current point (i) is the end of the segment
		v2 = p->vertex[i];

		// if it is not a horizontal line
		if( (int)(v1.val[1]+0.5) != (int)(v2.val[1]+0.5) ) {
			Edge *edge;
			// if the first coordinate is smaller (top edge)
			if( v1.val[1] < v2.val[1] ){
				edge = makeEdgeRec( v1, v2, src );
            } else {
				edge = makeEdgeRec( v2, v1, src );
            }
			// insert the edge into the list of edges if it's not null
			if( edge ) {
				ll_insert( edges, edge, compYStart );
            }
		}
		v1 = v2;
	}

	// check for empty edges (like nothing in the viewport)
	if( ll_empty( edges ) ) {
		ll_delete( edges, NULL );
		edges = NULL;
	}
	return(edges);
}

/*
	Draw one scanline of a polygon given the scanline, the active edges,
	a DrawState, the image, and some Lights (for Phong shading only).
 */
static void fillScan(int scan, LinkedList *active, Image *src, Color c, DrawState* ds) {
  Edge *p1, *p2;
  int i, f;

  // loop over the list
  p1 = ll_head( active );
  while(p1) {
	  // the edges have to come in pairs, draw from one to the next
	  p2 = ll_next( active );
	  if( !p2 ) {
		  printf("bad bad bad (your edges are not coming in pairs)\n"); // lol
		  break;
	  }

	  // if the xIntersect values are the same, don't draw anything.
	  // Just go to the next pair.
	  if( p2->xIntersect == p1->xIntersect ) {
		  p1 = ll_next( active );
		  continue;
	  }

		/**** Your code goes here ****/
      float curZ = p1->zIntersect;
      float dzPerColumn = (p2->zIntersect - p1->zIntersect) / (p2->xIntersect - p1->xIntersect);

	  // identify the starting column
	  i = floor(p1->xIntersect);

	  // clip to the left side of the image
	  if (i < 0) {
          curZ = curZ - i * dzPerColumn; // adjust curZ to 0th column val
		  i = 0;
	  }

	  // identify the ending column
	  f = floor(p2->xIntersect);
	  // clip to the right side of the image
	  if (f >= src->cols) {
		  f = (src->cols);
	  }

	  // loop from start to end and color in the pixels
	  while (i < f) {
          if (curZ > image_getz(src, scan, i) && (curZ - image_getz(src, scan, i) >= 0.03)) {
              switch (ds->shade) {
                  case ShadeConstant:
                    image_setColor(src, scan, i, c);
                    break;

                  case ShadeDepth:;
                    Color newColor;
                    color_set(&newColor, 1.4*c.c[0] - 1/curZ, 1.4*c.c[1] - 1/curZ, 1.4*c.c[2] - 1/curZ);
                    // printf("curZ = %f at (%d, %d)\n", curZ, scan, i);

                    image_setColor(src, scan, i, newColor);
                    break;
                  default:
                    printf("Unhandled shading case!\n");
                    break;
              }
            // image_setColor(src, scan, i, c);
            image_setz(src, scan, i, curZ);
          }
		  i++;
          curZ = curZ + dzPerColumn;
	  }

	  // move ahead to the next pair of edges
	  p1 = ll_next( active );
  }

	return;
}

/* 
	 Process the edge list, assumes the edges list has at least one entry
*/
static int processEdgeList( LinkedList *edges, Image *src, Color c, DrawState* ds) {
	LinkedList *active = NULL;
	LinkedList *tmplist = NULL;
	LinkedList *transfer = NULL;
	Edge *current;
	Edge *tedge;
	int scan = 0;

	active = ll_new( );
	tmplist = ll_new( );

	// get a pointer to the first item on the list and reset the current pointer
	current = ll_head( edges );

	// start at the first scanline and go until the active list is empty
	for (scan = current->yStart; scan < src->rows; scan++) {

		// grab all edges starting on this row
		while( current != NULL && current->yStart == scan ) {
			ll_insert(active, current, compXIntersect);
			current = ll_next(edges);
		}
		// current is either null, or the first edge to be handled on some future scanline

		if( ll_empty(active) ) {
			break;
		}

		// if there are active edges
		// fill out the scanline
		fillScan( scan, active, src, c, ds);

		// remove any ending edges and update the rest
		for(tedge = ll_pop(active); tedge != NULL; tedge = ll_pop(active)) {

			// keep anything that's not ending
			if( tedge->yEnd > scan ) {
				float a = 1.0;

				// update the edge information with the dPerScan values
				tedge->xIntersect += tedge->dxPerScan;
                tedge->zIntersect += tedge->dzPerScan;

				// adjust in the case of partial overlap
				if( tedge->dxPerScan < 0.0 && tedge->xIntersect < tedge->x1 ) {
					a = (tedge->xIntersect - tedge->x1) / tedge->dxPerScan;
					tedge->xIntersect = tedge->x1;
				}
				else if( tedge->dxPerScan > 0.0 && tedge->xIntersect > tedge->x1 ) {
					a = (tedge->xIntersect - tedge->x1) / tedge->dxPerScan;
					tedge->xIntersect = tedge->x1;
				}

				ll_insert( tmplist, tedge, compXIntersect );
			}
		}

		transfer = active;
		active = tmplist;
		tmplist = transfer;
	}

	// get rid of the lists, but not the edge records
	ll_delete(active, NULL);
	ll_delete(tmplist, NULL);

	return(0);
}

/**
 * Draw the filled polygon using color c with the scanline z-buffer rendering 
 * algorithm.
 */
void polygon_drawFill(Polygon *p, Image *src, Color c, DrawState* ds) {
	LinkedList *edges = NULL;

    if (ds->shade == ShadeFrame) {
        polygon_draw(p, src, c);
        return;
    }

	// set up the edge list
	edges = setupEdgeList(p, src);
	if(!edges)
		return;
	
	// process the edge list (should be able to take an arbitrary edge list)
	processEdgeList(edges, src, c, ds);

	// clean up
	ll_delete( edges, (void (*)(const void *))free );
	return;
}

/****************************************
End Scanline Fill
*****************************************/

/**
 * Draws the polygon into a src image, applying 4x4 supersampling to anti-alias
 * the polygon. Note that to avoid destroying the info in the src image, the src
 * image pixels are scaled to the superimage prior to drawing in the polygon.
 * When rendering many polygons, as in a video game or animation, this is 
 * *extremely* inefficient, as the src image is scaled up and down every time
 * a new polygon is rendered. A more effective approach for scenes involving
 * many polygons is to keep track of all the polygons that need to be drawn into
 * a scene in a list, then draw all of the polygons onto a blank supersized
 * canvas, and only then scale down to the final image.
 */
void polygon_drawFill_SuperSampled(Polygon *p, Image *src, Color c, DrawState* ds) {
    // Create superimage with 4x4 superpixels
    Image *superimage = image_create(src->rows * 4, src->cols * 4);

    Color aliased, superColor, ogColor;

    // Define floats to keep track of our averaged color channels:
    float super_r = 0.0;
    float super_g = 0.0;
    float super_b = 0.0;

    int i, j, super_i, super_j;
    Polygon *largePolygon = polygon_create();
    polygon_init(largePolygon);
    polygon_copy(largePolygon, p);

    // Scale up the input image into the supersized image:
    for (i = 0; i < src->rows; i++) {
        for (j = 0; j < src->cols; j++) {
            ogColor = image_getColor(src, i, j);
            for (super_i = i * 4; super_i < (i*4) + 4; super_i++) {
                for (super_j = j * 4; super_j < (j * 4) + 4; super_j++) {
                    image_setColor(superimage, super_i, super_j, ogColor);
                }
            }
        }
    }

    // Scale the large polygon
    for (i = 0; i < largePolygon->nVertex; i++) {
        largePolygon->vertex[i].val[0] = largePolygon->vertex[i].val[0] * 4;
        largePolygon->vertex[i].val[1] = largePolygon->vertex[i].val[1] * 4;        
    }

    // Draw the polygon onto a big image (4x4 superpixels)
    polygon_drawFill(largePolygon, superimage, c, ds);

    // Iterate over all the pixels in the original image
    for (i = 0; i < src->rows; i++) {
        for (j = 0; j < src->cols; j++) {
            ogColor = image_getColor(src, i, j);
            // Iterate over the 4x4 superpixel and compute value of normal pixel
            for (super_i = i * 4; super_i < (i * 4) + 4; super_i++) {
                for (super_j = j * 4; super_j < (j * 4) + 4; super_j++) {                    
                    superColor = image_getColor(superimage, super_i, super_j);

                    if (super_i == i * 4 && super_j == j * 4) {
                        // Initialize color channels
                        super_r = superColor.c[0];
                        super_g = superColor.c[1];
                        super_b = superColor.c[2];
                    } else {
                        // Update color channels
                        super_r = super_r + superColor.c[0];
                        super_g = super_g + superColor.c[1];
                        super_b = super_b + superColor.c[2];
                    }
                }
            }

            color_set(&aliased, super_r / 16.0, super_g / 16.0, super_b / 16.0);

            image_setColor(src, i, j, aliased);
        }
    }
}