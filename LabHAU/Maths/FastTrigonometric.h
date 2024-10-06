#ifndef FASTTRIGONOMETRIC_H
#define FASTTRIGONOMETRIC_H

#include <math.h>
#include <sys/attribs.h>

#define PI_DIV_6   0.523599f// *PI/6
#define PI_DIV_4   0.785398f// *PI/4
#define PI_DIV_3   1.047198f// *PI/3
#define PI_DIV_2   1.570796f// *PI/2
#define PI2_DIV_3  2.094395f// 2*PI/3
#define PI3_DIV_4  2.356194f// 3*PI/4
#define PI5_DIV_6  2.617994f// 5*PI/6
#define PI_NUM     3.141593f// *PI
#define PI7_DIV_6  3.665191f// 7*PI/6
#define PI5_DIV_4  3.926991f// 5*PI/4
#define PI4_DIV_3  4.188790f// 4*PI/3
#define PI3_DIV_2  4.712389f// 3*PI/2
#define PI5_DIV_3  5.235988f// 5*PI/3
#define PI7_DIV_4  5.497787f// 7*PI/4
#define PI11_DIV_6 5.759586f// 11*PI/6
#define PI2_NUM    6.283185f// 2*PI
#define NPI_NUM    -3.141593f// -*PI
#define NPI2_NUM   -6.283185f// -2*PI
#define NPI_DIV_6  PI11_DIV_6// -*PI/6
#define NPI_DIV_4  PI7_DIV_4// -*PI/4
#define NPI_DIV_3  PI5_DIV_3// -*PI/3
#define NPI_DIV_2  PI3_DIV_2// -*PI/2
#define NPI2_DIV_3 PI4_DIV_3// -2*PI/3
#define NPI3_DIV_4 PI5_DIV_4// -3*PI/4
#define NPI5_DIV_6 PI7_DIV_6// -5*PI/6

#undef  SQRT2
#define SQRT2       1.414214f// SQRT(2)

#undef  SQRT3
#define SQRT3       1.732051f// SQRT(3)

#define SQRT3_DIV_2 0.866025f// SQRT(3)/2
#define SQRT2_DIV_2 0.707107f// SQRT(2)/2

typedef struct SINE_COSINE_CONTEXT {
    float Sin;
    float Cos;
} sin_cos_t;

float fastsin(float Angle);
float fastcos(float Angle);
void fastsincos(sin_cos_t *SinCosCxt, float Angle);
float fastatan2(float y, float x);
float Runge_Kutta_Integration(float y_dot, float last_y, float T);

#endif