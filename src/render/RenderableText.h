#pragma once

#include <wchar.h>

#include <util/FreeTypeGX.h>

#include "Font.h"
#include "Renderable.h"

class RenderableText : public Renderable {
	f32 x, y;
	u32 color = 0xFFFFFFFF;
	wchar_t* chars;
	FreeTypeGX *font = Roboto::Regular;
	
	public:
	
	RenderableText(const wchar_t *t, f32 x = 0, f32 y = 0) : x(x), y(y) {
		chars = new wchar_t[wcslen(t)];
		wcscpy(chars, t);
	}
	
	RenderableText(const char *t, f32 x = 0, f32 y = 0) : x(x), y(y) {
		chars = charToWideChar(t);
	}

	void setText(const char *t) {
		delete[] chars;
		chars = charToWideChar(t);
	}

	wchar_t *getText() {
		return chars;
	}
	
	virtual ~RenderableText();
	virtual void render();
};
