#pragma once

#define LightWeapon 0.2f
#define MiddleWeapon 0.3f
#define HeavyWeapon 0.6f

#define ReLoad 1.0f;

struct Weapon // 무기
{
	Weapon();
	virtual ~Weapon();
	virtual void attack() = 0;

	int atk; //무기공격력
	float speed; //무기공격 속도
	//이미지
};


struct Gun : Weapon 
{
	Gun();
	virtual ~Gun();
	virtual void attack();

	int bulletCount;
};