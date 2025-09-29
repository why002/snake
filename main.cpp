#include <iostream>
#include <SDL3/SDL.h>
#include <algorithm>
#include <deque>
#include <thread>
#include <chrono>
#include <string>
#include <format>
#include <atomic>

#include "display.h"

std::atomic<bool> isRunning = true;
enum class Toward
{
	up,
	down,
	right,
	left
};
std::atomic<Toward> direction = Toward::up;
std::atomic<Toward> nextDirection = Toward::up;

void renderLoop(SDL_Window* window,std::chrono::high_resolution_clock::time_point start)
{
	auto renderer = SDL_CreateRenderer(window, nullptr);
	int duration = 0;
	int score = 0;
	Point apple;
	apple = displayApple(renderer, {},1);
	std::deque<Point>snake;
	snake.push_back({ 20,21 });
	while (isRunning)
	{
		direction.store(nextDirection.load());

		SDL_SetRenderDrawColor(renderer, 0, 0, 0, 0);
		SDL_RenderClear(renderer);
		auto end = std::chrono::high_resolution_clock::now();
		duration = std::chrono::duration_cast<std::chrono::seconds>(end - start).count()%1000;

#ifdef _DEBUG
		std::thread([&end,&start,showDirection=direction.load()]()
			{ 
				std::cout<<std::format("Now:{}us\n", std::chrono::duration_cast<std::chrono::microseconds>(end - start).count());
				std::cout << std::format("Direction: {}\n", showDirection == Toward::up ? "up" : showDirection == Toward::down ? "down" : showDirection == Toward::left ? "left" : "right");
			}).detach();
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

		//snake
		SDL_SetRenderDrawColor(renderer, 0, 255, 0, 0);
		switch (direction.load())
		{
		case Toward::up:
			snake.push_front({ snake.front().x,snake.front().y - 1 });
			break;
		case Toward::down:
			snake.push_front({ snake.front().x,snake.front().y + 1 });
			break;
		case Toward::left:
			snake.push_front({ snake.front().x - 1,snake.front().y });
			break;
		case Toward::right:
			snake.push_front({ snake.front().x + 1,snake.front().y });
			break;
		}
		if (snake.front().x<0 || snake.front().x >= mapX || snake.front().y < 0 || snake.front().y >= mapY)
		{
			snake.pop_front();
			displaySnake(renderer, snake);
			break;
		}
		bool breakSign = false;
		for (auto& i : snake)
		{
			if (i.x == snake.front().x && i.y == snake.front().y && &i != &snake.front())
			{
				snake.pop_front();
				displaySnake(renderer, snake);
				breakSign = true;
				break;
			}
		}
		if (breakSign == true)
		{
			break;
		}
		if (snake.front().x == apple.x && snake.front().y == apple.y)
		{
			score++;
			apple = displayApple(renderer, snake, 1);
		}
		else
		{
			snake.pop_back();
			displayApple(renderer, {});
		}
		displaySnake(renderer, snake);

		SDL_RenderPresent(renderer);

		//delay
		std::this_thread::sleep_for(std::chrono::milliseconds(250- std::chrono::duration_cast<std::chrono::milliseconds>(std::chrono::high_resolution_clock::now()-start).count() % 250));
	}
	if (isRunning)
	{
		SDL_SetRenderDrawColor(renderer, 255, 0, 0, 0);
		SDL_FRect gameOverRects[] =
		{
			//O
			SDL_FRect{10 * 20.0f,18 * 20.0f + numberY,20.0f,5 * 20.0f},
			SDL_FRect{11 * 20.0f,18 * 20.0f + numberY,20.0f * 3,20.0f},
			SDL_FRect{11 * 20.0f,22 * 20.0f + numberY,20.0f * 3,20.0f},
			SDL_FRect{14 * 20.0f,18 * 20.0f + numberY,20.0f,20.0f * 5},
			//V
			SDL_FRect{16 * 20.0f,18 * 20.0f + numberY,20.0f,20.0f * 2},
			SDL_FRect{17 * 20.0f,20 * 20.0f + numberY,20.0f,20.0f * 2},
			SDL_FRect{18 * 20.0f,22 * 20.0f + numberY,20.0f,20.0f},
			SDL_FRect{19 * 20.0f,20 * 20.0f + numberY,20.0f,20.0f * 2},
			SDL_FRect{20 * 20.0f,18 * 20.0f + numberY,20.0f,20.0f * 2},
			//E
			SDL_FRect{22 * 20.0f,18 * 20.0f + numberY,20.0f,20.0f * 5},
			SDL_FRect{23 * 20.0f,18 * 20.0f + numberY,20.0f * 3,20.0f},
			SDL_FRect{23 * 20.0f,20 * 20.0f + numberY,20.0f * 3,20.0f},
			SDL_FRect{23 * 20.0f,22 * 20.0f + numberY,20.0f * 3,20.0f},
			//R
			SDL_FRect{27 * 20.0f,18 * 20.0f + numberY,20.0f,20.0f*5},
			SDL_FRect{28 * 20.0f,18 * 20.0f + numberY,20.0f*3,20.0f},
			SDL_FRect{28 * 20.0f,20 * 20.0f + numberY,20.0f*3,20.0f},
			SDL_FRect{30 * 20.0f,19 * 20.0f + numberY,20.0f,20.0f},
			SDL_FRect{29 * 20.0f,21 * 20.0f + numberY,20.0f,20.0f},
			SDL_FRect{30 * 20.0f,22 * 20.0f + numberY,20.0f,20.0f},
		};
		SDL_RenderFillRects(renderer, gameOverRects, 19);
		SDL_RenderPresent(renderer);
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
	std::thread rendererThread(renderLoop, win,start);
	while (true)
	{
		while (SDL_PollEvent(&event))
		{
			if (event.type == SDL_EVENT_QUIT)
			{
				isRunning = false;
				SDL_DestroyWindow(win);
				SDL_Quit();
				rendererThread.join();
				return 0;
			}
			if (event.type == SDL_EVENT_KEY_DOWN)
			{
				switch (event.key.key)
				{
				case SDLK_UP:
					if (direction != Toward::down)
					{
						nextDirection = Toward::up;
					}
					break;
				case SDLK_DOWN:
					if (direction != Toward::up)
					{
						nextDirection = Toward::down;
					}
					break;
				case SDLK_RIGHT:
					if (direction != Toward::left)
					{
						nextDirection = Toward::right;
					}
					break;
				case SDLK_LEFT:
					if (direction != Toward::right)
					{
						nextDirection = Toward::left;
					}
					break;
				}
			}
		}
	}
	return 0;
}
