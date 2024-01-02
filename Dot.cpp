#include "Dot.h"

void Dot::render()
{
	//Render red filled quad
	SDL_Rect fillRect = { posX, posY, DOT_WIDTH, DOT_HEIGHT };
	SDL_SetRenderDrawColor( gRenderer, 0x00, 0x7F, 0x00, 0xFF );
	SDL_RenderFillRect( gRenderer, &fillRect );
	SDL_SetRenderDrawColor( gRenderer, 0xFF, 0x00, 0x00, 0xFF );
	SDL_RenderDrawLine( gRenderer, posX + DOT_WIDTH/2, posY + DOT_HEIGHT/2, posX  + DOT_WIDTH/2 + pdX*5, posY + DOT_HEIGHT/2 + pdY*5);
}

void Dot::move()
{
	if (eController == nullptr) return;

	velX = 0;
	velY = 0;

	bool* keysPressed = eController->getKeysPressed();

	// //Diagonal movement speed correction
	// if (velX != 0 && velY != 0) {
	// 	velX = velX > 0 ? diagonalVelocity : -diagonalVelocity;
	// 	velY = velY > 0 ? diagonalVelocity : -diagonalVelocity;
	// }

	if (keysPressed[SupportedKeys::A]) {
		angle -= 0.1;
		if (angle < 0)
		{
			angle += 2 * M_PI;
		}
		pdX = cos(angle) * 3;
		pdY = sin(angle) * 3;
	}
	else if (keysPressed[SupportedKeys::D]) {
		angle += 0.1;
		if (angle > 2 * M_PI)
		{
			angle -= 2 * M_PI;
		}
		pdX = cos(angle) * 3;
		pdY = sin(angle) * 3;
	}

	if (keysPressed[SupportedKeys::LEFT_ARROW]) {
		float leftAngle = angle - M_PI/2;
		if (leftAngle < 0)
		{
			leftAngle += 2 * M_PI;
		}
		float pdX = cos(leftAngle) * 3;
		float pdY = sin(leftAngle) * 3;
		velX = pdX;
		velY = pdY;
	}
	else if (keysPressed[SupportedKeys::RIGHT_ARROW]) {
		float rightAngle = angle + M_PI/2;
		if (rightAngle > 2 * M_PI)
		{
			rightAngle -= 2 * M_PI;
		}
		float pdX = cos(rightAngle) * 3;
		float pdY = sin(rightAngle) * 3;
		velX = pdX;
		velY = pdY;
	}

	if (keysPressed[SupportedKeys::UP_ARROW]) {
		//velY = -DOT_VEL;
		velX = pdX;
		velY = pdY;
	}
	else if (keysPressed[SupportedKeys::DOWN_ARROW]) {
		//velY = DOT_VEL;
		velX = -pdX;
		velY = -pdY;
	}

	posX += velX;
	posY += velY;

}