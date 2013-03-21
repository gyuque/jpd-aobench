#include "Types.h"
#include "AppGlobal.h"
#include "driver.h"
#include "handlers.h"
#include "aobench.h"

FBPixel* pFrameBuffer1;
FBPixel* pFrameBuffer2;
static short* pRAMSinTable;
Scene gScene;

static void initGlobals();
static void buildAOBenchScene(Scene* pScene);

int main() {
	const int renderWidth  = 128;
	const int renderHeight = 128;
	const int sx = (kFrameBufferWidth  - renderWidth) >> 1;
	const int sy = (kFrameBufferHeight - renderHeight) >> 1;
	int fbOffset = kFrameBufferWidth * sy + sx;

	initGlobals();
	buildSinTable(pRAMSinTable);
	buildAOBenchScene(&gScene);

	// Init screen
	clearScreenDMA(pFrameBuffer1, 21130);
	fillRectDMA(pFrameBuffer1, sx, sy, renderWidth, renderHeight, 0);

	// Run aobench
	aoInit(renderWidth, renderHeight, 6, &gScene);
	aoRender(pFrameBuffer1 + fbOffset, kFrameBufferWidth);

	// Stop
	while(1) {
		showScreen(pFrameBuffer1, pFrameBuffer2);
	}

	return 0;
}

void onPixelRendered() {
	showScreen(pFrameBuffer1, pFrameBuffer2);
}

void initGlobals() {
	// Set frame buffer addresses
	pFrameBuffer1 = (FBPixel*)FB1_ADDR;
	pFrameBuffer2 = (FBPixel*)FB2_ADDR;

	// Set new interruptions
	writeInteruptJump(5, IRQ5FUNC_ADDR);

	// RAM pointers
	pRAMSinTable = (short*)LOWRAM_ADDR;
}

void buildAOBenchScene(Scene* pScene) {
    Sphere* spheres = pScene->spheres;
    Plane* plane = &pScene->plane;
    
	spheres[0].center.x = 0;
	spheres[0].center.y = float_to_fixed( 0.501f);
	spheres[0].center.z = float_to_fixed(-3.5f  );
	spheres[0].radius   = float_to_fixed( 1.0f  );

	plane->p.x = 0;
	plane->p.y = - (FIX_SCALE/2);
	plane->p.z = 0.0;
	
	plane->n.x = 0.0;
	plane->n.y = FIX_SCALE;
	plane->n.z = 0.0;
}

