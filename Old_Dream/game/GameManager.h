#pragma once
//게임 매니저 (인게임 관리) 
 //로드 > 업데이트(딜레이 타임)> 드로우 > 프리
//계층 구조

#include "iStd.h" 

//씬
#include "Scene.h" 
#include "Game.h"
#include "MainMenu.h"

//드로우 순서 - 레이어
#include "BackGround.h" //배경
#include "Map.h" //맵

#include "Player.h"
#include "Monster.h"

#include "Weapon.h"
#include "Effect.h"
#include "Interface.h" //UI


//싱글톤이여야함
class GameManager
{
private :
	GameManager(); //생성자 보호
public:
	static GameManager* instance(); //싱글톤 생성
	virtual ~GameManager(); //소멸자

	void loadGame(); //게임 로드
	void freeGame(); //할당 해제

	void drawGame(float dt); //게임드로우

	bool keyGame(iKeyState stat, iPoint p);

};