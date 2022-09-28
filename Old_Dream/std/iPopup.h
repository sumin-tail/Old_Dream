#pragma once

#include "iCommon.h"
#include "iArray.h"
#include "iPoint.h"
#include "iRect.h"
#include "iImage.h"

class iPopup;
typedef void (*MethodPopOpenClose)(iPopup* pop);
typedef void (*MethodPopDraw)(float dt, iPopup* pop);

struct Back;

enum iPopupStyle
{
	iPopupStyleAlpha = 0,
	iPopupStyleMove,
	iPopupStyleAlphaMove,
	iPopupStyleZoom,
	iPopupStyleTong,
	iPopupStyleRotate,
};

enum iPopupState
{
	iPopupStateOpen = 0,
	iPopupStateProc,
	iPopupStateClose
};

class iPopup
{
public:
	iPopup();
	virtual ~iPopup();

	static void cb(void* data);

	void paint(float dt);

	void addObject(iImage* img);

	void show(bool show);

public:
	iArray* imgs;// iImage*

	iPopupStyle style;
	iPopupState state;
	iPoint openPoint, closePoint;
	float aniDt, _aniDt;
	bool bShow;
	int selected;

	MethodPopOpenClose methodOpen, methodClose;
	MethodPopDraw methodDrawBefore, methodDrawAfter;
};

#define default_popDt 0.2f

