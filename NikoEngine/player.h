#ifndef PLAYER_H
#define PLAYER_H

#include "nikoCollision.h"
#include "nikoMath.h"
#include "nikoGamePad.h"

struct NIKO_Player
{
    int x;
    int y;
    int width;
    int height;
    int layer;

    NIKO_Collision pla_collision;
    NIKO_Math pla_math;
    NIKO_Button pla_controls[2];

    void renderPlayer();
};

#endif