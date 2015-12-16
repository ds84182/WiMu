#include "RenderMain.h"

#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include <malloc.h>
#include <math.h>
#include <gccore.h>

#include <algorithm>
#include <vector>

#include <render/Font.h>
#include <render/GlobalMatrix.h>
#include <render/VertexDescriptions.h>

#include <util/FreeTypeGX.h>
#include <util/Log.h>

#define	FIFO_SIZE (256*1024)

namespace Renderer {

vu8 stopThread = 0;
MessageQueue renderQueue;
Thread *thread = nullptr;

void start() {
	thread = new Thread(Renderer::main);
}

void stop() {
	stopThread = 1;
	thread->join();
}

// Actual Thread Code starts here

static std::vector<Renderable*> renderables;
static std::vector<EasingState*> animations;

static GXRModeObj *screenMode;
static void *frameBuffer;
static vu8 readyForCopy;

static void update_screen();
static void	copy_buffers(u32 unused);

void *main(void *args) {
	Logger::log("Render Thread starting");
	
	GXColor	backgroundColor	= {0, 0, 0,	255};
	void *fifoBuffer = NULL;
	
	VIDEO_Init();
	
	screenMode = VIDEO_GetPreferredMode(NULL);

	frameBuffer	= MEM_K0_TO_K1(SYS_AllocateFramebuffer(screenMode));

	VIDEO_Configure(screenMode);
	VIDEO_SetNextFramebuffer(frameBuffer);
	VIDEO_SetPostRetraceCallback(copy_buffers);
	VIDEO_SetBlack(FALSE);
	VIDEO_Flush();

	fifoBuffer = MEM_K0_TO_K1(memalign(32,FIFO_SIZE));
	memset(fifoBuffer,	0, FIFO_SIZE);

	GX_Init(fifoBuffer, FIFO_SIZE);
	GX_SetCopyClear(backgroundColor, 0x00ffffff);
	GX_SetViewport(0, 0, screenMode->fbWidth, screenMode->efbHeight, 0, 1);
	GX_SetDispCopyYScale((f32)screenMode->xfbHeight/(f32)screenMode->efbHeight);
	GX_SetScissor(0, 0, screenMode->fbWidth, screenMode->efbHeight);
	GX_SetDispCopySrc(0, 0, screenMode->fbWidth, screenMode->efbHeight);
	GX_SetDispCopyDst(screenMode->fbWidth, screenMode->xfbHeight);
	GX_SetCopyFilter(screenMode->aa, screenMode->sample_pattern, GX_TRUE, screenMode->vfilter);
	GX_SetFieldMode(screenMode->field_rendering, ((screenMode->viHeight == 2*screenMode->xfbHeight) ? GX_ENABLE : GX_DISABLE));

	GX_SetCullMode(GX_CULL_NONE);
	GX_CopyDisp(frameBuffer, GX_TRUE);
	GX_SetDispCopyGamma(GX_GM_1_0);

	guOrtho(projectionMatrix, 0, screenMode->efbHeight, 0, screenMode->fbWidth, 0.0F, 30000.0F);
	GX_LoadProjectionMtx(projectionMatrix, GX_ORTHOGRAPHIC);
	
	GX_ClearVtxDesc();
	GX_SetVtxDescv(I8_POSITION_COLOR);
	GX_SetVtxAttrFmt(VA_S16, GX_VA_POS,	GX_POS_XYZ,	GX_S16,	0);
	GX_SetVtxAttrFmt(VA_S16, GX_VA_CLR0, GX_CLR_RGBA, GX_RGBA8,	0);
	GX_SetVtxAttrFmt(VA_S16, GX_VA_TEX0, GX_TEX_ST,	GX_F32,	0);
	GX_SetVtxAttrFmt(VA_F32, GX_VA_POS,	GX_POS_XYZ,	GX_F32,	0);
	GX_SetVtxAttrFmt(VA_F32, GX_VA_CLR0, GX_CLR_RGBA, GX_RGBA8,	0);
	GX_SetVtxAttrFmt(VA_F32, GX_VA_TEX0, GX_TEX_ST,	GX_F32,	0);
	GX_SetNumChans(1);
	GX_SetNumTexGens(1);
	GX_SetTexCoordGen(GX_TEXCOORD0, GX_TG_MTX2x4, GX_TG_TEX0, GX_IDENTITY);
	GX_SetTevOrder(GX_TEVSTAGE0, GX_TEXCOORD0, GX_TEXMAP0, GX_COLOR0A0);
	GX_SetTevOp(GX_TEVSTAGE0, GX_PASSCLR);
	GX_SetBlendMode(GX_BM_BLEND, GX_BL_SRCALPHA, GX_BL_INVSRCALPHA, GX_LO_CLEAR);
	
	InitFreeType();
	Font::init();
	
	while (!stopThread) {
		RenderMessage *message = reinterpret_cast<RenderMessage*>(renderQueue.receive(MQ_MSG_NOBLOCK));
		while (message) {
			if (message->type == ADD_RENDERABLE) {
				Logger::logf("Adding Renderable %p", message->renderable);
				renderables.emplace_back(message->renderable);
			} else if (message->type == REMOVE_RENDERABLE) {
				Logger::logf("Removing Renderable %p", message->renderable);
				renderables.erase(std::remove(renderables.begin(), renderables.end(), message->renderable));
			} else if (message->type == RUN_ANIMATION) {
				Logger::logf("Running Animation %p %f", message->runAnimationArgs->subject, message->runAnimationArgs->duration);
				EasingState *state = new EasingState(message->runAnimationArgs->subject, message->runAnimationArgs->duration, message->runAnimationArgs->easing);
				animations.emplace_back(state);
			} else if (message->type == STOP_ANIMATION) {
				Logger::log("Stop Animation NYI");
			}
			
			delete message; //I guess... I don't know how sloppy this may become.
			
			message = reinterpret_cast<RenderMessage*>(renderQueue.receive(MQ_MSG_NOBLOCK));
		}

		std::vector<EasingState*> finished;
		for (EasingState *state : animations) {
			if (state->update(1.0f/60.0f)) {
				finished.emplace_back(state);
			}
		}

		for (EasingState *state : finished) {
			animations.erase(std::remove(animations.begin(), animations.end(), state));
			delete state;
		}
		
		resetMatrixStack();
		guMtxIdentity(viewMatrix);

		GX_SetViewport(0, 0, screenMode->fbWidth, screenMode->efbHeight, 0, 1);
		GX_InvVtxCache();
		GX_InvalidateTexAll();
		update_screen();
	}

	for (Renderable *r : renderables) {
		delete r;
	}
	
	return nullptr;
}

static void update_screen() {
	for (Renderable *r : renderables) {
		if (r) {
			r->render();
		}
	}
	
	GX_LoadPosMtxImm(viewMatrix, GX_PNMTX0);
	//Roboto::Regular->drawText(128, 320, L"uwotm8i", (GXColor){0xFF, 0x00, 0x00, 0xFF});

	GX_DrawDone();
	readyForCopy = GX_TRUE;

	VIDEO_WaitVSync();
	return;
}

static void	copy_buffers(u32 count __attribute__ ((unused))) {
	if (readyForCopy==GX_TRUE) {
		GX_SetZMode(GX_TRUE, GX_LEQUAL,	GX_TRUE);
		GX_SetColorUpdate(GX_TRUE);
		GX_CopyDisp(frameBuffer,GX_TRUE);
		GX_Flush();
		readyForCopy = GX_FALSE;
	}
}

}
