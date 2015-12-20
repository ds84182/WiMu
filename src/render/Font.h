#pragma once

#include <util/FreeTypeGX.h>

#define FONTSIZE(x) (x*2)

namespace Roboto {
	extern int BodySize;
	extern FreeTypeGX *Body;

	extern int TitleSize;
	extern FreeTypeGX *Title;
};

namespace Font {
	void init();
};
