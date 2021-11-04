#include <stdlib.h>
#include <stdio.h>
#include "polygon.h"


int main(int argc, char *argv[]) {
    Color White;
    Color Blue;
    Color Gray;
    Color lightGray;
	// Line line;
    Point p1;
    Point p2;
    Ellipse e;
    Circle circ;
    Point pt[500];
    color_set(&Blue, 0.0, 0.0, 1.0);
    color_set( &White, 1.0, 1.0, 1.0 );
    color_set( &Gray, 0.5, 0.5, 0.5);
    color_set(&lightGray, 0.75, 0.75, 0.75);
    Polygon *p;


    Image *src = image_create(500, 500);
    // line_set2D(&line, 50, 50, 30, 45);
    Line l;
    // Draw roof
    point_set2D(&(pt[0]), 200, 200);
    point_set2D(&(pt[1]), 100, 100); // left roof

    point_set2D(&(pt[2]), 100, 100);
    point_set2D(&(pt[3]), 189, 85); // top roof

    point_set2D(&(pt[4]), 189, 85); // right roof
    point_set2D(&(pt[5]), 299, 175);
    p = polygon_createp(6, &pt);
    polygon_drawFill(p, src, White);

    // Windscreen
    polygon_clear(p);
    point_set2D(&(pt[0]), 200, 200);
    point_set2D(&(pt[1]), 235, 280); 

    point_set2D(&(pt[2]), 235, 280);
    point_set2D(&(pt[3]), 334, 255);

    point_set2D(&(pt[4]), 334, 255);
    point_set2D(&(pt[5]), 299, 175);

    polygon_set(p, 6, &pt);
    polygon_drawFill(p, src, Blue);

    // Front wheel
    point_set2D(&p1, 207, 291);
    ellipse_set(&e, p1, 20, 28);
    ellipse_draw(&e, src, White);
    ellipse_set(&e, p1, 17, 25);
    ellipse_drawFill(&e, src, White);
    ellipse_set(&e, p1, 25, 33);
    ellipse_draw(&e, src, White);

    // rear wheel
    point_set2D(&p1, 89, 175);
    ellipse_set(&e, p1, 20, 28);
    ellipse_draw(&e, src, White);
    ellipse_set(&e, p1, 17, 25);
    ellipse_drawFill(&e, src, White);
    ellipse_set(&e, p1, 25, 33);
    ellipse_draw(&e, src, White);

    // Draw the carriage:
    polygon_clear(p);
    point_set2D(&pt[0],60, 102);
    point_set2D(&pt[1],43, 140);
    point_set2D(&pt[2],70,159);
    point_set2D(&pt[3],104,200);
    point_set2D(&pt[4],175,274);
    point_set2D(&pt[5],203,262);
    point_set2D(&pt[6],228,290);
    point_set2D(&pt[7],235,320);
    point_set2D(&pt[8],300,389);
    point_set2D(&pt[9], 300, 359);
    point_set2D(&pt[10],235,280);

    polygon_set(p, 11, &pt);
    polygon_drawFill(p, src, Gray);

    // Draw the window framing
    point_set2D(&pt[0],60, 102);
    point_set2D(&pt[1],100, 100);
    point_set2D(&pt[2],199, 199);
    point_set2D(&pt[3],234, 279);
    polygon_set(p, 4, &pt);
    polygon_drawFill(p, src, lightGray);

    // Front passenger window
    polygon_clear(p);
    point_set2D(&(pt[0]), 173, 181);
    point_set2D(&(pt[1]), 196, 203); 

    point_set2D(&(pt[2]), 196, 203);
    point_set2D(&(pt[3]), 220, 257);

    point_set2D(&(pt[4]), 220, 257);
    point_set2D(&(pt[5]), 218, 259);

    point_set2D(&(pt[6]), 218, 259);
    point_set2D(&(pt[7]), 161, 198);

    polygon_set(p, 8, &pt);
    polygon_drawFill(p, src, Blue);

    // Rear passenger window
    polygon_clear(p);
    point_set2D(&(pt[0]), 159, 167);
    point_set2D(&(pt[1]), 150, 189); 

    point_set2D(&(pt[2]), 150, 189);
    point_set2D(&(pt[3]), 96, 135);

    point_set2D(&(pt[4]), 96, 135);
    point_set2D(&(pt[5]), 112, 119);

    polygon_set(p, 6, &pt);
    polygon_drawFill(p, src, Blue);

    // Rear side window
    polygon_clear(p);
    point_set2D(&(pt[0]), 69, 105);
    point_set2D(&(pt[1]), 97, 105);

    point_set2D(&(pt[2]), 97, 105);
    point_set2D(&(pt[3]), 107, 116);

    point_set2D(&(pt[4]), 107, 116);
    point_set2D(&(pt[5]), 94, 130);

    polygon_set(p, 6, &pt);
    polygon_drawFill(p, src, Blue);



    // Draw hood
    point_set2D(&pt[0],234, 280);
    point_set2D(&pt[1],334,255);
    point_set2D(&pt[2],399, 335);
    point_set2D(&pt[3],300,360);
    polygon_set(p, 4, &pt);
    polygon_drawFill(p, src, lightGray);

    // Draw hood v
    point_set2D(&pt[0],234, 280);
    point_set2D(&pt[1],334,255);
    point_set2D(&pt[2],340,340);
    polygon_set(p, 3, &pt);
    polygon_drawFill(p, src, White);


 
    line_set2D(&l, 399, 365, 399, 335); // right grille
    line_draw(&l, src, White);

    line_set2D(&l, 300, 360, 300, 390); // left grille
    line_draw(&l, src, White);

    line_set2D(&l, 399, 365, 300, 390); // bottom grille
    line_draw(&l, src, White);


    image_write(src, "testprims.ppm");
	image_free(src);

    return 0;

}