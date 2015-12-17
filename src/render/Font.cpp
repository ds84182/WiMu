#include "Font.h"

#include "Roboto_Regular_ttf.h"

namespace Roboto {
	FreeTypeGX *Body;
	FreeTypeGX *Title;
}

namespace Font {
	void init() {
		Roboto::Body = new FreeTypeGX(Roboto_Regular_ttf, Roboto_Regular_ttf_size, FONTSIZE(14));
		Roboto::Title = new FreeTypeGX(Roboto_Regular_ttf, Roboto_Regular_ttf_size, FONTSIZE(20));
	}
}
