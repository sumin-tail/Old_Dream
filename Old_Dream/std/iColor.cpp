#include "iColor.h"

iColor4f iColor4fMake(float r, float g, float b, float a)
{
	iColor4f c;
	c.r = r;
	c.g = g;
	c.b = b;
	c.a = a;
	return c;
}
iColor4b iColor4bMake(uint8 r, uint8 g, uint8 b, uint8 a)
{
	iColor4b c;
	c.r = r;
	c.g = g;
	c.b = b;
	c.a = a;
	return c;
}