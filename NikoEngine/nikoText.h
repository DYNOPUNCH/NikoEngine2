#ifndef NIKO_TEXT_H
#define NIKO_TEXT_H

#include <iostream>
#include <string>
#include <vector>
#include <sstream>
#include "nikoSSU.h"
#include "nikoGamePad.h"
#include "nikoMath.h"

#include "include/pugi/pugiconfig.hpp"
#include "include/pugi/pugixml.hpp"

enum dialogue_type
{
	DIALOGUE_TYPE_NONE = 0,
	DIALOGUE_TYPE_END = 1,
	DIALOGUE_TYPE_DIALOGUE = 2,
	DIALOGUE_TYPE_BRANCH = 3
};

class NIKO_Text
{
private:

	int currentstorage;

	int charcount;

	struct fontdata
	{
		//number of characters

		//font info

		std::string face;
		unsigned int  size;
		unsigned int lineheight;


		std::string pageid;
		std::string textpage;

		//load in characters

		std::vector<std::string> charid;

		std::vector<unsigned int> xrect;
		std::vector<unsigned int> yrect;
		std::vector<unsigned int> wrect;
		std::vector<unsigned int> hrect;

		std::vector<unsigned int> xoffset;
		std::vector<unsigned int> yoffset;

		std::vector<unsigned int> xadvance;
		std::vector<int> page;

	};

	unsigned int index;

	bool colorswitch;

	int findtextcode(std::string charindex);

	bool nikoTextCode(std::string code);

	NIKO_Texture pagetexture;

	int wordwrapcm;
	int wordwrapcounter;

	bool newwrap;

	std::string stringcopy;

	unsigned int loadcount;
	unsigned int loaddelaycount;

public:

	int current_max;
	int possible_max;
	int load_count;

	bool is_loading;

	bool rewrap;
	bool textloading;

	unsigned int loaddelay;

	unsigned int widthsum;
	unsigned int heightsum;

	fontdata textdata;

	std::string codebreak;

	bool nikoLoadFont(const char* xmlfile);
	void nikoFreeFont();

	void picoResetLoad()
	{
		load_count = 0;
		current_max = 0;
		possible_max = 0;
	}
	void picoOverRideLoad()
	{
		current_max = possible_max;
	}

	int nikoRenderTextLoad(std::string text, int x, int y, int r, int g, int b, int a, int delayTimer, int textRate);
	int nikoRenderText(std::string text, int x, int y, int r, int g, int b, int a);
	int nikoRenderTextWW(std::string text, int x, int y, int maxwidth, int r, int g, int b, int a);
	int nikoStat(std::string text, float number, int x, int y, int r, int g, int b, int a);
};

struct NIKO_Dialogue
{
private:

	//how the text assets are positioned 
	struct fielddetails
	{
		int alpha;
		SDL_Color fieldcolor;
		int x;
		int y;
	};

	struct listdetails
	{
		int listalpha;
		int selectalpha;
		SDL_Color listcolor;
		SDL_Color selectcolor;
		int x;
		int y;
		int space;
		int maxSafetyTime;
	};

	struct idcheck
	{
		std::uint64_t id;
		dialogue_type type;
	};

	//stores all of the basic line information like name, text image, etc
	//most inner part of the xml;
	struct dialoguepacket
	{
		std::string name;
		std::string text;
		std::string imgpath;
		std::string vacode;
	};

	//stores the id and where to go to when you get to the end of lines' size.
	struct dialoguepackage
	{
		std::uint64_t diologueid;
		std::vector<dialoguepacket> lines;
		std::uint64_t nextid;
		dialogue_type nexttype;
	};

	//stores all of the game's dialogue.
	std::vector<dialoguepackage> dialoguebank;

	//stores all of the choices and where to go if there is a selection
	struct branchpacket
	{
		std::string text;
		std::uint64_t nextid;
		dialogue_type nexttype;
	};

	//stores the id of a packet and the questions to them
	struct branchpackage
	{
		std::uint64_t branchid;
		std::vector<branchpacket> questions;
	};

	//stores all game question sets
	std::vector<branchpackage> branchbank;

	NIKO_Sprite textboxsprites;
	NIKO_Math screenborders;

	pugi::xml_document mastertextfile;

	int questionSafety;

public:

	bool inProgress;

	std::string nameram;
	bool newname;

	fielddetails namefieldinfo;
	fielddetails textfieldinfo;
	listdetails listfieldinfo;

	bool loadmaster;
	unsigned int counter;
	int questionindex;

	int blockcache;

	dialogue_type activetype;
	idcheck currentblock;
	bool signal;
	bool keepreading;

	bool searchDataBlock();

	NIKO_Text namefield;
	NIKO_Text textfield;

	bool begintalking;

	//remove later
	NIKO_Button betabutton;
	NIKO_Button betaup;
	NIKO_Button betadown;

	//Number of frames to way before spiting out more text. if 0 then load all of it like normal

	bool picoSetTextboxAudio(std::string chatterSFX, std::string actionButtonSFX, std::string upButtonSFX, std::string downButtonSFX, std::string continueSFX)
	{


	}

	bool picoLoadGT(const char* file);

	void picoFreeTextbox();

	void picoDialogue(int textRate);

	void picoDialogueINIT(const char *nameFieldFont, const char *textFieldFont)
	{
		inProgress = false;

		nameram = "";

		namefieldinfo.x = 0;
		namefieldinfo.y = 0;
		namefieldinfo.alpha = 255;
		namefieldinfo.fieldcolor = { 255, 255, 255 };

		textfieldinfo.x = 0;
		textfieldinfo.y = 0;
		textfieldinfo.alpha = 255;
		textfieldinfo.fieldcolor = { 255, 255, 255 };

		listfieldinfo.x = 0;
		listfieldinfo.y = 0;
		listfieldinfo.space = 0;
		listfieldinfo.listalpha = 255;
		listfieldinfo.selectalpha = 255;
		listfieldinfo.listcolor = { 255, 255, 255 };
		listfieldinfo.selectcolor = { 255, 255, 255 };
		listfieldinfo.maxSafetyTime = 10;

		loadmaster = false;

		begintalking = true;

		counter = 0;
		questionindex = 0;

		blockcache = -1;

		activetype = DIALOGUE_TYPE_NONE;
		currentblock.id = 0;
		currentblock.type = DIALOGUE_TYPE_NONE;

		signal = false;
		keepreading = false;

		namefield.nikoLoadFont(nameFieldFont);
		textfield.nikoLoadFont(textFieldFont);

		//textboxsprites.nikoLoadSprites("resc/graphics/ui/textbox/textbox.xml");
	}

	void updateidcheck(std::uint64_t id, dialogue_type type)
	{
		if (blockcache == -1)
		{
			currentblock.id = id;
			currentblock.type = type;
		}
	}

	~NIKO_Dialogue()
	{
		mastertextfile.reset();

		dialoguebank.clear();
		branchbank.clear();
		loadmaster = false;
		textboxsprites.nikoFreeSprites();

		inProgress = false;

		nameram = "";

		namefieldinfo.x = 0;
		namefieldinfo.y = 0;
		namefieldinfo.alpha = 255;
		namefieldinfo.fieldcolor = { 255, 255, 255 };

		textfieldinfo.x = 0;
		textfieldinfo.y = 0;
		textfieldinfo.alpha = 255;
		textfieldinfo.fieldcolor = { 255, 255, 255 };

		listfieldinfo.x = 0;
		listfieldinfo.y = 0;
		listfieldinfo.space = 0;
		listfieldinfo.listalpha = 255;
		listfieldinfo.selectalpha = 255;
		listfieldinfo.listcolor = { 255, 255, 255 };
		listfieldinfo.selectcolor = { 255, 255, 255 };

		loadmaster = false;

		begintalking = true;

		counter = 0;
		questionindex = 0;

		blockcache = -1;

		activetype = DIALOGUE_TYPE_NONE;
		currentblock.id = 0;
		currentblock.type = DIALOGUE_TYPE_NONE;

		signal = false;
		keepreading = false;
	}

	NIKO_Dialogue()
	{
		mastertextfile.reset();

		dialoguebank.clear();
		branchbank.clear();
		loadmaster = false;
		textboxsprites.nikoFreeSprites();

		inProgress = false;

		nameram = "";

		namefieldinfo.x = 0;
		namefieldinfo.y = 0;
		namefieldinfo.alpha = 255;
		namefieldinfo.fieldcolor = { 255, 255, 255 };

		textfieldinfo.x = 0;
		textfieldinfo.y = 0;
		textfieldinfo.alpha = 255;
		textfieldinfo.fieldcolor = { 255, 255, 255 };

		listfieldinfo.x = 0;
		listfieldinfo.y = 0;
		listfieldinfo.space = 0;
		listfieldinfo.listalpha = 255;
		listfieldinfo.selectalpha = 255;
		listfieldinfo.listcolor = { 255, 255, 255 };
		listfieldinfo.selectcolor = { 255, 255, 255 };

		loadmaster = false;

		begintalking = true;

		counter = 0;
		questionindex = 0;

		blockcache = -1;

		activetype = DIALOGUE_TYPE_NONE;
		currentblock.id = 0;
		currentblock.type = DIALOGUE_TYPE_NONE;

		signal = false;
		keepreading = false;
	}

};

#endif