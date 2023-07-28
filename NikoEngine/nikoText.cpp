#include "nikoText.h"


extern NIKO_System sys;

//ADD FUNCTION TO GET TEXT WIDTH AND HEIGHT 

// TEXT API

int NIKO_Text::findtextcode(std::string charindex)
{
	//iterate through the character index to find the right character number

	for (int i = 0; i < textdata.charid.size(); i++)
		if (charindex == textdata.charid[i])
			return i;

	return 0;
}

bool NIKO_Text::nikoLoadFont(const char* xmlfile)
{
	//Intigrate top code into bottom code tomorrow
	//Make an initialize function.
	
	if (textdata.charid.size() > 0)
	return false;

	pugi::xml_document doc;

	if (!doc.load_file(xmlfile))
	{
		printf("Unable to load bitmap font: %s\n", xmlfile);
		return false;
	}
	else
	printf("Bitmap Font opened successfully: %s\n", xmlfile);

	std::string pathedit = xmlfile;

	for (int i = pathedit.size() - 1; i < pathedit.size(); i--)
	{
		if (pathedit[i] != '/')
			pathedit.erase(pathedit.begin() + i);

		if (pathedit[i] == '/')
			break;
	}
	
	pugi::xml_node xmlDia = doc.child("font");


	for (pugi::xml_node chunks = xmlDia; chunks; chunks = chunks.next_sibling("font"))
	{
		textdata.face = chunks.child("info").attribute("face").as_string();
		textdata.size = chunks.child("info").attribute("size").as_uint();
		textdata.lineheight = chunks.child("common").attribute("lineHeight").as_uint();

		textdata.textpage = chunks.child("pages").child("page").attribute("file").as_string();

		pathedit += textdata.textpage;

		pagetexture.nikoTexLibraryAdd(textdata.face, pathedit.c_str());

		for (pugi::xml_node box = chunks.child("chars").child("char"); box; box = box.next_sibling("char"))
		{
			std::string tempstring(1, box.attribute("id").as_uint());

			textdata.charid.push_back(tempstring);

			textdata.xrect.push_back(box.attribute("x").as_uint());
			textdata.yrect.push_back(box.attribute("y").as_uint());
			textdata.wrect.push_back(box.attribute("width").as_uint());
			textdata.hrect.push_back(box.attribute("height").as_uint());

			textdata.xoffset.push_back(box.attribute("xoffset").as_uint());
			textdata.yoffset.push_back(box.attribute("yoffset").as_uint());

			textdata.xadvance.push_back(box.attribute("xadvance").as_uint());

		}

	}

	SDL_SetHint(SDL_HINT_RENDER_SCALE_QUALITY, "0");
	
	return true;
}

bool NIKO_Text::nikoTextCode(std::string code)
{
	char command[32]; /* tweak buffer size as needed */
	int arg1 = 0;
	int arg2 = 0;
	int arg3 = 0;
	int arg4 = 0;

	static char arg1s[32];
	char charwipe[32];

	sscanf(code.c_str(), "%s %d,%d,%d,%d", command, &arg1, &arg2, &arg3, &arg4);

	//|RGB A,B,C|
	//turn on text highlight with alpha channel.
	//A: Red color value (between 0 and 255)
	//B: Green color value (between 0 and 255)
	//C: Blue color value (between 0 and 255)
	//D: Alpha value (between 0 and 255)

	if (!strcmp(command, "RGB"))
	{

		colorswitch = true;
		pagetexture.nikoColorLibrarian(textdata.face, arg1, arg2, arg3);

		return true;
	}

	//<RGB/> 
	//Turn off text highlight (the text may loop the color if you do not turn it off!!)

	if (!strcmp(command, "RGB/"))
	{
		colorswitch = false;
		return true;
	}

	//<RGBA A,B,C,D>
	//turn on text highlight.
	//A: Red color value (between 0 and 255)
	//B: Green color value (between 0 and 255)
	//C: Blue color value (between 0 and 255)

	if (!strcmp(command, "RGBA"))
	{
		colorswitch = true;
		pagetexture.nikoColorLibrarian(textdata.face, arg1, arg2, arg3);
		pagetexture.nikoAlphaLibrarian(textdata.face, arg4);
		return true;
	}

	//<RGBA/> 
	//Turn off text highlight (the text may loop the color if you do not turn it off!!)

	if (!strcmp(command, "RGBA/"))
	{
		colorswitch = false;
		return true;
	}

	//<RETURN>
	//Start newline of text. 
	//A: will lower the text furter.

	if (!strcmp(command, "RETURN"))
	{
		widthsum = 0;
		heightsum += textdata.lineheight + arg1;

		return true;
	}

	//<RAND A,B>
	//Generate a randrom number between the ranges of A and B.
	//A: First range (i.e. 100 = 0 to 99)
	//B: Second Range (i.e. 100,1 = 1 to 100) "or" (i.e. 30,1985 = 1985-2014)

	if (!strcmp(command, "RAND"))
	{
		srand(time(NULL));

		sys.randomnum = rand() % arg1 + arg2;

		return true;
	}

	//<MUSIC A>
	//Change system music
	//A: Music to change to from index

	if (!strcmp(command, "MUSIC"))
	{
		//sys.gs.systemData.ostNumber = arg1;

		return true;
	}



	if (!strcmp(command, "ROOMCHANGE"))
	{
		//FIX LATER

		//sys.gs.systemData.gamearea = arg1;

		return true;
	}

	//<NEXTCONVO A,B>
	//Update a conversation link chain
	//A: The object to update
	//B: the new dialogue chain

	if (!strcmp(command, "NEXTCONVO"))
	{
		/*
		if (sys.convolinkedit == false)
		{

			sscanf_s(code.c_str(), "%s %d,%s", command, &arg1, &arg1s);

			sys.idcopys = arg1;
			sys.convocopys = arg1s;

			//if convo copy has been updated

			if (sys.convocopys != "")
				sys.convoupdated = true;

			//if link chain has been broken by a NONE command

			if (sys.convocopys == "NONE")
				sys.convocopys = "";

			printf(sys.convocopys.c_str());

			sys.convolinkedit = true;
		}
		return true;
		*/
	}

	return false;
}





int NIKO_Text::nikoRenderTextLoad(std::string text, int x, int y, int r, int g, int b, int a, int delayAmount, int textRate)
{
	std::string textcache;

	widthsum = 0;
	heightsum = 0;
	index = 0;

	pagetexture.nikoColorLibrarian(textdata.face, r, g, b);
	pagetexture.nikoAlphaLibrarian(textdata.face, a);

	//text cache stuff

	possible_max = text.size();

	if (text.size() != textcache.size())
		is_loading = true;
	else
		is_loading = false;

	for (int i = 0; i < current_max; i++)
	{
		textcache += text[i];

		if (text[current_max] == '<')
		{
			loadcount = 0;
			while(text[current_max] != '>')
			{
				i++;
				textcache += text[i];
				current_max++;
			}
		}

		if (text[current_max] == '>')
		{
			current_max++;
			loadcount = 0;
			if (current_max > text.size())
				current_max = text.size();
		}
		
	}

	if (load_count == delayAmount && current_max != text.size())
	{
		load_count = 0;
		current_max += textRate;
		
		if (current_max > text.size())
			current_max = text.size();
	}
	else
	{
		load_count++;
	}
		

	//end of text cache stuff

	for (int i = 0; i < textcache.size(); i++)
	{
		if (colorswitch == false)
		{
			pagetexture.nikoColorLibrarian(textdata.face, r, g, b);
			pagetexture.nikoAlphaLibrarian(textdata.face, a);
		}

		std::string tempstr(1, textcache[i]);

		index = findtextcode(tempstr);

		if (textcache[i] == '<')
		{
			if (textcache[i + 1] == '>')
			{
				i++;
				tempstr = textcache[i + 1];
				index = findtextcode(tempstr);
				codebreak = "F";
			}
			else
				for (int ii = i; ii < textcache.size(); i++, ii++)
				{
					if (textcache[i + 1] == '>')
					{
						i++;
						tempstr = textcache[i + 1];
						index = findtextcode(tempstr);
						break;
					}

					codebreak += textcache[i + 1];
				}

		}

		if (codebreak.size() > 0)
			nikoTextCode(codebreak);

		if (codebreak.size() == 0)
		{
			pagetexture.nikoRenderTexture(
				textdata.face,
				textdata.wrect[index],
				textdata.hrect[index],
				textdata.xrect[index],
				textdata.yrect[index],
				textdata.xoffset[index] + widthsum + x,
				textdata.yoffset[index] + heightsum + y,
				textdata.wrect[index],
				textdata.hrect[index],
				(SDL_RendererFlip)SDL_FLIP_NONE);

			widthsum += textdata.xadvance[index];
		}

		codebreak = "";

	}

	return widthsum;

}

int NIKO_Text::nikoRenderText(std::string text, int x, int y, int r, int g, int b, int a)
{
	widthsum = 0;
	heightsum = 0;
	index = 0;

	pagetexture.nikoColorLibrarian(textdata.face, r, g, b);
	pagetexture.nikoAlphaLibrarian(textdata.face, a);

	for (int i = 0; i < text.size(); i++)
	{
		if (colorswitch == false)
		{
			pagetexture.nikoColorLibrarian(textdata.face, r, g, b);
			pagetexture.nikoAlphaLibrarian(textdata.face, a);
		}

		std::string tempstr(1, text[i]);

		index = findtextcode(tempstr);

		if (text[i] == '<')
		{
			if (text[i + 1] == '>')
			{
				i++;
				tempstr = text[i + 1];
				index = findtextcode(tempstr);
				codebreak = "F";
			}
			else
			for (int ii = i; ii < text.size(); i++, ii++)
			{
				if (text[i + 1] == '>')
				{
					i++;
					tempstr = text[i + 1];
					index = findtextcode(tempstr);
					break;
				}

				codebreak += text[i + 1];
			}

		}

		if (codebreak.size() > 0)
			nikoTextCode(codebreak);

		if (codebreak.size() == 0)
		{
			pagetexture.nikoRenderTexture(
				textdata.face,
				textdata.wrect[index],
				textdata.hrect[index],
				textdata.xrect[index],
				textdata.yrect[index],
				textdata.xoffset[index] + widthsum + x,
				textdata.yoffset[index] + heightsum + y,
				textdata.wrect[index],
				textdata.hrect[index],
				(SDL_RendererFlip)SDL_FLIP_NONE);

			widthsum += textdata.xadvance[index];
		}

		codebreak = "";

	}

	return widthsum;
}

int NIKO_Text::nikoRenderTextWW(std::string text, int x, int y, int maxwidth, int r, int g, int b, int a)
{
	widthsum = 0;
	heightsum = 0;

	if (rewrap == false)
	{

		stringcopy = text;
		loadcount = 0;
		loaddelaycount = 0;

		for (int i = 0; i < stringcopy.size(); i++)
		{


			//skip any text code
			if (stringcopy[i] == '<')
				for (int ii = i; ii < stringcopy.size(); i++, ii++)
				{
					if (stringcopy[i + 1] == '>')
					{
						i++;
						break;
					}
				}

			if (stringcopy[i] == ' ')
			{
				wordwrapcm = i;
			}



			wordwrapcounter++;

			//modify text for word wrapping

			if (wordwrapcm == 0 && wordwrapcounter > maxwidth)
			{
				stringcopy.insert(i + 1, "<RETURN 1>");
				wordwrapcounter = 0;
				wordwrapcm = 0;
			}

			if (wordwrapcounter > maxwidth)
			{
				stringcopy.insert(wordwrapcm + 1, "<RETURN 1>");
				wordwrapcounter = 0;
				wordwrapcm = 0;
			}


		}

		wordwrapcounter = 0;
		wordwrapcm = 0;
		rewrap = true;
	}

	if (textloading == false)
	{

		for (int i = 0; i < stringcopy.size(); i++)
		{
			if (colorswitch == false)
			{
				pagetexture.nikoColorLibrarian(textdata.face, r, g, b);
				pagetexture.nikoAlphaLibrarian(textdata.face, a);
			}

			std::string tempstr(1, stringcopy[i]);

			index = findtextcode(tempstr);

			if (stringcopy[i] == '<')
			{
				for (int ii = i; ii < stringcopy.size(); i++, ii++)
				{
					if (stringcopy[i + 1] == '>')
					{
						i++;
						tempstr = stringcopy[i + 1];
						index = findtextcode(tempstr);
						break;
					}

					codebreak += stringcopy[i + 1];
				}

			}

			if (codebreak.size() > 0)
				nikoTextCode(codebreak);

			if (codebreak.size() == 0)
			{
				pagetexture.nikoRenderTexture(
					textdata.face,
					textdata.wrect[index],
					textdata.hrect[index],
					textdata.xrect[index],
					textdata.yrect[index],
					textdata.xoffset[index] + widthsum + x,
					textdata.yoffset[index] + heightsum + y,
					textdata.wrect[index],
					textdata.hrect[index],
					(SDL_RendererFlip)SDL_FLIP_NONE);

				widthsum += textdata.xadvance[index];
			}

			codebreak = "";

		}

	}
	else if (textloading == true)
	{
		if (loadcount < stringcopy.size())
			loadcount++;

		for (int i = 0; i < stringcopy.size(); i++)
		{

			//fix text loader you left off at the part where you were
			//going to add the loop breaks 

			//still in beta. use base version still you fix it.

			if (colorswitch == false)
			{
				pagetexture.nikoColorLibrarian(textdata.face, r, g, b);
				pagetexture.nikoAlphaLibrarian(textdata.face, 255);
			}

			std::string tempstr(1, stringcopy[i]);

			index = findtextcode(tempstr);

			if (stringcopy[i] == '<')
			{
				for (int ii = i; ii < stringcopy.size(); i++, ii++)
				{
					if (stringcopy[i + 1] == '>')
					{
						i++;
						tempstr = stringcopy[i + 1];
						index = findtextcode(tempstr);
						break;
					}

					codebreak += stringcopy[i + 1];
				}

			}

			if (codebreak.size() > 0)
				nikoTextCode(codebreak);

			if (codebreak.size() == 0)
			{
				pagetexture.nikoRenderTexture(
					textdata.face,
					textdata.wrect[index],
					textdata.hrect[index],
					textdata.xrect[index],
					textdata.yrect[index],
					textdata.xoffset[index] + widthsum + x,
					textdata.yoffset[index] + heightsum + y,
					textdata.wrect[index],
					textdata.hrect[index],
					(SDL_RendererFlip)SDL_FLIP_NONE);

				widthsum += textdata.xadvance[index];
			}



			codebreak = "";

			if (i >= loadcount)
				break;


		}

	}

	return widthsum;

}

void NIKO_Text::nikoFreeFont()
{
	if (textdata.charid.size() > 0)
	{

		pagetexture.nikoFreeTextures();
	}
}

int NIKO_Text::nikoStat(std::string text, float number, int x, int y, int r, int g, int b, int a)
{
	std::ostringstream osr;
	osr << text << number;
	return nikoRenderText(osr.str(), x, y, r, g, b, a);
}

//DIALOGUE API

void NIKO_Dialogue::picoDialogue(int textRate)
{
	//image and va stuff not implemented 
	//create a portrait object function tomorrow

	//just updates signal with a button I will be replacing later

	NIKO_Math posdebug;

	signal = betabutton.singlepress(betabutton.state[SDL_SCANCODE_Z]);

	searchDataBlock();

	if (blockcache >= 0)
	{
		inProgress = true;

		if (signal == true && activetype != DIALOGUE_TYPE_BRANCH)
			questionindex = 0;
		

		switch (activetype)
		{


		case DIALOGUE_TYPE_DIALOGUE:

			if (signal == true)
			{
				if (textfield.current_max < textfield.possible_max)
				{
					textfield.current_max = textfield.possible_max;
					
				}
				else
				{
					textfield.picoResetLoad();
					counter++;
				}
			}

			if (counter < dialoguebank[blockcache].lines.size())
			{
				if (nameram != dialoguebank[blockcache].lines[counter].name)
				{
					newname = true;
					nameram = dialoguebank[blockcache].lines[counter].name;
				}
				namefield.nikoRenderText(dialoguebank[blockcache].lines[counter].name, namefieldinfo.x, namefieldinfo.y, namefieldinfo.fieldcolor.r, namefieldinfo.fieldcolor.g, namefieldinfo.fieldcolor.b, namefieldinfo.alpha);
				
				if(textRate == 0)
				textfield.nikoRenderText(dialoguebank[blockcache].lines[counter].text, textfieldinfo.x, textfieldinfo.y, textfieldinfo.fieldcolor.r, textfieldinfo.fieldcolor.g, textfieldinfo.fieldcolor.b, textfieldinfo.alpha);
				else
				textfield.nikoRenderTextLoad(dialoguebank[blockcache].lines[counter].text, textfieldinfo.x, textfieldinfo.y, textfieldinfo.fieldcolor.r, textfieldinfo.fieldcolor.g, textfieldinfo.fieldcolor.b, textfieldinfo.alpha, 0, textRate);
			}
			else
			{
				//counter = dialoguebank[blockcache].lines.size() - 1;

				namefield.nikoRenderText(dialoguebank[blockcache].lines[counter - 1].name, namefieldinfo.x, namefieldinfo.y, namefieldinfo.fieldcolor.r, namefieldinfo.fieldcolor.g, namefieldinfo.fieldcolor.b, namefieldinfo.alpha);
				
				if (textRate == 0)
				textfield.nikoRenderText(dialoguebank[blockcache].lines[counter - 1].text, textfieldinfo.x, textfieldinfo.y, textfieldinfo.fieldcolor.r, textfieldinfo.fieldcolor.g, textfieldinfo.fieldcolor.b, textfieldinfo.alpha);
				else
				textfield.nikoRenderTextLoad(dialoguebank[blockcache].lines[counter - 1].text, textfieldinfo.x, textfieldinfo.y, textfieldinfo.fieldcolor.r, textfieldinfo.fieldcolor.g, textfieldinfo.fieldcolor.b, textfieldinfo.alpha, 0, textRate);
				
				keepreading = true;
			}
			break;

		case DIALOGUE_TYPE_BRANCH:

			if (betaup.singlepress(betabutton.state[SDL_SCANCODE_UP]))
			{
				questionindex--;

				if (questionindex < 0)
					questionindex = branchbank[blockcache].questions.size() - 1;
			}

			if (betadown.singlepress(betabutton.state[SDL_SCANCODE_DOWN]))
			{
				questionindex++;

				if (questionindex >= branchbank[blockcache].questions.size())
					questionindex = 0;
			}

			if (signal == true && questionSafety == listfieldinfo.maxSafetyTime)
			{
				questionSafety = 0;
				currentblock.id = branchbank[blockcache].questions[questionindex].nextid;
				currentblock.type = branchbank[blockcache].questions[questionindex].nexttype;
				keepreading = true;
			}

			if (questionSafety < listfieldinfo.maxSafetyTime)
				questionSafety++;

			for (int i = 0; i < branchbank[blockcache].questions.size(); i++)
				if (i == questionindex)
					textfield.nikoRenderText(branchbank[blockcache].questions[i].text, listfieldinfo.x, (listfieldinfo.space * i) + listfieldinfo.y, listfieldinfo.selectcolor.r, listfieldinfo.selectcolor.g, listfieldinfo.selectcolor.b, listfieldinfo.selectalpha);
				else
					textfield.nikoRenderText(branchbank[blockcache].questions[i].text, listfieldinfo.x, (listfieldinfo.space * i) + listfieldinfo.y, listfieldinfo.listcolor.r, listfieldinfo.listcolor.g, listfieldinfo.listcolor.b, listfieldinfo.listalpha);
			break;

		}

	}
	else
	{
		inProgress = false;
	}

	//beta position stuff
	static double ease[3] = { 0, 0, 0 };
	static int displacepos = 0;
	static float position[3] = { 0, 0, 0 };
	static bool negate = false;

	static NIKO_Button debugbutton[2];

	if (debugbutton[0].singlepress(debugbutton[0].state[SDL_SCANCODE_DOWN]))
	{
		ease[0] = 0;
		ease[1] = 0;
		ease[2] = 0;
		if (negate == false)
			displacepos++;
		if (negate == true)
			displacepos--;
		negate = false;
	}

	if (debugbutton[1].singlepress(debugbutton[1].state[SDL_SCANCODE_UP]))
	{
		ease[0] = 0;
		ease[1] = 0;
		ease[2] = 0;
		if (negate == false)
			displacepos++;
		if (negate == true)
			displacepos--;
		negate = true;
	}
}

bool NIKO_Dialogue::searchDataBlock()
{
	if (currentblock.id != 0 && currentblock.type != DIALOGUE_TYPE_NONE && signal == true && blockcache == -1 || keepreading == true)
	{
		
		switch (currentblock.type)
		{
		case DIALOGUE_TYPE_DIALOGUE:

			printf("searching...");

			questionSafety = 0;

			for (int i = 0; i < dialoguebank.size(); i++)
			{
				if (currentblock.id == dialoguebank[i].diologueid)
				{
					printf("dialogue batch loaded\n");

					counter = begintalking == true ? -1 : 0;

					blockcache = i;
					activetype = DIALOGUE_TYPE_DIALOGUE;
					currentblock.id = dialoguebank[i].nextid;
					currentblock.type = dialoguebank[i].nexttype;
					begintalking = false;

					keepreading = false;

					return true;
				}

				if (i == dialoguebank.size() - 1)
					printf("cound not find dialogue line package\n");
			}
			break;
		case DIALOGUE_TYPE_BRANCH:

			printf("searching...");

			questionSafety = 0;

			for (int i = 0; i < branchbank.size(); i++)
			{
				if (currentblock.id == branchbank[i].branchid)
				{
					printf("dialogue branch loaded\n");

					blockcache = i;
					activetype = DIALOGUE_TYPE_BRANCH;
					keepreading = false;
					return true;
				}

				if (i == branchbank.size() - 1)
					printf("cound not find dialogue branch\n");

			}
			break;
		case DIALOGUE_TYPE_END:

			printf("[ END OF CONVERSATION! ]");

			//where all exiting code goes

			questionSafety = 0;

			textfield.picoResetLoad();

			signal = 0;

			begintalking = true;

			counter = 0;
			questionindex = 0;

			blockcache = -1;

			activetype = DIALOGUE_TYPE_NONE;
			currentblock.id = -1;
			currentblock.type = DIALOGUE_TYPE_NONE;

			keepreading = false;
			break;
		}

	}
}

bool NIKO_Dialogue::picoLoadGT(const char* file)
{
	//load the master file into memory
	if (loadmaster == false)
	{
		if (!mastertextfile.load_file(file))
		{
			printf("Unsuccessful loading of text file...\n");
			return false;
		}
		printf("Text file loaded successfully!\n");
		loadmaster = true;
	}

	pugi::xml_node xmlgametext = mastertextfile.child("gametext");

	//chunk of code that may need to be replaced by design

	dialoguepacket emptydiopacket;
	dialoguepackage emptydiopackage;
	branchpacket emptybranchpacket;
	branchpackage emptybranchpackage;

	std::string comparestr;

	for (pugi::xml_node chunks = xmlgametext.child("dialogue"); chunks; chunks = chunks.next_sibling("dialogue"))
	{

		//clean out the structs before using
		dialoguepacket tempdiopacket = emptydiopacket;
		dialoguepackage tempdiopackage = emptydiopackage;

		//assign the id so that the actual iterator can find where it needs to go. 
		tempdiopackage.diologueid = chunks.attribute("id").as_ullong();

		//of course spell out the places to go next.
		tempdiopackage.nextid = chunks.attribute("nextid").as_ullong();

		//depending on what the next type is assign the appropriate enum values
		comparestr = chunks.attribute("nextidtype").as_string();

		if (comparestr == "dialogue")
			tempdiopackage.nexttype = DIALOGUE_TYPE_DIALOGUE;
		else if (comparestr == "branch")
			tempdiopackage.nexttype = DIALOGUE_TYPE_BRANCH;
		else if (comparestr == "")
			tempdiopackage.nexttype = DIALOGUE_TYPE_END;

		//pack the packet and then store it in the package (xml line data)
		for (pugi::xml_node box = chunks.child("line"); box; box = box.next_sibling("line"))
		{
			tempdiopacket.name = box.attribute("name").as_string();
			tempdiopacket.text = box.attribute("text").as_string();
			tempdiopacket.imgpath = box.attribute("imgpath").as_int();
			tempdiopacket.vacode = box.attribute("vacode").as_int();

			tempdiopackage.lines.push_back(tempdiopacket);
		}

		//store the temppackage into the master bank and loop around
		//until there is no more diologue data.
		dialoguebank.push_back(tempdiopackage);

		comparestr.clear();
	}


	for (pugi::xml_node chunks = xmlgametext.child("branch"); chunks; chunks = chunks.next_sibling("branch"))
	{

		//clean out the structs before using
		branchpacket tempbranchpacket = emptybranchpacket;
		branchpackage tempbranchpackage = emptybranchpackage;

		//assign the id so that the actual iterator can find where it needs to go. 
		tempbranchpackage.branchid = chunks.attribute("id").as_ullong();

		//pack the packet and then store it in the package (xml question data)
		for (pugi::xml_node box = chunks.child("question"); box; box = box.next_sibling("question"))
		{

			tempbranchpacket.text = box.attribute("text").as_string();

			//of course spell out the places to go next.
			tempbranchpacket.nextid = box.attribute("nextid").as_ullong();

			comparestr = box.attribute("nextidtype").as_string();

			if (comparestr == "dialogue")
				tempbranchpacket.nexttype = DIALOGUE_TYPE_DIALOGUE;
			else if (comparestr == "branch")
				tempbranchpacket.nexttype = DIALOGUE_TYPE_BRANCH;
			else if (comparestr == "")
				tempbranchpacket.nexttype = DIALOGUE_TYPE_END;

			tempbranchpackage.questions.push_back(tempbranchpacket);
		}

		//store the temppackage into the master bank and loop around
		//until there is no more data.
		branchbank.push_back(tempbranchpackage);

		comparestr.clear();
	}

	return true;
}

void NIKO_Dialogue::picoFreeTextbox()
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

	//have it dump out the old text at the project's end 
}