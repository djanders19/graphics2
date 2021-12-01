#include "graphicslib.h"

/* LIGHT FUNCTIONS */

/**
 * Initialize the light to default values.
 */
void light_init(Light *light) {
    if (!light) {
        printf("light_init(): passed NULL pointer\n");
        return;
    }

    // Initialize the light:
    light->type = LightNone; // default light isn't set up; so is LightNone
    color_set(&light->color, 1.0, 1.0, 1.0); // Default color is white.
    vector_set(&light->direction, 1.0, 1.0, 1.0); // default direction
    point_set3D(&light->position, 0.0, 0.0, 0.0); // default position is origin
    light->cutoff = 0.9238; // ~cos(22.5 deg), gives a 45 deg spotlight
    light->sharpness = 1.0;
}

/**
 * Copy the light information from <from> to <to>.
 */
void light_copy(Light *to, Light *from) {
    if (!to || !from) {
        printf("light_copy(): passed NULL pointer\n");
        return;
    }

    to->type = from->type;
    color_copy(&to->color, &from->color);
    vector_copy(&to->direction, &from->direction);
    point_copy(&to->position, &from->position);
    to->cutoff = from->cutoff;
    to->sharpness = from->sharpness;
}

/* LIGHTING FUNCTIONS */

/**
 * Allocate and return a new lighting structure set to default values.
 */
Lighting *lighting_create(void) {
    Lighting *lights = (Lighting *) malloc(sizeof(Lighting));
    if (!lights) {
        printf("lighting_create(): malloc failed. Returning.\n");
        return;
    }

    lighting_init(lights);
    return lights;
}

/**
 * Initialize the lighting structure to default values.
 */
void lighting_init(Lighting *l) {
    if (!l) {
        printf("lighting_init(): passed NULL pointer\n");
        return;
    }

    l->nLights = 0;
    for (int i = 0; i < MAX_LIGHTS; i++) {
        light_init(&l->light[i]); // Set all lights to default values
    }
}

/**
 * Add a new light to the Lighting struture given the parameters, some of which
 * may be NULL. Make sure not to add more lights than MAX_LIGHTS, defined as a
 * macro in the header file.
 */
void lighting_add(Lighting *l, LightType type, Color *c, Vector *dir,
                  Point *pos, float cutoff, float sharpness ) {
    if (!l) {
        printf("lighting_add(): passed NULL pointer as lighting arg.\n");
        return;
    }

    // We don't want to add more lights than MAX_LIGHTS.
    if (l->nLights >= MAX_LIGHTS) {
        printf("lighting_add(): Already reached MAX_LIGHTS limit - cannot\n"\
                "add more lights. Increase MAX_LIGHTS in lighting.h to\n"\
                "resolve this error.");
        return;
    }

    // Keep track of how many lights we have/where the light is that's added
    int i = l->nLights;
    l->nLights = l->nLights + 1;
    light_init(&l->light[i]); // Reset ith light

    l->light[i].type = type; // Set the LightType

    // Set the color if passed - otherwise defaults to white.
    if (c) {
        color_copy(&l->light[i].color, c);
    }

    // Set the direction if passed - otherwise defaults to (1, 1, 1).
    if (dir) {
        vector_copy(&l->light[i].direction, dir);
    }

    // Set the position if passed - otherwise defaults to origin
    if (pos) {
        point_copy(&l->light[i].position, pos);
    }

    // Set cutoff and sharpness:
    l->light[i].cutoff = cutoff;
    l->light[i].sharpness = sharpness;
}

/**
 * Calculate the proper color given the normal N, view vector V, 3d point P,
 * body color Cb, surface color Cs, sharpness value s, the lighting, and whether
 * the polygon is one-sided or two-sided. Put the result in color C. 
 */
void lighting_shading(Lighting *l, Vector *N, Vector *V, Point *p, Color *Cb, 
                      Color *Cs, float s, int oneSided, Color *c ) {
    if (!l || !N || !V || !p || !Cb || !Cs || !c) {
        printf("lighting_shading(): Passed NULL pointer as argument.\n");
        return;
    }

    // If point is one-sided and we can't see it's normal, we can just ignore it
    if (oneSided != 0 && vector_dot(V, N) <= 0) {
        return;                
    }

    Vector L = {{0.0, 0.0, 0.0}}; // Placeholder for L-vector
    Vector negative_L = {{0.0, 0.0, 0.0}}; // Used in spotlighting
    Vector H = {{0.0, 0.0, 0.0}}; // Placeholder for H-vector

    // evaluate lighting for every light at point p with passed params:
    for (int i = 0; i < l->nLights; i++) {
        Light light = l->light[i];
        switch (light.type) {
            case LightNone:
                break;
            
            case LightAmbient:
                c->c[0] = c->c[0] + Cb->c[0] * light.color.c[0]; // Set red
                c->c[1] = c->c[1] + Cb->c[1] * light.color.c[1]; // Set green
                c->c[2] = c->c[2] + Cb->c[2] * light.color.c[2]; // Set blue
                break;
            
            case LightDirect:
                // In the case of direct lighting, the light is positionless
                // (or more accurately, it's located at infinity along the
                // vector light.direction). So, we just copy the light direction
                // into L:
                vector_copy(&L, &light.direction);
                vector_set(&H, L.val[0] + V->val[0] / 2,  
                               L.val[1] + V->val[1] / 2,
                               L.val[2] + V->val[2] / 2);
                
                // Update color accumulator using light equation:
                c->c[0] =   c->c[0] + 
                          (Cb->c[0] * light.color.c[0] * vector_dot(&L, N) + 
                           Cs->c[0] * light.color.c[0] * vector_dot(&H, N));
                c->c[1] =   c->c[1] + 
                          (Cb->c[1] * light.color.c[1] * vector_dot(&L, N) + 
                           Cs->c[1] * light.color.c[1] * vector_dot(&H, N));
                c->c[2] =   c->c[2] + 
                          (Cb->c[2] * light.color.c[2] * vector_dot(&L, N) + 
                           Cs->c[2] * light.color.c[2] * vector_dot(&H, N));
                break;
            
            case LightPoint:
                // This only differs from directional light in that we need to
                // calculate the L vector based on the Point we are coloring and
                // the light source location.
                vector_set(&L, light.position.val[0] - p->val[0],
                               light.position.val[1] - p->val[1],
                               light.position.val[2] - p->val[2]);
                vector_set(&H, L.val[0] + V->val[0] / 2,  
                               L.val[1] + V->val[1] / 2,
                               L.val[2] + V->val[2] / 2);

                // Update color accumulator using light equation:
                c->c[0] =   c->c[0] + 
                          (Cb->c[0] * light.color.c[0] * vector_dot(&L, N) + 
                           Cs->c[0] * light.color.c[0] * vector_dot(&H, N));
                c->c[1] =   c->c[1] + 
                          (Cb->c[1] * light.color.c[1] * vector_dot(&L, N) + 
                           Cs->c[1] * light.color.c[1] * vector_dot(&H, N));
                c->c[2] =   c->c[2] + 
                          (Cb->c[2] * light.color.c[2] * vector_dot(&L, N) + 
                           Cs->c[2] * light.color.c[2] * vector_dot(&H, N));
                break;
            
            case LightSpot:
                // Calculate light vector:
                vector_set(&L, light.position.val[0] - p->val[0],
                               light.position.val[1] - p->val[1],
                               light.position.val[2] - p->val[2]);
                vector_set(&H, L.val[0] + V->val[0] / 2,  
                               L.val[1] + V->val[1] / 2,
                               L.val[2] + V->val[2] / 2);
                
                break;
            
            default:
                printf("Unrecognized lighting type.\n");
                break;

        }
    }
}