#include "Font.h"

#include "Roboto_Regular_ttf.h"

namespace Roboto {
	int BodySize = FONTSIZE(14);
	FreeTypeGX *Body;

	int TitleSize = FONTSIZE(20);
	FreeTypeGX *Title;
}

namespace Font {
	void init() {
		Roboto::Body = new FreeTypeGX(Roboto_Regular_ttf, Roboto_Regular_ttf_size, Roboto::BodySize);
		Roboto::Title = new FreeTypeGX(Roboto_Regular_ttf, Roboto_Regular_ttf_size, Roboto::TitleSize);
	}
}
