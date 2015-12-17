#pragma once

#include <util/FreeTypeGX.h>

namespace Roboto {
	extern FreeTypeGX *Body;
	extern FreeTypeGX *Title;
};

#define FONTSIZE(x) (x*2)

namespace Font {
	void init();
};
