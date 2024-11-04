#include "vector.h"
#include <math.h>

Vec3 add(Vec3 v1, Vec3 v2){
    Vec3 res;
    res.x = v1.x + v2.x;
    res.y = v1.y + v2.y;
    res.z = v1.z + v2.z;
    return res;
}

Vec3 subtract(Vec3 v1, Vec3 v2){ // returns v1-v2
    Vec3 res;
    res.x = v1.x - v2.x;
    res.y = v1.y - v2.y;
    res.z = v1.z - v2.z;
    return res;
}

Vec3 scalarMultiply(float s, Vec3 v){
    Vec3 res;
    res.x = s*v.x;
    res.y = s*v.y;
    res.z = s*v.z;
    return res;
}

Vec3 scalarDivide(Vec3 v, float d){
    Vec3 res;
    res.x = v.x/d;
    res.y = v.y/d;
    res.z = v.z/d;
    return res;
}

float dot(Vec3 v1, Vec3 v2){
    float dot_prod = v1.x * v2.x + v1.y * v2.y + v1.z * v2.z;
    return dot_prod;
}

float length2(Vec3 v){
    return dot(v,v);
}

float length(Vec3 v){
    return sqrt(length2(v));
}

float distance2(Vec3 v1, Vec3 v2){
    Vec3 dist_vect = subtract(v1, v2);
    return length2(dist_vect);
}

float distance(Vec3 v1, Vec3 v2){
    return sqrt(distance2(v1, v2));
}

Vec3 normalize(Vec3 v){
    Vec3 res = scalarDivide(v, length(v));
    return res;
}