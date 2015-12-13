#include <fat.h>
#include <gccore.h>
#include <malloc.h>
#include <math.h>
#include <ogc/lwp.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <wiiuse/wpad.h>

#include <vector>

#include <async/EventQueue.h>
#include <async/Future.h>

#include <render/Texture.h>
#include <render/RenderableGroup.h>
#include <render/RenderableRectangle.h>
#include <render/RenderableTriangle.h>
#include <render/RenderableTexture.h>
#include <render/RenderableText.h>

#include <thread/loader/LoaderMain.h>
#include <thread/render/RenderMain.h>

#include <ui/state/HomebrewAppState.h>

#include <util/FileSystem.h>
#include <util/Network.h>
#include <util/Log.h>
#include <util/Thread.h>
#include <util/tinyxml2.h>

#include "Globals.h"

void *testThread(Future *f);

int	main() {
	printf("Starting WiMu\n");
	
	Renderer::start();
	
	fatInitDefault();
	WPAD_Init();
	
	//Renderer::addRenderable(new RenderableRectangle(64, 64, 128, 128));
	//Renderer::addRenderable(new RenderableTriangle());
	RenderableText *networkText = new RenderableText("Initializing Network", 64, 64);
	Renderer::addRenderable(networkText);
	
	Network::init();
	Logger::init();
	
	Renderer::removeRenderable(networkText);
	delete networkText;

	Loader::start();
	Globals::init();
	//UI::init();
	
	Logger::log("Adding triangle to render thread");
	Renderer::addRenderable(new RenderableTriangle());

	HomebrewAppState has("sd:/apps/3dmaze");
	Loader::queue(&has);
	//UI::add(&has);
	//has should call UI::update at the end
	//UI should ignore updates for elements it doesn't actually have

	/*Completer *c = new Completer();

	Thread b(reinterpret_cast<ThreadEntryPoint>(testThread), c->future);

	LWP_YieldThread();
	LWP_YieldThread();
	LWP_YieldThread();
	LWP_YieldThread();

	Logger::log("Completing completer");
	c->complete(nullptr);*/

	/*File pngFile("sd:/apps/3dmaze/icon.png");
	Logger::logf("fopen %d", pngFile.open("rb"));
	long size = pngFile.seek(0, SEEK_END);
	pngFile.seek(0, SEEK_SET);
	u8* buffer = new u8[size];
	pngFile.read(buffer, size);
	pngFile.close();
	Logger::logf("Buffer %p size: %d", buffer, size);
	//Texture *tex = Texture::createFromPNG(test_png);
	Texture *tex = Texture::createFromPNG(buffer);
	if (tex) {
		Logger::logf("PNG: %p %d %d", tex->data, tex->width, tex->height);
	} else {
		Logger::log("Texture is null");
	}

	Renderer::addRenderable(new RenderableGroup({
		new RenderableTexture(tex, 0, 0),
		new RenderableText("3DMaze", 0, 64+4),
		nullptr
	}, 64, 64));*/

	//TODO: Abstract all this stuff out into UI elements and data states
	//UI Elements manage their Renderables, and are linked to data states
	//Data States handle both the data and the state of the UI element
	//UI Elements (which are static) receive a Renderable and a data state 
	//Their job is to update the Renderable with the data state

	//Data States contains position information and a function pointer to the UI Element update method
	//UI Elements to create: HomebrewAppElement (HomebrewAppState), ListElement (ListState), VBoxElement (BoxState)

	//UI Element updates are called on the UI thread. The Data States also contain a Mutex that locks when they are
	//being updated. This is to make sure the Data Threads (threads that load data into Data States) will not modify
	//the data in the Data State while updating the UI for rendering.

	//UI Element updates are not called per frame, but are scheduled.

	//If a Data State wants to periodically update or handle buttons, it must subscribe with an event handler.
	//There will be a Timer event handler.
	//There will be a global post render event.
	//Events will only update on the main thread.
	//Custom Events allow you to schedule an event for the main thread from another thread.

	/*std::vector<RenderableGroup*> apps;
	std::vector<Texture*> loadedTextures;
	
	f32 y = 64;
	Directory dir("sd:/apps");
	dir.open();
	FilesystemEntity *entity = dir.read();
	while (entity) {
		Logger::logf("%d %s", entity->type, entity->path.c_str());

		if (entity->name != "." && entity->name != "..") {
			

			Texture *texture = noIcon;
			std::string title = entity->name;

			File pngFile(entity->path+"/icon.png");
			if (pngFile.open("rb")) {
				long size = pngFile.seek(0, SEEK_END);
				pngFile.seek(0, SEEK_SET);
				u8* buffer = new u8[size];
				pngFile.read(buffer, size);
				pngFile.close();

				texture = Texture::createFromPNG(buffer);
				if (!texture) {
					texture = noIcon;
				} else {
					loadedTextures.emplace_back(texture);
				}

				delete[] buffer;
			}

			File xmlFile(entity->path+"/meta.xml");
			if (xmlFile.open("r")) {
				tinyxml2::XMLDocument doc;
				doc.LoadFile(xmlFile.getHandle());

				tinyxml2::XMLElement *appElement = doc.FirstChildElement("app");

				if (appElement) {
					tinyxml2::XMLElement *nameElement = appElement->FirstChildElement("name");
					if (nameElement) {
						title = nameElement->GetText();
					}
				}
			}

			RenderableGroup *group = new RenderableGroup({
				new RenderableTexture(texture, 0, 0),
				new RenderableText(title.c_str(), 0, 48+24+4),
				nullptr
			}, 64, y);

			Renderer::addRenderable(group);
			apps.emplace_back(group);

			y += 48+24+4+4;
		}

		delete entity;
		entity = dir.read();
	}
	dir.close();*/
	
	while (1) {
		LWP_YieldThread();
		
		//Renderer::addRenderable(new RenderableRectangle(rand()%640, rand()%480, rand()%256, rand()%256));

		WPAD_ScanPads();

		u32 down = WPAD_ButtonsDown(0);
		if (down & WPAD_BUTTON_HOME)
			break;

		/*if (down & WPAD_BUTTON_DOWN) {
			for (RenderableGroup *group : apps) {
				group->y -= 16;
			}
		}*/
	}

	Loader::stop();

	Logger::log("Stopping render thread");
	Renderer::stop();

	/*for (Texture *tex : loadedTextures) {
		delete tex;
	}*/

	Globals::deinit();	
	
	Logger::log("Bye!");
	Logger::deinit();
	Network::deinit();
	
	return 0;
}

void *testThread(Future *f) {
	EventQueue queue;
	Logger::log("Test thread started");
	
	f->then([](void *arg){
		Logger::log("Future completed");
		return nullptr;
	});
	f->listeningQueue = &queue;

	queue.process();

	return nullptr;
}
