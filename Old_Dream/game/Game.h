#pragma once

#include "Scene.h"
#include "Map.h"

#include "Player.h"
#include "Monster.h"
#include "Interface.h"
#include "Item.h"

struct Game : Scene
{
	virtual void load();
	virtual void free();
	virtual void draw(float dt);
	virtual bool key(iKeyState stat, iPoint p);

	void loadGameUI();
	void gameUI();

	float uidt;
	bool gameOver;
	bool gameClear;

	Player* player;

	iStrTex** stGold;
	static Texture* methodStGold(const char* str);
};


