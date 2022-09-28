#include "BackGround.h"

BackGround* bg;

BackGround::BackGround()
{
	bgtexs = new Texture * [4]; 
	for (int i = 0; i < 4; i++)
		bgtexs[i] = createImage("assets/background/%d.png", i);
	off = iPointZero;

	bgLength = 2560;//개발 사이즈 2배
	offMax = iPointZero;
	offMin = iPointMake(devSize.width - bgLength, 0);
}

BackGround::~BackGround()
{
	for (int i = 0; i < 4; i++)
		freeImage(bgtexs[i]);
	delete bgtexs;
}

void BackGround::move(iPoint mp, bool loop)
{
	off += mp;

	if (loop)
		return;

	if (off.x < offMin.x)
		off.x = offMin.x;
	else if (off.x > offMax.x)
		off.x = offMax.x;

	if (off.y < offMin.y)
		off.y = offMin.y;
	else if (off.y > offMax.y)
		off.y = offMax.y;
}

void BackGround::Draw(float dt)
{
	setRGBA(1, 1, 1, 1);

	Texture* tex;
	iPoint p;
	int i;

	float s = 2.0f; //스케일 값

	tex = bgtexs[0]; //제일 뒷 배경(안움직임)
	drawImage(tex, 0, 0, s, s, TOP | LEFT,
		0, 0, tex->width, tex->height, 2, 0);

	for (i = 0; i < 2; i++) //배경 사이즈는 개발사이즈 절반, 스케일 x2 로 
	{
		tex = bgtexs[1];

		p.x = off.x * 0.3f;
		p.y = off.y;
		while (p.x < -tex->width * s)
			p.x += tex->width * s;

		drawImage(tex, p.x + (tex->width*s) * i, p.y, s, s, TOP | LEFT,
			0, 0, tex->width, tex->height, 2, 0);
	}
	for (i = 0; i < 2; i++) 
	{
		tex = bgtexs[2];

		p.x = off.x * 0.7f;
		p.y = off.y;
		while (p.x < -tex->width * s)
			p.x += tex->width * s;

		drawImage(tex, p.x + (tex->width * s) * i, p.y, s, s, TOP | LEFT,
			0, 0, tex->width, tex->height, 2, 0);
	}
	for (int i = 0; i < 2; i++)
	{
		tex = bgtexs[3];

		p.x = off.x; //땅은 플레이어 이동거리랑 똑같이움직임
		p.y = off.y;
		while (p.x < -tex->width * s)
			p.x += tex->width * s;

		drawImage(tex, p.x + (tex->width * s) * i, p.y, s, s, TOP | LEFT,
			0, 0, tex->width, tex->height, 2, 0);
	}
}
