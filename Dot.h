#pragma once

//Using SDL and standard IO
#include <SDL.h>
#include <iostream>

//The dot that will move around on the screen
class Dot
{
    public:
		//The dimensions of the dot
		static const int DOT_WIDTH = 20;
		static const int DOT_HEIGHT = 20;

		//Maximum axis velocity of the dot
		static const int DOT_VEL = 10;

		//Initializes the variables
		Dot () : mPosX(0), mPosY(0), mVelX(0), mVelY(0)
		{}

		Dot (int mPosX, int mPosY, SDL_Renderer* gRenderer) : mPosX(mPosX), mPosY(mPosY), gRenderer(gRenderer), mVelX(0), mVelY(0)
		{}

		//Shows the dot on the screen
		void render();

	private:

		//The position of the dot
		int mPosX, mPosY;

		//The velocity of the dot
		int mVelX, mVelY;

		SDL_Renderer* gRenderer;
};