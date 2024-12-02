#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <math.h>
#include "vector.h"
#include "spheres.h"
#include "color.h"
#define HEX_STR_SIZE 8
#define SHADOWFACTOR 0.1

#define MIN(a, b) ((a) < (b) ? (a) : (b))
#define MAX(a, b) ((a) > (b) ? (a) : (b))


//initialize all vars as global variables
int imw, imh, num_colors, m, bg_ind, n;
float vh, vw, focal, brightness;
Vec3 light;
World world;
char** colors;

void output(FILE* out);
Vec3 calcColor(Sphere* sphere, Vec3 light_pos, float brightness, Vec3 rayPos, Vec3 rayDir, float t);

int main(int argc, char *argv[]){
    FILE* inp = fopen(argv[1], "r");
    if (inp == NULL){
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

    FILE* out = fopen(argv[2], "w");
    if (out == NULL){
        fprintf(stderr, "ERROR: output.txt does not exist.\n");
        exit(EXIT_FAILURE);
    }
    output(out);
    fclose(out);

    freeWorld(&world);
    for (int i=0; i<m; i++){
        free(colors[i]);
    }
    free(colors);

    return 0;
}

#ifdef MS1
void output(FILE* out){
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
#endif
#ifdef MS2
void output(FILE* out){
    float x_inc = vw/imw; // set the x increment to be partitioned by num of width pixels
    float y_inc = vh/imh; // set the y increment to be paritition by num of height pixels
    float x_start = -vw/2 + x_inc/2;
    float y_start = vh/2-y_inc/2;
    float t;
    Vec3 rayPos = {0, 0, 0}; // ray always starts where camera is, and camera is always at 0,0,0
    Vec3 rayDir = {0, 0, -focal}; //intialize direction vector of the ray, set it to begin rendering at the top left of the image
    Vec3 bg_col = {0, 0, 0};
    Vec3 pixel_col;

    //intitialize the ppm file
    fprintf(out, "P3\n");
    fprintf(out, "%d %d\n", imw, imh);
    fprintf(out, "255\n");

    //iterate through each pixel
    for (int i=0; i<imh; i++){
        rayDir.y = y_start - i*y_inc;
        for (int j=0; j<imw; j++){
            rayDir.x = x_start + j*x_inc;
            float closest_t = INFINITY;
            Sphere *closest_sphere = NULL;
            for (Sphere **ptr = world.spheres; ptr < world.spheres+world.size; ptr++){
                if (doesIntersect(*ptr, rayPos, rayDir, &t) && t < closest_t){
                    closest_t = t;
                    closest_sphere = *ptr;
                }
            }
            if (closest_sphere){
                pixel_col = calcColor(closest_sphere, light, brightness, rayPos, rayDir, closest_t);
            }
            else {
                pixel_col = bg_col;
            }
            writeColour(out, pixel_col);
        }
        fprintf(out, "\n");
    }
}

Vec3 calcColor(Sphere* sphere, Vec3 light_pos, float brightness, Vec3 rayPos, Vec3 rayDir, float t){
    Vec3 p = add(rayPos, scalarMultiply(t, rayDir)); // intersection point vector
    Vec3 n = normalize(subtract(p, sphere->pos)); // normal at the intersectionn point
    Vec3 d = normalize(subtract(light_pos, p));
    Vec3 pixel_col;

    float I0 = brightness*(MAX(dot(d, n), 0)/distance2(light, p));
    float I = MIN(I0, 1);
    
    pixel_col = scalarMultiply(I, sphere->color);

    Vec3 shadow_point = add(p, scalarMultiply(0.001, n));
    Vec3 shadow_dir = subtract(light_pos, shadow_point);
    float a;
    for (Sphere **ptr = world.spheres; ptr < world.spheres+world.size; ptr++){
        if (doesIntersect(*ptr, shadow_point, shadow_dir, &a)){
            pixel_col = scalarMultiply(SHADOWFACTOR, pixel_col);
            break;
        }
    }

    return pixel_col;
}
#endif
