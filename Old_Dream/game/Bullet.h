#pragma once
#include "iStd.h"

struct Object;

struct Bullet
{
	bool alive;
	int atk; //공격력
	iImage* img;
	iPoint p;
	iPoint v;
	Object* own;
	iRect bCollider;//불릿 콜라이더

	bool paint(float dt, iPoint off);
	iRect collider();
};

void loadBullet();
void freeBullet();
void drawBullet(float dt, iPoint off);
void addBullet(int index, Object* own, iPoint p, iPoint v); //스피드랑 포지션


