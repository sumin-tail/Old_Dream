#pragma once

#include "iStd.h"

#include "Weapon.h"
#include "Bullet.h" //투사체
#include "Effect.h" //이펙트

enum ObjectState
{
	ObjectStateIdleR = 0, //대기
	ObjectStateIdleL,
	ObjectStateMoveR, //2
	ObjectStateMoveL,
	ObjectStateJumpR,//4
	ObjectStateJumpL,
	ObjectStateAttackR, //6
	ObjectStateAttackL,
	ObjectStateHitR, //8
	ObjectStateHitL,
	ObjectStateDeadR, //10
	ObjectStateDeadL,

	ObjectStateMax
};

//애니메이션 정보
struct AniInfo
{
	int num;	// img count
	float aniDt;
	int loop;	// 0:inf, 1~ loop count
	iPoint pos;
};

//초기화 값
#define _Hp		   100
#define _Atk	   10

#define _moveSpeed 500 //원래 300
#define jumpPow	   600 //원래 600
#define _gravity  1000 //중력

#define groundY 640 //나중에 제거
// (300, 200) (300/200 = 1.5s) 300 * 1.5 / 2 = 225
// (300, 600) (300/600 = 0.5s) 300 * 0.5 / 2 = 75
// (600, 1800) (600/1800 = 0.33s) 600 * 0.33 / 2 = 100
// (600, 2400) (600/2400 = 0.25s) 600 * 0.25 / 2 = 75
//공식 기억

struct Object
{
	Object();
	virtual ~Object();
	void jump();
	virtual bool canAttack();
	virtual bool attack(iPoint off);
	static void cbAttack(void* parm);
	virtual void setDmg(float dmg);
	static void cbDead(void* parm);
	virtual void draw(float dt, iPoint off);
	iRect collider();

	void drawHp(float dt ,iPoint off);

	iImage** imgs;
	iImage* imgCurr;
	ObjectState state; //플레이어 상태
	iPoint position;
	iRect rtCollider; //충돌체크
	iRect atkCollider; //공격 콜라이더

	int type; //타입
	int hp, maxHp, hpStart, hpEnd;
	float hpDt;
	float moveSpeed;
	int atk;

	bool jumpcheck;
	float gravity;
};

struct AttRect
{
	bool alive;
	Object* o;
	iRect rt;
};

void loadAttRect();
void freeAttRect();
void drawAttRect(float dt);
void addAttRect(Object* o, iRect rt);

