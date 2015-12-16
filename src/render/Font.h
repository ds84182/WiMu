#pragma once

#include <util/FreeTypeGX.h>

namespace Roboto {
	extern FreeTypeGX *Body;
	extern FreeTypeGX *Title;
};

#include "Roboto_Regular_ttf.h"

#define FONTSIZE(x) (x*2)

namespace Font {
	inline void init() {
		Roboto::Body = new FreeTypeGX(Roboto_Regular_ttf, Roboto_Regular_ttf_size, FONTSIZE(14));
		Roboto::Title = new FreeTypeGX(Roboto_Regular_ttf, Roboto_Regular_ttf_size, FONTSIZE(20));
	}
};
