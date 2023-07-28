#ifndef NIKO_MATH_H
#define NIKO_MATH_H

#include <SDL.h>
#include <cmath>
#include <vector>
#include <string>


class NIKO_Math
{
private:

	std::string vcolorCache;
	void swapf(int* xp, int* yp)
	{
		int temp = *xp;
		*xp = *yp;
		*yp = temp;
	}

public:

	float nikoLineDistance(float ax, float ay, float bx, float by);
	void niko2DLine(SDL_Point points[], int numPoints, int red, int green, int blue, int alpha);
	void nikoRectangle(int x, int y, int width, int height, SDL_Color textColor, int alpha, bool fill);
	float nikoLawCosineSSS(float a, float b, float c, bool radians);
	double nikoArcTangent(float p1x, float p1y, float p2x, float p2y);
	float nikoIntMonoMod(float number1, float number2);
	bool nikoInRange(int low, int high, int number);
	void nikoBubbleSortVector(std::vector<int>& vect);
	int getPt(int n1, int n2, float perc);
	void drawcurve(SDL_Point point1, SDL_Point point2, SDL_Point point3);

};

#endif
