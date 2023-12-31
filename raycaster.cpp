#pragma once

//Using SDL and standard IO
#include <SDL.h>
#include <stdio.h>
#include <iostream>

#include "Dot.h"

//Screen dimension constants
const int SCREEN_WIDTH = 640;
const int SCREEN_HEIGHT = 480;

//The window we'll be rendering to
SDL_Window* gWindow = NULL;

//The surface contained by the window
SDL_Surface* gScreenSurface = NULL;

//The window renderer
SDL_Renderer* gRenderer = NULL;

typedef struct {
	float posX;
	float posY;
} Player;

Player player;

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
			//Fill the surface gray
			//SDL_FillRect( gScreenSurface, NULL, SDL_MapRGB( gScreenSurface->format, 0x7F, 0x7F, 0x7F ) );

			SDL_Rect fillBackgroundRect = { 0, 0, SCREEN_WIDTH-1, SCREEN_HEIGHT-1 };
			SDL_SetRenderDrawColor( gRenderer, 0x7F, 0x7F, 0x7F, 0xFF );
			SDL_RenderFillRect( gRenderer, &fillBackgroundRect );
			
			Dot dot(10, 10, gRenderer);
			dot.render();

			//Update screen
			SDL_RenderPresent( gRenderer );

			//Hack to get window to stay up
			SDL_Event e; bool quit = false; while( quit == false ){ while( SDL_PollEvent( &e ) ){ if( e.type == SDL_QUIT ) quit = true; } }
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
