#pragma once

#define _USE_MATH_DEFINES
#include <cmath>

typedef struct SOARConfiguration
{
	const int SCREEN_WIDTH = 1024;

	const int GAME_WIDTH = 512;

	const int SCREEN_HEIGHT = 512;

	const int DOF = 3; //8;//depth of field

	const int FOV = 60; //field of view in degrees

	const int VERTICAL_DRAW_WIDTH = 8; //width of the vertical draw in pixels

	//precomputed constants follow

	const float FOV_RADIANS = (float) FOV * M_PI / 180;

	const int RAYS_TO_CAST = GAME_WIDTH / VERTICAL_DRAW_WIDTH;

	const float M_PI_HALF = M_PI / 2;

	const float M_PI_X_2 = 2 * M_PI;

	const float M_PI_X_3_HALF = 3 * M_PI / 2;

} SOARCFG;

extern SOARCFG soarCfg;
