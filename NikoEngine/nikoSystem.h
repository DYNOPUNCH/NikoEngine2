#ifndef NIKO_SYSTEM_H
#define NIKO_SYSTEM_H

#include <cstdint>
#include <SDL.h>
#include <fstream>
#include <cstdio>
#include <SDL_mixer.h>

const int FPS = 60;

struct NIKO_System
{

	bool systempause;

	int mousex;
	int mousey;
	int pointwidth;
	int pointheight;

	bool processing;
	bool leftmousebutton;
	bool rightmousebutton;
	bool characterdrawn;

	bool fullscreen;
	bool worldon;
	bool lockcontrols;

	int nativeresw;
	int nativeresh;

	int randomnum;

	SDL_Event event;
	Uint32 initial_time;

	//initialize system variables.
	void NIKO_INIT_SYSTEM();
	
	//function for everything that should be updated every frame on a system level.
	void nikoUpdate();

	//functions for saving and loading game data. 
	int niKoSaveGame(const char* fn, struct savedata* src);
	int nikoLoadSave(const char* fn, struct savedata* dst);
	void NIKO_inputUpdate();

	~NIKO_System();

};

#endif
