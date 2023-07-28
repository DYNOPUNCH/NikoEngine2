#include "nikoSystem.h"

extern SDL_Window* window;
extern SDL_Renderer* renderer;
extern SDL_Event window_key;

NIKO_System sys;

void NIKO_System::NIKO_INIT_SYSTEM()
{
	processing = true;
}

void NIKO_System::nikoUpdate()
{
	SDL_SetRenderDrawColor(renderer, 26, 26, 26, 255);
	SDL_RenderPresent(renderer);
	SDL_RenderClear(renderer);

	initial_time = SDL_GetTicks();

	//limit game frame rate
	if (1000 / FPS > SDL_GetTicks() - initial_time)
		SDL_Delay(1000 / FPS - (SDL_GetTicks() - initial_time));
}

void NIKO_System::NIKO_inputUpdate()
{
	//pump window events, change window color, flip the screen buffer, and clear the screen.
	SDL_PumpEvents();

	//poll for window exit event
	while (SDL_PollEvent(&event) != 0)
		if (event.type == SDL_QUIT)
			processing = false;
}

NIKO_System::~NIKO_System()
{
	Mix_Quit();
	SDL_DestroyRenderer(renderer);
	SDL_DestroyWindow(window);
	SDL_Quit();
}

int NIKO_System::niKoSaveGame(const char* filename, struct savedata* src)
{
	FILE* fp = NULL;

	if (!fopen_s(&fp, filename, "wb"))
	{
		fwrite(src, 1, sizeof(src), fp);
		fclose(fp);
		printf("[ %s ] game data loaded successfully\n", filename);
		return 1;
	}

	// return -1 if file could not open
	printf("[ %s ] encountered a problem opening game data\n", filename);
	return -1;
}

int NIKO_System::nikoLoadSave(const char* filename, struct savedata* dst)
{
	FILE* fp = NULL;

	if (!fopen_s(&fp, filename, "rb"))
	{
		fread(dst, 1, sizeof(dst), fp);
		fclose(fp);
		printf("[ %s ] game data loaded successfully\n", filename);
		return 1;
	}

	// return -1 if file could not open
	printf("[ %s ] encountered a problem opening game data\n", filename);
	return -1;
}