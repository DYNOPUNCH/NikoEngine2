#include "player.h"

void NIKO_Player::renderPlayer()
{
    pla_math.nikoRectangle(x, y, 16, 16, { 255, 0, 0 }, 255, true);
}