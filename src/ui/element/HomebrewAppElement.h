#include "UIElement.h"

#include <render/RenderableGroup.h>
#include <render/RenderableText.h>
#include <render/RenderableTexture.h>

#include <ui/state/HomebrewAppState.h>

class HomebrewAppElement : public UIElement {
	RenderableTexture *textureRenderable = new RenderableTexture(nullptr, 0, 0);
	RenderableText *textRenderable = new RenderableText("", 0, 48+24+4);
	RenderableGroup *mainState = new RenderableGroup({
				textureRenderable,
				textRenderable,
				nullptr
			});
	HomebrewAppState *app;
	
public:
	HomebrewAppElement() : UIElement(HOMEBREW_APP_ELEMENT) {};

	virtual void attach(State *s);
	virtual void detach();
	virtual Renderable *update();
};
