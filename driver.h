#ifndef DRIVER_H_INCLUDED
#define DRIVER_H_INCLUDED

#include "Types.h"

#define kFrameBufferWidth  320
#define kFrameBufferHeight 240

#define KEY_MAP_ADDR_BASE 0x0012
#define kPPUControlBaseAddress 0x7000
#define	kDMABaseAddress        0x7A80

#define syspPPURun     (volatile unsigned int*)(kPPUControlBaseAddress + 0x7C)
#define syspFBInAddrL  (volatile unsigned int*)(kPPUControlBaseAddress + 0x78)
#define syspFBInAddrH  (volatile unsigned int*)(kPPUControlBaseAddress + 0x79)
#define syspFBOutAddrL (volatile unsigned int*)(kPPUControlBaseAddress + 0x7A)
#define syspFBOutAddrH (volatile unsigned int*)(kPPUControlBaseAddress + 0x7B)

#define syspDMACtrl0       (volatile unsigned int*)(kDMABaseAddress + 0x00)
#define syspDMASrcAddrL0   (volatile unsigned int*)(kDMABaseAddress + 0x01)
#define syspDMADestAddrL0  (volatile unsigned int*)(kDMABaseAddress + 0x02)
#define syspDMADestCountL0 (volatile unsigned int*)(kDMABaseAddress + 0x03)
#define syspDMASrcAddrH0   (volatile unsigned int*)(kDMABaseAddress + 0x04)
#define	syspDMADestAddrH0  (volatile unsigned int*)(kDMABaseAddress + 0x05)
#define syspDMADestCountH0 (volatile unsigned int*)(kDMABaseAddress + 0x06)
#define syspDMAInt         (volatile unsigned int*)(kDMABaseAddress + 0x3F)

void setFrameBufferAddress(Uint32 fbi, Uint32 fbo);
void showScreen(FBPixel* pFBIn, FBPixel* pFBOut);
void fillDMA(Uint32 destAddress, Int16 fillValue, Uint32 count);
void fillRectDMA(FBPixel* p, int x, int y, int w, int h, FBPixel fillColor);
void clearScreenDMA(FBPixel* pFB, FBPixel clearColor);

#endif