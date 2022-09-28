#pragma once
#include "iStd.h" //필수자료

//백그라운드(배경이미지) 관리
struct BackGround
{
	BackGround();
	virtual ~BackGround();
	//이미지 저장 변수

	void move(iPoint mp, bool loop = false);
	void Draw(float dt);

	Texture** bgtexs; //백그라운드 이미지
	iPoint off, offMin, offMax; // 좌표값
	float bgLength;
};

extern BackGround* bg;
