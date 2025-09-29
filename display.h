#pragma once

#include <SDL3/SDL.h>
#include <deque>
#include "conf.h"

int displayNumber(SDL_Renderer* renderer,short num,float x,float y);
int displayApple(SDL_Renderer* renderer, std::deque<Point> snake, bool newApple = 0);
