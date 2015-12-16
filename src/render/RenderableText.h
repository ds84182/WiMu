#pragma once

#include <wchar.h>

#include <util/FreeTypeGX.h>

#include "Font.h"
#include "Renderable.h"

class RenderableText : public Renderable {
	wchar_t* chars;
	
public:
	f32 x, y;
	u32 color = 0xFFFFFFFF;
	FreeTypeGX *font = Roboto::Body;
	
	RenderableText(const wchar_t *t, f32 x = 0, f32 y = 0, FreeTypeGX *font = Roboto::Body) : x(x), y(y), font(font) {
		chars = new wchar_t[wcslen(t)];
		wcscpy(chars, t);
	}
	
	RenderableText(const char *t, f32 x = 0, f32 y = 0, FreeTypeGX *font = Roboto::Body) : x(x), y(y), font(font) {
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
