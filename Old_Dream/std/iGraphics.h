#pragma once

#include <Windows.h>
#include <gdiplus.h>
#pragma comment(lib, "Gdiplus.lib")

#include "iSize.h"
#include "iColor.h"

typedef Gdiplus::Image igImage;

class iGraphics
{
public:
	iGraphics();
	virtual ~iGraphics();

	static ULONG_PTR startGdiplus();
	static void stopGdiplus(ULONG_PTR token);

	void init(iSize size);
	void init(int width, int height);
	void clean();
	void clear(iColor4f c);
	Texture* getTexture();

	static void initGraphics(Gdiplus::Graphics* g);
	
	void setClip(int x, int y, int w, int h);

	void drawLine(iPoint sp, iPoint ep);
	void drawLine(float x0, float y0, float x1, float y1);

	void drawRect(iRect rt, float radius = 0.0f);
	void drawRect(float x, float y, float width, float height, float radius = 0.0f);
	void fillRect(iRect rt, float radius = 0.0f);
	void fillRect(float x, float y, float width, float height, float radius = 0.0f);

	static uint8* bmp2rgba(Gdiplus::Bitmap* bmp, uint32& width, uint32& height);

	igImage* createIgImage(const char* szFormat, ...);
	igImage** createIgImageDivide(int numX, int numY, const char* szFormat, ...);
	void freeIgImage(igImage* tex);
	void drawIgImage(igImage* tex, iPoint p, int anc = TOP | LEFT);
	void drawIgImage(igImage* tex, float x, float y, int anc = TOP | LEFT);
	void drawIgImage(igImage* tex, float x, float y, float ratX, float ratY, int anc = TOP | LEFT);
	void drawIgImage(igImage* tex, float x, float y, float ratX, float ratY, int anc,// dst
		int ix, int iy, int iw, int ih,// src
		int xyz, int degree, int reverse = REVERSE_NONE);

	void drawString(float x, float y, int anc, const char* szFormat, ...);
	static iRect rectOfString(const char* szFormat, ...);
#if 0
	static iSize sizeOfString(const char* szFormat, ...);
#endif
public:
	Gdiplus::Bitmap* bmp;
	Gdiplus::Graphics* g;
};

struct RectOfString
{
	Gdiplus::Bitmap* bmp;
	Gdiplus::Graphics* g;

	RectOfString();
	virtual ~RectOfString();

	static void checkFontFamily(Gdiplus::FontFamily* ff, int& fontStyle);
	void drawString(const char* str);

	iRect rectOfString(const char* str);
};
