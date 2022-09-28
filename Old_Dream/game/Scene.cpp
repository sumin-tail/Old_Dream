#include "Scene.h"

SceneMgt::SceneMgt()
{
	gs = GameSceneTitle;
	gsNew = GameSceneTitle;
	currScene = NULL;
	nextScene = NULL;
	delta = 0.0f;
}

static SceneMgt* _instance = NULL;
SceneMgt* SceneMgt::instance()
{
	if (_instance == NULL)
		_instance = new SceneMgt();
	return _instance;
}

SceneMgt::~SceneMgt()
{
	if (currScene)
		currScene->free();
	if (nextScene)
		nextScene->free();

	_instance = NULL;
}

void SceneMgt::set(GameScene nextGs, Scene* nextScene, bool now)
{
	if (now)
	{
		gs = nextGs;
		currScene = nextScene;
		currScene->load();
		return;
	}

	if (delta)
		return;

	gsNew = nextGs;
	this->nextScene = nextScene;
	delta = 0.000001f;
}


void SceneMgt::draw(float dt)
{
	if (currScene)
		currScene->draw(dt);

	if (delta == 0.0f)
		return;

	float alpha = 1.0f;

	// 어두워짐
	if (delta < sceneDelta)
	{
		// 어두워지고 있음.
		alpha = delta / sceneDelta;
		delta += dt;
		if (delta > sceneDelta)
		{
			currScene->free();
			delete currScene;
			currScene = NULL;

			gs = GameStateNone;
			delta = sceneDelta;
		}
	}
	else if (delta < sceneDelta + 0.7f)
	{
		// 완전 어두워짐
		alpha = 1.0f;

		delta += dt;
		if (delta > sceneDelta + 0.6f)
		{
			nextScene->load();
			currScene = nextScene;
			nextScene = NULL;
			gs = gsNew;
			delta = sceneDelta + 0.7f;
		}
	}
	else // if (loadingDt < sceneDelta * 2 + 0.7f)
	{
		// 점점 밝아짐
		alpha = 1.0f - (delta - sceneDelta - 0.7f) / sceneDelta;

		delta += dt;
		if (delta > sceneDelta * 2 + 0.7f)
			delta = 0.0f;// 완전 밝아짐
	}

	printf("%f\n", alpha);
	//검은 색을 알파값에 따라 그림
	setRGBA(0, 0, 0, alpha);
	fillRect(0, 0, devSize.width, devSize.height);
	setRGBA(1, 1, 1, 1);
}

bool SceneMgt::key(iKeyState stat, iPoint p)
{
	if (currScene)
		return currScene->key(stat, p);
	return true;
}
