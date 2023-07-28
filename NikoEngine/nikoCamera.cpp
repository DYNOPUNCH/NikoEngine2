#include "nikoCamera.h"


void NIKO_Camera::nikoCamera(int levelwidth, int levelheight, int objx, int objy, int objw, int objh)
{
	camerax = (objx + objw / 2) - (480 / 2);

	cameray = (objy + objh / 2) - (270 / 2);


	if (camerax < 0)
		camerax = 0;
	if (cameray < 0)
		cameray = 0;

	
	if (camerax > levelwidth*16 - 480)
		camerax = levelwidth*16 - 480;
	if (cameray > levelheight*16 - 270)
		cameray = levelheight*16 - 270;
	
	if (levelwidth*16 < 480)
		camerax = -(480 / 2 - levelwidth*16 / 2);
	if (levelheight*16 < 270)
		cameray = -(270 / 2 - levelheight*16 / 2);
	
}

void NIKO_Camera::nikoViewport(SDL_Renderer* renderer, int x, int y, int width, int height, float scalex, float scaley)
{
	SDL_Rect viewport;

	viewport.x = x;
	viewport.y = y;
	viewport.w = width;
	viewport.h = height;

	SDL_RenderSetViewport(renderer, &viewport);
	SDL_RenderSetScale(renderer, scalex, scaley);
}
