#include "Utils.h";

float lerp(float a, float b, float f)
{
	return (a * (1.0 - f)) + (b * f);
}