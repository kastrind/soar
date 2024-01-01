#include "Dot.h"

void Dot::render()
{
	//Render red filled quad
	SDL_Rect fillRect = { posX, posY, DOT_WIDTH, DOT_HEIGHT };
	SDL_SetRenderDrawColor( gRenderer, 0x00, 0x7F, 0x00, 0xFF );
	SDL_RenderFillRect( gRenderer, &fillRect );
}

void Dot::move()
{
	if (eController == nullptr) return;

	velX = 0;
	velY = 0;

	bool* keysPressed = eController->getKeysPressed();
	if (keysPressed[SupportedKeys::LEFT_ARROW]) {
		velX = -DOT_VEL;
	}
	if (keysPressed[SupportedKeys::RIGHT_ARROW]) {
		velX = DOT_VEL;
	}

	if (keysPressed[SupportedKeys::UP_ARROW]) {
		velY = -DOT_VEL;
	}
	if (keysPressed[SupportedKeys::DOWN_ARROW]) {
		velY = DOT_VEL;
	}

	//Diagonal movement speed correction
	if (velX != 0 && velY != 0) {
		velX = velX > 0 ? diagonalVelocity : -diagonalVelocity;
		velY = velY > 0 ? diagonalVelocity : -diagonalVelocity;
	}

	posY += velY;
	posX += velX;
}