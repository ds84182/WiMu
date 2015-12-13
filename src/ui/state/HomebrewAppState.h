#pragma once

#include <render/Texture.h>

#include <util/FileSystem.h>

#include "State.h"

#define HOMEBREW_APP_LOAD_EXTRA 1

class HomebrewAppState : public State {
public:
	Directory dir;
	std::string name;
	Texture *icon = nullptr;
	std::string description;

	HomebrewAppState(std::string path) : State(HOMEBREW_APP_ELEMENT), dir(path) {

	}

	virtual ~HomebrewAppState();
	virtual void load(u32 flags);
	virtual void unload();
};
