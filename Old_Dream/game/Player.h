#pragma once
#include "iStd.h"

#include "ODObject.h"


struct Player : Object
{
	Player();
	virtual ~Player();
	virtual bool canAttack();
	virtual bool attack(iPoint off);
	virtual void draw(float dt, iPoint off);

	float stamina, maxStamina, bufStamina;
	float staminaDt;

	Weapon* weapon[2]; //장착 중인 무기 데이터
	bool shot(iPoint off);

	void setCol(float dam);
	void updateCol(float dt);
	float coldt;
	bool col;
};

#define _hpDt 0.5f// 0.5f
#define _staminaDt 1.0f// 0.5f
#define _Stamina	   50
#define _coldt 1.0f

