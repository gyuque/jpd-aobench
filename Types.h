#ifndef TYPES_H_INCLUDED
#define TYPES_H_INCLUDED

typedef unsigned long Uint32;
typedef unsigned int Uint16;
typedef int Int16;
typedef long Int32;
typedef unsigned int FBPixel;
typedef Int32 Fixed;

typedef struct _DoubleBuffer {
	FBPixel* pBuffer1;
	FBPixel* pBuffer2;
	
	FBPixel* pCurentBackBuffer;
	FBPixel* pCurentPrimaryBuffer;
} DoubleBuffer;

#endif
