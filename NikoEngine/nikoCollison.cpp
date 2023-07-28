#include "nikoCollision.h"

bool NIKO_Collision::nikoCollision(short int obj1x, short int obj1y, short int obj1w, short int obj1h, short int obj2x, short int obj2y, short int obj2w, short int obj2h)
{

	if (obj1x < obj2x + obj2w &&
		obj1x + obj1w > obj2x &&
		obj1y < obj2y + obj2h &&
		obj1h + obj1y > obj2y)

		return true;

	return false;
}

unsigned char NIKO_Collision::nikoSector(short int obj1x, short int obj1y, short int obj1w, short int obj1h, short int obj2x, short int obj2y, short int obj2w, short int obj2h)
{

	short int cent1x = obj1x + obj1w / 2;
	short int cent1y = obj1y + obj1h / 2;

	short int cent2x = obj2x + obj2w / 2;
	short int cent2y = obj2y + obj2h / 2;

	unsigned char sector = 0;

	//top left
	if (cent1x <= cent2x && cent1y <= cent2y)
		sector += 0x01;

	//top right
	if (cent1x > cent2x && cent1y <= cent2y)
		sector += 0x11;

	//bottom right
	if (cent1x > cent2x && cent1y > cent2y)
		sector += 0x10;

	//bottom left
	if (cent1x <= cent2x && cent1y >= cent2y)
		sector += 0x00;

	return sector;

}

void NIKO_Collision::nikoUnmoveObject(short int ox, short int oy, short int ow, short int oh, int& charx, int& chary, int charw, int charh, short int charoffsetx, short int charoffsety)
{


	//square 1
	short int point5x = ox;
	short int point6x = ox + ow;
	short intpoint7x = ox + ow;
	short intpoint8x = ox;

	short int point5y = oy;
	short int point6y = oy;
	short int point7y = oy + oh;
	short int point8y = oy + oh;

	//square 2
	short int point1x = charx + charoffsetx;
	short int point2x = charx + charw + charoffsetx;
	short int point3x = charx + charw + charoffsetx;
	short int point4x = charx + charoffsetx;

	short int point1y = chary + charoffsety;
	short int point2y = chary + charoffsety;
	short int point3y = chary + charh + charoffsety;
	short int point4y = chary + charh + charoffsety;

	short int cent1x;
	short int cent1y;
	short int cent2x;
	short int cent2y;

	//find the quadrant the rectangle is located in relationship to the other rectangle

	if (nikoSector(charx + charoffsetx, chary + charoffsety, charw, charh, ox, oy, ow, oh) == 0x01)
	{
		cent1x = charx + charw + charoffsetx;
		cent1y = chary + charh + charoffsety;
		cent2x = ox;
		cent2y = oy;
	}

	else if (nikoSector(charx + charoffsetx, chary + charoffsety, charw, charh, ox, oy, ow, oh) == 0x11)
	{
		cent1x = charx + charoffsetx;
		cent1y = chary + charh + charoffsety;
		cent2x = ox + ow;
		cent2y = oy;
	}

	else if (nikoSector(charx + charoffsetx, chary + charoffsety, charw, charh, ox, oy, ow, oh) == 0x10)
	{
		cent1x = charx + charoffsetx;
		cent1y = chary + charoffsety;
		cent2x = ox + ow;
		cent2y = oy + oh;
	}

	else if (nikoSector(charx + charoffsetx, chary + charoffsety, charw, charh, ox, oy, ow, oh) == 0x00)
	{
		cent1x = charx + charw + charoffsetx;
		cent1y = chary + charoffsety;
		cent2x = ox;
		cent2y = oy + oh;
	}



	if (nikoCollision(charx + charoffsetx, chary + charoffsety, charw, charh, ox, oy, ow, oh) == true)
	{
		if (triangle.nikoLawCosineSSS(triangle.nikoLineDistance(cent1x, 0, cent2x, 0),
			triangle.nikoLineDistance(cent1x, cent1y, cent2x, cent2y),
			triangle.nikoLineDistance(0, cent1y, 0, cent2y), false) == 45)
		{
			if (charx + charoffsetx > ox)
				charx += -(point1x - point6x);
			else if (charx + charw + charoffsetx < ox + ow)
				charx -= point2x - point5x;
			if (chary + charoffsety < oy)
				chary -= point3y - point6y;
			else if (chary + charoffsety - charh > oy - oh)
				chary += -(point1y - point8y);
		}

		else
		{
			if (triangle.nikoLawCosineSSS(triangle.nikoLineDistance(cent1x, 0, cent2x, 0),
				triangle.nikoLineDistance(cent1x, cent1y, cent2x, cent2y),
				triangle.nikoLineDistance(0, cent1y, 0, cent2y), false) > 45)
			{
				if (charx + charoffsetx > ox)
				{
					collileft = true;
					collisionx = true;
					charx += -(point1x - point6x);

				}
				else if (charx + charoffsetx < ox)
				{
					colliright = true;
					collisionx = true;
					charx -= point2x - point5x;
				}
			}

			else if (triangle.nikoLawCosineSSS(triangle.nikoLineDistance(cent1x, 0, cent2x, 0),
				triangle.nikoLineDistance(cent1x, cent1y, cent2x, cent2y),
				triangle.nikoLineDistance(0, cent1y, 0, cent2y), false) < 45)
			{
				if (chary + charoffsety < oy)
				{
					onfloor = true;
					collisiony = true;
					chary -= point3y - point6y;
				}
				else if (chary + charoffsety > oy)
				{
					hithead = true;
					collisiony = true;
					chary += -(point1y - point8y);
				}
			}
		}
	}

}

void NIKO_Collision::nikoUpdateSides()
{
	collisionx = false;
	collisiony = false;
	hithead = false;
	onfloor = false;
	collileft = false;
	colliright = false;
}