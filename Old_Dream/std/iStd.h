#pragma once

#include <gl/glew.h>
#include <gl/GL.h>
#pragma comment(lib, "OpenGL32.lib")

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <stdarg.h>
#include <time.h>
#include <math.h>

#include "iCommon.h"
#include "iPoint.h"
#include "iSize.h"
#include "iRect.h"
#include "iString.h"
// Texture* < iImage* < iPopup* < iPage || iTable
#include "iArray.h"
#include "iImage.h"
#include "iPopup.h"
#include "iGraphics.h"
#include "iStrTex.h"

#include "iUtil.h"
#include "iSort.h"

#include "iParticleSystem.h"
#include "iOpenAL.h"

#define va_start_end(szFormat, strText)  {					\
	va_list args;											\
	va_start(args, szFormat);								\
	vsnprintf(strText, sizeof(strText), szFormat, args);	\
	va_end(args);											\
}

#define keyboard_none	0
#define keyboard_left	1
#define keyboard_right	2
#define keyboard_up		4
#define keyboard_down	8
#define keyboard_space	16
#define keyboard_enter	32
#define keyboard_z		64
#define keyboard_esc	128

void setKeyboard(bool down, int key);
void updateKeyboard();

bool getKeyStat(int key);
bool getKeyDown(int key);

#define DEV_WIDTH 1280
#define DEV_HEIGHT 720
extern iSize devSize;
extern iRect viewport;

// App.cpp
void setZoom(iPoint c, float scale, float zoomDt, float delayDt);

void setRGBA(float r, float g, float b, float a);
void getRGBA(float& r, float& g, float& b, float& a);

void clearScreen();

void setClip(int x, int y, int w, int h);

void setDotSize(float size);
void drawDot(iPoint p);
void drawDot(float x, float y);

void setLineWidth(float width);
float getLineWidth();
void drawLine(iPoint sp, iPoint ep);
void drawLine(float x0, float y0, float x1, float y1);

void drawRect(iRect rt, float radius = 0.0f);
void drawRect(float x, float y, float width, float height, float radius = 0.0f);
void fillRect(iRect rt, float radius = 0.0f);
void fillRect(float x, float y, float width, float height, float radius = 0.0f);

enum TextureWrap {
	CLAMP = 0,	// GL_CLAMP_TO_EDGE
	REPEAT,		// GL_REPEAT
};

enum TextureFilter {
	LINEAR = 0,	// GL_LINEAR
	NEAREST,	// GL_NEAREST
};

void setTexture(TextureFilter filter, TextureWrap wrap = CLAMP);
void setTexture(TextureFilter minFilter, TextureFilter magFilter, TextureWrap wrap = CLAMP);
void applyTexture();


void setTexture(uint32 texID, TextureFilter filter, TextureWrap wrap = CLAMP);
void setTexture(uint32 texID, TextureFilter minFilter, TextureFilter magFilter, TextureWrap wrap = CLAMP);

struct Texture
{
	uint32 texID;
	float width, height;
	uint32 potWidth, potHeight;
	int retainCount;
};

uint32 nextPOT(uint32 x);
Texture* createImageWithRGBA(int width, int height, uint8* rgba);

Texture* createImage(const char* szFormat, ...);
Texture** createImageDivide(int numX, int numY, const char* szFormat, ...);
void freeImage(Texture* tex);
void drawImage(Texture* tex, iPoint p, int anc = TOP | LEFT);
void drawImage(Texture* tex, float x, float y, int anc = TOP | LEFT);
void drawImage(Texture* tex, float x, float y, float ratX, float ratY, int anc = TOP | LEFT);
void drawImage(Texture* tex, float x, float y, float ratX, float ratY, int anc,// dst
	int ix, int iy, int iw, int ih,// src
	int xyz, int degree, int reverse = REVERSE_NONE);

void setStringName(const char* stringName);
const char* getStringName();
void setStringSize(float size);
float getStringSize();
void setStringBorder(float width);
float getStringBorder();
void setStringRGBA(float r, float g, float b, float a);
void getStringRGBA(float& r, float& g, float& b, float& a);
void setStringBorderRGBA(float r, float g, float b, float a);
void getStringBorderRGBA(float& r, float& g, float& b, float& a);
iRect rectOfString(const char* szFormat, ...);
void drawString(float x, float y, int anc, const char* szFormat, ...);

Texture* createTexture(int width, int height);

class iFBO
{
public:
	iFBO(int width, int height);
	virtual ~iFBO();

	static void clear(float r, float g, float b, float a);
	void bind();
	void bind(Texture* tex);
	void unbind();

	void save(const char* path);

public:
	uint32 fbo;

	uint32 depthBuffer;

	// colorBuffer;
	Texture* tex;

	Texture** listTex;
	int listNum;
	int prevFBO;
};

extern iFBO* fbo;

enum iBlendFunc
{
	iBlendFuncAlpha = 0,// 일반적
	iBlendFuncPremultipliedAlpha,// 팝업, 백버퍼
	iBlendFuncAdd // 파티클
};

void setGlBlendFunc(iBlendFunc bf);

