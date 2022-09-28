#pragma once
//로딩 관리 + 씬관리
#include "iStd.h"

enum GameScene
{
	GameSceneTitle = 0,//타이틀 씬
	GameSceneMenu,//메인메뉴 씬
	GameScenePlay,//인게임(플레이중) 씬
	GameSceneEnd, //사망 씬

	GameStateNone //아무것도 아님
};

struct Scene
{
	virtual void load() = 0;
	virtual void free() = 0;
	virtual void draw(float dt) = 0;
	virtual bool key(iKeyState stat, iPoint p) = 0;
};

class SceneMgt
{
private:
	SceneMgt();
public:
	static SceneMgt* instance();
	virtual ~SceneMgt();

	void set(GameScene gs, Scene* nextScene, bool now=false);
	void draw(float dt);
	bool key(iKeyState stat, iPoint p);

	GameScene gs, gsNew;
	Scene* currScene, * nextScene;
	float delta;
};
#define sceneDelta 0.7f
