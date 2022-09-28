#include "iStrTex.h"

#include "iStd.h"

iStrTex::iStrTex(MethodST m)
{
	strLength = 64;
	str = new char[strLength];
	tex = NULL;
	method = m;
}

iStrTex::~iStrTex()
{
	delete str;
	if (tex)
		freeImage(tex);
}

iStrTex& iStrTex::operator = (const char* s)
{
	setString(s);
	return *this;
}

void iStrTex::setString(const char* szFormat, ...)
{
	char szText[512];
	va_start_end(szFormat, szText);

	if (strcmp(str, szText))
	{
		int len = strlen(szText) + 1;
		if (strLength < len)
		{
			delete str;
			str = new char[len];
			strLength = len;
		}

		strcpy(str, szText);

		Texture* t;
		if (method)
			t = method(szText);
		else
		{
			iGraphics* g = new iGraphics();
			iRect rt = rectOfString(str);
			g->init(rt.size);

			g->drawString(0, 0, TOP | LEFT, str);

			t = g->getTexture();
			delete g;
		}

		if (tex == NULL)
		{
			tex = t;
		}
		else// if (tex != NULL)
		{
			glDeleteTextures(1, &tex->texID);
			tex->texID = t->texID;
			tex->width = t->width;
			tex->height = t->height;
			tex->potWidth = t->potWidth;
			tex->potHeight = t->potHeight;
			//tex->retainCount = t->retainCount;
			delete t;
		}
	}
}

void iStrTex::drawString(float x, float y, int anc, const char* szFormat, ...)
{
	char szText[512];
	va_start_end(szFormat, szText);

	setString(szText);
	drawImage(tex, x, y, anc);
}
