#include "AppGlobal.h"

void onIRQ5() {
	asm("push r1, r5 to [sp]");

	asm("r1 = [0x1100]");
	asm("r1 = r1 + 1");
	asm("[0x1100] = r1");

	asm("pop r1, r5 from [sp]");
	asm("reti");
}


void writeInteruptJump(int irqNum, unsigned long handlerAddr) {
	// Split jump target address
	unsigned int hi = (handlerAddr >> 16L) & 0x3f;
	unsigned int lo = handlerAddr & 0xffff;

	// Address of jump code
	unsigned int* pCode = (unsigned int*)(0x6FF0 + (irqNum << 1));
	unsigned int gotoCode = pCode[0] & 0xffc0;

	// Rewrite jump code
	asm("irq off");
	pCode[0] = gotoCode | hi;
	pCode[1] = lo;
	asm("irq on");
}