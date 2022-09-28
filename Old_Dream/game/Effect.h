#pragma once
#include "iStd.h"
#include "BackGround.h"
//이펙트 관리

struct Effect
{
	iImage* img;
	bool alive;
	iPoint p;

	bool paint(float dt, iPoint off);
};

void loadEffect();
void freeEffect();
void drawEffect(float dt, iPoint off);
void addEffect(int index, iPoint p); 




// 데미지 출력
struct Number
{
	int num;
	float anidt;
	iPoint position;
	int color;

	void start(iPoint p,int num);
	bool paint(float dt);

};

void loadNumber();
void freeNumber();
void drawNumber(float dt);
void addNumber(iPoint p, int Num);

