#include "iPopup.h"

#include "iStd.h"

static Texture* texPop = NULL;

iPopup::iPopup()
{
	imgs = new iArray(cb);
	style = iPopupStyleAlpha;
	state = iPopupStateClose;
	openPoint = closePoint = iPointZero;
	aniDt = 0.0f;
	_aniDt = default_popDt;
	bShow = false;
	methodOpen = NULL;
	methodClose = NULL;
	methodDrawBefore = NULL;
	methodDrawAfter = NULL;
	selected = -1;

	if (texPop == NULL)
		texPop = createTexture(devSize.width, devSize.height);
}

iPopup::~iPopup()
{
	delete imgs;
}

void iPopup::cb(void* data)
{
	iImage* img = (iImage*)data;
	delete img;
}

void iPopup::paint(float dt)
{
	if (bShow == false)
		return;

	iPoint p = closePoint;
	float scaleX = 1.0f, scaleY = 1.0f;
	float alpha = 1.0f, degree = 0.0f;

	if (style == iPopupStyleAlpha)
	{
		p = closePoint;
		if (state == iPopupStateOpen)
		{
			alpha = aniDt / _aniDt;
			printf("000alpha = %f\n", alpha);
			aniDt += dt;
			if (aniDt > _aniDt)
			{
				state = iPopupStateProc;
				if (methodOpen)
					methodOpen(this);
			}
		}
		else if (state == iPopupStateProc)
		{
			alpha = 1.0f;
		}
		else// if (state == iPopupStateClose)
		{
			alpha = 1.0f - aniDt / _aniDt;
			printf("111alpha = %f\n", alpha);
			aniDt += dt;
			if (aniDt > _aniDt)
			{
				bShow = false;
				if (methodClose)
					methodClose(this);
				return;
			}
		}
	}
	else if (style == iPopupStyleMove)
	{
		if (state == iPopupStateOpen)
		{
			p = linear(aniDt / _aniDt, openPoint, closePoint);
			aniDt += dt;
			if (aniDt > _aniDt)
			{
				state = iPopupStateProc;
				if (methodOpen)
					methodOpen(this);
			}
		}
		else if (state == iPopupStateProc)
		{
			p = closePoint;
		}
		else// if (state == iPopupStateClose)
		{
			p = linear(aniDt / _aniDt, closePoint, openPoint);
			aniDt += dt;
			if (aniDt > _aniDt)
			{
				bShow = false;
				if (methodClose)
					methodClose(this);
				return;
			}
		}
	}
	else if (style == iPopupStyleAlphaMove)
	{
		if (state == iPopupStateOpen)
		{
			alpha = aniDt / _aniDt;
			p = linear(alpha, openPoint, closePoint);
			aniDt += dt;
			if (aniDt > _aniDt)
			{
				state = iPopupStateProc;
				if (methodOpen)
					methodOpen(this);
			}
		}
		else if (state == iPopupStateProc)
		{
			alpha = 1.0f;
			p = closePoint;
		}
		else// if (state == iPopupStateClose)
		{
			alpha = 1.0f - aniDt / _aniDt;
			p = linear(alpha, openPoint, closePoint);
			aniDt += dt;
			if (aniDt > _aniDt)
			{
				bShow = false;
				if (methodClose)
					methodClose(this);
				return;
			}
		}
	}
	else if (style == iPopupStyleZoom)
	{
		if (state == iPopupStateOpen)
		{
			scaleX = scaleY = aniDt / _aniDt;
			p = linear(scaleX, openPoint, closePoint);
			aniDt += dt;
			if (aniDt > _aniDt)
			{
				state = iPopupStateProc;
				if (methodOpen)
					methodOpen(this);
			}
		}
		else if (state == iPopupStateProc)
		{
			scaleX = scaleY = 1.0f;
			p = closePoint;
		}
		else// if (state == iPopupStateClose)
		{
			scaleX = scaleY = 1.0f - aniDt / _aniDt;
			p = linear(scaleX, openPoint, closePoint);
			aniDt += dt;
			if (aniDt > _aniDt)
			{
				bShow = false;
				if (methodClose)
					methodClose(this);
				return;
			}
		}
	}
	else if (style == iPopupStyleTong)
	{
		if (state == iPopupStateOpen)
		{
		}
		else if (state == iPopupStateProc)
		{
		}
		else// if (state == iPopupStateClose)
		{
		}
	}
	else if (style == iPopupStyleRotate)
	{
	if (state == iPopupStateOpen)
	{
		scaleX = scaleY = aniDt / _aniDt;
		p = linear(scaleX, openPoint, closePoint);
		degree = 360 * scaleX;
		aniDt += dt;
		if (aniDt > _aniDt)
		{
			state = iPopupStateProc;
			if (methodOpen)
				methodOpen(this);
		}
	}
	else if (state == iPopupStateProc)
	{
		scaleX = scaleY = 1.0f;
		p = closePoint;
		degree = 0;
	}
	else// if (state == iPopupStateClose)
	{
		scaleX = scaleY = 1.0f - aniDt / _aniDt;
		p = linear(scaleX, openPoint, closePoint);
		degree = 360 * scaleX;
		aniDt += dt;
		if (aniDt > _aniDt)
		{
			bShow = false;
			if (methodClose)
				methodClose(this);
			return;
		}
	}
	}

	if (methodDrawBefore)
		methodDrawBefore(dt, this);

	fbo->bind(texPop);///////////////////////
	iFBO::clear(0, 0, 0, 0);
	setRGBA(1, 1, 1, 1);
	for (int i = 0; i < imgs->count; i++)
	{
		iImage* img = (iImage*)imgs->objectAtIndex(i);
		img->paint(dt, iPointZero);
	}
	fbo->unbind();///////////////////////

	//setGlBlendFunc(iBlendFuncPremultipliedAlpha);
	setRGBA(1, 1, 1, alpha);
	if (degree == 0.0f)
	{
		drawImage(texPop, p.x, p.y, scaleX, scaleY, TOP | LEFT,
			0, 0, texPop->width, texPop->height, 2, degree, REVERSE_HEIGHT);
	}
	else
	{
#if 1// #issue
		drawImage(texPop, p.x, p.y, scaleX, scaleY, TOP | LEFT,
			0, 0, 120, 180, 2, degree, REVERSE_HEIGHT);
#endif
	}
	setRGBA(1, 1, 1, 1);
	//wsetGlBlendFunc(iBlendFuncAlpha);

	if (methodDrawAfter)
		methodDrawAfter(dt, this);
}

void iPopup::addObject(iImage* img)
{
	imgs->addObject(img);
}

void iPopup::show(bool show)
{
	if (show)
	{
		bShow = true;
		state = iPopupStateOpen;
	}
	else
	{
		state = iPopupStateClose;
	}
	aniDt = 0.0f;
}
