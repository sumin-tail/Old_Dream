#include "iGraphics.h"

#include "iStd.h"
#include "iWindow.h"
using namespace Gdiplus;

iGraphics::iGraphics()
{
	bmp = NULL;
	g = NULL;
}

iGraphics::~iGraphics()
{
	clean();
}

ULONG_PTR iGraphics::startGdiplus()
{
	GdiplusStartupInput gdiplusStartupInput;
	ULONG_PTR gdiplusToken;
	GdiplusStartup(&gdiplusToken, &gdiplusStartupInput, NULL);
	return gdiplusToken;
}
void iGraphics::stopGdiplus(ULONG_PTR token)
{
	GdiplusShutdown(token);
}


void iGraphics::init(iSize size)
{
	init(size.width, size.height);
	clear(iColor4fMake(0, 0, 0, 0));
}

void iGraphics::init(int width, int height)
{
	clean();

	bmp = new Bitmap(width, height, PixelFormat32bppARGB);
	g = Graphics::FromImage(bmp);
	initGraphics(g);
}

void iGraphics::clean()
{
	if (bmp)
		delete bmp;
	if (g)
		delete g;
}

void iGraphics::clear(iColor4f c)
{
	if (g == NULL)
		return;
	g->Clear(Color(	c.a * 0xff,
					c.r * 0xff,
					c.g * 0xff,
					c.b * 0xff));
}

Texture* iGraphics::getTexture()
{
	uint32 width, height;
	uint8* rgba = bmp2rgba(bmp, width, height);

	Texture* tex = createImageWithRGBA(width, height, rgba);
	delete rgba;

	//delete bmp; bmp = NULL;
	//delete g; g = NULL;

	return tex;
}

void iGraphics::initGraphics(Graphics* g)
{
	g->SetPageUnit(UnitPixel);
	//g->SetPageScale(1.0f);
	g->SetPixelOffsetMode(PixelOffsetModeHalf);
	g->SetTextRenderingHint(TextRenderingHintClearTypeGridFit);
	g->SetTextContrast(0xffffffff);
	g->SetCompositingMode(CompositingModeSourceOver);
	g->SetCompositingQuality(CompositingQualityAssumeLinear);
	g->SetSmoothingMode(SmoothingModeAntiAlias8x8);
	g->SetInterpolationMode(InterpolationModeBicubic);
}

void iGraphics::setClip(int x, int y, int w, int h)
{
	Rect rt(x, y, w, h);
	g->SetClip(rt);
}

void iGraphics::drawLine(iPoint sp, iPoint ep)
{
	drawLine(sp.x, sp.y, ep.x, ep.y);
}
void iGraphics::drawLine(float x0, float y0, float x1, float y1)
{
	float ca, cr, cg, cb;
	getRGBA(cr, cg, cb, ca);
	Pen pen(Color(	ca * 0xFF,
					cr * 0xFF,
					cg * 0xFF,
					cb * 0xFF), getLineWidth());
	g->DrawLine(&pen, x0, y0, x1, y1);

}

void iGraphics::drawRect(iRect rt, float radius)
{
	drawRect(rt.origin.x, rt.origin.y, rt.size.width, rt.size.height, radius);
}
void iGraphics::drawRect(float x, float y, float width, float height, float radius)
{
	GraphicsPath path;
	path.AddLine(x + radius, y, x + width - (radius * 2), y);
	path.AddArc(x + width - (radius * 2), y, radius * 2, radius * 2, 270, 90);
	path.AddLine(x + width, y + radius, x + width, y + height - (radius * 2));
	path.AddArc(x + width - (radius * 2), y + height - (radius * 2), radius * 2, radius * 2, 0, 90);
	path.AddLine(x + width - (radius * 2), y + height, x + radius, y + height);
	path.AddArc(x, y + height - (radius * 2), radius * 2, radius * 2, 90, 90);
	path.AddLine(x, y + height - (radius * 2), x, y + radius);
	path.AddArc(x, y, radius * 2, radius * 2, 180, 90);
	path.CloseFigure();

	float ca, cr, cg, cb;
	getRGBA(cr, cg, cb, ca);
	Pen pen(Color(	ca * 0xFF,
					cr * 0xFF,
					cg * 0xFF,
					cb * 0xFF), getLineWidth());
	g->DrawPath(&pen, &path);
}
void iGraphics::fillRect(iRect rt, float radius)
{
	fillRect(rt.origin.x, rt.origin.y, rt.size.width, rt.size.height, radius);
}
void iGraphics::fillRect(float x, float y, float width, float height, float radius)
{
	GraphicsPath path;
	path.AddLine(x + radius, y, x + width - (radius * 2), y);
	path.AddArc(x + width - (radius * 2), y, radius * 2, radius * 2, 270, 90);
	path.AddLine(x + width, y + radius, x + width, y + height - (radius * 2));
	path.AddArc(x + width - (radius * 2), y + height - (radius * 2), radius * 2, radius * 2, 0, 90);
	path.AddLine(x + width - (radius * 2), y + height, x + radius, y + height);
	path.AddArc(x, y + height - (radius * 2), radius * 2, radius * 2, 90, 90);
	path.AddLine(x, y + height - (radius * 2), x, y + radius);
	path.AddArc(x, y, radius * 2, radius * 2, 180, 90);
	path.CloseFigure();

	float ca, cr, cg, cb;
	getRGBA(cr, cg, cb, ca);
	SolidBrush brush(Color(	ca * 0xFF,
							cr * 0xFF,
							cg * 0xFF,
							cb * 0xFF));
	g->FillPath(&brush, &path);
}

igImage* iGraphics::createIgImage(const char* szFormat, ...)
{
	char szText[256];
	va_start_end(szFormat, szText);

	wchar_t* ws = utf8_to_utf16(szText);
	Image* i = new Image(ws);
	delete ws;

	return i;
}

// stride = bytesPerPixels * rawLength
//			gdi+				opengl/dx/metal
// 3   = 3 + 1(dummy) => 4		=> 4
// 122 = 122 + 2(dummy => 124	=> 128(+6)
uint8* iGraphics::bmp2rgba(Bitmap* bmp, uint32& width, uint32& height)
{
	width = bmp->GetWidth();
	height = bmp->GetHeight();
	Rect rt(0, 0, width, height);
	BitmapData bmpData;
	bmp->LockBits(&rt, ImageLockModeRead, PixelFormat32bppARGB, &bmpData);

	uint8* bgra = (uint8*)bmpData.Scan0;				// stride * height * 4
	int potWidth = nextPOT(width);
	int potHeight = nextPOT(height);
	uint8* rgba = new uint8[potWidth * potHeight * 4];	// width * height * 4
	memset(rgba, 0x00, potWidth * potHeight * 4);
	for (int j = 0; j < height; j++)
	{
		for (int i = 0; i < width; i++)
		{
			uint8* dst = &rgba[potWidth * 4 * j + 4 * i];
			uint8* src = &bgra[bmpData.Stride * j + 4 * i];
			dst[2] = src[0];
			dst[1] = src[1];
			dst[0] = src[2];
			dst[3] = src[3];
		}
	}

	bmp->UnlockBits(&bmpData);

	return rgba;
}

igImage** iGraphics::createIgImageDivide(int numX, int numY, const char* szFormat, ...)
{
	char szText[256];
	va_start_end(szFormat, szText);

	wchar_t* ws = utf8_to_utf16(szText);
	Bitmap* bmp = Bitmap::FromFile(ws);
	delete ws;

	uint32 width, height;
	uint8* rgba = bmp2rgba(bmp, width, height);//////////////////////////
	delete bmp;

	igImage** ii = new igImage*[numX * numY];
	int w = width / numX;
	int h = height / numY;
	for (int j = 0; j < numY; j++)
	{
		for (int i = 0; i < numX; i++)
		{
			Bitmap* bmp = new Bitmap(w, h, PixelFormat32bppARGB);
			Rect rt(0, 0, w, h);
			BitmapData bmpData;
			bmp->LockBits(&rt, ImageLockModeWrite, PixelFormat32bppARGB, &bmpData);
			uint8* bgra = (uint8*)bmpData.Scan0;
			for (int n = 0; n < h; n++)
			{
				for (int m = 0; m < w; m++)
				{
					uint8* dst = &bgra[bmpData.Stride * n + 4 * m];
					uint8* src = &rgba[width * 4 * (h * j + n) + 4 * (w * i + m)];
					dst[0] = src[2];
					dst[1] = src[1];
					dst[2] = src[0];
					dst[3] = src[3];
				}
			}
			bmp->UnlockBits(&bmpData);

			ii[numX * j + i] = bmp;
		}
	}
	delete rgba;//////////////////////////

	return ii;
}

void iGraphics::freeIgImage(igImage* tex)
{
	delete tex;
}

void iGraphics::drawIgImage(igImage* tex, iPoint p, int anc)
{
	drawIgImage(tex, p.x, p.y, 1.0f, 1.0f, anc,
		0, 0, tex->GetWidth(), tex->GetHeight(),
		2, 0);
}
void iGraphics::drawIgImage(igImage* tex, float x, float y, int anc)
{
	drawIgImage(tex, x, y, 1.0f, 1.0f, anc,
		0, 0, tex->GetWidth(), tex->GetHeight(),
		2, 0);
}
void iGraphics::drawIgImage(igImage* tex, float x, float y, float ratX, float ratY, int anc)
{
	drawIgImage(tex, x, y, ratX, ratY, anc,
		0, 0, tex->GetWidth(), tex->GetHeight(),
		2, 0);
}
void iGraphics::drawIgImage(igImage* tex, float x, float y, float ratX, float ratY, int anc,// dst
	int ix, int iy, int iw, int ih,// src
	int xyz, int degree, int reverse)
{
	Image* img = tex;
	float dx = iw * ratX;
	float dy = ih * ratY;

	switch (anc) {
	case TOP | LEFT:							break;
	case TOP | RIGHT:		x -= dx;			break;
	case TOP | HCENTER:		x -= dx / 2;		break;
	case BOTTOM | LEFT:					 y -= dy; break;
	case BOTTOM | RIGHT:	x -= dx;	 y -= dy; break;
	case BOTTOM | HCENTER:	x -= dx / 2; y -= dy; break;
	case VCENTER | LEFT:				 y -= dy / 2; break;
	case VCENTER | RIGHT:	x -= dx;	 y -= dy / 2; break;
	case VCENTER | HCENTER:	x -= dx / 2; y -= dy / 2; break;
	}
#if 0
	RectF dstRect = { x, y, dx, dy };
	RectF srcRect = { ix, iy, iw, ih };
	graphics->DrawImage(img, dstRect, srcRect, UnitPixel);
#endif
	iPoint dstPoint[3] = {
		{x, y}, {x + dx, y}, { x, y + dy}
	};
	if (degree)
	{
		if (xyz == 0)// x축으로 회전
		{
			dstPoint[0].y =
				dstPoint[1].y = y + dy / 2 - dy / 2 * _cos(degree);
			dstPoint[2].y = y + dy / 2 + dy / 2 * _cos(degree);
		}
		else if (xyz == 1)// y축으로 회전
		{
			dstPoint[0].x =
				dstPoint[2].x = x + dx / 2 - dx / 2 * _cos(degree);
			dstPoint[1].x = x + dx / 2 + dx / 2 * _cos(degree);
		}
		else// if (xyz == 2)// z축으로 회전
		{
			iPoint t = iPointMake(x + dx / 2, y + dy / 2);
			for (int i = 0; i < 3; i++)
				dstPoint[i] = iPointRotate(dstPoint[i], t, degree);
		}
	}

	if (reverse & REVERSE_WIDTH)
	{
		float t = dstPoint[0].x;
		dstPoint[0].x = dstPoint[1].x;
		dstPoint[1].x = t;

		dstPoint[2].x = dstPoint[0].x;
	}
	if (reverse & REVERSE_HEIGHT)
	{
		float t = dstPoint[0].y;
		dstPoint[0].y = dstPoint[2].y;
		dstPoint[2].y = t;

		dstPoint[1].y = dstPoint[0].y;
	}

	float cr, cg, cb, ca;
	getRGBA(cr, cg, cb, ca);
	ColorMatrix mat = {
		  cr, 0.0f, 0.0f, 0.0f, 0.0f,
		0.0f,   cg, 0.0f, 0.0f, 0.0f,
		0.0f, 0.0f,   cb, 0.0f, 0.0f,
		0.0f, 0.0f, 0.0f,   ca, 0.0f,
		0.0f, 0.0f, 0.0f, 0.0f, 0.0f,
	};
	ImageAttributes attr;
	attr.SetColorMatrix(&mat, ColorMatrixFlagsDefault, ColorAdjustTypeBitmap);

	g->DrawImage(img, (PointF*)dstPoint, 3, ix, iy, iw, ih, UnitPixel, &attr);
}

void iGraphics::drawString(float x, float y, int anc, const char* szFormat, ...)
{
	char szText[512];
	va_start_end(szFormat, szText);

#if 0
	wchar_t* sn = utf8_to_utf16(stringName);
	FontFamily ff(sn);
	delete sn;
	int fontSytle = FontStyleRegular;
#else
	FontFamily ff;
	int fontStyle;
	RectOfString::checkFontFamily(&ff, fontStyle);
#endif

#if 0
	iSize size = sizeOfString(szText);
#else
	iRect rect = rectOfString(szText);
	x -= rect.origin.x;
	y -= rect.origin.y;
	iSize& size = rect.size;
#endif
	float dx = size.width, dy = size.height;
	switch (anc) {
	case TOP | LEFT:							break;
	case TOP | RIGHT:		x -= dx;			break;
	case TOP | HCENTER:		x -= dx / 2;		break;
	case BOTTOM | LEFT:					 y -= dy; break;
	case BOTTOM | RIGHT:	x -= dx;	 y -= dy; break;
	case BOTTOM | HCENTER:	x -= dx / 2; y -= dy; break;
	case VCENTER | LEFT:				 y -= dy / 2; break;
	case VCENTER | RIGHT:	x -= dx;	 y -= dy / 2; break;
	case VCENTER | HCENTER:	x -= dx / 2; y -= dy / 2; break;
	}

	GraphicsPath path;
	PointF position(x, y);
	StringFormat sf;
	wchar_t* ws = utf8_to_utf16(szText);
	path.AddString(ws, wcslen(ws), &ff, fontStyle, getStringSize(), position, &sf);
	delete ws;

	float cr, cg, cb, ca;
	if (getStringBorder())
	{
		getStringBorderRGBA(cr, cg, cb, ca);
		Pen pen(Color(	ca * 0xFF,
						cr * 0xFF,
						cg * 0xFF,
						cb * 0xFF), getStringBorder());
		g->DrawPath(&pen, &path);
	}
	getStringRGBA(cr, cg, cb, ca);
	SolidBrush brush(Color(	ca * 0xFF,
							cr * 0xFF,
							cg * 0xFF,
							cb * 0xFF));
	g->FillPath(&brush, &path);
}

RectOfString* ros = NULL;
iRect iGraphics::rectOfString(const char* szFormat, ...)
{
	char szText[512];
	va_start_end(szFormat, szText);

	if (ros == NULL)
		ros = new RectOfString();
	return ros->rectOfString(szText);
}

RectOfString::RectOfString()
{
	bmp = new Bitmap(devSize.width, devSize.height, PixelFormat32bppARGB);
	g = Graphics::FromImage(bmp);
}

RectOfString::~RectOfString()
{
	delete bmp;
	delete g;
}

void RectOfString::checkFontFamily(FontFamily* ff, int& fontStyle)
{
	const char* path = getStringName();
	if (path[0] != 'a' ||
		path[1] != 's' ||
		path[2] != 's' ||
		path[3] != 'e')
	{
		wchar_t* sn = utf8_to_utf16(path);
		FontFamily f(sn);
		delete sn;
		memcpy(ff, &f, sizeof(FontFamily));
		fontStyle = FontStyleRegular;
	}
	else
	{
		PrivateFontCollection pfc;

		wchar_t* sn = utf8_to_utf16(path);
		pfc.AddFontFile(sn);
		delete sn;

		int count = pfc.GetFamilyCount();
		int found;
		pfc.GetFamilies(count, ff, &found);

		for (int i = 0; i < found; i++)
		{
			if (ff->IsStyleAvailable(FontStyleRegular))
			{
				fontStyle = FontStyleRegular;
				return;
			}
			else if (ff->IsStyleAvailable(FontStyleBold))
			{
				fontStyle = FontStyleBold;
				return;
			}
		}
	}
}

void RectOfString::drawString(const char* str)
{
	g->Clear(Color(0, 0, 0, 0));

#if 0
	wchar_t* sn = utf8_to_utf16(stringName);
	FontFamily ff(sn);
	delete sn;
	int fontSytle = FontStyleRegular;
#else
	FontFamily ff;
	int fontStyle;
	checkFontFamily(&ff, fontStyle);
#endif

	GraphicsPath path;
	PointF position(0, 0);
	StringFormat sf;
	wchar_t* ws = utf8_to_utf16(str);
	path.AddString(ws, wcslen(ws), &ff, fontStyle, getStringSize(), position, &sf);
	delete ws;

	float cr, cg, cb, ca;
	if (getStringBorder())
	{
		getStringBorderRGBA(cr, cg, cb, ca);
		Pen pen(Color(	ca * 0xFF,
						cr * 0xFF,
						cg * 0xFF,
						cb * 0xFF), getStringBorder());
		g->DrawPath(&pen, &path);
	}
	getStringRGBA(cr, cg, cb, ca);
	SolidBrush brush(Color(	ca * 0xFF,
							cr * 0xFF,
							cg * 0xFF,
							cb * 0xFF));
	g->FillPath(&brush, &path);
}

iRect RectOfString::rectOfString(const char* str)
{
	drawString(str);

	int width = bmp->GetWidth();
	int height = bmp->GetHeight();
	Rect rt(0, 0, width, height);
	BitmapData bmpData;
	bmp->LockBits(&rt, ImageLockModeRead, PixelFormat32bppARGB, &bmpData);

	uint8* bgra = (uint8*)bmpData.Scan0;
	// 왼쪽에서 오른쪽 찾아감
	int left = 0;
	for (int i = 0; i < width; i++)
	{
		bool meet = false;
		for (int j = 0; j < height; j++)
		{
			if (bgra[bmpData.Stride * j + 4 * i + 3])
			{
				meet = true;
				break;
			}
		}
		if (meet)
		{
			left = i;
			break;
		}
	}
	// 오른쪽에서 왼쪽으로 찾아감
	int right = width - 1;
	for (int i = width - 1; i > -1; i--)
	{
		bool meet = false;
		for (int j = 0; j < height; j++)
		{
			if (bgra[bmpData.Stride * j + 4 * i + 3])
			{
				meet = true;
				break;
			}
		}
		if (meet)
		{
			right = i;
			break;
		}
	}
	// 위쪽에서 아래쪽으로 찾아감
	int top = 0;
	for (int j = 0; j < height; j++)
	{
		bool meet = false;
		for (int i = 0; i < width; i++)
		{
			if (bgra[bmpData.Stride * j + 4 * i + 3])
			{
				meet = true;
				break;
			}
		}
		if (meet)
		{
			top = j;
			break;
		}
	}
	// 아래쪽에서 위쪽으로 찾아감
	int bottom = height - 1;
	for (int j = height - 1; j > -1; j--)
	{
		bool meet = false;
		for (int i = 0; i < width; i++)
		{
			if (bgra[bmpData.Stride * j + 4 * i + 3])
			{
				meet = true;
				break;
			}
		}
		if (meet)
		{
			bottom = j;
			break;
		}
	}

	bmp->UnlockBits(&bmpData);

	return iRectMake(left, top, right - left + 1, bottom - top + 1);
}
#if 0
iSize iGraphics::sizeOfString(const char* szFormat, ...)
{
	char szText[512];
	va_start_end(szFormat, szText);

	wchar_t* ws = utf8_to_utf16(szText);
	wchar_t* sn = utf8_to_utf16(getStringName());
	Font font(sn, getStringSize(), FontStyleRegular);
	delete sn;
	PointF position(0, 0);
	RectF rt;
	g->MeasureString(ws, wcslen(ws), &font, position, &rt);
	delete ws;

	return iSizeMake(rt.Width, rt.Height);
}
#endif