#include "iPoint.h"

#include "iStd.h"

iPoint::iPoint()
{
	x = 0;
	y = 0;
}

iPoint::iPoint(const iPoint& p)
{
	x = p.x;
	y = p.y;
}

iPoint::iPoint(float x, float y)
{
	this->x = x;
	this->y = y;
}

bool iPoint::operator == (const iPoint& p)
{
	return x == p.x && y == p.y;
}

bool iPoint::operator != (const iPoint& p)
{
	return x != p.x || y != p.y;
}

iPoint& iPoint::operator = (const iPoint& p)
{
	x = p.x;
	y = p.y;
	return *this;
}

//iPoint& iPoint::operator = (const iSize& s)
//{
//	x = s.width;
//	y = s.height;
//	return *this;
//}


iPoint iPoint::operator + (const iPoint& p)
{
	iPoint r;
	r.x = x + p.x;
	r.y = y + p.y;
	return r;
}
iPoint& iPoint::operator += (const iPoint& p)
{
	x += p.x;
	y += p.y;
	return *this;
}

iPoint iPoint::operator - (const iPoint& p)
{
	iPoint r;
	r.x = x - p.x;
	r.y = y - p.y;
	return r;
}
iPoint& iPoint::operator -= (const iPoint& p)
{
	x -= p.x;
	y -= p.y;
	return *this;
}

iPoint iPoint::operator * (float s)
{
	iPoint r;
	r.x = x * s;
	r.y = y * s;
	return r;
}
iPoint& iPoint::operator *= (float s)
{
	x *= s;
	y *= s;
	return *this;
}

iPoint iPoint::operator / (float s)
{
	iPoint r;
	r.x = x / s;
	r.y = y / s;
	return r;
}
iPoint& iPoint::operator /= (float s)
{
	x /= s;
	y /= s;
	return *this;
}

float iPoint::length()
{
	return sqrtf(x * x + y * y);
}

iPoint iPointMake(float x, float y)
{
	iPoint r;
	r.x = x;
	r.y = y;
	return r;
}

float iPointLength(iPoint p)
{
	return sqrtf(p.x * p.x + p.y * p.y);
}

iPoint iPointRotate(iPoint p, iPoint t, float degree)
{
	while (degree < 0) degree += 360;
	while (degree > 360) degree -= 360;
	float sinr = _sin(degree);
	float cosr = _cos(degree);

	iPoint r;
	r.x = t.x + (p.x - t.x)*cosr - (p.y - t.y)*sinr;
	r.y = t.y + (p.x - t.x)*sinr + (p.y - t.y)*cosr;

	return r;
}

float linear(float r, float s, float e)
{
	return s + (e - s) * r;
}

iPoint linear(float r, iPoint s, iPoint e)
{
	return s + (e - s) * r;
}

float easeIn(float r, float s, float e)
{
	//return s + (e - s) * _sin(90 * r);
	return s + (e - s) * _sin( 90 * _sin(90 * r) );
}

iPoint easeIn(float r, iPoint s, iPoint e)
{
	//return s + (e - s) * _sin(90 * r);
	return s + (e - s) * _sin(90 * _sin(90 * r));
}

float easeOut(float r, float s, float e)
{
	//return s + (e - s) * (1.0f - _cos(90 * r));
	return s + (e - s) * (r * r * r);
}

iPoint easeOut(float r, iPoint s, iPoint e)
{
	//return s + (e - s) * (1.0f - _cos(90 * r));
	return s + (e - s) * (r * r * r);
}

float easeInOut(float r, float s, float e)
{
	if (r < 0.5f)
		return easeIn(r * 2, s, s + (e - s) / 2);
	return easeOut((r - 0.5f) * 2, s + (e - s) / 2, e);
}

iPoint easeInOut(float r, iPoint s, iPoint e)
{
	if (r < 0.5f)
		return easeIn(r * 2, s, s + (e - s) / 2);
	return easeOut((r - 0.5f) * 2, s + (e - s) / 2, e);
}

bool movePoint(iPoint& p, iPoint sp, iPoint ep, float speedDt)
{
	iPoint v = (ep - sp);
	v /= iPointLength(v);

	//p += v * (_speed * dt);
	v *= speedDt;

	if (p.x < ep.x)
	{
		p.x += v.x;
		if (p.x > ep.x)
			p.x = ep.x;
	}
	else if (p.x > ep.x)
	{
		p.x += v.x;
		if (p.x < ep.x)
			p.x = ep.x;
	}

	if (p.y < ep.y)
	{
		p.y += v.y;
		if (p.y > ep.y)
			p.y = ep.y;
	}
	else if (p.y > ep.y)
	{
		p.y += v.y;
		if (p.y < ep.y)
			p.y = ep.y;
	}

	return p == ep;
}