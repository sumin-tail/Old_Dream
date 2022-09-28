#pragma once
//맵 드로우
#include "BackGround.h"

void loadMap();
void freeMap();
void drawMap(float dt);
bool keyMap(iKeyState stat, iPoint p);

#define TILE_WIDTH 40
#define TILE_HEIGHT 40

#define tileNumX 64
#define tileNumY 18

static int mapCount = 0;
static bool mapClear = false;
static int killCount = 0;

void setMapCount(int n);
void mapClearCheck();
void MapCountCheck();
int mapcount();


void resetKillCount();
void upKillCount();
bool killCheck();

//맵 타일 
struct Tile
{
	int attr;// 타일 속성 
	//일단 이미지 없이 그냥 사각형 색칠로
};

//타일 좍 가지고잇는 구조체
struct TileData
{
	TileData(int numW, int numH); //맵크기
	virtual ~TileData();

	int numW, numH;
	int tileNum;
	Tile* tiles;
};

extern Tile tile[][tileNumX * tileNumY];
extern int mapKillCount[2];

//타일 리스트 가지고 잇는 구조체
struct Map
{
	Map();
	virtual ~Map();
	void paint(float dt);

	TileData* td;
};


int maxMoveR(iPoint p, iRect rt, bool player = true); //오
int maxMoveL(iPoint p, iRect rt, bool player = true); //왼
int maxMoveT(iPoint p, iRect rt); //위
int maxMoveB(iPoint p, iRect rt); //아래