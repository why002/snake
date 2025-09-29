#include <SDL3/SDL.h>
#include <array>
#include <deque>
#include <algorithm>
#include "conf.h"
#include <random>

#ifdef _DEBUG
#include <thread>
#include <iostream>
#endif

int displayNumber(SDL_Renderer* renderer, short num, float x, float y)
{
	std::array<SDL_FRect, 7> coordinates =
	{
		SDL_FRect{ x+2.0f,y+0.0f,18.0f,4.0f },
		SDL_FRect{ x+0.0f,y+2.0f,4.0f,18.0f},
		SDL_FRect{ x+19.0f,y+2.0f,4.0f,18.0f },
		SDL_FRect{x+2.0f,y+19.0f,18.0f,4.0f},
		SDL_FRect{x+0.0f,y+21.0f,4.0f,18.0f},
		SDL_FRect{x+19.0f,y+21.0f,4.0f,18.0f},
		SDL_FRect{x+2.0f,y+38.0f,18.0f,4.0f},
	};
	const std::array<std::array<bool, 7>, 10> numbers =
	{
		std::array<bool, 7>{true,true,true,false,true,true,true}, //0
		std::array<bool, 7>{false,false,true,false,false,true,false}, //1
		std::array<bool, 7>{true,false,true,true,true,false,true}, //2
		std::array<bool, 7>{true,false,true,true,false,true,true}, //3
		std::array<bool, 7>{false,true,true,true,false,true,false}, //4
		std::array<bool, 7>{true,true,false,true,false,true,true}, //5
		std::array<bool, 7>{true,true,false,true,true,true,true}, //6
		std::array<bool, 7>{true,false,true,false,false,true,false}, //7
		std::array<bool, 7>{true,true,true,true,true,true,true}, //8
		std::array<bool, 7>{true,true,true,true,false,true,true} //9
	};
	if (num < 0 || num>9) return -1;
	for (int i = 0; i < 7; i++)
	{
		if (numbers[num][i])
		{
			SDL_RenderFillRect(renderer, &coordinates[i]);
		}
	}
	return num;
}

int displayApple(SDL_Renderer* renderer, std::deque<Point> snake, bool newApple = 0)
{
	static std::random_device rd;
	static std::mt19937 gen(rd());
	static std::uniform_int_distribution<> disX(0, mapX - 1);
	static std::uniform_int_distribution<> disY(0, mapY - 1);
	static Point apple{ disX(gen),disY(gen) };
	static SDL_Vertex vert[8];
	const float shift1 = 5.857894376, shift2 = 14.14213562;
	const int indices[]={ 0,1,2, 0,2,3, 0,3,4, 0,4,5, 0,5,6, 0,6,7, 0,7,1 };
	if (newApple == 1)
	{
		apple = { disX(gen),disY(gen) };
#ifdef _DEBUG
		std::thread([]()
			{
				std::cout << std::format("New apple at ({},{})\n", apple.x, apple.y);
			}
		).detach();
#endif
		while (std::find_if(snake.begin(), snake.end(), [](const Point& p) { return p.x == apple.x && p.y == apple.y; }) != snake.end())
		{
			apple = { disX(gen),disY(gen) };
		}
		vert[0].position = { apple.x * 20.0f + shift1,apple.y * 20.0f + numberY };
		vert[1].position = { apple.x * 20.0f + shift2,apple.y * 20.0f + numberY };
		vert[2].position = { apple.x * 20.0f + 20.0f,apple.y * 20.0f + shift1 + numberY };
		vert[3].position = { apple.x * 20.0f + 20.0f,apple.y * 20.0f + shift2 + numberY };
		vert[4].position = { apple.x * 20.0f + shift2,apple.y * 20.0f + 20.0f + numberY };
		vert[5].position = { apple.x * 20.0f + shift1,apple.y * 20.0f + 20.0f + numberY };
		vert[6].position = { apple.x * 20.0f,apple.y * 20.0f + shift2 + numberY };
		vert[7].position = { apple.x * 20.0f,apple.y * 20.0f + shift1 + numberY };
		for (int i = 0; i < 8; i++)
		{
			vert[i].color = { 1.0f,0.0f,0.0f,0.0f };
		}
	}
	SDL_RenderGeometry(renderer, nullptr, vert, 8, indices, 21);
	std::cout << SDL_GetError();
	return 0;
}
