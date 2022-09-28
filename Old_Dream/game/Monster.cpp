#include "Monster.h"
#include "BackGround.h"
#include "Game.h"

//#define monType 2
#define monKind 2
#define monMax 10

//각 몬스터가 2종류(근거리/원거리 타입) - monType ->현재 monKind
//근 원거리 타입으로 각각 몬스터 종류(갯수) - monKind ->로 하고싶음
//최대 몬스터 스폰 수 - monMax

Monster*** _monster;
Monster** monster;
int monsterNum;

MonsterRegen* monsterRegen;

MonsterInfo monsterInfo[2] =
{
	{"SkeletonMan", 
	{	{ 4, 0.18, 0, {-64, -128} },
		{ 6, 0.18, 0, {-64, -128} },
		{ 1, 0.18, 1, {-64, -128} },
		{ 6, 0.05, 1, {-64, -128} },
		{ 5, 0.10, 1, {-64, -128} },
		{ 7, 0.18, 1, {-64, -128} }}, 
		iRectMake(-20, -45, 40, 35), 300, 100, 400, 1.0f, NULL},

	{"SkeletonArcherMan" , 
	{	{ 4, 0.18, 0, {-64, -128} },
		{ 6, 0.18, 0, {-64, -128} },
		{ 1, 0.18, 1, {-64, -128} },
		{ 10, 0.05, 1, {-64, -128} },
		{ 4, 0.10, 1, {-64, -128} },
		{ 6, 0.18, 1, {-64, -128} }},
		iRectMake(-20, -45, 40, 35), 600 , 400, 300, 1.0f, 100}
};


void loadMonster()
{
	_monster = new Monster * *[monKind];
	for (int i = 0; i < monKind; i++)
		_monster[i] = new Monster*[monMax];

	for (int j = 0; j < monMax; j++)
	{
		_monster[0][j] = new MeleeMonster(&monsterInfo[0]);
		_monster[1][j] = new RangedMonster(&monsterInfo[1]);
	}

	monster = new Monster * [monKind * monMax];
	monsterNum = 0;

	//몬스터 리젠
	monsterRegen = new MonsterRegen;
	monsterRegen->init();
}

void freeMonster()
{
	for (int i = 0; i < 2; i++)
	{
		for (int j = 0; j < 10; j++)
			delete _monster[i][j];
		delete _monster[i];
	}
	delete _monster;
}

void drawMonster(float dt, iPoint off)
{
	monsterRegen->update(dt);

	for (int i = 0; i < monsterNum; i++)
	{
		monster[i]->draw(dt, off);
		if (monster[i]->dead == true)
		{
			monsterNum--;
			monster[i] = monster[monsterNum];
			i--;
		}
	}
}

void addMonster(int index, iPoint p)
{
	for (int i = 0; i < monMax; i++)
	{
		if (_monster[index][i]->dead == true)
		{
			_monster[index][i]->dead = false;
			_monster[index][i]->alive = true;
			monster[monsterNum] = _monster[index][i];
			Regeninit(_monster[index][i], p);
			monsterNum++;
			return;
		}

	}
}

int liveMonsterNum()
{
	return monsterNum;
}

void MonsterRegen::init()
{
	regenNum = 2; // +rand() % 2;
	regenTime = 0;
	spawn[0] = { devSize.width / 3, 640 };
	spawn[1] = { devSize.width, 640 };
	spawn[2] = { 800, 440 };
}

void MonsterRegen::update(float dt)
{
	if (monsterNum == regenNum)
		return;

	regenTime += dt;
	if (regenTime > _regenTime)
	{
		regenTime = 0.0f;

		// addmonster
		addMonster(rand() % 2, spawn[rand() % 2]); //#수정
	}
}

//리젠될때 애니메이션 및 상태/체력초기화
void Regeninit(Monster* m, iPoint p)
{
	for (int i = 0; i < ObjectStateMax; i++)
	{
		m->imgs[i]->repeatNum = 0;
		m->imgs[i]->startAnimation();
	}
	m->position = p;
	m->tPosition = p;
	m->hp = m->maxHp;
	m->rtCollider = monsterInfo[0].Collider;
	m->state = ObjectStateIdleR;
	m->imgCurr = m->imgs[m->state];
	m->aiDt = 0.0f;
	m->rate = 0.0f;
	m->regen = 0.0f;
}



Monster::Monster()
{
	imgs = new iImage * [ObjectStateMax];
}

Monster::~Monster()
{
}

void Monster::draw(float dt,iPoint off)
{
	ObjectState ps;
	bool hit = false;
	iPoint p;
	if (state >= ObjectStateHitR)
	{
		if (imgs[state]->animation != false)
		{
			ps = state;
			hit = true;
		}
	}

	// ai
	if (position.x != tPosition.x)
	{
		state = (ObjectState)(ObjectStateIdleR + state % 2);
		if (position.x < tPosition.x)
		{
			position.x += moveSpeed * dt;
			if (position.x > tPosition.x)
				position.x = tPosition.x;
			else
				state = ObjectStateMoveR;
		}
		else if (position.x > tPosition.x)
		{
			position.x -= moveSpeed * dt;
			if (position.x < tPosition.x)
				position.x = tPosition.x;
			else
				state = ObjectStateMoveL;
		}
	}
	else
	{
		if (hp)
			methodAI(dt, this); 
	}

	if (hit)
	{
		state = ps;
	}

	bool r = false;
	if (regen < _regenWait)
	{
		regen += dt;
		r = true;
		float a = fabsf( _sin(720 * regen / _regenWait) );
		setRGBA(1, 1, 1, a);
	}

	Object::draw(dt, off);

	if (r)
	{
		setRGBA(1, 1, 1, 1);
		//return;
	}

	Object::drawHp(dt, off);

//체크용 드로우
#if 0 
	 //탐지범위 테스트
	//iRect rt = detection;
	//rt.origin += position + off;
	//if (state % 2) //왼쪽 보고있을 때
	//{
	//	rt.origin.x -= rtCollider.size.width + rt.size.width;
	//}
	//setRGBA(1, 0, 0, 1);
	//drawRect(rt, 0);
	
	//플레이어 추적
	//Game* g = (Game*)SceneMgt::instance()->currScene;
	//if (containRect(g->player->collider(), rt))
	//{
	//	printf("find Player\n");
	//	if (g->player->position.x < position.x)
	//	{
	//		move = -1;
	//	}
	//	else
	//	{
	//		move = 1;
	//	}
	//}

	setRGBA(0, 1, 0, 1);
	iRect rt = iRectMake(spawn - 2, groundY - 2, 4, 4);
	rt.origin += off;
	drawRect(rt, 0); //스폰위치

	//스폰 귀환
	if (spawn-400 > position.x)
		move = 1;
	else if (spawn + 400 < position.x)
		move = -1;
	rt = iRectMake(spawn - 400, groundY + 10, 800, 4);
	rt.origin += off;
	drawRect(rt, 0); //이동거리
	setRGBA(1, 1, 1, 1);

	//플레이어 발견했을때 머리에 띄우는 ! 
	//iPoint p = rtCollider.origin + off + position;
	//p.x += 10;
	//p.y -= 50;
	//drawImage(warring, p , TOP | LEFT);
#endif // 1
}

void Monster::setDmg(float dmg)
{
	hp -= dmg;

	addNumber(position + iPointMake(0, -80), dmg);

	if (hp <= 0)
	{
		hp = 0;
		state = (ObjectState)(ObjectStateDeadR + state % 2);
		imgs[state]->startAnimation(cbDead, this);
		rtCollider = iRectMake(0, 0, 0, 0);
		alive = false;
		//아이템 드랍
		dropItem();
		upKillCount(); //맵 킬카운트 올려줌
		if (killCheck())
		{
			addItem(2, iPointMake(500, 500));
		}

		return;
	}

	state = (ObjectState)(ObjectStateHitR + state % 2);

	if (!imgs[state]->animation)
	{
		imgs[state]->startAnimation(cbAttack, this);

	}

}

void Monster::cbDead(void* parm)
{
	Monster* m = (Monster*)parm;
	m->dead = true;
}

void Monster::dropItem()
{
	for (int i = 0; i < 20; i++)
	{
		addItem(rand() % 2, iPointMake( position.x + rand()%100-50 , position.y - rand() % 20), 100);
	}
}


/// 
/// AI 함수존
/// 
//근거리 AI
void updateAINear(float dt, Monster* m)
{
	int max = maxMoveR(m->position, m->rtCollider, false);
	int min = maxMoveL(m->position, m->rtCollider, false);


	//추적/노멀/공격
#if 1
	m->aiDt += dt;
	if (m->aiDt > m->_aiDt)
	{
		Game* g = (Game*)SceneMgt::instance()->currScene;
		Player* player = g->player;
		if (iPointLength(m->position-player->position) < m->range)// 사정거리안에 있을 경우
		{
			// 공격
			if (m->state/2 != ObjectStateAttackR/2)
				m->rate += dt;
			if (m->rate > m->_rate)
			{
				m->rate = 0.0f;
				m->state = (ObjectState)(ObjectStateAttackR + m->state % 2);
				//공격용
				m->imgs[m->state]->startAnimation(m->cbAttack, m);
				if (m->state % 2) //나머지가 있으면 왼쪽 보는거/ 없으면 오른쪽
				{
					m->atkCollider = iRectMake(m->rtCollider.origin.x + m->rtCollider.size.width / 2 - 45, m->rtCollider.origin.y, 25, 30);//20,30
				}
				else
				{
					m->atkCollider = iRectMake(m->rtCollider.origin.x + 45, m->rtCollider.origin.y, 25, 30);//20,30
				}
				m->atkCollider.origin += m->position;
				addAttRect(m, m->atkCollider);

				//
				printf("attack\n");
			}
		}
		else// 사정거리 밖
		{
#elif 0
			// 주인공을 발견했을 경우
			if (iPointLength(m->position - player->position) < m->find )
			{
				m->tPosition = player->position;
			}
			// 뒹군다
			else
			{
				m->aiDt = 0.0f;
				float d = 100 + rand() % 200;
				m->tPosition.x = m->position.x + d * (rand()%100 < 50 ? -1 : 1);
			}
			if (m->tPosition.x < m->spawn - 400)
				m->tPosition.x = m->spawn - 400;
			else if (m->tPosition.x > m->spawn + 400)
				m->tPosition.x = m->spawn + 400;
#endif
			bool follow = false;
			if (iPointLength(m->position - player->position) < m->find)
			{
				follow = true;

				iPoint tp = player->position;
				if (tp.x < m->spawn - 400)
					tp.x = m->spawn - 400;
				else if (tp.x > m->spawn + 400)
					tp.x = m->spawn + 400;
				if (m->tPosition == tp)
					follow = false;
			}

			if (follow)
			{
				m->tPosition = player->position;
			}
			// 뒹군다
			else
			{
				m->aiDt = 0.0f;
				float d = 100 + rand() % 200;
				m->tPosition.x = m->position.x + d * (rand() % 100 < 50 ? -1 : 1);
			}

			if (m->tPosition.x < m->spawn - 400)
				m->tPosition.x = m->spawn - 400;
			else if (m->tPosition.x > m->spawn + 400)
				m->tPosition.x = m->spawn + 400;
		}
		if (m->tPosition.x > max)
			m->tPosition.x = max;

		if (m->tPosition.x < min)
			m->tPosition.x = min;
	}
	return;
}

//원거리 AI
void updateAIFar(float dt, Monster* m)
{
	int max = maxMoveR(m->position, m->rtCollider, false);
	int min = maxMoveL(m->position, m->rtCollider, false);

	RangedMonster* mo = (RangedMonster*)m; //원거리 필요할때만 사용

	//추적/노멀/공격
#if 1
	m->aiDt += dt;
	if (m->aiDt > m->_aiDt)
	{
		Game* g = (Game*)SceneMgt::instance()->currScene;
		Player* player = g->player;
		//도망칠 수 있을때 - 도망침/ 없을때 - 공격함

		if (iPointLength(m->position - player->position) < m->range)// 플레이어가 사정거리안에 있을 경우
		{

			if (iPointLength(m->position - player->position) < mo->escape)//도주거리 안 이면 도망침
			{
				if (m->position.x - player->position.x > 0)
				{
					m->tPosition.x += 200;
				}
				else
				{
					m->tPosition.x -= 200;
				}

			}

			// 공격dt 늘림
			if (m->state / 2 != ObjectStateAttackR / 2)
				m->rate += dt;
			if (m->rate > m->_rate) //공격
			{
				m->rate = 0.0f;
				m->state = (ObjectState)(ObjectStateAttackR + (m->position.x > player->position.x));// m->state % 2);
				//공격용
				m->imgs[m->state]->startAnimation(m->cbAttack, m);
				if (m->state % 2) //나머지가 있으면 왼쪽 보는거/ 없으면 오른쪽
				{
					//총알 만듬 - 스피드만 다르게
					addBullet(1, m, m->position + iPointMake(-10, -20), iPointMake(-500, 0)); //원래 스피드500 
				}
				else
				{
					addBullet(1, m, m->position + iPointMake(10, -20), iPointMake(500, 0));
				}
				m->atkCollider.origin += m->position;
				addAttRect(m, m->atkCollider);
				//
				printf("attack\n");
			}
		}
		else// 사정거리 밖
		{
			bool follow = false;
			if (iPointLength(m->position - player->position) < m->find)//플레이어 찾음
			{
				follow = true;

				//플레이어를 찾았으면 사정거리 끝자락에 간신히 닿게 쫒아가야함
				iPoint tp = m->position;//현재좌표넣고 //= player->position; 

				float x = iPointLength(m->position - player->position) - m->range;
				if (m->position.x < player->position.x) //플레이어가 몬스터 오른쪽에 있을경우		
					tp.x += x;				
				else				
					tp.x -= x;
				
				//이동할수있는 영역 안넘게 
				if (tp.x < m->spawn - 400)
					tp.x = m->spawn - 400;
				else if (tp.x > m->spawn + 400)
					tp.x = m->spawn + 400;

				if (m->tPosition == tp)
					follow = false;
			}

			if (follow)
			{
				m->tPosition = player->position;
			}
			// 뒹군다
			else
			{
				m->aiDt = 0.0f;
				float d = 100 + rand() % 200;
				m->tPosition.x = m->position.x + d * (rand() % 100 < 50 ? -1 : 1);
			}

			//스폰 이동 제한(도주할때 원거리는 맵 뚫고 나감)
			if (m->tPosition.x < m->spawn - 400)
				m->tPosition.x = m->spawn - 400;
			else if (m->tPosition.x > m->spawn + 400)
				m->tPosition.x = m->spawn + 400;
		}

		if (m->tPosition.x > max)
			m->tPosition.x = max;

		if (m->tPosition.x < min)
			m->tPosition.x = min;

		return;
#endif // 1

	}
}



///
/// 몬스터
/// 

//근거리 몬스터
MeleeMonster::MeleeMonster(MonsterInfo* m)
{
	iImage* img;
	AniInfo aniInfo[6] = {
		{ 4, 0.18, 0, {-64, -128} },
		{ 6, 0.18, 0, {-64, -128} },
		{ 1, 0.18, 1, {-64, -128} },
		{ 6, 0.05, 1, {-64, -128} },
		{ 5, 0.10, 1, {-64, -128} },
		{ 7, 0.18, 1, {-64, -128} }
	};

	//애니메이션 쭉 할당
	for (int i = 0; i < 6; i++)
	{
		AniInfo* ai = &aniInfo[i];

		img = new iImage();
		for (int j = 0; j < ai->num; j++)
		{
			Texture* tex = createImage("assets/monster/SkeletonMan/SkeletonMan%d%d.png", i, j);
			img->addObject(tex);
			freeImage(tex);
		}
		img->position = ai->pos;
		img->_aniDt = ai->aniDt;
		if (ai->loop)
		{
			img->_repeatNum = ai->loop; //무한반복 X ->루프 카운트 만큼 실행
		}
		else
		{
			img->startAnimation(); //무한반복
		}
		imgs[i * 2] = img;
		img = img->clone();
		img->reverse = REVERSE_WIDTH;
		imgs[i * 2 + 1] = img;
	}

	//시작 상태
	state = ObjectStateIdleR;
	imgCurr = imgs[state];

	tPosition = position = iPointMake(devSize.width / 3, groundY);
	aiDt = 0.0f;
	_aiDt = 1.0f;
	rtCollider = iRectMake(-20, -45, 40, 35);

	methodAI = updateAINear; //초반은 자유이동
	spawn = devSize.width / 3;
	find = 300;
	range = 100;
	moveSpeed = 400;
	rate = 0.0f;
	_rate = 1.0f;

	dead = true;
	alive = false;
	regen = _regenWait;
}

MeleeMonster::~MeleeMonster()
{

}

//원거리 몬스터
RangedMonster::RangedMonster(MonsterInfo* m)
{
	iImage* img;
	AniInfo aniInfo[6] = {
		{ 4, 0.18, 0, {-64, -128} },
		{ 6, 0.18, 0, {-64, -128} },
		{ 1, 0.18, 1, {-64, -128} },
		{ 10, 0.05, 1, {-64, -128} },
		{ 4, 0.10, 1, {-64, -128} },
		{ 6, 0.18, 1, {-64, -128} }
	};

	//애니메이션 쭉 할당
	for (int i = 0; i < 6; i++)
	{
		AniInfo* ai = &aniInfo[i];

		img = new iImage();
		for (int j = 0; j < ai->num; j++)
		{
			Texture* tex = createImage("assets/monster/SkeletonArcherMan/SkeletonArcherMan%d%d.png", i, j);
			img->addObject(tex);
			freeImage(tex);
		}
		img->position = ai->pos;
		img->_aniDt = ai->aniDt;
		if (ai->loop)
		{
			img->_repeatNum = ai->loop; //무한반복 X ->루프 카운트 만큼 실행
		}
		else
		{
			img->startAnimation(); //무한반복
		}
		imgs[i * 2] = img;
		img = img->clone();
		img->reverse = REVERSE_WIDTH;
		imgs[i * 2 + 1] = img;
	}

	//시작 상태
	state = ObjectStateIdleR;
	imgCurr = imgs[state];

	tPosition = position = iPointMake(devSize.width, groundY);
	aiDt = 0.0f;
	_aiDt = 1.0f;
	rtCollider = iRectMake(-20, -45, 40, 35);

	methodAI = updateAIFar; //초반은 자유이동(원거리 이동 AI)
	spawn = devSize.width;
	find = 600;//탐지범위
	range = 400; //사정거리
	moveSpeed = 300;
	escape = 100; //도주거리
	//공속
	rate = 0.0f;
	_rate = 1.0f;

	dead = true;
	alive = false;
	regen = _regenWait;
}

RangedMonster::~RangedMonster()
{

}

//보스 몬스터