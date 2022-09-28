#pragma once

#include "iCommon.h"
#include <stdio.h>

typedef Texture* (*MethodST)(const char* str);

class iStrTex
{
public:
	iStrTex(MethodST m=NULL);
	virtual ~iStrTex();

	iStrTex& operator = (const char* s);
	iStrTex& operator = (const iStrTex& s);

	void setString(const char* szFormat, ...);
	void drawString(float x, float y, int anc, const char* szFormat, ...);

public:
	char* str;
	int strLength;
	Texture* tex;
	MethodST method;
};

