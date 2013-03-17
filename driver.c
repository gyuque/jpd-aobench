// Video and DMA tiny driver
#include "driver.h"

void setFrameBufferAddress(Uint32 fbi, Uint32 fbo)
{
	*syspFBInAddrH  = fbi >> 16;
	*syspFBInAddrL  = fbi & 0xfff0;
	*syspFBOutAddrH = fbo >> 16;
	*syspFBOutAddrL = fbo & 0xfff8;
}

void showScreen(FBPixel* pFBIn, FBPixel* pFBOut) {
	setFrameBufferAddress((Uint32)pFBIn, (Uint32)pFBOut);

	while (!(*syspPPURun & 0x8000)) { /* Wait */ }
	*syspPPURun = 1;
}

void fillDMA(Uint32 destAddress, Int16 fillValue, Uint32 count)
{
	Int16 data = fillValue;
	asm("fiq off"); asm("irq off");

	while ((*syspDMAInt & 0x0100) == 1) {} // Wait for idle
	*syspDMACtrl0 = 0x200;

	// Set source and destination addresses
	*syspDMASrcAddrL0   = (Uint16)(Uint32) &data;
	*syspDMASrcAddrH0   = 0;

	*syspDMADestAddrL0  = destAddress & 0xffff;
	*syspDMADestAddrH0  = destAddress >> 16;

	*syspDMADestCountL0 = count & 0xffff;
	*syspDMADestCountH0 = (count >> 16) | 0x7000 ;   

	// Run
	*syspDMACtrl0       = 0x0089;
	while ((*syspDMAInt & 0x0001) == 0) {}
	*syspDMAInt  = 0x0001;

	asm("fiq on"); asm("irq on");
}

void fillRectDMA(FBPixel* p, int x, int y, int w, int h, FBPixel fillColor) {
	int i;
	p += (long)y * (long)kFrameBufferWidth;
	for (i = 0;i < h;++i) {
		fillDMA((Uint32)p + x, fillColor, w);
		p += kFrameBufferWidth;
	}
}

void clearScreenDMA(FBPixel* pFB, FBPixel clearColor) {
	fillDMA((Uint32)pFB, clearColor, (long)kFrameBufferWidth * (long)kFrameBufferHeight );
}
