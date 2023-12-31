#include "Dot.h"

void Dot::render()
{
	//Render red filled quad
	SDL_Rect fillRect = { mPosX, mPosY, DOT_WIDTH, DOT_HEIGHT };
	SDL_SetRenderDrawColor( gRenderer, 0xFF, 0x00, 0x00, 0xFF );
	SDL_RenderFillRect( gRenderer, &fillRect );
}