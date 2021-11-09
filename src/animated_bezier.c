/*
	David J Anderson
	Fall 2021
	Computer graphics

    write out a series of output images demonstrating bezier curves drawn with
    incrementally more subdivisions.
*/
#include <stdio.h>
#include "graphicslib.h"

int main(int argc, char *argv[]) {
	Color blue;
	Point p[4];
	BezierCurve bc;
	Image *src = image_create(300, 400);
    char filename[256];

	// set up four points
	point_set2D(&p[0], 50, 200);
	point_set2D(&p[1], 150, 40);
	point_set2D(&p[2], 250, 80);
	point_set2D(&p[3], 350, 250);

	color_set(&blue, .1, .2, .8);
	
    printf("Setting bezier curve\n");
	bezierCurve_set(&bc, p);

	// set and draw the curve
    for (int i = 0; i <= 10; i++) {
        printf("drawing bezier curve with %d subdivisions\n", i);
        bezierCurve_draw_with_subdivisions( &bc, i, 0, src, blue );
        
        // write the image
        sprintf(filename, "bezier_animated-%04d.ppm", i);
        image_write(src, filename);
        image_reset(src);
    }

	// Clean up
	image_free( src );

	return(0);
}
