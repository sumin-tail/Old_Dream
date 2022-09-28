#pragma once

#define LightWeapon 0.2f
#define MiddleWeapon 0.3f
#define HeavyWeapon 0.6f

#define ReLoad 1.0f;

struct Weapon // ����
{
	Weapon();
	virtual ~Weapon();
	virtual void attack() = 0;

	int atk; //������ݷ�
	float speed; //������� �ӵ�
	//�̹���
};


struct Gun : Weapon 
{
	Gun();
	virtual ~Gun();
	virtual void attack();

	int bulletCount;
};