#include "Bullet.h"
#include "Effect.h"

#define buKind 3
#define buMax 100

Bullet** _bu;
Bullet** bu;
int buNum;

void loadBullet()
{
	_bu = new Bullet*[buKind];
	bu = new Bullet * [buKind * buMax];
	buNum = 0;

	struct BuInfo
	{
		const char* path;
		float aniDt;
		iPoint p;
	};
	BuInfo bi[3] = {
		{ "assets/monster/magic%d.png", 0.05f, {-16,-16} },
		{ "assets/monster/arrow%d.png", 0.05f, {-16,-16} },
		{ "assets/monster/magic%d.png", 0.05f, {-16,-16} },
	};

	iImage* imgs[buKind];
	for (int i = 0; i < buKind; i++)
	{
		BuInfo* info = &bi[i];

		iImage* img = new iImage();
		for (int j = 0; j < 4; j++)
		{
			Texture* tex = createImage(info->path, j);
			img->addObject(tex);
			freeImage(tex);
		}
		img->position = info->p;
		img->_aniDt = info->aniDt;
		img->startAnimation(); //무한반복
		imgs[i] = img;
	}

	for (int i = 0; i < buKind; i++)
	{
		Bullet* b = new Bullet[buMax];
		memset(b, 0x00, sizeof(Bullet) * buMax);
		for (int j = 0; j < buMax; j++) //초기화
			b[j].img = imgs[i]->clone();

		_bu[i] = b;
	}

	for (int i = 0; i < buKind; i++)
		delete imgs[i];
}

void freeBullet()
{
	for (int j = 0; j < buKind; j++)
	{
		for (int i = 0; i < buMax; i++)
			delete _bu[j][i].img;
		delete _bu[j];
	}
	delete _bu;
	delete bu;
}

void drawBullet(float dt, iPoint off)
{
	for (int i = 0; i < buNum; i++)
	{
		if (bu[i]->paint(dt, off))
		{
			buNum--;
			bu[i] = bu[buNum];
			i--;
		}
	}
}

void addBullet(int index, Object* own, iPoint p, iPoint v)
{
	for (int i = 0; i < buMax; i++)
	{
		Bullet* b = &_bu[index][i];
		if (b->alive == false)
		{
			b->alive = true;
			b->img->reverse = (v.x < 0 ? REVERSE_WIDTH : REVERSE_NONE);
			b->own = own;
			b->p = p;
			b->v = v;
			b->bCollider = iRectMake(-5, -4, 16, 8);

			bu[buNum] = b;
			buNum++;
			return;
		}
	}
}

#include "Game.h"
//
bool Bullet::paint(float dt,iPoint off)
{

	int max = maxMoveR(p, bCollider);
	int min = maxMoveL(p, bCollider);

#if 0
	setRGBA(1, 0, 0, 1);
	iRect rt = bCollider;
	rt.origin += p;
	drawRect(rt, 0);
	setRGBA(1, 1, 1, 1);
#endif // 1

	Game* g = (Game*)SceneMgt::instance()->currScene;
	Player* player = g->player;

	for (int i = 0; i < liveMonsterNum(); i++)
	{
		if (own == player)
		{
			if (containRect(collider(), monster[i]->collider()))
			{
				addEffect(0, this->p);
				alive = false;
				printf("monster hit!");
				monster[i]->setDmg(30);
				return true;
			}
		}
		else
		{
			//플레이어 충돌체크
			if (containRect(collider(), player->collider()))
			{
				addEffect(0, this->p);
				alive = false;
				printf("player hit!");
				player->setDmg(10);
				return true;
			}
		}
	}

	p += v * dt;

	if (this->p.x > max || this->p.x < min)
	{
		addEffect(0, this->p);
		alive = false;
		return true;
	}

	iPoint p = this->p + off;
	img->paint(dt, p);


	//화면 밖으로 나가면 없앰
	if (containPoint(p, iRectMake(-20, -20, devSize.width + 40, devSize.height + 40)) == false)
	{
		alive = false;
		return true;
	}

	return false;
}

//콜라이더 반환
iRect Bullet::collider()
{
	iRect rt = bCollider;
	rt.origin += p;

	return rt;
}