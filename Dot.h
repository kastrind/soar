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
		static const int DOT_WIDTH = 10;
		static const int DOT_HEIGHT = 10;

		static const int RAYS_TO_CAST = 1;

		//Initializes the variables
		Dot () : posX(0), posY(0), velX(0), velY(0), pdX(3), pdY(0), angle(0), rayAngle(0), rayX(0), rayY(0), gRenderer(nullptr), eController(nullptr)
		{}

		Dot (int posX, int posY, SDL_Renderer* gRenderer, EventController* eController) : posX(posX), posY(posY), gRenderer(gRenderer), eController(eController), velX(0), velY(0), pdX(3), pdY(0), angle(0), rayAngle(0), rayX(0), rayY(0)
		{}

		//Shows the dot on the screen
		void render();

		//Moves the dot around
		void move(int* map);

		//Performs raycasting
		void castRay(int* map);

	private:

		//The position of the dot
		int posX, posY;

		//The velocity of the dot
		int velX, velY;

		//The rotation of the dot
		float angle;

		//The X and Y components of the trigonometry circle for the rotation
		float pdX, pdY;

		//The angle of the raycasting
		float rayAngle;

		float rayX, rayY;

		SDL_Renderer* gRenderer;

		EventController* eController;
};