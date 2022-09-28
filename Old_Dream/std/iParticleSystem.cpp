#include "iParticleSystem.h"

#include "iStd.h"

void iParticle::draw(float dt)
{
	float r = delta / _delta;
	delta += dt;

	iPoint p = linear(r, positionStart, positionEnd);
	float s = linear(r, scaleStart, scaleEnd);
	iColor4f c;
	c.r = (colorStart.r * (1 - r) + colorEnd.r * r) / 255.f;
	c.g = (colorStart.g * (1 - r) + colorEnd.g * r) / 255.f;
	c.b = (colorStart.b * (1 - r) + colorEnd.b * r) / 255.f;
	c.a = (colorStart.a * (1 - r) + colorEnd.a * r) / 255.f;

	setRGBA(c.r, c.g, c.b, c.a);
	fillRect(p.x - s / 2, p.y - s / 2, s, s);
}

iParticleSystem::iParticleSystem()
{
	ptcMax = 1000;
	positionStartVar = iPointMake(40, 40);
	positionEndVar = iPointMake(400, 400);
	crossStartEnd = false;

	colorStart = iColor4bMake(0, 0, 0, 0);
	colorStartVar = iColor4bMake(255, 255, 255, 255);

	colorEnd = iColor4bMake(0, 0, 0, 0);
	colorEndVar = iColor4bMake(255, 255, 255, 255);

	scaleStart = 3;	scaleStartVar = 2;
	scaleEnd = 3; scaleEndVar = 2;

	delta = 0.1f;
	deltaVar = 0.1f;

	_ptc = new iParticle[ptcMax];
	for (int i = 0; i < ptcMax; i++)
	{
		iParticle* p = &_ptc[i];
		p->colorStart = p->colorEnd = iColor4bMake(0, 0, 0, 0);
		p->positionStart = p->positionEnd = iPointMake(0, 0);
		p->scaleStart = p->scaleEnd = 0.0f;
		p->delta = p->_delta = 0.0f;
	}

	ptc = new iParticle*[ptcMax];
	ptcNum = 0;
}

iParticleSystem::iParticleSystem(const char* szFormat, ...)
{
	char szText[256];
	va_start_end(szFormat, szText);

	_ptc = NULL;
	ptc = NULL;
	load(szText);
}


iParticleSystem::~iParticleSystem()
{
	delete _ptc;

	delete ptc;
}

#define _createDt 0.01f

void iParticleSystem::paint(float dt)
{
	for (int i = 0; i < ptcNum; i++)
	{
		iParticle* p = ptc[i];
		p->draw(dt);

		if (p->delta >= p->_delta)
		{
			ptcNum--;
			ptc[i] = ptc[ptcNum];
			i--;
		}
	}

#if 0	
	createDt += dt;
	while (createDt >= _createDt)
	{
		createDt -= _createDt;

		// 1개의 particle 만들기
		for (int i = 0; i < ptcMax; i++)
		{
			iParticle* p = &_ptc[i];
			if (p->delta < p->_delta) continue;

			p->positionStart = iPointMake(	position.x - 20 + rand() % 40,
											position.y - 10 + rand() % 20);
			p->positionEnd = iPointMake(	position.x - 40 + rand() % 90,
											position.y - 20 + rand() % 40 - 100);
			// rgba(128~255, 64~127, 0, 220~255)
			p->colorStart = iColor4bMake(	128 + rand() % 128,
											64 + rand() % 64,
											0,
											220 + rand() % 36);
			// rgba(0~128, 0~64, 0, 0~64);
			p->colorEnd = iColor4bMake(		rand() % 128,
											rand() % 64,
											0,
											rand() % 64);
			p->scaleStart = 6.0f + rand() % 5;// 6 ~ 10
			p->scaleEnd = rand() % 4;// 0 ~ 3

			p->_delta = 0.2f + (rand() % 60) / 100.f;// 0.2 ~ 0.8
			p->delta = 0.0f;

			ptc[ptcNum] = p;
			ptcNum++;
			break;
		}
	}
#endif
}

void iParticleSystem::save(const char* fileName)
{
	FILE* pf = fopen(fileName, "wb");

	fwrite(&ptcMax, sizeof(int), 1, pf);

	fwrite(&positionStartVar, sizeof(float), 2, pf);
	fwrite(&positionEndVar, sizeof(float), 2, pf);
	fwrite(&crossStartEnd, sizeof(bool), 1, pf);

	fwrite(&colorStart, sizeof(iColor4b), 1, pf);
	fwrite(&colorStartVar, sizeof(iColor4b), 1, pf);

	fwrite(&colorEnd, sizeof(iColor4b), 1, pf);
	fwrite(&colorEndVar, sizeof(iColor4b), 1, pf);

	fwrite(&scaleStart, sizeof(int), 1, pf);
	fwrite(&scaleStartVar, sizeof(int), 1, pf);
	fwrite(&scaleEnd, sizeof(int), 1, pf);
	fwrite(&scaleEndVar, sizeof(int), 1, pf);

	fwrite(&delta, sizeof(float), 1, pf);
	fwrite(&deltaVar, sizeof(float), 1, pf);

	fclose(pf);

}

void iParticleSystem::load(const char* fileName)
{
	FILE* pf = fopen(fileName, "rb");

	fread(&ptcMax, sizeof(int), 1, pf);

	if (_ptc)
		delete _ptc;
	_ptc = new iParticle[ptcMax];
	for (int i = 0; i < ptcMax; i++)
	{
		iParticle* p = &_ptc[i];
		p->colorStart = p->colorEnd = iColor4bMake(0, 0, 0, 0);
		p->positionStart = p->positionEnd = iPointMake(0, 0);
		p->scaleStart = p->scaleEnd = 0.0f;
		p->delta = p->_delta = 0.0f;
	}

	if (ptc)
		delete ptc;
	ptc = new iParticle * [ptcMax];
	ptcNum = 0;

	fread(&positionStartVar, sizeof(float), 2, pf);
	fread(&positionEndVar, sizeof(float), 2, pf);
	fread(&crossStartEnd, sizeof(bool), 1, pf);

	fread(&colorStart, sizeof(iColor4b), 1, pf);
	fread(&colorStartVar, sizeof(iColor4b), 1, pf);

	fread(&colorEnd, sizeof(iColor4b), 1, pf);
	fread(&colorEndVar, sizeof(iColor4b), 1, pf);

	fread(&scaleStart, sizeof(int), 1, pf);
	fread(&scaleStartVar, sizeof(int), 1, pf);
	fread(&scaleEnd, sizeof(int), 1, pf);
	fread(&scaleEndVar, sizeof(int), 1, pf);

	fread(&delta, sizeof(float), 1, pf);
	fread(&deltaVar, sizeof(float), 1, pf);

	fclose(pf);
}

void iParticleSystem::startAnimation(iPoint pos)
{
	int num = 5 + rand() % 6;
	for (int i = 0; i < num; i++)
	{
		// 1개 추가
		for (int i = 0; i < ptcMax; i++)
		{
			iParticle* p = &_ptc[i];
			if (p->delta < p->_delta) continue;

			int sx = positionStartVar.x;
			int sy = positionStartVar.y;
			p->positionStart = iPointMake(	pos.x - sx / 2 + rand() % sx,
											pos.y - sy / 2 + rand() % sy);
			int ex = positionEndVar.x;
			int ey = positionEndVar.y;
			if (crossStartEnd)
			{
				p->positionEnd = iPointMake(pos.x - ex / 2 + rand() % ex,
											pos.y - ey / 2 + rand() % ey);
			}
			else
			{
				iRect rt = iRectMake(pos.x - sx / 2, pos.y - sy / 2, sx, sy);
				while (1)
				{
					p->positionEnd = iPointMake(pos.x - ex / 2 + rand() % ex,
												pos.y - ey / 2 + rand() % ey);
					if (containPoint(p->positionEnd, rt) == false)
						break;
				}
			}

			p->colorStart = iColor4bMake(	colorStart.r + rand() % (colorStartVar.r+1),
											colorStart.g + rand() % (colorStartVar.g+1),
											colorStart.b + rand() % (colorStartVar.b+1),
											colorStart.a + rand() % (colorStartVar.a+1));
			p->colorEnd = iColor4bMake(		colorEnd.r + rand() % (colorEnd.r+1),
											colorEnd.g + rand() % (colorEnd.g+1),
											colorEnd.b + rand() % (colorEnd.b+1),
											colorEnd.a + rand() % (colorEnd.a+1));
			p->scaleStart = scaleStart + rand() % (scaleStart + 1);// 3 ~ 5
			p->scaleEnd = scaleEnd + rand() % (scaleEnd + 1);// 3 ~ 5

#define _DT 1000.f
			int DT = deltaVar * _DT;
			p->_delta = delta + (rand() % DT) / _DT;
			p->delta = 0.0f;

			ptc[ptcNum] = p;
			ptcNum++;
			break;
		}
	}
}

void iParticleSystem::stopAnimation(bool immediately)
{
}
