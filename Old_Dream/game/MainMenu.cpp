#include "MainMenu.h"
#include "Game.h"

#include "BackGround.h" //백 그라운드

//타이틀 로드
void Title::load()
{
#if 0
	texTitle = createImage("assets/Title.png");
#else
	iGraphics* g = new iGraphics();
	iSize size = devSize;
	g->init(size);

	setRGBA(1, 1, 1, 1);
	igImage* ig = g->createIgImage("assets/Title.png");
	g->drawIgImage(ig, 0, 0, TOP | LEFT);
	g->freeIgImage(ig);

	setStringName("assets/font/DungGeunMo.ttf");
	setStringSize(20);
	setStringRGBA(1, 1, 1, 1);
	setStringBorder(1);
	setStringBorderRGBA(1, 0, 0, 1);
	const char* str[4] = { 
		"이름 : 김수민", 
		"개발도구 : WinAPI, OpenGL, GDI+, GLSL, OpenAL",
		"사용언어 : c/c++",
		"개발기간 : 3개월" };
	//GDI+ , GLSL
	for(int i=0; i<4; i++)
		g->drawString(0, 600+30*i, TOP | LEFT, str[i]);

	setStringSize(20);
	setStringBorder(0);
	g->drawString(devSize.width	/2, 500, VCENTER | HCENTER, "- 클릭하면 넘어갑니다 -");


	texTitle = g->getTexture();

#endif
}

//타이플 할당 해제
void Title::free()
{
	freeImage(texTitle);
}

void Title::draw(float dt)
{
	setRGBA(1, 1, 1, 1);
	drawImage(texTitle, 0, 0, TOP | LEFT);
}

bool Title::key(iKeyState stat, iPoint p)
{
	//아무 키나 누르면 넘어가도록

	//클릭하면 넘어감dgg
	switch (stat) {
	case iKeyStateBegan:
		SceneMgt::instance()->set(GameSceneMenu, new MainMenu);
		break;
	}
	return true;
}

void MainMenu::load() 
{
	bg = new BackGround;

	texLogo = createImage("assets/logo.png"); 

#if 1
	showPopMainMenu(true);

#else
	imgButton = new iImage* [3];
	for (int i = 0; i < 3; i++)
	{
		iImage* img = new iImage();
		Texture* tex = NULL;
		for (int j = 0; j < 2; j++)
		{
			tex = createImage("assets/button/MainMenuButton%d%d.png", i, j);
			img->addObject(tex);
			freeImage(tex);
		}
		img->position = iPointMake((devSize.width - tex->width) / 2,
									devSize.height / 2 + 100 + (tex->height + 10) * i);
		imgButton[i] = img;
	}

	selectedButton = -1;
#endif // 0

}

void MainMenu::free()
{
	delete bg;

	freeImage(texLogo);
#if 1

#else
	for (int i = 0; i < 3; i++)
		delete imgButton[i];
	delete imgButton;

#endif // 0

}


void MainMenu::draw(float dt)
{
	setRGBA(1, 1, 1, 1);
	bg->move(iPointMake(-100 * dt, 0), true); 
	bg->Draw(dt); //배경 드로잉

	drawImage(texLogo, 0, 0, 2, 2, TOP | LEFT,
		0, 0, texLogo->width, texLogo->height, 2, 0);

#if 1

	drawUI(dt);

#else
	for (int i = 0; i < 3; i++)
	{
		imgButton[i]->frame = (i == selectedButton);
		imgButton[i]->paint(0.0f, iPointZero);
	}
#endif // 0

}

bool MainMenu::key(iKeyState stat, iPoint p)
{

	//메인 키 
	 keyUI(stat, p);
#if 0
	int i, j = -1;
	switch (stat)
	{
	case iKeyStateBegan:
	{

		i = selectedButton;
		if (i == -1) break;

		if (i == 0)
		{
			//SceneMgt::instance()->set(GameSceneTitle, new Title);
			SceneMgt::instance()->set(GameScenePlay, new Game);
		}
		else if (i == 1)
		{
			// option
		}
		else// if (i == 2) //게임 종료
		{
			extern bool runApp;
			runApp = false;
		}

		break;

	}
	case iKeyStateMoved:
		for (i = 0; i < 3; i++)
		{
			if (containPoint(p, imgButton[i]->rect()))
			{
				j = i;
				break;
			}
		}
		if (selectedButton != j)
		{
			printf("btn[%d] high\n", j);
			selectedButton = j;
		}


		break;

	case iKeyStateEnded:
		break;
	}
#endif // 0
	return true;
}
