#include "ODObject.h"

Object::Object()
{
	imgs = NULL;
	imgCurr = NULL;
	state = ObjectStateIdleR;
	position = iPointZero;
	rtCollider = iRectMake(0, 0, 0, 0);

	type = 0;
	hp = maxHp = hpStart = hpEnd = _Hp;
	hpDt = 0.0f;
	moveSpeed = _moveSpeed;
	atk = _Atk;

	jumpcheck = false;
	gravity = 0.0f;
}

Object::~Object()
{
	for (int i = 0; i < ObjectStateMax; i++)// 이미지 할당 다 해체
	{
		if (imgs[i])
			delete imgs[i];
	}
	delete imgs;
}

void Object::jump()
{
	if (jumpcheck)
		return;

	jumpcheck = true;
	// 위로 올라감
	gravity -= jumpPow;

	state = (ObjectState)(ObjectStateJumpR + state % 2);
	printf("jump\n");
}

bool Object::canAttack()
{
	return !(state / 2 == ObjectStateAttackR / 2);
}

bool Object::attack(iPoint off)
{
	if (canAttack() == false)
		return false;

	state = (ObjectState)(ObjectStateAttackR + state % 2);
	imgs[state]->startAnimation(cbAttack, this);

	return true;
}

void Object::cbAttack(void* parm)
{
	printf("attck/hit end!\n");
	Object* o = (Object*)parm; 

	o->state = (ObjectState)(o->jumpcheck ? ObjectStateJumpR : ObjectStateIdleR
		+ (o->state % 2));

	o->imgCurr = o->imgs[o->state];

	o->atkCollider = iRectMake(0, 0, 0, 0); //어택범위 없앰
}

void Object::setDmg(float dmg)
{
	if (hpEnd == 0)
		return;

	addNumber(position + iPointMake(0, -80), dmg);

	//체력
	if (hpStart == hp) //스타트와 현제 hp가같으면 피가 깎이고있는 상태가 아님
	{
		hpStart = hp; //현재피 저장
		hpEnd = hp - dmg;
	}
	else
	{
		hpEnd -= dmg;
	}

	hpDt = 0.000001f;

	if (hpEnd < 0)
		hpEnd = 0;

	//hp -= dmg;
	state = (ObjectState)(ObjectStateHitR + state % 2);
	if (!imgs[state]->animation)
	{
		imgs[state]->startAnimation(cbAttack, this);
	}

}

void Object::cbDead(void* parm)
{
	printf("dead end!\n");
	Object* o = (Object*)parm;
	// 
}

void Object::draw(float dt, iPoint off)
{
	//setRGBA(1, 0, 0, 1);
	setLineWidth(2);
	iPoint p = position + off;

	//캐릭터 충돌 좌표 / 이미지 중앙에
	imgCurr = imgs[state];
	imgCurr->paint(dt, p);


#if 0 //콜라이더 확인용
	setRGBA(1, 0, 0, 1);
	iRect rt = collider();
	rt.origin += off;
	drawRect(rt, 0);
	setRGBA(1, 1, 1, 1);
#endif // 0
}

iRect Object:: collider()
{
	iRect rt = rtCollider;
	rt.origin += position;

	return rt;
}

void Object::drawHp(float dt,iPoint off)
{
	iRect rt = collider();
	rt.origin += off;
	rt.origin.x -= 2;
	rt.size.width += 4;
	rt.origin.y -= 15;
	rt.size.height = 10;
	setRGBA(1, 1, 1, 1);
	fillRect(rt, 1);

	rt.origin += iPointMake(1, 1);
	rt.size -= iSizeMake(2, 2);
	rt.size.width = rt.size.width * hp / maxHp;
	setRGBA(1, 0 ,0, 1);
	fillRect(rt, 0);
	setRGBA(1, 1, 1, 1);
}


AttRect* _ar;
AttRect** ar;
int arNum;
void loadAttRect()
{
	_ar = new AttRect[100];
	memset(_ar, 0x00, sizeof(AttRect) * 100);

	ar = new AttRect * [100];
	arNum = 0;
}
void freeAttRect()
{
	delete _ar;
	delete ar;
}

#include "Game.h"
void drawAttRect(float dt)
{
	Game* g = (Game*)SceneMgt::instance()->currScene;
	Player* player = g->player;

	for (int i = 0; i < liveMonsterNum(); i++)
	{
		for (int j = 0; j < arNum; j++)
		{
			if (ar[j]->o == player)
			{
				//for(int i=0; i<monsterNum; i++)
				//플레이어가 몬스터공격
				if (containRect(ar[j]->rt, monster[i]->collider()))
				{
					printf("monster hit!");
					printf("monster Hp= %d/%d\n", monster[i]->hp, monster[i]->maxHp);
					monster[i]->setDmg(10+ (rand()%10));
				}
			}
			else
			{
				//몬스터가 플레이어 공격
				if (containRect(player->collider(), ar[j]->rt))
				{
					printf("player hit!");
					player->setDmg(10 +(rand()% 10));
				}
			}
			ar[j]->alive = false;
		}
	}
	arNum = 0;
}
void addAttRect(Object* o, iRect rt)
{
	for (int i = 0; i < 100; i++)
	{
		AttRect* a = &_ar[i];
		if (a->alive == false)
		{
			a->alive = true;
			a->o = o;
			a->rt = rt;

			ar[arNum] = a;
			arNum++;
			return;
		}
	}
}
