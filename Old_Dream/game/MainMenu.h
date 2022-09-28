#pragma once
//메인 메뉴 /타이틀 드로잉
#include "iStd.h"

#include "Scene.h"


struct Title : Scene
{
	virtual void load(); //초기화
	virtual void free(); //타이플 할당 해제
	virtual void draw(float dt);
	virtual bool key(iKeyState stat, iPoint p);

	Texture* texTitle; //타이틀 
};


struct MainMenu : Scene
{
	virtual void load(); 
	virtual void free(); 
	virtual void draw(float dt);
	virtual bool key(iKeyState stat, iPoint p);

	Texture* texLogo;//로고
	iImage** imgButton; //버튼
	int selectedButton;
};


