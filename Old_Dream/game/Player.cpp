#include "Player.h"

#include "BackGround.h"
#include "Map.h"

Player::Player() : Object()
{
	imgs = new iImage * [ObjectStateMax]; 

	iImage* img; 

	AniInfo aniInfo[6] = {
		{ 4, 0.18, 0, {-64, -128} },
		{ 6, 0.18, 0, {-64, -128} },
		{ 2, 0.18, 1, {-64, -128} },
		{ 6, 0.05, 1, {-64, -128} },// atk
		{ 4, 0.10, 1, {-64, -128} },
		{ 5, 0.18, 1, {-64, -128} }
	};

	//애니메이션 쭉 할당
	for (int i = 0; i < 6; i ++)
	{
		AniInfo* ai = &aniInfo[i];

		img = new iImage();
		for (int j = 0; j < ai->num; j++)
		{
			Texture* tex = createImage("assets/player/player%d%d.png", i, j);
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
		imgs[i*2] = img;
		img = img->clone();
		img->reverse = REVERSE_WIDTH;
		imgs[i*2+1] = img;
	}

	//시작 상태
	state = ObjectStateIdleR;
	imgCurr = imgs[state];

	stamina = maxStamina = _Stamina;
	staminaDt = 0.0f;

	position = iPointMake(100, groundY); //캐릭터 생성 좌표
	rtCollider = iRectMake(-10, -45, 20, 45);
	atkCollider = iRectMake(0, 0, 0, 0);

	coldt = _coldt;
	col = false;
}

Player::~Player()
{
}

bool Player::canAttack()
{
	if (stamina < 10) //스테미나 소모 10으로 변경
		return false;

	return Object::canAttack();
}

//플레이어 어택 근거리/총
bool Player::attack(iPoint off)
{
	bool ret = Object::attack(off);
	if (ret)
	{
		audioPlay(2);//수정
		stamina -= 10; //스테미나 원래대로
		printf("attck/ stamina = %f\n", stamina);
		if (state%2) //나머지가 있으면 왼쪽 보는거/ 없으면 오른쪽
		{
			atkCollider = iRectMake(rtCollider.origin.x+ rtCollider.size.width/2 - 50, rtCollider.origin.y, 20, 30);
		}
		else
		{
			atkCollider = iRectMake(rtCollider.origin.x + 50, rtCollider.origin.y, 20, 30);
		}
		atkCollider.origin += position;
		addAttRect(this, atkCollider);

		//무기 콜라이더 확인용
#if 0
		setRGBA(1, 0, 0, 1);
		iRect rt = atkCollider; 
		rt.origin += off; 
		drawRect(rt, 0);
		setRGBA(1, 1, 1, 1);
#endif // 1 
	}

	return ret;
}


bool Player::shot(iPoint off)
{
	printf("Gun shot\n");
	if (state % 2) //나머지가 있으면 왼쪽 보는거/ 없으면 오른쪽
	{
		addBullet(0, this, position + iPointMake(-10, -20), iPointMake(-500, 0)); //원래 스피드500 
	}
	else
	{
		addBullet(0, this, position + iPointMake(-10, -20), iPointMake(500, 0));
	}
	iRect rt = atkCollider;
	rt.origin +=  position;
	addAttRect(this, rt);
	return true;
}


void Player::setCol(float dam)
{
	if (col == false)
	{
		col = true;
		coldt = 0.0f;
		setDmg(dam);
	}
}

void Player::updateCol(float dt)
{
	if (col)
	{
		coldt += dt;
		if (coldt > _coldt)
		{
			col = false;
			coldt = 0.0f;
		}
	}
}




void Player::draw(float dt, iPoint off)
{
	Object::draw(dt, off);

	if (!hp)
	{
		return;
	}

	updateCol(dt);

	iPoint v = iPointZero;
	if (getKeyStat(keyboard_left))
		v.x = -1;
	else if (getKeyStat(keyboard_right))
		v.x = +1;

	if (getKeyDown(keyboard_space))
		jump();
	if (getKeyDown(keyboard_z))
		attack(off);
	if (getKeyDown(keyboard_enter))
		shot(off);

	ObjectState ps = state;

	if (v != iPointZero)
	{
		ps = ObjectStateMoveR;

		v /= iPointLength(v);
		//
		int minX = 0 ,maxX = devSize.width * 2;
		//
		if (v.x > 0)
		{
			ps = ObjectStateMoveR;
#if 0
			int x = ((int)position.x + rtCollider.size.width/2) / TILE_WIDTH;
			int sy = ((int)(position.y - rtCollider.size.height)) / TILE_HEIGHT; //위쪽y
			int ey = (int)(position.y) / TILE_HEIGHT; //아래쪽y

			for (int i = x + 1; i < tileNumX; i++)
			{
				bool col = false;
				for (int j = sy; j < ey; j++)
				{
					Tile* t = &tile[tileNumX * j + i];
					if (t->attr)
					{
						col = true;
						break;
					}
				}
				if (col)
				{
					maxX = TILE_WIDTH * i;
					break;
				}
			}

			maxX -= rtCollider.size.width/2 + 1;
#else
			maxX = maxMoveR(position, rtCollider);
#endif // 0
			//printf("maxX = %d\n", maxX);
		}
		else if (v.x < 0)
		{
			ps = ObjectStateMoveL;
#if 0

			int x = ((int)position.x - rtCollider.size.width / 2) / TILE_WIDTH;

			int sy = ((int)(position.y - rtCollider.size.height)) / TILE_HEIGHT; //위쪽y
			int ey = (int)(position.y) / TILE_HEIGHT; //아래쪽y

			for (int i = x - 1; i > -1; i--)
			{
				bool col = false;
				for (int j = sy; j < ey; j++)
				{
					Tile* t = &tile[tileNumX * j + i];
					if (t->attr)
					{
						col = true;
						break;
					}
				}
				if (col)
				{
					minX = TILE_WIDTH * (i + 1);
					break;
				}
			}

			minX += rtCollider.size.width / 2;
#else
			minX = maxMoveL(position, rtCollider);
#endif // 0

			//printf("minX = %d\n", minX);
		}
		else// if (v.x == 0)
			ps = (ObjectState)(ObjectStateJumpR + (state % 2));

		position += v * (moveSpeed * dt);//

		if (position.x < minX)
			position.x = minX;
		if (position.x > maxX)
			position.x = maxX;

		if (position.x < 50)
			position.x = 50;
		else if (position.x > bg->bgLength - 50)
			position.x = bg->bgLength - 50;

		iPoint p = position + bg->off;
		if (p.x < devSize.width / 3)
			bg->move(iPointMake(devSize.width / 3 - p.x, 0));
		else if (p.x > devSize.width * 2 / 3)
			bg->move(iPointMake(devSize.width * 2 / 3 - p.x, 0));

		/*	printf("%f, %f, %f, min = %f\n", position.x, bg->off.x,
			  position.x + bg->off.x, bg->offMin.x);*/
	}
	else
	{
		if (dt != 0)
			ps = (ObjectState)(ObjectStateIdleR + (state % 2));
	}

//중력+땅 충돌체크존

	// 바닥에 닿을때 까지 떨어져야 함
	int minY = 0;
	int maxY = groundY;
#if 0
	int y = ((int)position.y) / TILE_HEIGHT;
	if (y < (tileNumY - 2)) // 아래 -2 칸은 라인은 고정 땅
	{
		jumpcheck = true;

		int sx = ((int)(position.x - rtCollider.size.width / 2)) / TILE_WIDTH; //왼쪽 x
		int ex = ((int)(position.x + rtCollider.size.width / 2)) / TILE_WIDTH; //오른쪽 x
		//bool col = false;
		for (int x = sx; x < ex + 1; x++)
		{
			Tile* t = &tile[tileNumX * y + x];
			if (t->attr) //0이아닐때
			{
				maxY = TILE_HEIGHT * y;
				break;
			}
		}
	}
#else
	int y = ((int)position.y) / TILE_HEIGHT;
	if (y < (tileNumY - 2)) // 아래 -2 칸은 라인은 고정 땅
	{
		jumpcheck = true;
		maxY = maxMoveB(position ,rtCollider);
	}
#endif // 0
	minY = maxMoveT(position, rtCollider);

	position.y += gravity * dt;
	if (position.y > maxY)
	{
		position.y = maxY;
		// 바닥에 닿으면
		gravity = 0.0f;

		if (jumpcheck)
		{
			jumpcheck = false;
			// 먼지 더스트,
			// 소리...
			//printf("tak\n");
		}

		if (v.x == 0.0f)
		{
			ps = (ObjectState)(ObjectStateIdleR + state % 2);
		}

	}
	if (position.y < minY)
	{
		gravity = 0;
 		position.y = minY;
	}

	gravity += _gravity * dt; //jumpPow* dt; //원래코드
	if (gravity > jumpPow)
		gravity = jumpPow;

	if (Object::canAttack())//&& hp == hpEnd
	{
		state = ps;

		//스테미너
		stamina += 5 * dt;//(5 + bufStamina)* dt
		if (stamina > maxStamina)
			stamina = maxStamina;
	}

	//if (hpEnd)// for testing
	//{
	//	setDmg(20);
	//}

	if (hpDt)
	{
		hpDt += dt;
		float r = hpDt / _hpDt;
		if (hpDt >= _hpDt)
		{
			hpDt = 0.0f;
			r = 1.0f;
			hpStart = hp;
		}

		hp = linear(r, hpStart, hpEnd);

		if (hp <= 0)
		{
			hp = 0;
			state = (ObjectState)(ObjectStateDeadR + state % 2);
			imgs[state]->startAnimation(cbDead, this);
		}
	}

}

