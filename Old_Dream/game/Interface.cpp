#include "Interface.h"
#include "Game.h"


//오디오
AudioInfo ai[7] = {
	{ (char*)"assets/snd/btn0.wav", false, 1.0f },
	{ (char*)"assets/snd/btn1.wav", false, 1.0f },
	{ (char*)"assets/snd/swing.wav", false, 1.0f },
	{ (char*)"assets/snd/clear.wav", false, 1.0f },
	{ (char*)"assets/snd/item.wav", false, 1.0f },

	{ (char*)"assets/snd/bgm0.wav", true, 1.0f },
	{ (char*)"assets/snd/bgm1.wav", true, 1.0f },
};


void loadUI()
{
	loadAudio(ai, snd_max); //로드
	audioVolume(1, 1, snd_eff_max);
	audioVolume(0.3f, 0.3f, snd_eff_max);

	createPopMainMenu();
	createPopSetting();
	createPopQuit();
	createPopGameOver();
	createPopPause();
	createPopClear();

	audioPlay(snd_bgm_title);
}

void freeUI()
{
	freePopMainMenu();
	freePopSetting();
	freePopQuit();
	freePopGameOver();
	freePopPause();
	freePopClear();
}

void drawUI(float dt)
{
	drawPopMainMenu(dt);
	drawPopSetting(dt);
	drawPopPause(dt); //드로우 순서만 바꿈
	drawPopGameOver(dt);
	drawPopClear(dt);
	drawPopQuit(dt);//게임종료가 가장 상위
}

void keyUI(iKeyState stat, iPoint p)
{
	if (keyPopQuit(stat, p))
		return;

	if (keyPopClear(stat, p))
		return;

	if (keyPopSetting(stat, p))
		return;

	if (keyPopMainMenu(stat, p))
		return;

	if (keyPopGameOver(stat, p))
		return;

	if (keyPopPause(stat, p))
		return;
}


//기타
void drawPopCommon(float dt, iPopup* pop, iColor4f c)
{
	float r;
	if (pop->state == iPopupStateOpen)
	{
		r = pop->aniDt / pop->_aniDt;
	}
	else if (pop->state == iPopupStateProc)
	{
		r = 1.0f;
	}
	else
	{
		r = 1 - pop->aniDt / pop->_aniDt;
	}
	setRGBA(c.r, c.g, c.b, c.a * r);
	fillRect(0, 0, devSize.width, devSize.height);
	setRGBA(1, 1, 1, 1);
}


// -----------------------------------
// popMainMenu
// -----------------------------------

iPopup* popMainMenu;
iImage** imgMainMenuBtn;

void draPopMainMenuBefore(float dt, iPopup* pop);

void createPopMainMenu()
{
	iPopup* pop = new iPopup();

	iGraphics* g = new iGraphics();
	//버튼
	iSize size = iSizeMake(120, 40);
	Texture* tex;
	iImage* img;

	// imgMianMenuBtn[3]
	const char* str[3] = { "게임 시작", "옵 션", "게임 종료" };
	setStringName("assets/font/DungGeunMo.ttf");
	setStringSize(20);
	setStringBorder(0);

	imgMainMenuBtn = new iImage * [3];

	igImage* ig = g->createIgImage("assets/button/Button.png");

	for (int i = 0; i < 3; i++)
	{
		img = new iImage();

		for (int j = 0; j < 2; j++)
		{
			g->init(size);

			if (j == 0)
			{
				setRGBA(1, 1, 1, 1);
				setStringRGBA(0.5, 0.6, 0.5, 1);
			}
			else// if (j == 1)
			{
				setRGBA(0.5, 0.5, 0.5, 1);
				setStringRGBA(1, 1, 1, 1);
			}
			g->drawIgImage(ig, 0, 0, 1.2, 1);
			g->drawString(size.width / 2, size.height / 2, VCENTER | HCENTER, str[i]);

			tex = g->getTexture();

			img->addObject(tex);
			freeImage(tex);
		}
		img->position = iPointMake(devSize.width / 2 - img->tex->width/2 , devSize.height*2/3 + 70 * i);
		pop->addObject(img);
		imgMainMenuBtn[i] = img;
	}

	g->freeIgImage(ig);
	setRGBA(1, 1, 1, 1);

	pop->style = iPopupStyleAlpha;
	pop->openPoint = iPointMake(0, -60);
	pop->closePoint = iPointZero;
	pop->methodDrawBefore = draPopMainMenuBefore;
	popMainMenu = pop;
	delete g;
}

void freePopMainMenu()
{
	delete popMainMenu;
	delete imgMainMenuBtn;
}

void draPopMainMenuBefore(float dt, iPopup* pop)
{
	for (int i = 0; i < 3; i++)
		imgMainMenuBtn[i]->frame = (i == popMainMenu->selected);
}

void drawPopMainMenu(float dt)
{
	popMainMenu->paint(dt);
}

bool keyPopMainMenu(iKeyState stat, iPoint p)
{
	if (popMainMenu->bShow == false)
		return false;
	if (popMainMenu->state != iPopupStateProc)
		return true;

	int i, j = -1;

	switch (stat) {

	case iKeyStateBegan:
		i = popMainMenu->selected;
		if (i == -1) break;

		audioPlay(snd_btn_ok);
		if (i == 0)
		{
			printf("게임 시작\n");
			showPopMainMenu(false);
			SceneMgt::instance()->set(GameScenePlay, new Game);
		}
		else if (i == 1)
		{
			printf("옵 션\n");
			showPopSetting(true);
		}
		else// if (i == 2)
		{
			printf("게임 종료\n");
			showPopQuit(true);
		}
		break;
	
		case iKeyStateMoved:
		for (i = 0; i < 3; i++)
		{
			if (containPoint(p, imgMainMenuBtn[i]->rect(popMainMenu->closePoint)))
			{
				j = i;
				break;
			}
		}
		if (j != popMainMenu->selected)
		{
			printf("audio play btn\n");
			audioPlay(snd_btn_sel);
			popMainMenu->selected = j;
		}
		break;

	case iKeyStateEnded:
		break;
	}

	return true;
}

void showPopMainMenu(bool show)
{
	popMainMenu->show(show);
}

// -----------------------------------
// popSetting
// -----------------------------------
iPopup* popSetting;
iImage** imgSettingBtn;

void draPopSettingBefore(float dt, iPopup* pop);

float rangeBtn[2];// min / max about x

void createPopSetting()
{
	iPopup* pop = new iPopup();
	Texture* tex = createImage("assets/button/UI.png");
	setTexture(tex->texID, NEAREST);
	tex->width *= 10;
	tex->height *= 10;
	tex->potWidth *= 10;
	tex->potHeight *= 10;
	iImage* img = new iImage();
	img->addObject(tex);
	freeImage(tex);
	pop->addObject(img);

	iGraphics* g = new iGraphics();
	iSize size = iSizeMake(320, 320);
	g->init(size);
	setRGBA(1,1,1,1);


	setStringName("assets/font/DungGeunMo.ttf");
	setStringSize(30);
	setStringRGBA(0.5, 0.6, 0.5, 1);
	setStringBorder(0);
	g->drawString(size.width / 2, 50, TOP | HCENTER, "옵 션");

	const char* strOption[2] = { "배경음악", "효과음" };
	setStringSize(20);
	setStringBorder(0);

	setRGBA(0.5, 0.6, 0.5, 1);
	for (int i = 0; i < 2; i++)
	{
		g->drawString(120, 160 + 40 * i, VCENTER | RIGHT, strOption[i]);
		g->fillRect(140, 160 - 1 + 40 * i, 140, 2, 1);
	}

	tex = g->getTexture();
	img = new iImage();
	img->addObject(tex);
	freeImage(tex);

	pop->addObject(img);

	// 버튼
	imgSettingBtn = new iImage * [3];

	// [0] -    [1] - clone
	img = new iImage();
	size = iSizeMake(20, 20);
	for (int i = 0; i < 2; i++)
	{
		g->init(size);

		if (i == 0)
			setRGBA(0.5, 0.6, 0.5, 1);
		else
			setRGBA(0.5, 0.6, 0.5, 0.5f);
		g->fillRect(0, 0, size.width, size.height, size.width / 2);

		tex = g->getTexture();
		img->addObject(tex);
		freeImage(tex);
	}
	img->position = iPointMake(140 - 10, 160 - 10);
	pop->addObject(img);
	imgSettingBtn[0] = img;

	img = img->clone();
	img->position.y += 40;
	pop->addObject(img);
	imgSettingBtn[1] = img;

	rangeBtn[0] = 140 - 10; 
	rangeBtn[1] = rangeBtn[0] + 140;

	igImage* ig = g->createIgImage("assets/button/Button.png");
	// [2]
	img = new iImage();
	size = iSizeMake(120, 40);
	for (int j = 0; j < 2; j++)
	{
		g->init(size);

		if (j == 0)
		{
			setRGBA(1, 1, 1, 1);
			setStringRGBA(0.5, 0.6, 0.5, 1);
		}
		else// if (j == 1)
		{
			setRGBA(0.5, 0.5, 0.5, 1);
			setStringRGBA(1, 1, 1, 1);
		}
		g->drawIgImage(ig, 0, 0, 1.2, 1);
		g->drawString(size.width / 2, size.height / 2, VCENTER | HCENTER, "닫기");

		tex = g->getTexture();

		img->addObject(tex);
		freeImage(tex);
	}

	img->position = iPointMake(100, 250);
	pop->addObject(img);
	imgSettingBtn[2] = img;

	pop->style = iPopupStyleZoom;
	pop->openPoint = imgMainMenuBtn[1]->center(popMainMenu->closePoint);
	pop->closePoint = iPointMake((devSize.width - 320) / 2, (devSize.height - 320) / 2);
	pop->methodDrawBefore = draPopSettingBefore;
	popSetting = pop;
	delete g;

	//시작 볼륨을 30 퍼센트로 맞춤 1 퍼센트당 1.4f 커짐 
	imgSettingBtn[0]->position.x += 42;
	imgSettingBtn[1]->position.x += 42;
}

void freePopSetting()
{
	delete popSetting;
	delete imgSettingBtn;
}

void draPopSettingBefore(float dt, iPopup* pop)
{
	drawPopCommon(dt, pop, iColor4fMake(0, 0, 0, 0.6f));
	for (int i = 0; i < 3; i++)
		imgSettingBtn[i]->frame = (i == popSetting->selected);
}

void drawPopSetting(float dt)
{
	popSetting->paint(dt);
}

bool settingDrag = false;
iPoint prevPoint;

bool keyPopSetting(iKeyState stat, iPoint p)
{
	if (popSetting->bShow == false)
		return false;
	if (popSetting->state != iPopupStateProc)
		return true;

	int i, j = -1;

	switch (stat) {

	case iKeyStateBegan:
		i = popSetting->selected;
		if (i == -1) break;

		if (i == 0)
		{
			printf("thumb-up\n");
			settingDrag = true;
			prevPoint = p;
		}
		else if (i == 1)
		{
			printf("thumb-down\n");
			settingDrag = true;
			prevPoint = p;
		}
		else// if (i == 2)
		{
			printf("close\n");
			audioPlay(snd_btn_ok);
			showPopSetting(false);

		}
		break;

	case iKeyStateMoved:
		if (settingDrag)
		{
			iPoint mp = (p - prevPoint);
			prevPoint = p;
			float& x = imgSettingBtn[popSetting->selected]->position.x;
			x += mp.x;
			if (x < rangeBtn[0])		x = rangeBtn[0];
			else if (x > rangeBtn[1])	x = rangeBtn[1];

			float percent = (x - rangeBtn[0]) / (rangeBtn[1] - rangeBtn[0]);
			printf("%s %.f%%\n",
				popSetting->selected == 0 ? "bgm" : "sfx",
				percent * 100);
#if 0
			printf("%s %.f%%\n",
				popSetting->selected == 0 ? "bgm" : "sfx",
				(x - rangeBtn[0]) / (rangeBtn[1] - rangeBtn[0]) * 100);
#endif // 0
			/// 볼륨 조절
			float vol[2];
			for (int i = 0; i < 2; i++)
			{
				float& x = imgSettingBtn[i]->position.x;
				vol[i] = (x - rangeBtn[0]) / (rangeBtn[1] - rangeBtn[0]);
			}
			audioVolume(vol[0], vol[1], snd_eff_max);
			break;
		}
		for (i = 0; i < 3; i++)
		{
			if (containPoint(p, imgSettingBtn[i]->rect(popSetting->closePoint)))
			{
				j = i;
				break;
			}
		}
		if (j != popSetting->selected)
		{
			//printf("audio play btn\n");
			//audioPlay(snd_btn_sel);
			popSetting->selected = j;
		}
		break;

	case iKeyStateEnded:
		if (settingDrag)
		{
			settingDrag = false;
			popSetting->selected = -1;
			audioPlay(snd_btn_ok);
		}
		break;

	}

	return true;
}

void showPopSetting(bool show)
{
	popSetting->show(show);
}

// -----------------------------------
// popQuit(게임 종료) 
// -----------------------------------
iPopup* popQuit;
iImage** imgQuitBtn;

void draPopQuitBefore(float dt, iPopup* pop);
void closePopQuit(iPopup* pop);

void createPopQuit()
{
	iPopup* pop = new iPopup();
#if 1
	Texture* tex = createImage("assets/button/UI.png");
	setTexture(tex->texID, NEAREST);
	tex->width *= 10;
	tex->height *= 5;
	tex->potWidth *= 10;
	tex->potHeight *= 5;
#endif // 1
	iImage* img = new iImage();
	img->addObject(tex);
	freeImage(tex);
	pop->addObject(img);

	iGraphics* g = new iGraphics();
	iSize size = iSizeMake(320, 160);
	g->init(size);
	setRGBA(1, 1, 1, 1);

	setStringName("assets/font/DungGeunMo.ttf");
	setStringSize(25);
	setStringRGBA(0.5, 0.6, 0.5, 1);
	setStringBorder(0);
	g->drawString(size.width / 2, 40, TOP | HCENTER, "종료하시겠습니까?");

	setStringSize(20);
	const char* strOption[2] = { "예", "아니오" };
	for (int i = 0; i < 2; i++)
	{
		g->drawString(100 + 110 * i, 110, TOP | LEFT, strOption[i]);
	}

	tex = g->getTexture();
	img = new iImage();
	img->addObject(tex);
	freeImage(tex);
	pop->addObject(img);


	//버튼
	imgQuitBtn = new iImage * [2];
	igImage* igs[2];

	for (int i = 0; i < 2; i++)
	{
		igs[i] = g->createIgImage("assets/button/Select%d.png", i);
	}

	size = iSizeMake(32, 32);

	for (int i = 0; i < 2; i++)
	{
		img = new iImage();

		for (int j = 0; j < 2; j++)
		{
			g->init(size);
			if (j == 0)
			{
				setRGBA(1, 1, 1, 1);
			}
			else// if (j == 1)
			{
				setRGBA(0.5, 0.5, 0.5, 1);
			}
			g->drawIgImage(igs[i], 0, 0);
			tex = g->getTexture();

			img->addObject(tex);
			freeImage(tex);
		}
		img->position = iPointMake(60+110*i, 100);
		pop->addObject(img);
		imgQuitBtn[i] = img;
	}

	for (int i = 0; i < 2; i++)
	{
		g->freeIgImage(igs[i]);
	}
	setRGBA(1, 1, 1, 1);

	pop->style = iPopupStyleAlpha;
	pop->openPoint = iPointMake(devSize.width/2, devSize.height/2);
	pop->closePoint = iPointMake((devSize.width - 320) / 2, (devSize.height - 160) / 2);
	pop->methodDrawBefore = draPopQuitBefore;
	pop->methodClose = closePopQuit;
	popQuit = pop;
	delete g;
}

void freePopQuit()
{
	delete popQuit;
	delete imgQuitBtn;
}


void draPopQuitBefore(float dt, iPopup* pop)
{
	drawPopCommon(dt, pop, iColor4fMake(0, 0, 0, 0.6f));

	for (int i = 0; i < 2; i++)
		imgQuitBtn[i]->frame = (i == popQuit->selected);
}

void drawPopQuit(float dt)
{
	popQuit->paint(dt);
}

bool exeQuit;
void closePopQuit(iPopup* pop)
{
	if (exeQuit)
	{
		extern bool runApp;
		runApp = false;
	}
	extern iPopup* popPause;
	if (popPause->bShow == false)//일시정지 상태가 아닐때
	{
		extern bool gamePause;
		gamePause = false;
	}
}

bool keyPopQuit(iKeyState stat, iPoint p)
{
	if (popQuit->bShow == false)
		return false;
	if (popQuit->state != iPopupStateProc)
		return true;

	int i, j = -1;

	switch (stat) {

	case iKeyStateBegan:
		i = popQuit->selected;
		if (i == -1) break;

		audioPlay(snd_btn_ok);
		if (i == 0)
		{
			exeQuit = true;
		}
		else //if (i == 1)
		{
			exeQuit = false;
		}
		showPopQuit(false);
		break;

	case iKeyStateMoved:
		for (i = 0; i < 2; i++)
		{
			if (containPoint(p, imgQuitBtn[i]->rect(popQuit->closePoint)))
			{
				j = i;
				break;
			}
		}
		if (j != popQuit->selected)
		{
			printf("audio play btn\n");
			audioPlay(snd_btn_sel);
			popQuit->selected = j;
		}
		break;

	case iKeyStateEnded:
		break;

	}

	return true;
}


void showPopQuit(bool show)
{
	popQuit->show(show);
}


// -----------------------------------
// popGameOver(게임 오버) 
// -----------------------------------
iPopup* popGameOver;
iImage** imgGameOverBtn;

void draPopGameOverBefore(float dt, iPopup* pop);
void closePopGameOver(iPopup* pop);

void createPopGameOver()
{
	iPopup* pop = new iPopup();
#if 1
	Texture* tex = createImage("assets/button/UI.png");
	setTexture(tex->texID, NEAREST);
	tex->width *= 10;
	tex->height *= 5;
	tex->potWidth *= 10;
	tex->potHeight *= 5;
#endif // 1
	iImage* img = new iImage();
	img->addObject(tex);
	freeImage(tex);
	pop->addObject(img);

	iGraphics* g = new iGraphics();
	iSize size = iSizeMake(320, 160);
	g->init(size);
	setRGBA(1, 1, 1, 1);

	setStringName("assets/font/DungGeunMo.ttf");
	setStringSize(25);
	setStringRGBA(0.5, 0.6, 0.5, 1);
	setStringBorder(0);
	g->drawString(size.width / 2, 40, TOP | HCENTER, "재도전하시겠습니까?");

	setStringSize(20);
	const char* strOption[2] = { "예", "아니오" };
	for (int i = 0; i < 2; i++)
	{
		g->drawString(100 + 110 * i, 110, TOP | LEFT, strOption[i]);
	}

	tex = g->getTexture();
	img = new iImage();
	img->addObject(tex);
	freeImage(tex);
	pop->addObject(img);


	//버튼
	imgGameOverBtn = new iImage * [2];
	igImage* igs[2];

	for (int i = 0; i < 2; i++)
	{
		igs[i] = g->createIgImage("assets/button/Select%d.png", i);
	}

	size = iSizeMake(32, 32);

	for (int i = 0; i < 2; i++)
	{
		img = new iImage();

		for (int j = 0; j < 2; j++)
		{
			g->init(size);
			if (j == 0)
			{
				setRGBA(1, 1, 1, 1);
			}
			else// if (j == 1)
			{
				setRGBA(0.5, 0.5, 0.5, 1);
			}
			g->drawIgImage(igs[i], 0, 0);
			tex = g->getTexture();

			img->addObject(tex);
			freeImage(tex);
		}
		img->position = iPointMake(60 + 110 * i, 100);
		pop->addObject(img);
		imgGameOverBtn[i] = img;
	}

	for (int i = 0; i < 2; i++)
	{
		g->freeIgImage(igs[i]);
	}
	setRGBA(1, 1, 1, 1);

	pop->style = iPopupStyleAlpha;
	pop->openPoint = iPointMake(devSize.width / 2, devSize.height / 2);
	pop->closePoint = iPointMake((devSize.width - 320) / 2, (devSize.height - 160) / 2);
	pop->methodDrawBefore = draPopGameOverBefore;
	pop->methodClose = closePopGameOver;
	popGameOver = pop;
	delete g;
}

void freePopGameOver()
{
	delete popGameOver;
	delete imgGameOverBtn;
}

void draPopGameOverBefore(float dt, iPopup* pop) 
{
	drawPopCommon(dt, pop, iColor4fMake(0, 0, 0, 0.6f));

	for (int i = 0; i < 2; i++)
		imgGameOverBtn[i]->frame = (i == popQuit->selected);
}



void drawPopGameOver(float dt)
{
	popGameOver->paint(dt);
}

//dn...dj...
bool exeRestart;
void closePopGameOver(iPopup* pop)
{
	if (exeRestart)
	{
		extern bool gameRestart;
		gameRestart = true;
	}
}

bool keyPopGameOver(iKeyState stat, iPoint p)
{
	if (popGameOver->bShow == false)
		return false;
	if (popGameOver->state != iPopupStateProc)
		return true;

	int i, j = -1;

	switch (stat) {

	case iKeyStateBegan:
		i = popQuit->selected;
		if (i == -1) break;

		if (i == 0)
		{
			exeRestart = true;
		}
		else //if (i == 1)
		{
			exeRestart = false;
		}
		showPopGameOver(false);
		break;

	case iKeyStateMoved:
		for (i = 0; i < 2; i++)
		{
			if (containPoint(p, imgQuitBtn[i]->rect(popQuit->closePoint)))
			{
				j = i;
				break;
			}
		}
		if (j != popQuit->selected)
		{
			printf("audio play btn\n");
			popQuit->selected = j;
		}
		break;

	case iKeyStateEnded:
		break;

	}

	return true;
}

void showPopGameOver(bool show)
{
	popGameOver->show(show);
}


// -----------------------------------
// popPause(게임 정지) 
// -----------------------------------

iPopup* popPause;

void draPopPauseBefore(float dt, iPopup* pop);

void createPopPause()
{
	iPopup* pop = new iPopup();

	Texture* tex = createImage("assets/button/UI.png");
	setTexture(tex->texID, NEAREST);
	tex->width *= 10;
	tex->height *= 5;
	tex->potWidth *= 10;
	tex->potHeight *= 5;

	iImage* img = new iImage();
	img->addObject(tex);
	freeImage(tex);
	pop->addObject(img);

	iGraphics* g = new iGraphics();
	iSize size = iSizeMake(320, 160);
	g->init(size);
	setRGBA(1, 1, 1, 1);

	setStringName("assets/font/DungGeunMo.ttf");
	setStringSize(30);
	setStringRGBA(0.5, 0.6, 0.5, 1);
	setStringBorder(0);
	g->drawString(size.width / 2, 50, TOP | HCENTER, "-일시정지-");
	setStringSize(20);
	g->drawString(size.width / 2, 100, TOP | HCENTER, "한번 더 누르면 돌아갑니다");

	tex = g->getTexture();
	img = new iImage();
	img->addObject(tex);
	freeImage(tex);
	pop->addObject(img);

	pop->style = iPopupStyleAlpha;
	pop->openPoint = iPointMake(devSize.width / 2, devSize.height / 2);
	pop->closePoint = iPointMake((devSize.width - 320) / 2, (devSize.height - 160) / 2);
	pop->methodDrawBefore = draPopPauseBefore;
	popPause = pop;
	delete g;

}
void freePopPause()
{
	delete popPause;
}

void draPopPauseBefore(float dt, iPopup* pop)
{
	drawPopCommon(dt, pop, iColor4fMake(0, 0, 0, 0.6f));
}


void drawPopPause(float dt)
{
	popPause->paint(dt);
}

bool keyPopPause(iKeyState stat, iPoint p)
{
	if (popQuit->bShow == false)
		return false;
	if (popQuit->state != iPopupStateProc)
		return true;

	switch (stat) {

	case iKeyStateBegan:
		break;
	case iKeyStateMoved:
		break;
	case iKeyStateEnded:
		break;
	}

	return true;
}
void showPopPause(bool show)
{
	popPause->show(show);
}


// -----------------------------------
// popClear(게임 클리어) 
// -----------------------------------

iPopup* popClear;
iImage** imgClearBtn;

void draPopClearBefore(float dt, iPopup* pop);
void closePopClear(iPopup* pop);

void createPopClear()
{
	iPopup* pop = new iPopup();
#if 1
	Texture* tex = createImage("assets/button/UI.png");
	setTexture(tex->texID, NEAREST);
	tex->width *= 10;
	tex->height *= 10;
	tex->potWidth *= 10;
	tex->potHeight *= 10;
#endif // 1
	iImage* img = new iImage();
	img->addObject(tex);
	freeImage(tex);
	pop->addObject(img);

	iGraphics* g = new iGraphics();
	iSize size = iSizeMake(320, 320);
	g->init(size);
	setRGBA(1, 1, 1, 1);

	setStringName("assets/font/DungGeunMo.ttf");
	setStringSize(40);
	setStringRGBA(0.5, 0.6, 0.5, 1);
	setStringBorder(0);
	g->drawString(size.width / 2, 45, TOP | HCENTER, "Stage Clear!");

	//임시
	setStringSize(20);
	g->drawString(size.width / 2, 90, TOP | HCENTER, "________________________"); 
	g->drawString(size.width / 2, 120, TOP | HCENTER, "◆ 2분 30초 이내 클리어"); 
	g->drawString(size.width / 2, 150, TOP | HCENTER, "◆ 1분 이내 클리어"); 
	g->drawString(size.width / 2, 180, TOP | HCENTER, "◇ 체력 50 이상으로 클리어"); 
	//

	//임시
	setStringSize(10); 
	g->drawString(75, 298, TOP | HCENTER, "메인메뉴");
	g->drawString(160, 298, TOP | HCENTER, "다시하기");
	g->drawString(245, 298, TOP | HCENTER, "다음스테이지");
	//
	tex = g->getTexture();
	img = new iImage();
	img->addObject(tex);
	freeImage(tex);

	pop->addObject(img);

	//버튼 
	size = iSizeMake(64, 64);

	const char* str[3] = {"▣", "@", "▶"};
	setStringName("assets/font/DungGeunMo.ttf");
	setStringSize(30);
	setStringBorder(0);

	imgClearBtn = new iImage * [3];
	igImage* ig = g->createIgImage("assets/button/ui.png");
	for (int i = 0; i < 3; i++)
	{
		img = new iImage();
		for (int j = 0; j < 2; j++)
		{
			g->init(size);

			if (j == 0)
			{
				setRGBA(1, 1, 1, 1);
				setStringRGBA(0.5, 0.6, 0.5, 1);
			}
			else// if (j == 1)
			{
				setRGBA(0.5, 0.5, 0.5, 1);
				setStringRGBA(1, 1, 1, 1);
			}
			g->drawIgImage(ig, 0, 0, 2, 2);
			g->drawString(size.width / 2, size.height / 2, VCENTER | HCENTER, str[i]);

			tex = g->getTexture();

			img->addObject(tex);
			freeImage(tex);
		}
		img->position = iPointMake( 45 + img->tex->width*i + 20*i, 230);
		pop->addObject(img);
		imgClearBtn[i] = img;
	}

	g->freeIgImage(ig);
	setRGBA(1, 1, 1, 1);
	//

	pop->style = iPopupStyleZoom;
	pop->openPoint = iPointMake(devSize.width/ 2, devSize.height / 2);
	pop->closePoint = iPointMake((devSize.width - 320) / 2, (devSize.height - 320) / 2);
	pop->methodDrawBefore = draPopClearBefore;
	pop->methodClose = closePopGameOver;
	popClear = pop;
	delete g;
}

void freePopClear()
{
	delete popClear;
	delete imgClearBtn;
}

void draPopClearBefore(float dt, iPopup* pop)
{
	drawPopCommon(dt, pop, iColor4fMake(0, 0, 0, 0.6f));
	for (int i = 0; i < 3; i++)
		imgClearBtn[i]->frame = (i == popClear->selected);
}

void drawPopClear(float dt)
{
	popClear->paint(dt);
}

bool keyPopClear(iKeyState stat, iPoint p)
{
	if (popClear->bShow == false)
		return false;
	if (popClear->state != iPopupStateProc)
		return true;

	int i, j = -1;

	switch (stat) {

	case iKeyStateBegan:
		i = popClear->selected;
		if (i == -1) break;

		if (i == 0) //메인으로
		{
			exeRestart = false;
			mapClearCheck();
		}
		else if (i == 1) //다시하기
		{
			exeRestart = true;
			resetKillCount();
		}
		else if (i == 2)//다음스테이지
		{
			exeRestart = true;
			mapClearCheck();
		}

		showPopClear(false);
		break;
	case iKeyStateMoved:
		for (i = 0; i < 3; i++)
		{
			if (containPoint(p, imgClearBtn[i]->rect(popClear->closePoint)))
			{
				j = i;
				break;
			}
		}
		if (j != popClear->selected)
		{
			printf("Claer btn\n");
			popClear->selected = j;
		}
		break;
	case iKeyStateEnded:
		break;
	}

	return true;
}

void showPopClear(bool show)
{
	popClear->show(show);
}