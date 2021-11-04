#include "graphics.h"
#include <stdlib.h>
int main(int argc, char *argv[]) {
    Color White;
	// Line line;
    Point p1;
    Point p2;
    Ellipse e;
    Circle circ;
    color_set( &White, 1.0, 1.0, 1.0 );

    Image *src = image_create(500, 500);
    // line_set2D(&line, 50, 50, 30, 45);
    Line l;
    // Draw roof
    line_set2D(&l, 200, 200, 100, 100); // left roof
    line_draw(&l, src, White);

    line_set2D(&l, 200, 200, 235, 280); // left windscreen
    line_draw(&l, src, White);

    line_set2D(&l, 235, 280, 334, 255); // bottom windscreen
    line_draw(&l, src, White);

    line_set2D(&l, 200, 200, 299, 175); // top windscreen
    line_draw(&l, src, White);

    line_set2D(&l, 334, 255, 299, 175); // right windscreen
    line_draw(&l, src, White);

    line_set2D(&l, 100, 100, 60, 102); // back left windscreen
    line_draw(&l, src, White);

    line_set2D(&l, 235, 280, 60, 102); // bottom of passenger windows
    line_draw(&l, src, White);

    line_set2D(&l, 100, 100, 189, 85); // top roof
    line_draw(&l, src, White);

    line_set2D(&l, 299, 175, 189, 85); // right roof
    line_draw(&l, src, White);

    // Draw hood
    line_set2D(&l, 300, 360, 235, 280); // left side of hood
    line_draw(&l, src, White);

    line_set2D(&l, 340, 340, 235, 280); // hood v left
    line_draw(&l, src, White);

    line_set2D(&l, 334, 255, 340, 340); // right hood
    line_draw(&l, src, White);

    line_set2D(&l, 334, 255, 399, 335); // right hood
    line_draw(&l, src, White);

    line_set2D(&l, 300, 360, 399, 335); // right hood
    line_draw(&l, src, White);

    line_set2D(&l, 399, 365, 399, 335); // right grille
    line_draw(&l, src, White);

    line_set2D(&l, 300, 360, 300, 390); // left grille
    line_draw(&l, src, White);

    line_set2D(&l, 399, 365, 300, 390); // bottom grille
    line_draw(&l, src, White);

    line_set2D(&l, 235, 320, 300, 390); // left bottom hood
    line_draw(&l, src, White);

    // Front wheel
    point_set2D(&p1, 207, 291);
    ellipse_set(&e, p1, 20, 28);
    ellipse_draw(&e, src, White);
    ellipse_set(&e, p1, 17, 25);
    ellipse_draw(&e, src, White);

    line_set2D(&l, 235, 320, 228, 290); // wheel well
    line_draw(&l, src, White);

    line_set2D(&l, 203, 262, 175, 274); // wheel well rear
    line_draw(&l, src, White);

    // rear wheel
    point_set2D(&p1, 89, 175);
    ellipse_set(&e, p1, 20, 28);
    ellipse_draw(&e, src, White);
    ellipse_set(&e, p1, 17, 25);
    ellipse_draw(&e, src, White);

    line_set2D(&l, 104, 200, 175, 274); // wheel well rear
    line_draw(&l, src, White);

    line_set2D(&l, 43, 140, 60, 102); // back left windscreen
    line_draw(&l, src, White);

    line_set2D(&l, 43, 140, 70, 159); // back left windscreen
    line_draw(&l, src, White);


    image_write(src, "testprims.ppm");
	image_free(src);

    return 0;

}