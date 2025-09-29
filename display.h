#pragma once

#include <SDL3/SDL.h>
#include <deque>
#include "conf.h"

int displayNumber(SDL_Renderer* renderer,short num,float x,float y);
Point displayApple(SDL_Renderer* renderer, const std::deque<Point> &snake, bool newApple = 0);
int displaySnake(SDL_Renderer* renderer, const std::deque<Point> &snake);
