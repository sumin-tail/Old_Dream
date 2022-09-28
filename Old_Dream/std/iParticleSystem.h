﻿#pragma once

#include "iCommon.h"
#include "iColor.h"
#include "iPoint.h"

struct iParticle
{
	iColor4b colorStart, colorEnd;
	iPoint positionStart, positionEnd;
	float scaleStart, scaleEnd;

	float delta, _delta;

	void draw(float dt);
};

class iParticleSystem
{
public:
	iParticleSystem();
	iParticleSystem(const char* szFormat, ...);
	virtual ~iParticleSystem();

	void paint(float dt);

	void startAnimation(iPoint p);
	void stopAnimation(bool immediately = false);

	void save(const char* fileName);
	void load(const char* fileName);

public:
	iPoint position;

	iParticle* _ptc;
	int ptcMax;

	iParticle** ptc;
	int ptcNum;

	float createDt;

	iPoint positionStartVar, positionEndVar;
	bool crossStartEnd;

	iColor4b colorStart, colorStartVar,
		colorEnd, colorEndVar;

	int scaleStart, scaleStartVar,
		scaleEnd, scaleEndVar;

	float delta, deltaVar;
};


