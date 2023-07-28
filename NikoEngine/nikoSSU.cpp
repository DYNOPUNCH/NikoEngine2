#include "nikoSSU.h"

extern SDL_Window* window;
extern SDL_Renderer* renderer;
extern SDL_Event window_key;

//TEXTURES

SDL_Texture* NIKO_Texture::nikoTexLibrarian(std::string texID)
{

	if (texBank.size() > 0)
	{
		for (int i = 0; i < TIDC.size(); i++)
		{
			if (texID == TIDC[i])
				return texBank[i];
		}
	}
	return nullptr;
}

bool NIKO_Texture::nikoColorLibrarian(std::string texID, int red, int green, int blue)
{
	if (texBank.size() > 0)
	{
		for (int i = 0; i < TIDC.size(); i++)
		{
			if (texID == TIDC[i])
			{
				SDL_SetTextureColorMod(texBank[i], red, green, blue);
				return true;
			}
		}
	}
	return false;
}

bool NIKO_Texture::nikoAlphaLibrarian(std::string texID, int alpha)
{
	if (texBank.size() > 0)
	{
		for (int i = 0; i < TIDC.size(); i++)
		{
			if (texID == TIDC[i])
			{
				SDL_SetTextureAlphaMod(texBank[i], alpha);
				return true;
			}
		}
	}
	return false;
}

bool NIKO_Texture::nikoTextureCheck(std::string texID)
{
	if (texBank.size() > 0)
	{
		for (int i = 0; i < TIDC.size(); i++)
		{
			if (texID == TIDC[i])
			{
				return true;
			}
		}
	}
	return false;
}

SDL_Texture* NIKO_Texture::nikoLoadTexture(const char* filename)
{
	SDL_Surface* surface = NULL;
	SDL_Texture* loadedtexture = NULL;
	surface = IMG_Load(filename);

	if (surface)
	{
		loadedtexture = SDL_CreateTextureFromSurface(renderer, surface);
		SDL_FreeSurface(surface);
		printf("texture loaded successfully: %s\n", filename);
		return loadedtexture;
	}
	else
	{
		SDL_FreeSurface(surface);
		printf("--could not load texture: %s\n", filename);
		return NULL;
	}

	printf("unknown error: %s\n", filename);
	return nullptr;
}

bool NIKO_Texture::nikoTextureSwap(std::string texID, const char* filename)
{
	if (texBank.size() > 0)
	{
		for (int i = 0; i < TIDC.size(); i++)
		{
			if (texID == TIDC[i])
			{
				if (texBank[i])
					SDL_DestroyTexture(texBank[i]);

				texBank[i] = nikoLoadTexture(filename);

				return true;
			}
		}
	}
	return false;
}

bool NIKO_Texture::nikoTexLibraryAdd(std::string texID, const char* filename)
{
	SDL_Texture* loadedtexture = NULL;
	loadedtexture = nikoLoadTexture(filename);

	if (loadedtexture)
	{
		texBank.push_back(loadedtexture);
		TIDC.push_back(texID);
		return true;
	}
	else
		return false;
}

void NIKO_Texture::nikoFreeTextures()
{
	if (texBank.size() > 0)
	{
		for (int i = 0; i < TIDC.size(); i++)
		{
			if (texBank[i])
				SDL_DestroyTexture(texBank[i]);
		}
		texBank.clear();
		TIDC.clear();
	}
}

void NIKO_Texture::nikoRenderTexture(std::string texID, int cutwidth, int cutheight, int cutx, int cuty, int destx, int desty, int strechw, int strechh, SDL_RendererFlip flipflags)
{
	SDL_RendererFlip flip;

	//Renders the sprite using SDL_Render_Copy and makes a local Rect so that it can be used multiple times.
	SDL_Rect source;
	SDL_Rect dest;
	SDL_Point center;

	source.x = cutx;
	source.y = cuty;
	source.w = cutwidth;
	source.h = cutheight;

	dest.x = destx;
	dest.y = desty;
	dest.w = strechw;
	dest.h = strechh;

	center.x = cutwidth / 2;
	center.y = cutheight / 2;

	SDL_RenderCopyEx(renderer, nikoTexLibrarian(texID), &source, &dest, 0, 0, flipflags);
}

void NIKO_Texture::nikoRenderTextureDirect(int texpointer, int cutwidth, int cutheight, int cutx, int cuty, int destx, int desty, int strechw, int strechh, SDL_RendererFlip flipflags)
{
	SDL_RendererFlip flip;

	//Renders the sprite using SDL_Render_Copy and makes a local Rect so that it can be used multiple times.
	SDL_Rect source;
	SDL_Rect dest;
	SDL_Point center;

	source.x = cutx;
	source.y = cuty;
	source.w = cutwidth;
	source.h = cutheight;

	dest.x = destx;
	dest.y = desty;
	dest.w = strechw;
	dest.h = strechh;

	center.x = cutwidth / 2;
	center.y = cutheight / 2;

	flip = SDL_FLIP_HORIZONTAL;

	if (texBank.size() > texpointer)
		SDL_RenderCopyEx(renderer, texBank[texpointer], &source, &dest, 0, 0, flipflags);
	else
		printf("Texture pointer is out of bounds");
}

void NIKO_Texture::nikoRenderTextureData(SDL_Texture* texture, int cutwidth, int cutheight, int cutx, int cuty, int destx, int desty, int strechw, int strechh, SDL_RendererFlip flipflags)
{
	//Renders the sprite using SDL_Render_Copy and makes a local Rect so that it can be used multiple times.
	SDL_Rect source;
	SDL_Rect dest;
	SDL_Point center;

	source.x = cutx;
	source.y = cuty;
	source.w = cutwidth;
	source.h = cutheight;

	dest.x = destx;
	dest.y = desty;
	dest.w = strechw;
	dest.h = strechh;

	center.x = NULL;
	center.y = NULL;

	SDL_RenderCopyEx(renderer, texture, &source, &dest, 0, 0, flipflags);
}
 
// SPRITES --OLD--

bool NIKO_Sprite::nikoLoadSprites(std::string xmlfile)
{
	//loads the whole giffy xml file

	if (loadpoll == true)
		return false;

	pugi::xml_document doc;
	if (!doc.load_file(xmlfile.c_str()))
	{
		printf("--can't load sprite file... %s\n", xmlfile);
		return false;
	}
	else
		printf("sprite file loaded successfully: %s\n", xmlfile);

	std::string pathedit = xmlfile;

	for (int i = pathedit.size() - 1; i < pathedit.size(); i--)
	{
		if (pathedit[i] != '/')
			pathedit.erase(pathedit.begin() + i);

		if (pathedit[i] == '/')
			break;
	}

	spriteData dummy;

	pugi::xml_node xmlTex = doc.child("gfs");


	for (pugi::xml_node tcxml = xmlTex; tcxml; tcxml = tcxml.next_sibling("gfs"))
		numberOfAnimations = tcxml.attribute("animations").as_uint();

	for (pugi::xml_node tcxml = xmlTex.child("sheet"); tcxml; tcxml = tcxml.next_sibling("sheet"))
	{
		std::string fakepath;

		fakepath += pathedit;
		fakepath += tcxml.attribute("src").as_string();

		textures.nikoTexLibraryAdd(tcxml.attribute("idx").as_string(), fakepath.c_str());
	}

	for (pugi::xml_node tcxml = xmlTex.child("animation"); tcxml; tcxml = tcxml.next_sibling("animation"))
	{
		dummy.name = tcxml.attribute("name").as_string();
		dummy.nof = tcxml.attribute("frames").as_uint();

		for (pugi::xml_node txml = tcxml.child("frame"); txml; txml = txml.next_sibling("frame"))
		{
			dummy.texindex.push_back(txml.attribute("sheet").as_string());

			dummy.xcut.push_back(txml.attribute("x").as_int());
			dummy.ycut.push_back(txml.attribute("y").as_int());

			dummy.boxwidth.push_back(txml.attribute("w").as_int());
			dummy.boxheight.push_back(txml.attribute("h").as_int());

			dummy.mirrorx.push_back(txml.attribute("ox").as_int());
			dummy.mirrory.push_back(txml.attribute("oy").as_int());
		}

		spritedata.push_back(dummy);
		dummy = {};
	}

	loadpoll = true;

	return true;
}

void NIKO_Sprite::nikoAddSprite(unsigned int numToAdd)
{
	//adds a sprite object to the animationdata vector
	while (numberOfSprites < numToAdd)
	{
		anidata.push_back(packer);
		numberOfSprites++;
	}
}

bool NIKO_Sprite::nikoRenderSprite(std::string nameid, int spriteNum, int delay, int destx, int desty, bool flipx, bool flipy, bool loop, int alpha)
{
	int x = 0, y = 0;

	//if there are no animations and if the sprite object number does not exist, exit the funtion
	if (numberOfAnimations == 0 || anidata.size() == 0 || anidata.size() - 1 < spriteNum)
		return false;

	//if it is not a sprite loop
	if (loop == true)
	{

		//searching for the loaded animation
		for (int i = 0; i < numberOfAnimations; i++)
		{
			if (spritedata[i].name == nameid)
			{
				currentid = i;
				break;
			}
		}

		//if a diffrent animation is being played then reset the timers
		if (spritedata[currentid].changepoll != nameid)
		{
			anidata[spriteNum].framenum = 0;
			anidata[spriteNum].delayresp = 0;
			spritedata[currentid].changepoll = nameid;
		}

		textures.nikoAlphaLibrarian(spritedata[currentid].texindex[anidata[spriteNum].framenum], alpha);

		//flip and render sprite 
		if (flipx == true)
			x -= spritedata[currentid].mirrorx[anidata[spriteNum].framenum] + spritedata[currentid].boxwidth[anidata[spriteNum].framenum];
		else
			x += spritedata[currentid].mirrorx[anidata[spriteNum].framenum];

		if (flipy == true)
			y -= spritedata[currentid].mirrory[anidata[spriteNum].framenum] + spritedata[currentid].boxheight[anidata[spriteNum].framenum];
		else
			y += spritedata[currentid].mirrory[anidata[spriteNum].framenum];

		textures.nikoRenderTexture(spritedata[currentid].texindex[anidata[spriteNum].framenum],
			spritedata[currentid].boxwidth[anidata[spriteNum].framenum],
			spritedata[currentid].boxheight[anidata[spriteNum].framenum],
			spritedata[currentid].xcut[anidata[spriteNum].framenum],
			spritedata[currentid].ycut[anidata[spriteNum].framenum],
			x + destx + (spritedata[currentid].boxwidth[anidata[spriteNum].framenum] / 2),
			y + desty + spritedata[currentid].boxheight[anidata[spriteNum].framenum],
			spritedata[currentid].boxwidth[anidata[spriteNum].framenum],
			spritedata[currentid].boxheight[anidata[spriteNum].framenum],
			(SDL_RendererFlip)SDL_FLIP_NONE);

		//counts up to the delay time and then increments the animation frame
		anidata[spriteNum].delayresp++;

		if (anidata[spriteNum].delayresp >= delay)
		{
			anidata[spriteNum].delayresp = 0;
			anidata[spriteNum].framenum++;
		}

		//if you run out of frames it changes the frame number back to zero
		if (anidata[spriteNum].framenum >= spritedata[currentid].nof)
		{
			anidata[spriteNum].framenum = 0;
			return true;
		}

		return false;
	}

	//if it is not a sprite loop
	if (loop == false)
	{

		//searching for the loaded animation
		for (int i = 0; i < numberOfAnimations; i++)
		{
			if (spritedata[i].name == nameid)
			{
				currentid = i;
				break;
			}
		}

		//if a diffrent animation is being played then reset the timers
		if (spritedata[currentid].changepoll != nameid)
		{
			anidata[spriteNum].framenum = 0;
			anidata[spriteNum].delayresp = 0;
			spritedata[currentid].changepoll = nameid;
		}

		textures.nikoAlphaLibrarian(spritedata[currentid].texindex[anidata[spriteNum].framenum], alpha);


		//flip and render sprite
		if (flipx == true)
			x -= spritedata[currentid].mirrorx[anidata[spriteNum].framenum] + spritedata[currentid].boxwidth[anidata[spriteNum].framenum];
		else
			x += spritedata[currentid].mirrorx[anidata[spriteNum].framenum];

		if (flipy == true)
			y -= spritedata[currentid].mirrory[anidata[spriteNum].framenum] + spritedata[currentid].boxheight[anidata[spriteNum].framenum];
		else
			y += spritedata[currentid].mirrory[anidata[spriteNum].framenum];

		textures.nikoRenderTexture(spritedata[currentid].texindex[anidata[spriteNum].framenum],
			spritedata[currentid].boxwidth[anidata[spriteNum].framenum],
			spritedata[currentid].boxheight[anidata[spriteNum].framenum],
			spritedata[currentid].xcut[anidata[spriteNum].framenum],
			spritedata[currentid].ycut[anidata[spriteNum].framenum],
			x + destx + (spritedata[currentid].boxwidth[anidata[spriteNum].framenum] / 2),
			y + desty + spritedata[currentid].boxheight[anidata[spriteNum].framenum],
			spritedata[currentid].boxwidth[anidata[spriteNum].framenum],
			spritedata[currentid].boxheight[anidata[spriteNum].framenum],
			(SDL_RendererFlip)SDL_FLIP_NONE);



		//counts up to the delay time and then increments the animation frame until the last frame
		anidata[spriteNum].delayresp++;

		if (anidata[spriteNum].delayresp >= delay && anidata[spriteNum].framenum < spritedata[currentid].nof - 1)
		{
			anidata[spriteNum].delayresp = 0;
			anidata[spriteNum].framenum++;

		}

		if (anidata[spriteNum].framenum > spritedata[currentid].nof - 1)
		{
			anidata[spriteNum].framenum = spritedata[currentid].nof - 1;


		}

		if (anidata[spriteNum].framenum == spritedata[currentid].nof - 1)
			return false;
		else
			return true;

		return false;
	}

	return true;
}

unsigned int NIKO_Sprite::nikoGetCurrentPos(int spriteid)
{
	//keep an eye on this one. i'm tired, lol
	unsigned int tempnum;

	if (spriteid > numberOfSprites || numberOfSprites < 1)
		return 0;

	tempnum = anidata[spriteid].framenum;

	return tempnum;
}

void NIKO_Sprite::nikoFreeSprites()
{
	textures.nikoFreeTextures();

	spritedata.clear();

	anidata.clear();

	loadpoll = false;

	currentid = 0;

	numberOfSprites = 0;

	numberOfAnimations = 0;
}

// SPRITES NEW

bool NIKO_SpriteRenderer::nikoLoadSprites(std::string file)
{
	//loads the whole giffy xml file

	if (loadpoll == true)
		return false;

	pugi::xml_document doc;
	if (!doc.load_file(file.c_str()))
	{
		printf("--can't load sprite file... %s\n", file);
		return false;
	}
	else
		printf("sprite file loaded successfully: %s\n", file);

	std::string pathedit = file;

	for (int i = pathedit.size() - 1; i < pathedit.size(); i--)
	{
		if (pathedit[i] != '/')
			pathedit.erase(pathedit.begin() + i);

		if (pathedit[i] == '/')
			break;
	}

	spriteData dummy;

	pugi::xml_node xmlTex = doc.child("gfs");


	for (pugi::xml_node tcxml = xmlTex; tcxml; tcxml = tcxml.next_sibling("gfs"))
		numberOfAnimations = tcxml.attribute("animations").as_uint();

	for (pugi::xml_node tcxml = xmlTex.child("sheet"); tcxml; tcxml = tcxml.next_sibling("sheet"))
	{
		std::string fakepath;

		fakepath += pathedit;
		fakepath += tcxml.attribute("src").as_string();

		textures.nikoTexLibraryAdd(tcxml.attribute("idx").as_string(), fakepath.c_str());
	}

	for (pugi::xml_node tcxml = xmlTex.child("animation"); tcxml; tcxml = tcxml.next_sibling("animation"))
	{
		dummy.name = tcxml.attribute("name").as_string();
		dummy.nof = tcxml.attribute("frames").as_uint();

		for (pugi::xml_node txml = tcxml.child("frame"); txml; txml = txml.next_sibling("frame"))
		{
			dummy.texindex.push_back(txml.attribute("sheet").as_string());

			dummy.xcut.push_back(txml.attribute("x").as_int());
			dummy.ycut.push_back(txml.attribute("y").as_int());

			dummy.boxwidth.push_back(txml.attribute("w").as_int());
			dummy.boxheight.push_back(txml.attribute("h").as_int());

			dummy.mirrorx.push_back(txml.attribute("ox").as_int());
			dummy.mirrory.push_back(txml.attribute("oy").as_int());
		}

		spritedata.push_back(dummy);
		dummy = {};
	}

	loadpoll = true;

	return true;
}