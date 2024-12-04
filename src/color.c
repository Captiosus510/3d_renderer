#include "color.h"

//define color masks
unsigned int r_mask = 255 << 16;
unsigned int g_mask = 255 << 8;
unsigned int b_mask = 255;

Vec3 unpackRGB(unsigned int packedRGB){
    float r = ((packedRGB & r_mask) >> 16)/255.0;
    float g = ((packedRGB & g_mask) >> 8)/255.0;
    float b = (packedRGB & b_mask)/255.0;
    Vec3 color = {r, g, b};
    return color;
}
void writeColour(FILE *ppmFile, Vec3 color){
    int r = (int)(color.x * 255.0f);
    int g = (int)(color.y * 255.0f);
    int b = (int)(color.z * 255.0f);
    fprintf(ppmFile, "%d %d %d ", r, g, b);
}

int compareColor(const void *a, const void *b)
{
    int a1 = 0, b1 = 0;
    for (int i = 0; i < sizeof(int); i++)
    {
        a1 |= (*((unsigned char*)a + i) & 0x0F) << (i * 8);
        b1 |= (*((unsigned char*)b + i) & 0x0F) << (i * 8);
    }
    
    return (a1 < b1) ? -1 : (b1 < a1) ? 1 : (*((int*)a) < *((int*)b)) ? -1 : (*((int*)a) > *((int*)b)) ? 1 : 0;
}