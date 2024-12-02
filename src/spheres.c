#include <stdlib.h>
#include <stdio.h>
#include <math.h>
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

float calcDiscriminant(float a, float b, float c){
    return b*b-4*a*c;
}
void solveQuadratic(float a, float b, float c, float disc, float *t1, float *t2){
    *t1 = (-b + sqrt(disc))/(2*a);
    *t2 = (-b - sqrt(disc))/(2*a);
}
float min(float t1, float t2){
    return (t1 > t2) ? t2 : t1;
}
int doesIntersect(const Sphere *sphere, Vec3 rayPos, Vec3 rayDir, float *t){
    float a, b, c, t1, t2;
    Vec3 V = subtract(rayPos, sphere->pos);
    a = dot(rayDir, rayDir);
    b = 2*dot(rayDir, V);
    c = dot(V, V) - sphere->r * sphere->r;

    float disc = calcDiscriminant(a, b, c);
    if (disc < 0){
        return 0;
    }else{
        solveQuadratic(a, b, c, disc, &t1, &t2);
        if (t1 < 0 && t2 < 0){
            return 0;
        }else if(t1 > 0 && t2 < 0){
            *t = t1;
        }else if(t1 < 0 && t2 > 0){
            *t = t2;
        }else{
            *t = min(t1, t2);
        }
        return 1;
    }

}

