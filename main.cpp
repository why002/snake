#include <iostream>
#include <SDL3/SDL.h>
#include <algorithm>
#include <list>
#include <thread>
#include <chrono>
#include <string>
#include <format>
#include <atomic>

#include "display.h"

std::atomic<bool> isRunning = true;

#ifdef _DEBUG
void printlog(std::string msg)
{
	std::cout << std::format("[LOG]{}\n", msg);
}
#endif

void renderLoop(SDL_Window* window,std::chrono::high_resolution_clock::time_point start)
{
	auto renderer = SDL_CreateRenderer(window, nullptr);
	int duration = 0;
	int score = 0;
	while (isRunning)
	{
		SDL_SetRenderDrawColor(renderer, 0, 0, 0, 0);
		SDL_RenderClear(renderer);
		auto end = std::chrono::high_resolution_clock::now();
		duration = std::chrono::duration_cast<std::chrono::seconds>(end - start).count()%1000;
#ifdef _DEBUG
		std::thread(printlog, std::format("Now:{}ms", std::chrono::duration_cast<std::chrono::microseconds>(end - start).count())).detach();
#endif
		SDL_SetRenderDrawColor(renderer, 255, 255, 255, 0);

		//time
		displayNumber(renderer, duration/100, 5.0f, 5.0f);
		displayNumber(renderer, duration/10%10, 30.0f, 5.0f);
		displayNumber(renderer, duration%10, 55.0f, 5.0f);


		//score
		displayNumber(renderer, score/100, 720.0f, 5.0f);
		displayNumber(renderer, score/10%10, 745.0f, 5.0f);
		displayNumber(renderer, score%10, 770.0f, 5.0f);
		SDL_FRect divider = { 0.0f, 50.0f,800.0f,10.0f };
		SDL_RenderFillRect(renderer, &divider);
		SDL_RenderPresent(renderer);

		//delay
		std::this_thread::sleep_for(std::chrono::milliseconds(500- std::chrono::duration_cast<std::chrono::milliseconds>(std::chrono::high_resolution_clock::now()-start).count() % 500));
	}
}

int main(int argc, char* argv[]) {
	if (SDL_Init(SDL_INIT_VIDEO|SDL_INIT_EVENTS) == 0) 
	{
		std::cerr << "SDL_Init Error: " << SDL_GetError() << std::endl;
		return 1;
	}
	SDL_Window* win = SDL_CreateWindow("Snake",800,860,0);
	if (win == nullptr) 
	{
		std::cerr << "SDL_CreateWindow Error: " << SDL_GetError() << std::endl;
		SDL_Quit();
		return 1;
	}
	SDL_Event event;
	auto start = std::chrono::high_resolution_clock::now();
	std::thread (renderLoop, win,start).detach();
	while (true)
	{
		while (SDL_PollEvent(&event))
		{
			if (event.type == SDL_EVENT_QUIT)
			{
				isRunning = false;
				SDL_DestroyWindow(win);
				SDL_Quit();
				return 0;
			}
		}
	}
	return 0;
}
