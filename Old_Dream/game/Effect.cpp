#include "Effect.h"

//-Effect-//

#define fxKind 3 //이펙트 종류
#define fxMax 100 //최대갯수

Effect** _fx;
Effect** fx;
int fxNum;

void loadEffect()
{
	_fx = new Effect * [fxKind];
	fx = new Effect * [fxKind * fxMax];
	fxNum = 0;

	struct FxInfo
	{
		const char* path;
		float aniDt;
		iPoint p;
	};
	FxInfo fi[3] = {
		{ "assets/monster/hit%d.png", 0.05f, {-16,-16} },
		{ "assets/monster/hit%d.png", 0.05f, {-16,-16} },
		{ "assets/monster/hit%d.png", 0.05f, {-16,-16} },
	};

	iImage* imgs[fxKind];
	for (int i = 0; i < fxKind; i++)
	{
		FxInfo* info = &fi[i];

		iImage* img = new iImage();
		for (int j = 0; j < 4; j++)
		{
			Texture* tex = createImage(info->path, j);
			img->addObject(tex);
			freeImage(tex);
		}
		img->position = info->p;
		img->_aniDt = info->aniDt;
		img->_repeatNum = 1; //1회만 실행
		imgs[i] = img;
	}

	for (int i = 0; i < fxKind; i++)
	{
		Effect* b = new Effect[fxMax];
		memset(b, 0x00, sizeof(Effect) * fxMax);
		for (int j = 0; j < fxMax; j++) //초기화
			b[j].img = imgs[i]->clone();

		_fx[i] = b;
	}

	for (int i = 0; i < fxKind; i++)
		delete imgs[i];
}

void freeEffect()
{
	for (int j = 0; j < fxKind; j++)
	{
		for (int i = 0; i < fxMax; i++)
			delete _fx[j][i].img;
		delete _fx[j];
	}
	delete _fx;
	delete fx;
}

void drawEffect(float dt, iPoint off)
{
	for (int i = 0; i < fxNum; i++)
	{
		if (fx[i]->paint(dt, off))
		{
			fxNum--;
			fx[i] = fx[fxNum];
			i--;
		}
	}
}

void addEffect(int index, iPoint p)
{
	for (int i = 0; i < fxMax; i++)
	{
		Effect* f = &_fx[index][i];
		if (f->alive == false)
		{
			f->img->startAnimation();
			f->alive = true;
			f->p = p;

			fx[fxNum] = f;
			fxNum++;
			return;
		}
	}
}


bool Effect::paint(float dt, iPoint off)
{
	img->paint(dt, p+off);

	if (img->animation == false)
	{
		alive = false;
		return true;
	}
	
	return false;
}

//-Number-//
Number* _number;
Number** number;
int numNumber;

Texture** texNumber;

void loadNumber()
{
	_number = new Number[100];
	memset(_number,0x00,sizeof(Number)*100);
	number = new Number* [100];
	numNumber = 0;

	texNumber = new Texture * [10];
	for (int i = 0; i < 10; i++)
		texNumber[i] = createImage("assets/number/num%d.png",i);
}

void freeNumber()
{
	delete _number;
	delete number;

	for (int i = 0; i < 10; i++)
		freeImage(texNumber[i]);
	delete texNumber;
}

void drawNumber(float dt)
{
	for (int i = 0; i < numNumber; i++)
	{
		if (number[i]->paint(dt))
		{
			numNumber--;
			number[i] = number[numNumber];
			i--;
		}
	}
}

void addNumber(iPoint p, int Num)
{
	for (int i = 0; i < 100; i++)
	{
		Number* n = &_number[i];
		if (n->anidt == 0.0f)
		{
			n->start(p, Num);

			number[numNumber] = n;
			numNumber++;
			return;
		}
	}
}

void Number::start(iPoint p, int num)
{
	this->position = p;
	this->num = num;
	anidt = 0.000001f;
	//color = rand() % 2;
	color = 0;
}

bool Number::paint(float dt)
{

	float scale = 1.0f;
	float alpha = 1.0f; //올라가면서 흐려지게
	float dy = 0.0f; //올라가는거

	//애니메이션 넣기
	if (anidt < 0.1f) 
	{
		float r = anidt / 0.1f;
		scale = linear(r, 5, 1);
		alpha = linear(r, 0, 1);
		dy = 0.f;
	}
	else if (anidt < 0.4f)
	{
		scale = 1.0f;
		alpha = 1.0f;
		dy = 0.f;
	}
	else// if (anidt < 1.4f)
	{
		float r = (anidt-0.4f)/1.0f;
		alpha = linear(r, 1.0f, 0.0f);
		dy = linear(r, 0, -100); // -50 까지
	}

	switch (color) {
	case 0:setRGBA(1, 1, 1, alpha); break;
	//case 1:setRGBA(1, 0.2, 0, alpha); break;//빨강
	//case 1:setRGBA(1, 1, 0, alpha); break;
	}
	

	char s[16];
	sprintf(s, "%d", num);

	int i, j = strlen(s);

	iPoint p = position;
	p.y += dy;
	Texture* t = texNumber[0];
	p.x -= (t->width * j + 1 * (j - 1)) / 2 * scale;
	p.y -= t->height / 2 * scale;

	for (int i = 0;  i < j; i++)
	{
		int n = s[i] - '0';
		//이미지 중간에 두기,,
		t = texNumber[n];
		drawImage(t, p.x+ bg->off.x, p.y, scale, scale, TOP|LEFT, 0, 0, t->width, t->height, 2, 0); //백그라운드 좌표 줘야함
		p.x += (t->width + 1)*scale;
	}
	setRGBA(1, 1, 1, 1);

	anidt += dt;
	if (anidt > 1.4f)
	{
		anidt = 0.0f;
		return true;
	}
	return false;

}
