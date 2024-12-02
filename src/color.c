#include "color.h"

Vec3 unpackRGB(unsigned int packedRGB);
void writeColour(FILE *ppmFile, Vec3 color){
    int r = (int)(color.x * 255.0f + 0.5f);
    int g = (int)(color.y * 255.0f + 0.5f);
    int b = (int)(color.z * 255.0f + 0.5f);
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