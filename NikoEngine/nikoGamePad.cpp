#include "nikoGamePad.h"

bool NIKO_Button::singlepress(const Uint8 keystates)
{
	if (keystates && flagger == false)
	{
		flagger = true;
		return true;
	}

	if (!keystates)
		flagger = false;

	return false;
}

bool NIKO_Button::singleclick(std::string clickID)
{
	static bool flagger = true;

	static bool clicker;

	//if (clickID == "11x00")
	//	clicker = sys.leftmousebutton;

	//if (clickID == "00x11")
	//	clicker = sys.rightmousebutton;

	if (clicker && flagger == true)
	{
		flagger = false;
		return true;
	}

	if (!clicker)
		flagger = true;

	return false;
}

int  NIKO_Button::eightway_input(bool gamepadmode)
{
	static const Uint8* keystate = SDL_GetKeyboardState(NULL);

	//PS4 controller mapping

	// D-PAD MAPPING
	// UP = 1
	// RIGHT = 2
	// DOWN = 4
	// LEFT = 8
	// UP-RIGHT = 3
	// DOWN-RIGHT = 6
	// DOWN-LEFT = 12
	// UP-LEFT = 9

	//BUTTON MAPPING
	// CROSS = 2
	// SQUARE = 1
	// CIRCLE = 3
	// TRIANGLE = 4
	// START = 10
	// SHARE = 9
	// R1 = 6
	// R2 = 8
	// R3 12
	// L1 = 5
	// L2 = 7
	// L3 11
	// HOME 13
	// TOUCH PAD 14

	if (!gamepadmode)
	{

		if (keystate[SDL_SCANCODE_UP] && !keystate[SDL_SCANCODE_LEFT] && !keystate[SDL_SCANCODE_RIGHT] && !keystate[SDL_SCANCODE_DOWN])
			return 1;
		if (keystate[SDL_SCANCODE_UP] && keystate[SDL_SCANCODE_RIGHT] && !keystate[SDL_SCANCODE_LEFT] && !keystate[SDL_SCANCODE_DOWN])
			return 2;
		if (keystate[SDL_SCANCODE_RIGHT] && !keystate[SDL_SCANCODE_LEFT] && !keystate[SDL_SCANCODE_UP] && !keystate[SDL_SCANCODE_DOWN])
			return 3;
		if (keystate[SDL_SCANCODE_DOWN] && keystate[SDL_SCANCODE_RIGHT] && !keystate[SDL_SCANCODE_UP] && !keystate[SDL_SCANCODE_LEFT])
			return 4;
		if (keystate[SDL_SCANCODE_DOWN] && !keystate[SDL_SCANCODE_LEFT] && !keystate[SDL_SCANCODE_RIGHT] && !keystate[SDL_SCANCODE_UP])
			return 5;
		if (keystate[SDL_SCANCODE_DOWN] && keystate[SDL_SCANCODE_LEFT] && !keystate[SDL_SCANCODE_UP] && !keystate[SDL_SCANCODE_RIGHT])
			return 6;
		if (keystate[SDL_SCANCODE_LEFT] && !keystate[SDL_SCANCODE_UP] && !keystate[SDL_SCANCODE_RIGHT] && !keystate[SDL_SCANCODE_DOWN])
			return 7;
		if (keystate[SDL_SCANCODE_UP] && keystate[SDL_SCANCODE_LEFT] && !keystate[SDL_SCANCODE_DOWN] && !keystate[SDL_SCANCODE_RIGHT])
			return 8;

	}
	else
	{
		if (SDL_NumJoysticks() == 1)
			gGameController = SDL_JoystickOpen(0);

		//controller buttons
		SDL_JoystickNumButtons(gGameController);

		//D-Pad
		switch (SDL_JoystickGetHat(gGameController, 0))
		{
		case 1:
			return 1;
			break;
		case 3:
			return 2;
			break;
		case 2:
			return 3;
			break;
		case 6:
			return 4;
			break;
		case 4:
			return 5;
			break;
		case 12:
			return 6;
			break;
		case 8:
			return 7;
			break;
		case 9:
			return 8;
			break;
		}

		//SDL_GameControllerUpdate();

	}

	return 0;
}