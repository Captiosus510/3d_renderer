#include "color.h"
#include <stdio.h>

int compareColor(const void* a, const void* b){
    unsigned int colorA = *(const unsigned int*)a;
    unsigned int colorB = *(const unsigned int*)b;
    return (colorA > colorB) - (colorA < colorB);
}

Vec3 unpackRGB(unsigned int color) {
    unsigned char r = (color >> 16) & 0xFF;
    unsigned char g = (color >> 8) & 0xFF;
    unsigned char b = color & 0xFF;
    return (Vec3){(float)r / 255.0f, (float)g / 255.0f, (float)b / 255.0f};
}
