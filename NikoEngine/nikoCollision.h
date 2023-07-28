#ifndef NIKO_COLLISION_H
#define NIKO_COLLISION_H

#include "nikoMath.h"

class NIKO_Collision
{
private:

	NIKO_Math triangle;

public:

	bool collisionx;
	bool collisiony;

	bool hithead;
	bool onfloor;
	bool colliright;
	bool collileft;

	bool nikoCollision(short int obj1x, short int obj1y, short int obj1w, short int obj1h, short int obj2x, short int obj2y, short int obj2w, short int obj2h);
	unsigned char nikoSector(short int obj1x, short int obj1y, short int obj1w, short int obj1h, short int obj2x, short int obj2y, short int obj2w, short int obj2h);
	void nikoUnmoveObject(short int ox, short int oy, short int ow, short int oh, int& charx, int& chary, int charw, int charh, short int charoffsetx, short int charoffsety);
	void nikoUpdateSides();
};


#endif