#ifndef TINYMATH_H_INCLUDED
#define TINYMATH_H_INCLUDED

#include "Types.h"

#define FIX_SCALE 4096L
#define FIX_BITS 12
#define F_PI 3.14159f
#define SINTABLE_LENGTH 128

typedef struct _vec {
	Fixed x;
	Fixed y;
	Fixed z;
} vec;

void buildSinTable(short* pTbl);
Fixed fxsin(Fixed angle);
Fixed fxcos(Fixed angle);

Fixed vdot(vec v0, vec v1);
void vcross(vec *c, vec v0, vec v1);
Fixed fxsqrt(Fixed n);
void vnormalize(vec* c);
Fixed fxrand();

float fixed_to_float(const Fixed x);
float float_to_fixed(const float f);

#endif