#pragma once

#include "iCommon.h"
#include "iArray.h"
#include "iPoint.h"
#include "iRect.h"

typedef void (*cbAnimation)(void* parm);

class iImage
{
public:
	iImage();
	virtual ~iImage();

	static void cb(void* data);

	void paint(float dt, iPoint p);
	void addObject(Texture* tex);

	void startAnimation(cbAnimation m=NULL, void* parm=NULL);
	void stopAnimation();

	iRect rect(iPoint p=iPointZero, iSize s=iSizeZero);
	iPoint center(iPoint p=iPointZero);
	iImage* clone();

public:
	iArray* imgs;// Texture*

	Texture* tex;// imgs[index]
	iPoint position;
	int frame;
	float aniDt, _aniDt;
	bool animation;
	uint8 repeatNum, _repeatNum;
	cbAnimation method;
	void* parm;

	float alpha;
	float degree;
	float scale;
	int reverse;
};

#define default_aniDt 0.18f