#pragma once

#include <util/FreeTypeGX.h>

namespace Roboto {
	extern FreeTypeGX *Regular;
};

#include "Roboto_Regular_ttf.h"

namespace Font {
	inline void init() {
		Roboto::Regular = new FreeTypeGX(Roboto_Regular_ttf, Roboto_Regular_ttf_size, 24);
	}
};
