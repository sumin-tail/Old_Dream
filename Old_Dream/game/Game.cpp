#include "Game.h"

#include "BackGround.h"
#include "MainMenu.h"

bool gameRestart;

void Game::load()
{
	bg = new BackGround;
	player = new Player();
	loadMonster(); //몬스터

	MapCountCheck();
	loadMap();
	loadItem();
	loadBullet();
	loadAttRect();
	loadEffect();
	//
	loadGameUI();//체력/스테/게임 내 재화 등
	//

	loadNumber();
	gameClear = false;
	gameOver = false;
	gameRestart = false;

	if (mapcount() == 0)
	{
		addMonster(0, iPointMake(devSize.width / 3, groundY));
		addMonster(1, iPointMake(1000, 440));
	}

	if (mapcount() == 1)
	{
		addMonster(0, iPointMake(devSize.width / 3, groundY));
		addMonster(1, iPointMake(800, 440));
	}

	for (int i = 0; i < monsterNum; i++)
	{
		monster[i]->regen = _regenWait;
	}


}

void Game::free()
{
	delete bg;
	delete player;
	freeMonster();

	freeBullet();
	freeAttRect();//충돌
	freeEffect();

	freeNumber();
	freeMap();
}

void Game::draw(float dt)
{
	setRGBA(1, 1, 1, 1);
	extern bool gamePause;
	extern iPopup* popQuit;
	extern iPopup* popClear;
	uidt = dt;
	//일시정지
	if (getKeyDown(keyboard_esc) && popQuit->bShow == false)
	{
		if (gamePause == true)
			gamePause = false;	
		else
			gamePause = true;
		showPopPause(gamePause);
	}

	if (gamePause == true || popClear->bShow == true)
	{
		dt = 0;
	}
	//일시정지존

	bg->Draw(dt); //배경 드로잉
	drawMap(dt); //맵
	//몬스터 드로우
	drawMonster(dt, bg->off);
	player->draw(dt,bg->off);//플레이어 드로우

	//플레이어가 몬스터와 닿았을때
	for (int i = 0; i < liveMonsterNum(); i++)
	{
		if (monster[i]->alive)//몬스터가 살아있을 때
		{
			if (containRect(player->collider(), monster[i]->collider()))
				player->setCol(1 + (rand()%5));
		}
	}

	gameUI();
	drawItem(dt, bg->off);
	drawAttRect(dt);
	drawBullet(dt,bg->off);
	drawEffect(dt,bg->off);
	drawNumber(dt);

	if (uiItem[2].itemCount())
	{
		setRGBA(1, 1, 1, 1);
		uiItem[2].paint(dt);

		extern iPopup* popClear;
		if (popClear->bShow == false && gameClear == false)
		{
			audioPlay(snd_btn_clear);
			gameClear = true;
			showPopClear(true);
		}
	}
	else
	{
		setRGBA(1, 1, 1, 0.5);
		uiItem[2].paint(dt);
	}
	setRGBA(1, 1, 1, 1);



	drawUI(uidt); //ui가 제일 마지막

	//무기 콜라이더 확인용
#if 0
	setRGBA(1, 0, 0, 1);
	iRect rt = monster[0]->atkCollider;
	rt.origin += bg->off;
	drawRect(rt, 0);
	setRGBA(1, 1, 1, 1);
#endif // 1 

	if (player->hp == 0 && gameOver == false )//플레이어 체력이 0이라면 게임오버 처리
	{
		if (player->imgs[player->state]->animation == false) //플레이어 애니메이션 다끝나고
		{
			showPopGameOver(true);//팝업띄움
			gameOver = true;
		}
	}

	if (gameOver || gameClear) //게임오버 또는 게임 클리어
	{
		extern iPopup* popGameOver;
		extern iPopup* popClear;
		if (popGameOver->bShow == false && popClear->bShow == false)
		{
			if (gameRestart)
				SceneMgt::instance()->set(GameSceneMenu, new Game);
			else //if (i == 1)
				SceneMgt::instance()->set(GameSceneMenu, new MainMenu);		
		}
	}
}

bool Game::key(iKeyState stat, iPoint p)
{
	keyUI(stat, p);
	return true;
}

void Game::loadGameUI()
{
	stGold = new iStrTex * [2];
	for (int i = 0; i < 2; i++)
		stGold[i] = new iStrTex(methodStGold);
}

Texture* Game::methodStGold(const char* str)
{
	int lineNum;
	char** line = iString::getStringLine(str, lineNum);
	int index = atoi(line[0]);
	int num = atoi(line[1]);
	iString::freeStringLine(line, lineNum);

	iGraphics* g = new iGraphics();
	iSize size = iSizeMake(100, 20);
	g->init(size);

	igImage* ig = g->createIgImage("assets/button/Button.png");
	g->drawIgImage(ig, 0, 0, 1, 0.5);
	g->freeIgImage(ig);

	ig = g->createIgImage("assets/%s.png", index == 0 ? "coin" : "gem");
	g->drawIgImage(ig, 5, size.height / 2, VCENTER | LEFT);
	g->freeIgImage(ig);

	setStringName("assets/font/DungGeunMo.ttf");
	setStringSize(20);
	setStringRGBA(0.3, 0.4, 0.3, 1);
	setStringBorder(0);
	if (num < 1000000)
		g->drawString(size.width - 5, size.height / 2, VCENTER | RIGHT, "%d", num);
	else
		g->drawString(size.width - 5, size.height / 2, VCENTER | RIGHT, "999999+");

	Texture* tex = g->getTexture();
	delete g;

	return tex;
}

void Game::gameUI()
{
	setRGBA(0, 0, 0, 1);
	fillRect(50, 50, 100, 20);
	fillRect(50, 80, 50, 20);
	float w = 96.0f * player->hpEnd / player->maxHp;
	setRGBA(1, 0, 0, 1);
	fillRect(50 + 2, 50 + 2, w, 20 - 4);

	//플레이어 체력
	if (player->hpDt)
	{
		float s = 96.0f * player->hp /player->maxHp;
		float l = w - s;

		setRGBA(1, 1, 0, 1);
		fillRect(50 + 2 + s, 50 + 2, l, 20 - 4);
	}
	//스테미너
	w = 46.0f * player->hp / player->maxHp;
	setRGBA(0, 1, 1, 1);
	fillRect(50 + 2, 80 + 2, player->stamina, 20 - 4);

	setRGBA(1, 1, 1, 1);

	//hp mp 단어
	setStringName("assets/font/DungGeunMo.ttf");
	setStringSize(20);
	setStringRGBA(0, 0, 0, 1);
	setStringBorder(1);
	setStringBorderRGBA(1, 0, 0, 1);
	iGraphics* g = new iGraphics();
	iSize size = iSizeMake(100, 100);
	g->init(size);
	g->drawString(1, 1, TOP | LEFT, "HP");
	setStringBorderRGBA(0, 1, 1, 1);
	g->drawString(1, 30, TOP | LEFT, "MP");
	drawImage(g->getTexture(), 25, 55, TOP | LEFT);

	//재화 
	for (int i = 0; i < 2; i++)
		stGold[i]->drawString(1000 + i * 120, 50 + 5, TOP | LEFT, "%d\n%d", i , i == 0 ? uiItem[i].itemCount() : uiItem[i].itemCount());
	setRGBA(1, 1, 1, 1); //색 돌려줄것
}

