#include "GameManager.h"
#include "Scene.h"

static GameManager* _GameManager = NULL; //게임매니저

GameManager* GameManager::instance()
{
	if (_GameManager == NULL)
		_GameManager = new GameManager();

	return _GameManager;
}

GameManager::GameManager() //생성자
{
	SceneMgt::instance(); //씬 매니저 만듬
}

GameManager::~GameManager()
{
	_GameManager = NULL;
	delete _GameManager;
}


void GameManager::loadGame()
{
	SceneMgt::instance()->set(GameSceneTitle, new Title, true);
}

void GameManager::freeGame()
{
	delete SceneMgt::instance();
}

void GameManager::drawGame(float dt)
{
	SceneMgt::instance()->draw(dt); //드로우
}

bool GameManager::keyGame(iKeyState stat, iPoint p)
{

	return SceneMgt::instance()->key(stat, p);
}