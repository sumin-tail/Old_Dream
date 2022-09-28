#pragma once
#include "istd.h"
//아이템 - 체력회복 / 열쇠 같은거 / 게임 클리어
//아이템 리젠

#define upPow  300 // 상승 -
#define _gravity  600 //중력 -
#define lostTime 10.0f

void loadItem();
void freeItem();
void drawItem(float dt, iPoint off);
void addItem(int index, iPoint p, int num = 1);

//아이템 정보
struct Item
{
	int index; //아이템 번호
	iImage* img; //이미지
	iRect itCollider;//충돌 체크
	bool live; //사라질 아이템인지 안사라지는(필수) 아이템인지
};

//아이템 목록 쫙 가지고 있음
struct ItemList
{
	Item** item;
};
extern ItemList* itlist;

void loadDropItem();
void loadEatItem();

//땅에 떨어진 아이템 - 10초안에 안먹으면 사라짐
struct DropItem
{
	bool alive;
	int index;
	int num; //떨어진 갯수
	iPoint position;
	iPoint v;
	float delta;
	float gravity;

	bool paint(float dt, iPoint off);
	iRect collider();
	void addItem(int index);
};

//먹어서 움직이는 아이템
#define speedEatItem 800
#define heightEatItem 200

struct EatItem
{
	bool alive;
	int index;
	int num;
	iPoint sp,ep;
	float delta, _delta;

	bool paint(float dt, iPoint off);
};

#define numberDt 0.5f

struct iNumber
{
	int num;
	int sn, en;
	float delta;

	void add(int n, bool check = false)
	{
		if (check)
		{
			num += n;
			delta = numberDt;
		}
		else
		{
			sn = num;
			if (delta < numberDt)
			{
				en += n;
			}
			else
			{
				en = sn + n;
				delta = 0.0f;
			}
		}
	}

	void update(float dt)
	{
		if (delta < numberDt)
		{
			delta += dt;
			if (delta > numberDt)
				delta = numberDt;

			float r = delta / numberDt;
			num = linear(r, sn, en);
		}
	}
};

struct UIItem
{
	int index;
	iNumber num; //숫자 애니메이션 용

	int itemCount();
	bool paint(float dt);
};

extern UIItem* uiItem;

//for (int i = 0; i < 3; i++)
//{
//	Item* it = itlist->item[i];
//
//	iPoint p = iPointMake(100, devSize.height - (100 + i * 100)) + off;
//	it->img->paint(dt, p);
//#if 1
//	setRGBA(1, 0, 0, 1);
//	iRect rt = it->itCollider;
//	rt.origin += p;
//	drawRect(rt, 0);
//	setRGBA(1, 1, 1, 1);
//}
//#endif // 1