#pragma once
#include "iStd.h"

#include "ODObject.h"

#define _MoveTime 2.0f


void loadMonster();
void freeMonster();
void drawMonster(float dt, iPoint p);
void addMonster(int index, iPoint p);

int liveMonsterNum();

struct Monster;
typedef void (*MethodAI)(float dt, Monster* m);

struct MonsterInfo //몬스터 정보
{
	const char* name; //파일 위치
	AniInfo anif[6];
	iRect Collider; //충돌 체크
	float find; //서치거리
	float range; //공격사거리
	float moveSpeed; //이속
	float rate; //공속
	float escape; //도주거리
};


struct MonsterRegen
{
	void init();
	void update(float dt);

	int regenNum;// 3~4

	float regenTime;
	iPoint spawn[3];
};

void Regeninit(Monster* m,iPoint p);

//리젠 타임
#define _regenTime 10.0f
//리젠 후 무적시간
#define _regenWait 2.0f

//몬스터 기본 기틀
struct Monster : Object
{
	Monster();
	virtual ~Monster();
	void draw(float dt, iPoint off);
	virtual void setDmg(float dmg);
	//void init();//초기화
	static void cbDead(void* parm);
	void dropItem();


	iPoint tPosition;
	float aiDt, _aiDt;

	float regen;
	bool dead; //사망체크(드로우도 안함)
	bool alive; //생존(애니메이션 끝날떄까지 드로우함)

	int ap; //공격력 
	int dp; //방어력
	float spawn; //스폰위치
	float find, range;// 300, 50 //탐지범위 /공격사정거리
	float rate, _rate; 

	MethodAI methodAI; //AI
};

extern Monster** monster;
extern int monsterNum;

void updateAINear(float dt, Monster* m); //근거리 이동
void updateAIFar(float dt, Monster* m); //원거리 이동

//몬스터는 상속으로 

//근거리 몬스터
struct MeleeMonster : Monster
{
	MeleeMonster(MonsterInfo* m);
	virtual ~MeleeMonster();

};

//원거리몬스터(점사/광역)
struct RangedMonster : Monster
{

	RangedMonster(MonsterInfo* m);
	virtual ~RangedMonster();

	float escape; //이 거리 안으로 들어오면 도망침
};

//보스 몬스터
struct BossMonster : Monster
{
	BossMonster();
	virtual ~BossMonster();
	//보스몬스터는 탐지 하지 않음
	bool a; //무적
};