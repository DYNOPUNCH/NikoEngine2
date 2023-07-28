#ifndef NIKO_GAMEPAD_H
#define NIKO_GAMEPAD_H

#include <SDL.h>
#include <string>

struct NIKO_Button
{
private:

	bool flagger;

	SDL_Joystick* gGameController;

public:

	const Uint8* state = SDL_GetKeyboardState(NULL);

	bool singlepress(const Uint8 keystates);
	bool singleclick(std::string clickID);
	int  eightway_input(bool gamepadmode);

	NIKO_Button()
	{
		flagger = false;
		gGameController = NULL;
	}

	~NIKO_Button()
	{
		SDL_JoystickClose(gGameController);
		gGameController = NULL;
	}
};

#endif
