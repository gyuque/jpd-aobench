// Math functions for fixed point numbers
#include "tinymath.h"

static Fixed gRandSeed;

static short* pSinTable; // Length : 128

float fixed_to_float(const Fixed x) {
    return (float)(x >> FIX_BITS) + (float)( x & ~(0xffffffff << FIX_BITS) ) / (float)FIX_SCALE;
}

float float_to_fixed(const float f) {
    return (Fixed)(f * (float)FIX_SCALE);
}

// Math functions

float taylor_sin(float rad) {
    int i;
    float t   = rad;
    float val = rad;
    for (i = 1;i < 10;++i) {
        t = -t*1.0f / (float)((2*i+1)*(2*i))*(rad*rad);
        val += t;
    }
    
    return val;
}

void buildSinTable(short* pTbl) {
    int i;
    pSinTable = pTbl;
    
    for (i = 0;i < SINTABLE_LENGTH;++i) {
        const float s = taylor_sin(F_PI * (float)i / (float)SINTABLE_LENGTH);
        pSinTable[i] = (short) float_to_fixed(s);
    }
    
}

Fixed fxsin(Fixed angle) {
	// in fixed: 0 <= angle < 2*SCALE
	// in real:  0 <= anfle < 2.0
	angle += FIX_SCALE;
	angle += FIX_SCALE;
    
	angle <<= 7;
	angle /= FIX_SCALE;
	angle &= 0xff;
    
	if (angle < 128) {
		return pSinTable[angle];
	} else {
		return -pSinTable[angle - 128];
	}
}

Fixed fxcos(Fixed angle) {
	return fxsin(angle + (FIX_SCALE >> 1));
}

Fixed fxrand() {
    gRandSeed = (gRandSeed * 1103515245 + 12345) >> 1;
    return gRandSeed & ~(0xffffffff << FIX_BITS);
}

Fixed fxsqrt(Fixed n) {
	Fixed s, t;
	if (n <= 0) {
		return 0;
	}
	
	n <<= FIX_BITS; // <- sqrt(n*scale * scale) = sqrt(n) * scale
	
	s = 1;
	t = n;
	while (s < t) {
		s <<= 1;
		t >>= 1;
	}
	
	do {
		t = s;
		s = (n / s + s) >> 1;
	} while (s < t);
	
	return t;
}


// Vector functions

inline Fixed vdot(vec v0, vec v1) {
	return(v0.x * v1.x + v0.y * v1.y + v0.z * v1.z) / FIX_SCALE;
	//           (n1*scale * n2*scale)/scale = n3 * scale  --^
}

void vcross(vec *c, vec v0, vec v1) {
	c->x = (v0.y * v1.z) / FIX_SCALE - (v0.z * v1.y) / FIX_SCALE;
	c->y = (v0.z * v1.x) / FIX_SCALE - (v0.x * v1.z) / FIX_SCALE;
	c->z = (v0.x * v1.y) / FIX_SCALE - (v0.y * v1.x) / FIX_SCALE;
}

void vnormalize(vec* c)
{
	Fixed length = fxsqrt(vdot((*c), (*c)));
	
	if (length) {
		c->x = c->x * FIX_SCALE / length;
		c->y = c->y * FIX_SCALE / length;
		c->z = c->z * FIX_SCALE / length;
	}
}

