#pragma once

#include "iStd.h"

//인터페이스(UI) 관리
void loadUI();
void freeUI();
void drawUI(float dt);
void keyUI(iKeyState stat, iPoint p);

//extern enum sndList;

enum sndList {
	snd_btn_sel = 0, //마우스 클릭
	snd_btn_ok, //마우스 오버
	snd_btn_atk, //공격
	snd_btn_clear, //클리어
	snd_btn_item, //아이템 습득

	snd_eff_max,
	//이아래로 브금
	snd_bgm_title = snd_eff_max,
	snd_bgm_map,

	snd_max,
};

//pop = popup
// -----------------------------------
// popMainMenu
// -----------------------------------
void createPopMainMenu();
void freePopMainMenu();
void drawPopMainMenu(float dt);
bool keyPopMainMenu(iKeyState stat, iPoint p);
void showPopMainMenu(bool show);

// -----------------------------------
// popSetting(옵션 쪽)
// -----------------------------------
void createPopSetting();
void freePopSetting();
void drawPopSetting(float dt);
bool keyPopSetting(iKeyState stat, iPoint p);
void showPopSetting(bool show);


// -----------------------------------
// popQuit(게임 종료) 
// -----------------------------------
void createPopQuit();
void freePopQuit();
void drawPopQuit(float dt);
bool keyPopQuit(iKeyState stat, iPoint p);
void showPopQuit(bool show);

// -----------------------------------
// popGameOver(게임 오버) 
// -----------------------------------
void createPopGameOver();
void freePopGameOver();
void drawPopGameOver(float dt);
bool keyPopGameOver(iKeyState stat, iPoint p);
void showPopGameOver(bool show);

// -----------------------------------
// popPause(게임 정지) 
// -----------------------------------
void createPopPause();
void freePopPause();
void drawPopPause(float dt);
bool keyPopPause(iKeyState stat, iPoint p);
void showPopPause(bool show);


// -----------------------------------
// popClear(게임 클리어) 
// -----------------------------------
void createPopClear();
void freePopClear();
void drawPopClear(float dt);
bool keyPopClear(iKeyState stat, iPoint p);
void showPopClear(bool show);
