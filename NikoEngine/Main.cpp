#include <iostream>
#include <SDL.h>
#include "niko.h"
#include "nikoDefine.h"
#include "player.h"


extern SDL_Window* window;
extern SDL_Renderer* renderer;
extern SDL_Event window_key;

NIKO_Texture texture;
NIKO_Text* text = new NIKO_Text;
NIKO_Dialogue DioTest;
NIKO_Map testmap;
NIKO_Camera testcam;
NIKO_Player testplayer{0, 0, 16, 16};
NIKO_Button test_controls;


int resolution = 2;

struct bgHandler
{
    NIKO_Texture textureHandler;

    void loadBgList(const char* filename)
    {


    }
};



class nikoGUI
{
    struct topnav
    {

    };
};

void playercontrols()
{
    static float velocity = 0;
    const int max_velocity = 4;
    static float velocity_momentum = 0.3;

    static float speed = 0;
    const int max_speed = 3;
    static float speed_momentum = 0.2;

    int x_base = 0;
    int y_base = 0;

    switch (test_controls.eightway_input(false))
    {
    case 0:
        x_base = 0;
        y_base = 0;
        break;
    case 1:
        x_base = 0;
        y_base = -1;
        break;
    case 2:
        x_base = 1;
        y_base = -1;
        break;
    case 3:
        x_base = 1;
        y_base = 0;
        break;
    case 4:
        x_base = 1;
        y_base = 1;
        break;
    case 5:
        x_base = 0;
        y_base = 1;
        break;
    case 6:
        x_base = -1;
        y_base = 1;
        break;
    case 7:
        x_base = -1;
        y_base = 0;
        break;
    case 8:
        x_base = -1;
        y_base = -1;
        break;
    };

    velocity_momentum += (max_velocity / .1);

    //if (velocity < max_velocity)
      //  velocity += 1 - pow(1 - velocity_momentum, 3);

    
    //return 1 - pow(1 - x, 3);

    if (testmap.map_colli.onfloor == false)
    {
        
    }

    testplayer.x += x_base;
    testplayer.y += std::ceil(velocity);
}

int main(int argc, char* args[])
{

    SDL_Init(SDL_INIT_EVERYTHING);
    sys.NIKO_INIT_SYSTEM();

    window = SDL_CreateWindow("Project IMY", SDL_WINDOWPOS_CENTERED, SDL_WINDOWPOS_CENTERED, 480 * resolution, 270 * resolution, SDL_WINDOW_SHOWN | SDL_WINDOW_RESIZABLE);
    renderer = SDL_CreateRenderer(window, -1, SDL_RENDERER_ACCELERATED | SDL_RENDERER_PRESENTVSYNC);

    texture.nikoTexLibraryAdd("0001", "shoko-0.png");

    text->nikoLoadFont("resc/img/fonts/igiari-16.fnt");

    DioTest.picoDialogueINIT("resc/img/fonts/igiari-16.fnt", "resc/img/fonts/igiari-16.fnt");
    DioTest.picoLoadGT("resc/text/beta/gametext1.xml");

    NIKO_Button test;

    int musicswitch = 0;

    testmap.nikoParseTMX("resc/field/testmap2.tmx");

    //link the player pointer
    testmap.setPlayerPtr(&testplayer);

    while (sys.processing)
    {
        sys.NIKO_inputUpdate();
        SDL_SetRenderDrawBlendMode(renderer, SDL_BLENDMODE_NONE);
        SDL_RenderSetScale(renderer, resolution, resolution);

        if (!sys.processing)
            return 0;
        
        sys.nikoUpdate();

        //update collision state
        testmap.map_colli.nikoUpdateSides();

        //player control logic
        playercontrols();

        //track camera
        testcam.nikoCamera(testmap.mapData.width, testmap.mapData.height, 0, 0, 32, 32);

        testmap.nikoCreateObjects(0, 0, 0, false);
        //render images and objects
        for (int i = 0; i < testmap.tilelayercount; i++)
        {
        testmap.nikoRenderTileLayerTMX(i, -testcam.camerax, -testcam.cameray);
        testmap.nikoRenderImageLayerTMX(i, 0, 0);
        }

        texture.nikoRenderTextureDirect(0, 256, 256, 0, 0, 0, 0, 256, 256, SDL_FLIP_NONE);

        //check dialogue
        DioTest.updateidcheck(1, DIALOGUE_TYPE_DIALOGUE);
        DioTest.textfieldinfo.y = 20;
        DioTest.listfieldinfo.space = 20;

        DioTest.listfieldinfo.selectcolor = { 255, 0, 0 };
        DioTest.picoDialogue(1);
    }

    DioTest.picoFreeTextbox();

    delete text;

    return EXIT_SUCCESS;
}