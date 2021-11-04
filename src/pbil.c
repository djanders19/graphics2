#include <stdio.h>
#include <stdlib.h>
#include <math.h>
#include <time.h>
#include "polygon.h"


double imageDiff(Image *ref, Image *compare) {
    float score = 1.0;
    for (int i = 0; i < ref->rows; i++) {
        for (int j = 0; j < ref->cols; j++) {
            Color refColor = image_getColor(ref, i, j);
            Color compareColor = image_getColor(compare, i, j);
            score = ((abs(refColor.c[0] - compareColor.c[0])) +
                     (abs(refColor.c[1] - compareColor.c[1])) + 
                     (abs(refColor.c[2] - compareColor.c[2]))) + score;
        }
    }
    return score;
}

int main(int argc, char *argv[]) {
    if (argc < 6) {
        printf("Correct arguments are pbil <ref.ppm> <popMembers> <numPolygons> <mu> <numIters>\n");
        return -1;
    }
    srand((unsigned int)time(NULL));

    // Load the reference image in
    Image *ref = image_read(argv[1]);
    int popMembers = atoi(argv[2]);
    int numPolygons = atoi(argv[3]);
    float mu = atof(argv[4]);
    int numIters = atoi(argv[5]);


    // Create a parent:
    Polygon *parent = malloc(sizeof(Polygon) * numPolygons);
    for (int i = 0; i < numPolygons; i++) {
        parent[i] = *polygon_create();

        parent[i].vertex = malloc(sizeof(Point) * 3);
        parent[i].color = malloc(sizeof(Color));
        parent[i].nVertex = 3;

        // Randomize color
        parent[i].color->c[0] = ((float)rand())/RAND_MAX;
        parent[i].color->c[1] = ((float)rand())/RAND_MAX;
        parent[i].color->c[2] = ((float)rand())/RAND_MAX;
        printf("Parent color = (%f,%f,%f)\n", parent[i].color->c[0],  parent[i].color->c[1],  parent[i].color->c[2]);

        // Randomize vertices
        parent[i].vertex[0].val[0] = (rand() % ref->cols);
        parent[i].vertex[0].val[1] = (rand() % ref->rows);
        parent[i].vertex[1].val[0] = (rand() % ref->cols);
        parent[i].vertex[1].val[1] = (rand() % ref->rows);
        parent[i].vertex[2].val[0] = (rand() % ref->cols);
        parent[i].vertex[2].val[1] = (rand() % ref->rows);
    }
    printf("Are you my mother?\n");
    Image *test = image_create(ref->rows, ref->cols);
    // Assess fitness of Parent:
    for (int i = 0; i < numPolygons; i++) {
        polygon_drawFill(&(parent[i]), test, parent->color[i]);
    }

    double parentFitness = imageDiff(ref, test);
    printf("My fitness is %f\n", parentFitness);

    image_write(test, "Test_genetic_alg.ppm");

    for (int epoch = 0; epoch < numIters; epoch++) {
        printf("Beginning epoch %d\n", epoch);
    for (int member = 0; member < popMembers; member++) {
        Polygon *child = malloc(sizeof(Polygon) * numPolygons);
        for (int i = 0; i < numPolygons; i++) {
            child[i] = *polygon_create();
            child[i].vertex = malloc(sizeof(Point) * 3);
            child[i].color = malloc(sizeof(Color));
            child[i].nVertex = 3;

            // Randomize color
            child[i].color->c[0] = parent[i].color->c[0] + mu * (-1+2*((float)rand())/RAND_MAX);
            child[i].color->c[1] = parent[i].color->c[1] + mu * (-1+2*((float)rand())/RAND_MAX);
            child[i].color->c[2] = parent[i].color->c[2] + mu * (-1+2*((float)rand())/RAND_MAX);
            // printf("Parent color = (%f,%f,%f)\nChild color = (%f,%f,%f)\n",
                    // parent[i].color->c[0],  parent[i].color->c[1],
                    // parent[i].color->c[2], child[i].color->c[0],
                    // child[i].color->c[1], child[i].color->c[2]);

            // Randomize vertices
            child[i].vertex[0].val[0] = parent[i].vertex[0].val[0] + mu * (rand() % ref->cols);
            child[i].vertex[0].val[1] = parent[i].vertex[0].val[1] + mu * (rand() % ref->rows);
            child[i].vertex[1].val[0] = parent[i].vertex[1].val[0] + mu * (rand() % ref->cols);
            child[i].vertex[1].val[1] = parent[i].vertex[1].val[1] + mu * (rand() % ref->rows);
            child[i].vertex[2].val[0] = parent[i].vertex[2].val[0] + mu * (rand() % ref->cols);
            child[i].vertex[2].val[1] = parent[i].vertex[2].val[1] + mu * (rand() % ref->rows);
        }

        image_reset(test); // Reset Test image
        for (int i = 0; i < numPolygons; i++) {
            // Draw the child in:
            polygon_drawFill(&(parent[i]), test, child->color[i]);
        }
        double childFitness = imageDiff(ref, test);
        printf("Parent fitness: %f     Child fitness: %f\n", parentFitness, childFitness);
        if (childFitness < parentFitness && childFitness > 0) {
            // Set the parent to the child
            printf("Yahoo\n");
            for (int i = 0; i < numPolygons; i++) {
                parent[i].color->c[0] = child[i].color->c[0];
                parent[i].color->c[1] = child[i].color->c[1];
                parent[i].color->c[2] = child[i].color->c[2];

                parent[i].vertex[0].val[0] = child[i].vertex[0].val[0];
                parent[i].vertex[0].val[1] = child[i].vertex[0].val[1];
                parent[i].vertex[1].val[0] = child[i].vertex[1].val[0];
                parent[i].vertex[1].val[1] = child[i].vertex[1].val[1];
                parent[i].vertex[2].val[0] = child[i].vertex[2].val[0];
                parent[i].vertex[2].val[1] = child[i].vertex[2].val[1];
            }
            // Set the parentFitness to the childFitness
            parentFitness = childFitness;
        }

        // Delete the child:
        for (int i = 0; i < numPolygons; i++) {
            free(child[i].vertex);
            free(child[i].color);
        }
        free(child);
    }
    }

    image_free(test);
    image_free(ref);

    return 1;
}