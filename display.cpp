#include <SDL3/SDL.h>
#include <array>
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

