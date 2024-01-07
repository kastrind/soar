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

void Dot::move(int* map)
{
	if (eController == nullptr) return;

	velX = 0;
	velY = 0;

	bool* keysPressed = eController->getKeysPressed();

	if (keysPressed[SupportedKeys::A]) {
		angle -= 0.1;
		if (angle < 0)
		{
			angle += soarCfg.M_PI_X_2;
		}
		pdX = cos(angle) * 3;
		pdY = sin(angle) * 3;
	}
	else if (keysPressed[SupportedKeys::D]) {
		angle += 0.1;
		if (angle > soarCfg.M_PI_X_2)
		{
			angle -= soarCfg.M_PI_X_2;
		}
		pdX = cos(angle) * 3;
		pdY = sin(angle) * 3;
	}

	if (keysPressed[SupportedKeys::LEFT_ARROW]) {
		float leftAngle = angle - soarCfg.M_PI_HALF;
		if (leftAngle < 0)
		{
			leftAngle += soarCfg.M_PI_X_2;
		}
		float pdX = cos(leftAngle) * 3;
		float pdY = sin(leftAngle) * 3;
		velX = pdX;
		velY = pdY;
	}
	else if (keysPressed[SupportedKeys::RIGHT_ARROW]) {
		float rightAngle = angle + soarCfg.M_PI_HALF;
		if (rightAngle > soarCfg.M_PI_X_2)
		{
			rightAngle -= soarCfg.M_PI_X_2;
		}
		float pdX = cos(rightAngle) * 3;
		float pdY = sin(rightAngle) * 3;
		velX = pdX;
		velY = pdY;
	}

	if (keysPressed[SupportedKeys::UP_ARROW]) {
		velX = pdX;
		velY = pdY;
	}
	else if (keysPressed[SupportedKeys::DOWN_ARROW]) {
		velX = -pdX;
		velY = -pdY;
	}

	// check for collisions of the dot corners with the map walls
	int mapIndex =  ((int)(posY + velY) / 64) * 8 + ((int)(posX + velX)/64);
	int mapIndex2 = ((int)(posY + velY + DOT_HEIGHT) / 64) * 8 + ((int)(posX + velX)/64);
	int mapIndex3 = ((int)(posY + velY + DOT_HEIGHT) / 64) * 8 + ((int)(posX + velX + DOT_WIDTH)/64);
	int mapIndex4 = ((int)(posY + velY) / 64) * 8 + ((int)(posX + velX + DOT_WIDTH)/64);
	if(map[mapIndex] == 0 && map[mapIndex2] == 0 && map[mapIndex3] == 0 && map[mapIndex4] == 0) {
		posX += velX;
		posY += velY;
	}
}

float dist(float ax, float ay, float bx, float by) { return sqrt(pow(bx-ax, 2) + pow(by-ay, 2)); }

double Det(double a, double b, double c, double d)
{
	return a*d - b*c;
}

bool findIntersection(float x1, float y1, float x2, float y2, float x3, float y3, float x4, float y4, float& intersectX, float& intersectY)
{
	double detL1 = Det(x1, y1, x2, y2);
	double detL2 = Det(x3, y3, x4, y4);
	double x1mx2 = x1 - x2;
	double x3mx4 = x3 - x4;
	double y1my2 = y1 - y2;
	double y3my4 = y3 - y4;
	double denom = Det(x1mx2, y1my2, x3mx4, y3my4);
	if (denom != 0)
	{
		double xnom = Det(detL1, x1mx2, detL2, x3mx4);
		double ynom = Det(detL1, y1my2, detL2, y3my4);
		double x = xnom / denom;	
		double y = ynom / denom;
		if (x <= x2 && x >= x1 && y <= y2 && y >= y1)
		{
			intersectX = x;
			intersectY = y;
			return true;
		}
		return false;
	}
	return false;
}

void Dot::castRay(int* map)
{
	int currRay;
	int dof=1;
	int maxDof = soarCfg.DOF;
	int mapIndex;
	int raysToCast = soarCfg.RAYS_TO_CAST;
	float fovRadians = soarCfg.FOV_RADIANS;//(float)SOARConfiguration.FOV * M_PI / 180;
	float distance;
	for (currRay = 0; currRay < raysToCast; currRay++)
	{
		rayAngle = angle - fovRadians/2;
		if (rayAngle < 0) { rayAngle += soarCfg.M_PI_X_2; }
		rayAngle += (float)currRay*(fovRadians/raysToCast);
		if (rayAngle >= soarCfg.M_PI_X_2) { rayAngle -= soarCfg.M_PI_X_2; }
		//if (rayAngle < 0) { rayAngle = soarCfg.M_PI_X_2 - (float)(raysToCast)*(fovRadians/raysToCast) + (float)(currRay)*(fovRadians/raysToCast);}
		//std::cout << "angle: " << angle << ", currRay: " << currRay << ", rayAngle: " << rayAngle << std::endl;
		//if (currRay >20 )continue;//TODO REMOVE!
		dof=1;
		rayX = 0;
		rayY = 0;
		while (dof <= maxDof)
		{
			//std::cout << "dof: " << dof << std::endl;
			int maxD = 64;
			int d = 8;
			while(d <= maxD) {
				//std::cout << "d: " << d << std::endl;
				rayX = cos(rayAngle) * d * dof;
				rayY = sin(rayAngle) * d * dof;
				mapIndex = ((int)(posY + rayY) / 64) * 8 + ((int)(posX + rayX)/64);

				int wallY = (mapIndex/8)*64;
				int wallX = (mapIndex%8)*64;

				if(map[mapIndex] == 1) {
					while(map[mapIndex]==1) {
						rayX = cos(rayAngle) * (--d) * dof;
						rayY = sin(rayAngle) * d * dof;
						mapIndex = ((int)(posY + rayY) / 64) * 8 + ((int)(posX + rayX)/64);

						wallY = (mapIndex/8)*64;
						wallX = (mapIndex%8)*64;
					}
					rayX = cos(rayAngle) * (++d) * dof;
					rayY = sin(rayAngle) * d * dof;
					mapIndex = ((int)(posY + rayY) / 64) * 8 + ((int)(posX + rayX)/64);

					wallY = (mapIndex/8)*64;
					wallX = (mapIndex%8)*64;


					bool doIntersectSideAB=false, doIntersectSideBC=false, doIntersectSideCD=false, doIntersectSideDA=false;
					float distanceAB=10000, distanceBC=10000, distanceCD=10000, distanceDA=10000;
					float rayXAB, rayYAB, rayXBC, rayYBC, rayXCD, rayYCD, rayXDA, rayYDA;

					if (rayAngle >= 0 && rayAngle < soarCfg.M_PI_HALF)
					{
						doIntersectSideAB = findIntersection(wallX, wallY, wallX+64, wallY, posX, posY, rayX+posX, rayY+posY, rayXAB, rayYAB);
						doIntersectSideDA = findIntersection(wallX, wallY, wallX, wallY+64, posX, posY, rayX+posX, rayY+posY, rayXDA, rayYDA);

						if (doIntersectSideAB) { distanceAB =  dist(posX, posY, rayXAB, rayYAB); }
						if (doIntersectSideDA) { distanceDA =  dist(posX, posY, rayXDA, rayYDA); }
						
						//std::cout << "DISTANCE AB = " << distanceAB << std::endl;
						//std::cout << "DISTANCE DA = " << distanceDA << std::endl;

						if (distanceAB < distanceDA)
						{
							rayX = rayXAB - posX;
							rayY = rayYAB - posY;
							distance = distanceAB;
							//std::cout << "INTERSECTION AT AB!!" << std::endl;
						}
						else if (distanceDA < distanceAB)
						{
							rayX = rayXDA - posX;
							rayY = rayYDA - posY;
							distance = distanceDA;
							//std::cout << "INTERSECTION AT DA!!" << distance <<  std::endl;
						}
					}
					else if (rayAngle < M_PI)
					{
						doIntersectSideAB = findIntersection(wallX, wallY, wallX+64, wallY, posX, posY, rayX+posX, rayY+posY, rayXAB, rayYAB);
						doIntersectSideBC = findIntersection(wallX+64, wallY, wallX+64, wallY+64, posX, posY, rayX+posX, rayY+posY, rayXBC, rayYBC);

						if (doIntersectSideAB) { distanceAB =  dist(posX, posY, rayXAB, rayYAB); }
						if (doIntersectSideBC) { distanceBC =  dist(posX, posY, rayXBC, rayYBC); }
						
						//std::cout << "DISTANCE AB = " << distanceAB << std::endl;
						//std::cout << "DISTANCE BC = " << distanceBC << std::endl;

						if (distanceAB < distanceBC)
						{
							rayX = rayXAB - posX;
							rayY = rayYAB - posY;
							distance = distanceAB;
							//std::cout << "INTERSECTION AT AB!!" << distance <<  std::endl;
						}
						else if (distanceBC < distanceAB)
						{
							rayX = rayXBC - posX;
							rayY = rayYBC - posY;
							distance = distanceBC;
							//std::cout << "INTERSECTION AT BC!!" << distance <<  std::endl;
						}
					}
					else if (rayAngle < soarCfg.M_PI_X_3_HALF)
					{
						doIntersectSideBC = findIntersection(wallX+64, wallY, wallX+64, wallY+64, posX, posY, rayX+posX, rayY+posY, rayXBC, rayYBC);
						doIntersectSideCD = findIntersection(wallX, wallY+64, wallX+64, wallY+64, posX, posY, rayX+posX, rayY+posY, rayXCD, rayYCD);

						if (doIntersectSideBC) { distanceBC =  dist(posX, posY, rayXBC, rayYBC); }
						if (doIntersectSideCD) { distanceCD =  dist(posX, posY, rayXCD, rayYCD); }
						
						//std::cout << "DISTANCE BC = " << distanceBC << std::endl;
						//std::cout << "DISTANCE CD = " << distanceCD << std::endl;

						if (distanceBC < distanceCD)
						{
							rayX = rayXBC - posX;
							rayY = rayYBC - posY;
							distance = distanceBC;
							std::cout << "INTERSECTION AT BC!!" << distance <<  std::endl;
						}
						else if (distanceCD < distanceBC)
						{
							rayX = rayXCD - posX;
							rayY = rayYCD - posY;
							distance = distanceCD;
							std::cout << "INTERSECTION AT CD!!" << distance <<  std::endl;
						}
					}
					else // rayAngle < 2*M_PI (or soarCfg.M_PI_X_2)
					{
						doIntersectSideCD = findIntersection(wallX, wallY+64, wallX+64, wallY+64, posX, posY, rayX+posX, rayY+posY, rayXCD, rayYCD);
						doIntersectSideDA = findIntersection(wallX, wallY, wallX, wallY+64, posX, posY, rayX+posX, rayY+posY, rayXDA, rayYDA);

						if (doIntersectSideCD) { distanceCD =  dist(posX, posY, rayXCD, rayYCD); }
						if (doIntersectSideDA) { distanceDA =  dist(posX, posY, rayXDA, rayYDA); }
						
						// std::cout << "DISTANCE CD = " << distanceCD << std::endl;
						// std::cout << "DISTANCE DA = " << distanceDA << std::endl;

						if (distanceCD < distanceDA)
						{
							rayX = rayXCD - posX;
							rayY = rayYCD - posY;
							distance = distanceCD;
							std::cout << "INTERSECTION AT CD!!" << distance << std::endl;
						}
						else if (distanceDA < distanceCD)
						{
							rayX = rayXDA - posX;
							rayY = rayYDA - posY;
							distance = distanceDA;
							std::cout << "INTERSECTION AT DA!!" << distance << std::endl;
						}
					}

					if (doIntersectSideAB || doIntersectSideBC || doIntersectSideCD || doIntersectSideDA) {
						//std::cout << "INTERSECTION HERE !!!! rayX " << rayX << "rayY" << rayY << std::endl;
						d = maxD;
						dof = maxDof;
					}

					float ca = angle - rayAngle;
					if (ca < 0) { ca += soarCfg.M_PI_X_2; }
					if (ca >= soarCfg.M_PI_X_2) { ca -= soarCfg.M_PI_X_2; }
					if (distance == 0.0f) { distance = 0.01f; }
					distance = distance * cos(ca); //fix fish-eye effect
					int lineH = (64*soarCfg.SCREEN_HEIGHT)/distance; if (lineH>soarCfg.SCREEN_HEIGHT) { lineH=soarCfg.SCREEN_HEIGHT; } //line height
					int lineOff = soarCfg.SCREEN_HEIGHT/2 - (lineH>>1); //line offset

					int itersV = (int)round(lineH/8); //vertical iterations to draw the fps perspective
					for (int i=0; i<itersV; i++)
					{
						SDL_Rect fillRect = { soarCfg.SCREEN_WIDTH - soarCfg.GAME_WIDTH + currRay*soarCfg.VERTICAL_DRAW_WIDTH, lineOff + (i-1)*8, soarCfg.VERTICAL_DRAW_WIDTH, 8 };
						float luminance = std::max( 0.3f, (1.0f - (float)abs(soarCfg.SCREEN_HEIGHT/16 - i)/(1.5f*soarCfg.SCREEN_HEIGHT/16))*(1.0f - distance/(1.5f*soarCfg.DOF*64))*(1.0f - (float)abs(raysToCast/2 - currRay)/(1.5f*raysToCast/2)) );
						SDL_SetRenderDrawColor( gRenderer, luminance*64, luminance*255, luminance*64, 255 );
						SDL_RenderFillRect( gRenderer, &fillRect );
					}

				}
				d += 8;
			}
			dof++;
		}
		//draw ray
		SDL_SetRenderDrawColor( gRenderer, 0xFF, 0xFF, 0x00, 0xFF );
		SDL_RenderDrawLine( gRenderer, posX + DOT_WIDTH/2, posY + DOT_HEIGHT/2, posX  + DOT_WIDTH/2 + rayX, posY + DOT_HEIGHT/2 + rayY);


	}
	//Render the dot
	render();
}
