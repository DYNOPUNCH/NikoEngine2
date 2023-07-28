#ifndef NIKO_SSU_H
#define NIKO_SSU_H

#include <vector>
#include <string>
#include <cstdint>
#include <SDL_image.h>
#include <SDL.h>
#include "nikoSystem.h"
#include <list>
#include <iostream>

#include "include/pugi/pugiconfig.hpp"
#include "include/pugi/pugixml.hpp"

class NIKO_Texture
{
public: 

	std::vector<SDL_Texture*> texBank;
	std::vector<std::string> TIDC;

	SDL_Texture* nikoTexLibrarian(std::string texID);

	bool nikoColorLibrarian(std::string texID, int red, int green, int blue);

	bool nikoAlphaLibrarian(std::string texID, int alpha);

	bool nikoTextureCheck(std::string texID);

	SDL_Texture* nikoLoadTexture(const char* filename);

	bool nikoTextureSwap(std::string texID, const char* filename);

	bool nikoTexLibraryAdd(std::string texID, const char* filename);

	void nikoFreeTextures();

	void nikoRenderTexture(std::string texID, int cutwidth, int cutheight, int cutx, int cuty, int destx, int desty, int strechw, int strechh, SDL_RendererFlip flipflags);

	void nikoRenderTextureDirect(int texpointer, int cutwidth, int cutheight, int cutx, int cuty, int destx, int desty, int strechw, int strechh, SDL_RendererFlip flipflags);

	void nikoRenderTextureData(SDL_Texture* texture, int cutwidth, int cutheight, int cutx, int cuty, int destx, int desty, int strechw, int strechh, SDL_RendererFlip flipflags);

};

class NIKO_Sprite
{
private:

	struct spriteData
	{
		std::string name;
		unsigned int nof;

		std::vector<std::string> texindex;

		std::vector<unsigned int> boxwidth;
		std::vector<unsigned int> boxheight;

		std::vector<int> mirrorx;
		std::vector<int> mirrory;

		std::vector<unsigned int> xcut;
		std::vector<unsigned int> ycut;

		std::string changepoll;
	};

	struct aniData
	{
		unsigned int framenum;
		unsigned int delayresp;
	};

	std::vector<spriteData> spritedata;

	bool loadpoll;

	unsigned int currentid;

	aniData packer;

public:

	unsigned int numberOfAnimations;
	unsigned int numberOfSprites;

	std::vector<aniData> anidata;

	NIKO_Texture textures;

	bool nikoLoadSprites(std::string xmlfile);

	void nikoAddSprite(unsigned int numToAdd);

	bool nikoRenderSprite(std::string name, int spriteNum, int delay, int destx, int desty, bool flipx, bool flipy, bool loop, int alpha);

	unsigned int nikoGetCurrentPos(int spriteid);

	void nikoFreeSprites();

};

class NIKO_SpriteRenderer
{
private:

	struct spriteData
	{
		std::string name;
		unsigned int nof;

		std::vector<std::string> texindex;

		std::vector<unsigned int> boxwidth;
		std::vector<unsigned int> boxheight;

		std::vector<int> mirrorx;
		std::vector<int> mirrory;

		std::vector<unsigned int> xcut;
		std::vector<unsigned int> ycut;

		std::string changepoll;
	};

	struct aniData
	{
		unsigned int framenum;
		unsigned int delayresp;
	};

	std::vector<spriteData> spritedata;

	bool loadpoll;

	unsigned int currentid;

	aniData packer;

public:

	unsigned int numberOfAnimations;
	unsigned int numberOfSprites;

	std::vector<aniData> anidata;

	NIKO_Texture textures;

	bool nikoLoadSprites(std::string file);

};

//Add sprite / animation class 

#endif