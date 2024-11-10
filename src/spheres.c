#include "vector.h"
#include "spheres.h"

void worldInit(World *world){
    world->capacity=1;
    world->size=0;
    world->spheres = malloc(world->capacity*sizeof(Sphere*));
    if (world->spheres == NULL){
        fprintf(stderr, "ERROR: No available space\n");
        exit(EXIT_FAILURE); 
    }
}

void freeWorld(World *world){
    for(int i=0; i<world->size; i++){
        free(world->spheres[i]);
    }
    free(world->spheres);
}

void addSphere(World *world, Sphere *sphere){
    world->size += 1;
    if (world->size > world->capacity){
        world->capacity *= 2;
        world->spheres = realloc(world->spheres, world->capacity*sizeof(Sphere*));
        if (world->spheres == NULL){
            fprintf(stderr, "ERROR: No available space\n");
            exit(EXIT_FAILURE); 
        }
    }
    world->spheres[world->size-1] = sphere;
}
Sphere *createSphere(float radius, Vec3 position, Vec3 color){
    Sphere *newSphere = malloc(sizeof(Sphere));
    if (newSphere == NULL){
        fprintf(stderr, "ERROR: No available space\n");
        exit(EXIT_FAILURE); 
    }
    newSphere->r=radius;
    newSphere->pos = position;
    newSphere->color=color;
    return newSphere;
}