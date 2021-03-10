#pragma once
#include "SDL.h"

class SDLEnvironment
{
public:

	SDLEnvironment():window(nullptr),renderer(nullptr) 
	{};
	 ~SDLEnvironment();
	bool init(const char* title, int xpos, int ypos, int width, int
		height);
	SDL_Renderer* getRenderer() const { return renderer; }
	SDL_Event getEvent() { return event; }
	const Uint8* getKeystate() { return keystate; }
	bool pressQuit();

private:

	SDL_Window* window;
	SDL_Renderer* renderer;
	SDL_Event event;
	const Uint8* keystate;
};

