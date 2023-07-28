#include "nikoMath.h"

extern SDL_Window* window;
extern SDL_Renderer* renderer;
extern SDL_Event window_key;

float NIKO_Math::nikoLineDistance(float ax, float ay, float bx, float by)
{
	return std::sqrt(((bx - ax) * (bx - ax)) + ((by - ay) * (by - ay)));
}

void NIKO_Math::niko2DLine(SDL_Point points[], int numPoints, int red, int green, int blue, int alpha)
{
	SDL_SetRenderDrawColor(renderer, red, green, blue, alpha);
	SDL_RenderDrawLines(renderer, points, numPoints);
}

void NIKO_Math::nikoRectangle(int x, int y, int width, int height, SDL_Color textColor, int alpha, bool fill)
{


	SDL_Rect Rectangle;

	Rectangle.x = x;
	Rectangle.y = y;
	Rectangle.w = width;
	Rectangle.h = height;

	SDL_SetRenderDrawColor(renderer, textColor.r, textColor.g, textColor.b, alpha);

	if (!fill)
		SDL_RenderDrawRect(renderer, &Rectangle);

	if (fill)
		SDL_RenderFillRect(renderer, &Rectangle);


}

float NIKO_Math::nikoLawCosineSSS(float a, float b, float c, bool radians)
{
	/* radians */
	if (radians)
		return std::acos(((a * a) + (b * b) - (c * c)) / (2 * a * b));

	/* degrees */
	return (std::acos(((a * a) + (b * b) - (c * c)) / (2 * a * b))) * (180 / M_PI);
}

double NIKO_Math::nikoArcTangent(float p1x, float p1y, float p2x, float p2y)
{
	double PI { 3.14159265 };
	[[maybe_unused]] float angle { 0 };

	if (angle < 0)
		angle = angle + 360;
	else
		angle = atan2(p1y - p2y, p1x - p2x) * 180 / PI;

	return angle;
}

void NIKO_Math::nikoBubbleSortVector(std::vector<int>& vect)
{
	int i, j;
	bool swapped;
	for (i = 0; i < vect.size() - 1; i++)
	{
		swapped = false;
		for (j = 0; j < vect.size() - i - 1; j++)
		{
			if (vect[j] > vect[j + 1])
			{
				swapf(&vect[j], &vect[j + 1]);
				swapped = true;
			}
		}

		// IF no two elements were swapped by inner loop, then break 
		if (swapped == false)
			break;
	}
}

float  NIKO_Math::nikoIntMonoMod(float number1, float number2)
{
	float bucket = number1 > number2 ? fmod(number1, number2) : fmod(number2, number1);

	return bucket;
}

bool NIKO_Math::nikoInRange(int low, int high, int number)
{
	return ((number - high) * (number - low) <= 0);
}

int NIKO_Math::getPt(int n1, int n2, float perc)
{
	int diff = n2 - n1;

	return n1 + (diff * perc);
}

void NIKO_Math::drawcurve(SDL_Point point1, SDL_Point point2, SDL_Point point3)
{
	for (float i = 0; i < 1; i += 0.01)
	{
		int xa = getPt(point1.x, point2.x, i);
		int ya = getPt(point1.y, point2.y, i);
		int xb = getPt(point2.x, point3.x, i);
		int yb = getPt(point2.y, point3.y, i);

		int x = getPt(xa, xb, i);
		int y = getPt(ya, yb, i);

		nikoRectangle(x, y, 2, 2, { 255, 0, 0 }, 255, true);
	}

}