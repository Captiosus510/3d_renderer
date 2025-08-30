#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <math.h>
#include "raylib.h"
#include "vector.h"
#include "spheres.h"
#include "color.h"
#define HEX_STR_SIZE 8
#define SHADOWFACTOR 0.1
#define ANTIALIASING 1

#define MIN(a, b) ((a) < (b) ? (a) : (b))
#define MAX(a, b) ((a) > (b) ? (a) : (b))

//initialize all vars as global variables
int imw, imh, num_colors, m, bg_ind, n;
float vh, vw, focal, brightness;
Vec3 light;
World world;
unsigned int* colors;

Vec3 calcColor(Sphere* sphere, Vec3 light_pos, float brightness, Vec3 rayPos, Vec3 rayDir, float t);
Vec3 pixelColor(Vec3 rayPos, Vec3 rayDir, Vec3 bg_col);

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
    colors = malloc(m*sizeof(unsigned int));
    if (colors == NULL){
        fprintf(stderr, "ERROR: No available space\n");
        fclose(inp);
        exit(EXIT_FAILURE);
    }
    // put all colors in colors array
    for (int i=0; i<m; i++){
        if (fscanf(inp, "%x", &colors[i]) != 1) {
            fprintf(stderr, "ERROR: Invalid color format\n");
            free(colors);
            fclose(inp);
            exit(EXIT_FAILURE);
        }
    }
    //sort list based on compaarison function
    qsort(colors, m, sizeof(unsigned int), compareColor);
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
        Vec3 color = unpackRGB(colors[color_ind]);
        Sphere* newSphere = createSphere(r, pos, color);
        addSphere(&world, newSphere);
    }
    fclose(inp);

    InitWindow(imw, imh, "3D Renderer");

    Vec3 rayPos = {0, 0, 0}; 

    while (!WindowShouldClose())
    {
        if (IsKeyDown(KEY_W)) rayPos.z -= 0.1f;
        if (IsKeyDown(KEY_S)) rayPos.z += 0.1f;
        if (IsKeyDown(KEY_A)) rayPos.x -= 0.1f;
        if (IsKeyDown(KEY_D)) rayPos.x += 0.1f;
        if (IsKeyDown(KEY_SPACE)) rayPos.y += 0.1f;
        if (IsKeyDown(KEY_LEFT_SHIFT)) rayPos.y -= 0.1f;
        
        BeginDrawing();
        
        float x_inc = vw / imw; // Width increment
        float y_inc = vh / imh; // Height increment
        int sample_num = (int)sqrt(ANTIALIASING); // Anti-aliasing sample count per axis
        float x_start = -vw / 2 + x_inc / (float) (sample_num*2); // Top-left x-coordinate
        float y_start = vh / 2 - y_inc / (float) (sample_num*2);  // Top-left y-coordinate

        Vec3 bg_col = unpackRGB(colors[bg_ind]); // Background color
        
        ClearBackground((Color){(unsigned char)(255.999 * bg_col.x), (unsigned char)(255.999 * bg_col.y), (unsigned char)(255.999 * bg_col.z), 255});

        // Iterate through each pixel
        for (int i = 0; i < imh; i++) {
            for (int j = 0; j < imw; j++) {
                Vec3 totalRGB = {0, 0, 0};

                // Compute anti-aliasing samples
                for (int sample_row = 0; sample_row < sample_num; sample_row++) {
                    for (int sample_col = 0; sample_col < sample_num; sample_col++) {
                        Vec3 rayDir = {
                            x_start + j * x_inc + sample_row * x_inc / (float) sample_num,
                            y_start - i * y_inc - sample_col * y_inc / (float) sample_num,
                            -focal
                        };

                        rayDir = normalize(rayDir);
                        Vec3 pixel_col = pixelColor(rayPos, rayDir, bg_col);
                        totalRGB = add(totalRGB, pixel_col);
                    }
                }

                Vec3 final_color = scalarDivide(totalRGB, 1.0*sample_num*sample_num);
                DrawPixel(j, i, (Color){(unsigned char)(255.999 * final_color.x), (unsigned char)(255.999 * final_color.y), (unsigned char)(255.999 * final_color.z), 255});
            }
        }
        EndDrawing();
    }

    CloseWindow();

    freeWorld(&world);
    free(colors);

    return 0;
}

Vec3 pixelColor(Vec3 rayPos, Vec3 rayDir, Vec3 bg_col){
    Vec3 pixel_col;
    float t;
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
    return pixel_col;
}

Vec3 calcColor(Sphere* sphere, Vec3 light_pos, float brightness, Vec3 rayPos, Vec3 rayDir, float t){
    Vec3 p = add(rayPos, scalarMultiply(t, rayDir)); // intersection point vector
    Vec3 n = normalize(subtract(p, sphere->pos)); // normal at the intersectionn point
    Vec3 d = normalize(subtract(light_pos, p));
    Vec3 pixel_col;

    float I0 = (float) (brightness)*(MAX(dot(d, n), 0)/distance2(light, p));
    float I = MIN(I0, 1);
    
    pixel_col = scalarMultiply(I, sphere->color);

    Vec3 shadow_point = add(p, scalarMultiply(0.001, n));
    Vec3 shadow_dir = subtract(light_pos, shadow_point);
    float a;
    for (Sphere **ptr = world.spheres; ptr < world.spheres+world.size; ptr++){
        if (doesIntersect(*ptr, shadow_point, shadow_dir, &a) && a < 1){
            return scalarMultiply(SHADOWFACTOR, pixel_col);
        }
    }

    return pixel_col;
}