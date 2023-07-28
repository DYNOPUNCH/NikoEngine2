#ifndef NIKO_CAMERA_H
#define NIKO_CAMERA_H

#include "nikoSystem.h"


struct NIKO_Camera
{
public:

	float camerax;
	float cameray;

	void nikoCamera(int levelwidth, int levelheight, int objx, int objy, int objw, int objh);
	void nikoViewport(SDL_Renderer* renderer, int x, int y, int width, int height, float scalex, float scaley);
};

#endif