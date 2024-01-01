#pragma once

//Using SDL and standard IO
#include <SDL.h>
#include <iostream>
#include <cmath>
#include "EventController.h"

//The dot that will move around on the screen
class Dot
{
	public:

		//The dimensions of the dot
		static const int DOT_WIDTH = 20;
		static const int DOT_HEIGHT = 20;

		//Maximum axis velocity of the dot
		static const int DOT_VEL = 10;

		int diagonalVelocity = sqrt(pow(DOT_VEL, 2)/2);

		//Initializes the variables
		Dot () : posX(0), posY(0), velX(0), velY(0), gRenderer(nullptr), eController(nullptr)
		{}

		Dot (int posX, int posY, SDL_Renderer* gRenderer, EventController* eController) : posX(posX), posY(posY), gRenderer(gRenderer), eController(eController), velX(0), velY(0)
		{}

		//Shows the dot on the screen
		void render();

		//Moves the dot around
		void move();

	private:

		//The position of the dot
		int posX, posY;

		//The velocity of the dot
		int velX, velY;

		SDL_Renderer* gRenderer;

		EventController* eController;
};