#include <iostream>
#include <SDL3/SDL.h>
#include <algorithm>
#include <list>

#include "display.h"

int main(int argc, char* argv[]) {
	if (SDL_Init(SDL_INIT_VIDEO|SDL_INIT_EVENTS) == 0) 
	{
		std::cerr << "SDL_Init Error: " << SDL_GetError() << std::endl;
		return 1;
	}
	SDL_Window* win = SDL_CreateWindow("Snake",800,800,0);
	if (win == nullptr) 
	{
		std::cerr << "SDL_CreateWindow Error: " << SDL_GetError() << std::endl;
		SDL_Quit();
		return 1;
	}
	auto renderer = SDL_CreateRenderer(win, nullptr);
	SDL_Event event;
	int a = 0;
	while (true)
	{
		SDL_SetRenderDrawColor(renderer, 0, 0, 0, 0);
		SDL_RenderClear(renderer);
		SDL_SetRenderDrawColor(renderer, 255, 255, 255, 0);
		displayNumber(renderer, a%10, 10.0f, 10.0f);
		a++;
		SDL_Delay(100 );
		SDL_FRect rect = { 10.0,10.0,20.0,20.0 };
		//SDL_RenderFillRect(renderer,&rect);
		SDL_RenderPresent(renderer);
		while (SDL_PollEvent(&event))
		{
			if (event.type == SDL_EVENT_QUIT)
			{
				SDL_DestroyWindow(win);
				SDL_Quit();
				return 0;
			}
		}
	}
	return 0;
}
