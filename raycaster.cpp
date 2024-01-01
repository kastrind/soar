#pragma once

//Using SDL and standard IO
#include <SDL.h>
#include <stdio.h>
#include <iostream>

#include "Dot.h"

//Screen dimension constants
const int SCREEN_WIDTH = 512;
const int SCREEN_HEIGHT = 512;

//The window we'll be rendering to
SDL_Window* gWindow = NULL;

//The surface contained by the window
SDL_Surface* gScreenSurface = NULL;

//The window renderer
SDL_Renderer* gRenderer = NULL;

int mapX=8, mapY=8, mapS=64;
int map[] =
{
1, 1, 1, 1, 1, 1, 1, 1,
1, 0, 0, 0, 0, 0, 0, 1,
1, 0, 1, 0, 1, 0, 1, 1,
1, 0, 0, 0, 0, 0, 0, 1,
1, 0, 0, 1, 1, 1, 0, 1,
1, 0, 0, 0, 0, 0, 0, 1,
1, 0, 0, 0, 1, 0, 0, 1,
1, 1, 1, 1, 1, 1, 1, 1
};

void drawMap2D(SDL_Renderer* gRenderer)
{
	int x, y;
	for (y=0; y<mapY; y++)
	{
		for (x=0; x<mapX; x++)
		{
			if (map[x + y*mapY]==0) continue;
			SDL_Rect fillRect = { x*mapS+1, y*mapS+1, mapS-1, mapS-1 };
			SDL_SetRenderDrawColor( gRenderer, 0x33, 0x33, 0x33, 0xFF );
			SDL_RenderFillRect( gRenderer, &fillRect );
		}
	}
}

bool init();

bool loadMedia();

void close();

int main( int argc, char* args[] )
{
	//Start up SDL and create window
	if (!init())
	{
		printf("Failed to initialize!\n");
	}
	else
	{
		//Load media
		if (!loadMedia())
		{
			printf("Failed to load media!\n");
		}
		else
		{
			//Fill the background gray
			SDL_Rect fillBackgroundRect = { 0, 0, SCREEN_WIDTH-1, SCREEN_HEIGHT-1 };
			SDL_SetRenderDrawColor( gRenderer, 0x7F, 0x7F, 0x7F, 0xFF );
			SDL_RenderFillRect( gRenderer, &fillBackgroundRect );

			EventController eventController;
			Dot dot(10, 10, gRenderer, &eventController);

			//Main loop flag
			bool quit = false;
			//Event handler
			SDL_Event e;
			//Handle events on queue
			while( quit == false )
			{

				//User requests quit
				while( SDL_PollEvent( &e ) )
				{
					//User requests quit
					if( e.type == SDL_QUIT )
					{
						quit = true;
					}
					//User presses or releases a key
					else if( e.type == SDL_KEYDOWN || e.type == SDL_KEYUP )
					{
						eventController.processEvent(&e);
					}

					//Move the dot
					dot.move();

					//Clear screen
					SDL_SetRenderDrawColor( gRenderer, 0x7F, 0x7F, 0x7F, 0xFF );
					SDL_RenderClear( gRenderer );
					
					drawMap2D(gRenderer);

					//Render objects
					dot.render();

					//Update screen
					SDL_RenderPresent( gRenderer );
				}
			}
		}
	}

	//Free resources and close SDL
	close();

	return 0;
}

bool init ()
{
	bool success = true;
	//Initialize SDL
	if( SDL_Init( SDL_INIT_VIDEO ) < 0 )
	{
		printf( "SDL could not initialize! SDL_Error: %s\n", SDL_GetError() );
		success = false;
	}
	else
	{
		//Create window
		gWindow = SDL_CreateWindow( "First Raycaster Attempt", SDL_WINDOWPOS_UNDEFINED, SDL_WINDOWPOS_UNDEFINED, SCREEN_WIDTH, SCREEN_HEIGHT, SDL_WINDOW_SHOWN );
		if( gWindow == NULL )
		{
			printf( "Window could not be created! SDL_Error: %s\n", SDL_GetError() );
			success = false;
		}
		else
		{
			//Create renderer for window
			gRenderer = SDL_CreateRenderer( gWindow, -1, SDL_RENDERER_ACCELERATED | SDL_RENDERER_PRESENTVSYNC );
			//Get window surface
			gScreenSurface = SDL_GetWindowSurface( gWindow );
		}
	}
	return success;
}

bool loadMedia()
{
	bool success = true;
	return success;
}

void close()
{
	//Destroy window
	SDL_DestroyWindow( gWindow );
	gWindow = NULL;
	//Quit SDL subsystems
	SDL_Quit();
}
