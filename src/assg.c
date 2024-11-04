#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "vector.h"
#include "spheres.h"
#define HEX_STR_SIZE 8

//initialize all vars as global variables
int imw, imh, num_colors, m, bg_ind, n;
float vh, vw, focal, brightness;
Vec3 light;
World world;
char** colors;

void firstMS(FILE* out);

int main(int argc, char *argv[]){
    FILE* inp = fopen(argv[1], "r");
    FILE* out = fopen(argv[2], "w");
    if (inp == NULL || out == NULL){
        fprintf(stderr, "ERROR: input.txt does not exist.\n");
        exit(EXIT_FAILURE);
    }
    fscanf(inp, "%d %d", &imw, &imh); // get img height and width
    fscanf(inp, "%f %f", &vh, &focal); // get viewport height and focal length
    vw = (((float) imw)/imh)*vh; // calc viewport width
    fscanf(inp, "%f %f %f %f", &light.x, &light.y, &light.z, &brightness); // get information about the light source

    fscanf(inp, "%d", &m); // get number of colors then allocate space for it
    colors = malloc(m*sizeof(char*));
    if (colors == NULL){
        fprintf(stderr, "ERROR: No available space\n");
        exit(EXIT_FAILURE);
    }
    // put all colors in colors array
    for (int i=0; i<m; i++){
        char hex[HEX_STR_SIZE+1];
        fscanf(inp, "%s", hex);

        colors[i] = malloc(HEX_STR_SIZE+1);
        if (colors[i] == NULL){
            fprintf(stderr, "ERROR: No available space\n");
            exit(EXIT_FAILURE);
        }

        strcpy(colors[i], hex);
    }

    fscanf(inp, "%d %d", &bg_ind, &n); // get bg index and num of spheres
    // initialize world
    worldInit(&world);
    for (int i=0; i<n; i++){
        int color_ind;
        float x, y, z, r;
        fscanf(inp, "%f %f %f %f %d", 
        &x, &y, &z, 
        &r, &color_ind);
        Vec3 pos = {x, y, z};
        // milestone 1 color
        Vec3 color = {1.0f, 1.0f, 1.0f};
        Sphere* newSphere = createSphere(r, pos, color);
        addSphere(&world, newSphere);
    }
    fclose(inp);
    firstMS(out);
    fclose(out);

    freeWorld(&world);
    for (int i=0; i<m; i++){
        free(colors[i]);
    }
    free(colors);

    return 0;
}

void firstMS(FILE* out){
    // Milestone 1
    Vec3 bg_col = {0.0f, 0.0f, 0.0f};
    Vec3 res;
    float ans;

    res = add(bg_col, light);
    fprintf(out, "(%.1f, %.1f, %.1f) + (%.1f, %.1f, %.1f) = (%.1f, %.1f, %.1f)\n", 
    bg_col.x, bg_col.y, bg_col.z,
    light.x, light.y, light.z,
    res.x, res.y, res.z);

    res = subtract(bg_col, light);
    fprintf(out, "(%.1f, %.1f, %.1f) - (%.1f, %.1f, %.1f) = (%.1f, %.1f, %.1f)\n", 
    bg_col.x, bg_col.y, bg_col.z,
    light.x, light.y, light.z,
    res.x, res.y, res.z);

    res = scalarMultiply(vw, light);
    fprintf(out, "%.1f * (%.1f, %.1f, %.1f) = (%.1f, %.1f, %.1f)\n", 
    vw,
    light.x, light.y, light.z,
    res.x, res.y, res.z);

    res = normalize(light);
    fprintf(out, "normalize(%.1f, %.1f, %.1f) = (%.1f, %.1f, %.1f)\n", 
    light.x, light.y, light.z,
    res.x, res.y, res.z);

    int n = world.size;

    for (int i=0; i<n; i++){
        fprintf(out, "\n");
        Sphere curr_sphere = *world.spheres[i];
        
        res = scalarDivide(curr_sphere.color, curr_sphere.r);
        fprintf(out, "(%.1f, %.1f, %.1f) / %.1f = (%.1f, %.1f, %.1f)\n", 
        curr_sphere.color.x, curr_sphere.color.y, curr_sphere.color.z,
        curr_sphere.r,
        res.x, res.y, res.z);

        ans = dot(light, curr_sphere.pos);
        fprintf(out, "dot((%.1f, %.1f, %.1f), (%.1f, %.1f, %.1f)) = %.1f\n", 
        light.x, light.y, light.z,
        curr_sphere.pos.x, curr_sphere.pos.y, curr_sphere.pos.z,
        ans);

        ans = distance(light, curr_sphere.pos);
        fprintf(out, "distance((%.1f, %.1f, %.1f), (%.1f, %.1f, %.1f)) = %.1f\n", 
        light.x, light.y, light.z,
        curr_sphere.pos.x, curr_sphere.pos.y, curr_sphere.pos.z,
        ans);

        ans = length(curr_sphere.pos);
        fprintf(out, "length(%.1f, %.1f, %.1f) = %.1f\n", 
        curr_sphere.pos.x, curr_sphere.pos.y, curr_sphere.pos.z,
        ans);
    }
}