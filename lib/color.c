#include <stdio.h>
#include <stdlib.h>
#include "graphicslib.h"

#define USECPP 0

/**
 * Copy the Color data from one Color, *from, to another Color *to.
 */
void color_copy(Color *to, Color *from) {
    for (int i = 0; i<3; i++) {
        to->c[i] = from->c[i];
    }
    return;
}


/**
 * Set Color data to r, g, b values in range [0.0, 1.0]
 */ 
void color_set(Color *to, float r, float g, float b) {
    // Check that inputs are valid:
    if (r > 1.0) {
        r = 1.0;
    }

    if (g > 1.0) {
        g = 1.0;
    }

    if (b > 1.0) {
        b = 1.0;
    }

    if (r < 0.0) {
        r = 0.0;
    }

    if (g < 0.0) {
        g = 0.0;
    }

    if (b < 0.0) {
        b = 0.0;
    }

    to->c[0] = r;
    to->c[1] = g;
    to->c[2] = b;
    return;
}