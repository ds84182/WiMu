#include "HomebrewAppState.h"

#include <Globals.h>

#include <util/Log.h>
#include <util/tinyxml2.h>

HomebrewAppState::~HomebrewAppState() {
	unload();
}

void HomebrewAppState::load(u32 flags) {
	icon = Globals::noIcon;
	name = dir.name;

	Logger::logf("Loading HAState for %s", dir.path.c_str());

	File pngFile(dir.path+"/icon.png");
	if (pngFile.open("rb")) {
		Logger::logf("%s exists, loading", pngFile.path.c_str());
		
		long size = pngFile.seek(0, SEEK_END);
		pngFile.seek(0, SEEK_SET);
		u8* buffer = new u8[size];
		pngFile.read(buffer, size);
		pngFile.close();

		icon = Texture::createFromPNG(buffer);
		if (!icon) {
			Logger::logf("%s loading failed", pngFile.path.c_str());
			icon = Globals::noIcon;
		}

		delete[] buffer;
	}

	File xmlFile(dir.path+"/meta.xml");
	if (xmlFile.open("r")) {
		Logger::logf("%s exists, loading", xmlFile.path.c_str());

		tinyxml2::XMLDocument doc;
		doc.LoadFile(xmlFile.getHandle());

		tinyxml2::XMLElement *appElement = doc.FirstChildElement("app");

		if (appElement) {
			tinyxml2::XMLElement *nameElement = appElement->FirstChildElement("name");
			if (nameElement) {
				name = nameElement->GetText();
				Logger::logf("XML Name: %s", name.c_str());
			}

			if (flags & HOMEBREW_APP_LOAD_EXTRA) {
				tinyxml2::XMLElement *descriptionElement = appElement->FirstChildElement("description");
				if (descriptionElement) {
					description = descriptionElement->GetText();
					Logger::logf("XML Description: %s", description.c_str());
				}
			}
		}
	}

	Logger::logf("Done loading %s", dir.path.c_str());
}

void HomebrewAppState::unload() {
	if (icon && icon != Globals::noIcon) {
		delete icon;
		icon = nullptr;
	}

	description.clear();
}
