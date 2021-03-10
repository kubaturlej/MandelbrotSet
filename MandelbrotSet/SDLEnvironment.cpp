#include <iostream>
#include <thread>
#include <vector>

#include "SDLEnvironment.h"
#include "Pixel.h"
#include "SDL.h"




SDLEnvironment::~SDLEnvironment()
{
	SDL_DestroyRenderer(renderer);
	SDL_DestroyWindow(window);
}

bool SDLEnvironment::init(const char* title, int xpos, int ypos, int width, int height)
{
	if (SDL_Init(SDL_INIT_EVERYTHING) == 0)
	{
		
		std::cout << "SDL init success\n";
		
		window = SDL_CreateWindow(title, xpos, ypos, width, height, 0);
		if (window != 0)
		{
			std::cout << "Window creation succes\n";
			renderer = SDL_CreateRenderer(window, -1, SDL_RENDERER_SOFTWARE);

			if (renderer != 0)
			{
				std::cout << "Renderer creation success\n";

				keystate = SDL_GetKeyboardState(0);
				//SDL_SetWindowFullscreen(window, SDL_WINDOW_FULLSCREEN);

				SDL_SetRenderDrawColor(renderer, 0, 0, 0, 255);
			}
			else
			{
				std::cout << "Renderer init fail\n";
				return false;
			}
		}
		else
		{
			std::cout << "Window init fail\n";
			return false;
		}
	}
	else
	{
		std::cout << "SDL init fail\n";
		return false;
	}

	return true;
}

bool SDLEnvironment::pressQuit()
{
	SDL_PollEvent(&event);

	if (keystate[SDL_SCANCODE_ESCAPE])
		return 1;
	else
		return 0;
}

