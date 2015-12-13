#include "Globals.h"

#include "no_icon_png.h"

namespace Globals {

Texture *noIcon;

void init() {
	noIcon = Texture::createFromPNG(no_icon_png);
}

void deinit() {
	delete noIcon;
}

}
