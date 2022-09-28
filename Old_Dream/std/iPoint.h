#pragma once

#include "iSize.h"

struct iPoint
{
	float x, y;

	iPoint();
	iPoint(const iPoint& p);
	iPoint(float x, float y);

	bool operator == (const iPoint& p);
	bool operator != (const iPoint& p);

	iPoint& operator = (const iPoint& p);
	//iPoint& operator = (const iSize& s);

	iPoint operator + (const iPoint& p);
	iPoint& operator += (const iPoint& p);
	//iPoint& operator += (const iSize& s);

	iPoint operator - (const iPoint& p);
	iPoint& operator -= (const iPoint& p);

	iPoint operator * (float s);
	iPoint& operator *= (float s);

	iPoint operator / (float s);
	iPoint& operator /= (float s);

	float length();
};

#define iPointZero iPointMake(0, 0)
iPoint iPointMake(float x, float y);
float iPointLength(iPoint p);
iPoint iPointRotate(iPoint p, iPoint t, float degree);

float linear(float r, float s, float e);
iPoint linear(float r, iPoint s, iPoint e);

float easeIn(float r, float s, float e);
iPoint easeIn(float r, iPoint s, iPoint e);

float easeOut(float r, float s, float e);
iPoint easeOut(float r, iPoint s, iPoint e);

float easeInOut(float r, float s, float e);
iPoint easeInOut(float r, iPoint s, iPoint e);

bool movePoint(iPoint& p, iPoint sp, iPoint ep, float speedDt);
