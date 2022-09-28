#include "Item.h"

#define itKind 3 //아이템 종류

ItemList* itlist; //아이템 목록
UIItem* uiItem; //아이템 목록 - 내가 가지고있는 아이템(종류/갯수)

void loadItem()
{
	itlist = new ItemList;
	itlist->item = new Item*[itKind];

	uiItem = new UIItem[itKind];
	for (int i = 0; i < itKind; i++)
	{
		uiItem[i].index = i;
		uiItem[i].num.num = 0;
		uiItem[i].num.delta = 1;
	}

	struct itInfo
	{
		const char* path;
		iPoint p;
		iRect collider;
		bool live;
	};

	itInfo itinfo[3] = {
		{"assets/coin.png",{-8,-16},  {-4, -12 , 8, 8} , false },
		{"assets/gem.png", {-8,-16},  {-8, -16 , 16, 16},false },
		{"assets/key.png", {-8,-16},  {-8, -16 , 16, 16}, true }
	};

	iImage* imgs[itKind];

	for (int i = 0; i < itKind; i++)
	{
		itInfo* info = &itinfo[i];
		iImage* img = new iImage();

		Texture* tex = createImage(info->path);
		img->addObject(tex);
		freeImage(tex);
		img->position = info->p;
		imgs[i] = img;
	}

	for (int i = 0; i < itKind; i++)
	{
		Item* it = new Item;
		memset(it, 0x00, sizeof(Item));
		it->img = imgs[i]->clone();

		itlist->item[i] = it;
		itlist->item[i]->index = i;
		itlist->item[i]->itCollider = itinfo[i].collider;
		itlist->item[i]->live = itinfo[i].live;
	}

	for (int i = 0; i < itKind; i++)
		delete imgs[i];

	loadDropItem();
	loadEatItem();
}

void freeItem()
{
	for (int i = 0; i < itKind; i++)
		delete itlist->item[i];
	delete itlist->item;
	delete itlist;
}

//DropItem
#define MaxDropItem 100
DropItem* _dropItem;
DropItem** dropItem;
int numDropItem;

void loadDropItem()
{
	_dropItem = new DropItem[MaxDropItem];
	memset(_dropItem, 0x00, sizeof(DropItem) * MaxDropItem);
	dropItem = new DropItem * [MaxDropItem];
	numDropItem = 0;
}

#define MaxEatItem 100
EatItem* _eatItem;
EatItem** eatItem;
int numEatItem;

void drawItem(float dt, iPoint off)
{
	for (int i = 0; i < numDropItem; i++)
	{
		if (dropItem[i]->paint(dt, off))
		{
			numDropItem--;
			dropItem[i] = dropItem[numDropItem];
			i--;
		}
	}

	for (int i = 0; i < numEatItem; i++)
	{
		if (eatItem[i]->paint(dt, off))
		{
			numEatItem--;
			eatItem[i] = eatItem[numEatItem];
			i--;
		}
	}


	for (int i = 0; i < itKind; i++)
	{
		if (uiItem[i].num.delta < numberDt)
		{
			uiItem[i].num.update(dt);
		}
	}

}
void addItem(int index, iPoint p, int num)
{
	for (int i = 0; i < MaxDropItem; i++)
	{
		DropItem* d = &_dropItem[i];
		if (d->alive == false)
		{
			d->alive = true;
			d->index = index;
			d->position = p;
			if (itlist->item[index]->live == true)
				d->delta = 0.0f;
			else
				d->delta = 0.000001f;

			d->num = num;
			d->gravity = -upPow;

			dropItem[numDropItem] = d;
			numDropItem++;
			return;
		}
	}
}



void loadEatItem()
{
	_eatItem = new EatItem[MaxEatItem];
	memset(_eatItem, 0x00, sizeof(EatItem) * MaxEatItem);
	eatItem = new EatItem * [MaxEatItem];
	numEatItem = 0;
}

#include "Game.h" 
bool DropItem::paint(float dt, iPoint off)
{
	float a = 1;
	if (delta != 0)
	{
		delta += dt;
		if (delta < lostTime / 2)
		{
			// keep
		}
		else if (delta < lostTime)
		{
			a = delta - lostTime / 2;
			a /= (lostTime / 2);
			a = fabsf( _sin(180 * a * 5) );
		}
		else// if (delta >= lostTime)
		{
			alive = false;
			return true;
		}
	}

	Item* it = itlist->item[index];

	int maxY =  maxMoveB(position, it->itCollider);
	position.y += gravity * dt;//
	if (position.y > maxY)
		position.y = maxY;

	iPoint p = this->position + off;
	setRGBA(1, 1, 1, a);
	it->img->paint(dt, p); 
	setRGBA(1, 1, 1, 1);

	gravity += _gravity * dt; //jumpPow* dt; //원래코드
	if (gravity > _gravity)
		gravity = _gravity;

#if 0 //콜라이더
	setRGBA(1, 0, 0, 1);
	iRect rt = it->itCollider;
	rt.origin += p;
	drawRect(rt, 0);
	setRGBA(1, 1, 1, 1);
#endif 

	//충돌 체크
	Game* g = (Game*)SceneMgt::instance()->currScene;
	Player* player = g->player;

	//플레이어 충돌체크
	if (containRect(collider(), player->collider()))
	{
		addEffect(0, this->position); 
		audioPlay(snd_btn_item);
		addItem(index);
		
		alive = false;
		return true;
	}
		
	return false;
}

iRect DropItem::collider()
{
	Item* it = itlist->item[index];
	iRect rt = it->itCollider;
	rt.origin += position;

	return rt;
}

void DropItem::addItem(int index)
{
	for (int i = 0; i < MaxEatItem; i++)
	{
		EatItem* e = &_eatItem[i];
		if (e->alive == false)
		{
			e->alive = true;
			e->index = index;
			e->num = num;
			e->sp = position + bg->off;
			if (index == 0)
				e->ep = iPointMake(1010,70);
			else if (index == 1)
				e->ep = iPointMake(1130,70);
			else
				e->ep = iPointMake(980, 70);

			e->_delta = iPointLength(e->ep - e->sp) / speedEatItem;
			e->delta = 0;

			eatItem[numEatItem] = e;
			numEatItem++;
			return;
		}
	}
}


bool EatItem::paint(float dt, iPoint off)
{
	Item* it = itlist->item[index];
	
	float r = delta / _delta;
	iPoint p = linear(r, sp, ep);
	p.y += heightEatItem * _sin(180 * r);

	it->img->paint(dt, p);

	delta += dt;
	if (delta > _delta)
	{
		if (index < 2)
			uiItem[index].num.add(num);
		else
			uiItem[index].num.add(num, true);
		//addEffect(0, p);
		alive = false;
		return true;
	}
	return false;
}

int UIItem::itemCount()
{
	return num.num;
}

bool UIItem::paint(float dt)
{
	Item* it = itlist->item[index];

	iPoint p = iPointMake(980, 70);
	it->img->paint(dt, p);

	return false;
}
