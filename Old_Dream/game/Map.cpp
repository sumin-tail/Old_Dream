#include "Map.h"

Map* map;

void loadMap()
{
	map = new Map;
}

void freeMap()
{

}

void drawMap(float dt)
{
	//여기서 타일 다 그림
	map->paint(dt);
}

bool keyMap(iKeyState stat, iPoint p)
{
	return false;
}

void setMapCount(int n)
{
	mapCount = n;
	resetKillCount(); //맵이동 할때마다 킬카운트 초기화
}

void mapClearCheck()
{
	mapClear = true;
}


void MapCountCheck()
{
	if (mapClear)
	{
		mapCount++;
		mapClear = false;
		resetKillCount();
	}
}

int mapcount()
{
	return mapCount;
}

void resetKillCount()
{
	killCount = 0;
}

void upKillCount()
{
	killCount++;
}

bool killCheck()
{
	if (killCount == mapKillCount[mapCount])
		return true;	
	else
		return false;
}

TileData::TileData(int numW, int numH)
{
	this->numW = numW;
	this->numH = numH;

	tileNum = numW * numH;
}

TileData::~TileData()
{

}

#define O {0}
#define X {1}
#define M {2}

Tile tile[2][tileNumX * tileNumY] = {
	{
	O,O,O,O,O,O,O,O,O,O,O,O,O,O,O,O,O,O,O,O,O,O,O,O,O,O,O,O,O,O,O,O,O,O,O,O,O,O,O,O,O,O,O,O,O,O,O,O,O,O,O,O,O,O,O,O,O,O,O,O,O,O,O,O,
	O,O,O,O,O,O,O,O,O,O,O,O,O,O,O,O,O,O,O,O,O,O,O,O,O,O,O,O,O,O,O,O,O,O,O,O,O,O,O,O,O,O,O,O,O,O,O,O,O,O,O,O,O,O,O,O,O,O,O,O,O,O,O,O,
	O,O,O,O,O,O,O,O,O,O,O,X,X,O,O,O,O,O,O,O,O,O,O,O,O,O,O,O,O,O,O,O,O,O,O,O,O,O,O,O,O,O,O,O,O,O,O,O,O,O,O,O,O,O,O,O,O,O,O,O,O,O,O,O,
	O,O,O,O,O,O,O,O,O,O,O,X,X,O,O,O,O,O,O,O,O,O,O,O,O,O,O,O,O,O,O,O,O,O,O,O,O,O,O,O,O,O,O,O,O,O,O,O,O,O,O,O,O,O,O,O,O,O,O,O,O,O,O,O,
	O,O,O,O,O,O,O,O,O,O,X,X,O,O,O,O,O,O,O,O,O,O,O,O,O,O,O,O,O,O,O,O,O,O,O,O,O,O,O,O,O,O,O,X,X,X,X,X,X,X,O,O,O,O,O,O,O,O,O,O,O,O,O,O,
	O,O,O,O,O,O,O,O,O,X,X,O,O,O,O,O,O,O,O,O,O,O,O,O,O,O,O,O,O,O,O,O,O,O,O,O,O,O,O,O,O,X,X,X,X,X,X,X,X,O,O,O,O,O,O,O,O,O,O,O,O,O,O,O,
	O,O,O,O,O,O,O,O,O,O,O,O,O,O,O,O,O,O,O,O,O,O,O,O,O,O,O,O,O,O,O,O,O,O,O,O,O,O,O,O,O,O,O,O,O,O,O,O,O,O,O,O,O,O,O,O,O,O,O,O,O,O,O,O,
	O,O,O,O,O,O,O,O,O,O,O,O,O,O,O,O,O,O,O,O,O,O,O,O,O,O,O,O,O,O,O,O,O,O,O,O,O,O,O,O,O,O,O,O,M,M,O,O,O,O,O,O,O,O,O,O,O,O,O,O,O,O,O,O,
	O,O,O,O,O,O,O,O,O,O,O,O,O,O,O,O,O,O,O,O,O,O,O,O,O,O,O,O,O,O,O,O,O,O,O,O,O,O,O,O,O,O,O,O,M,M,O,O,O,O,O,O,O,O,O,O,O,O,O,O,O,O,O,O,
	O,O,O,O,O,O,O,O,O,O,O,X,X,X,X,O,O,O,O,O,O,O,O,O,O,O,O,O,O,O,O,X,X,X,X,O,O,O,O,O,O,O,O,X,X,X,X,O,O,O,O,O,O,O,O,O,O,O,O,O,O,O,O,O,
	O,O,O,O,O,O,O,O,O,O,X,X,X,X,X,X,O,O,O,O,O,O,O,O,O,O,O,X,X,X,X,X,X,X,X,X,O,O,O,O,O,O,X,X,X,X,X,X,X,X,O,O,O,O,O,O,O,O,O,O,O,O,O,O,
	O,O,O,O,O,O,O,O,O,X,X,X,X,X,X,X,O,O,O,O,O,O,X,X,X,X,X,X,X,X,X,X,X,X,X,X,O,O,O,O,O,X,X,X,X,X,X,X,X,X,X,O,O,O,O,O,O,O,O,O,O,O,O,O,
	O,O,O,O,O,O,O,O,O,O,O,O,O,O,O,O,O,O,O,O,O,X,X,X,X,X,X,X,X,X,X,O,O,O,O,O,O,O,O,O,O,O,O,O,O,O,O,O,O,O,O,O,O,O,O,O,O,O,O,O,O,O,O,O,
	O,O,O,O,O,O,O,O,O,O,O,O,O,O,O,O,O,O,O,O,X,X,X,X,X,X,O,O,O,O,O,O,O,O,O,O,O,O,O,O,O,O,O,O,O,O,O,O,O,O,O,O,O,O,O,O,O,O,O,O,O,O,O,O,
	O,O,O,O,O,X,X,X,X,O,O,O,O,O,O,O,O,O,O,O,O,O,O,O,O,O,O,O,O,O,O,O,O,O,O,O,O,X,X,X,X,O,O,O,O,O,O,O,O,O,O,O,O,O,O,O,O,O,O,O,O,O,O,O,
	O,O,O,O,X,X,X,X,X,X,O,O,O,O,O,O,O,O,O,O,O,O,O,O,O,O,O,O,O,O,O,O,O,O,O,O,X,X,X,X,X,X,O,O,O,O,O,O,O,O,O,O,O,O,O,O,O,O,O,O,O,O,O,O,
	X,X,X,X,X,X,X,X,X,X,X,X,X,X,X,X,X,X,X,X,X,X,X,X,X,X,X,X,X,X,X,X,X,X,X,X,X,X,X,X,X,X,X,X,X,X,X,X,X,X,X,X,X,X,X,X,X,X,X,X,X,X,X,X,
	X,X,X,X,X,X,X,X,X,X,X,X,X,X,X,X,X,X,X,X,X,X,X,X,X,X,X,X,X,X,X,X,X,X,X,X,X,X,X,X,X,X,X,X,X,X,X,X,X,X,X,X,X,X,X,X,X,X,X,X,X,X,X,X,
	},
	{
	O,O,O,O,O,O,O,O,O,O,O,O,O,O,O,O,O,O,O,O,O,O,O,O,O,O,O,O,O,O,O,O,O,O,O,O,O,O,O,O,O,O,O,O,O,O,O,O,O,O,O,O,O,O,O,O,O,O,O,O,O,O,O,O,
	O,O,O,O,O,O,O,O,O,O,O,O,O,O,O,O,O,O,O,O,O,O,O,O,O,O,O,O,O,O,O,O,O,O,O,O,O,O,O,O,O,O,O,O,O,O,O,O,O,O,O,O,O,O,O,O,O,O,O,O,O,O,O,O,
	O,O,O,O,O,O,O,O,O,O,O,O,O,O,O,O,O,O,O,O,O,O,O,O,O,O,O,O,O,O,O,O,O,O,O,O,O,O,O,O,O,O,O,O,O,O,O,O,O,O,O,O,O,O,O,O,O,O,O,O,O,O,O,O,
	O,O,O,O,O,O,O,O,O,O,O,O,O,O,O,O,O,O,O,O,O,O,O,O,O,O,O,O,O,O,O,O,O,O,O,O,O,O,O,O,O,O,O,O,O,O,O,O,O,O,O,O,O,O,O,O,O,O,O,O,O,O,O,O,
	O,O,O,O,O,O,O,O,O,O,O,O,O,O,O,O,O,O,O,O,O,O,O,O,O,O,O,O,O,O,O,O,O,O,O,O,O,O,O,O,O,O,O,X,X,X,X,X,X,X,X,O,O,O,O,O,O,O,O,O,O,O,O,O,
	O,O,O,O,O,O,O,O,O,O,O,O,O,O,O,O,O,O,O,O,O,O,O,O,O,O,O,O,O,O,O,O,O,O,O,O,O,O,O,O,O,X,X,X,X,X,X,X,X,X,X,O,O,O,O,O,O,O,O,O,O,O,O,O,
	O,O,O,O,O,O,O,O,O,O,O,O,O,O,O,O,O,O,O,O,O,O,O,O,O,O,O,O,O,O,O,O,O,O,O,O,O,O,O,O,O,O,O,O,O,O,O,O,O,X,X,O,O,O,O,O,O,O,O,O,O,O,O,O,
	O,O,O,O,O,O,O,O,O,O,O,O,O,O,O,O,O,O,O,O,O,O,O,O,O,O,O,O,O,O,O,O,O,O,O,O,O,O,O,O,O,O,O,O,M,M,O,O,O,X,X,O,O,O,O,O,O,O,O,O,O,O,O,O,
	O,O,O,O,O,O,O,O,O,O,O,O,O,O,O,O,O,O,O,O,O,O,O,O,O,O,O,O,O,O,O,O,O,O,O,O,O,O,O,O,O,O,O,O,M,M,O,O,O,X,X,O,O,O,O,O,O,O,O,O,O,O,O,O,
	O,O,O,O,O,O,O,O,O,O,O,X,X,X,X,O,O,O,O,O,O,O,O,O,O,O,O,O,O,O,O,X,X,X,X,O,O,O,O,O,O,O,O,X,X,X,X,O,O,X,X,O,O,O,O,O,O,O,O,O,O,O,O,O,
	O,O,O,O,O,O,O,O,O,O,X,X,X,X,X,X,O,O,O,O,O,O,O,O,O,O,O,X,X,X,X,X,X,X,X,X,O,O,O,O,O,O,X,X,X,X,X,X,X,X,X,O,O,O,O,O,O,O,O,O,O,O,O,O,
	O,O,O,O,O,O,O,O,O,X,X,X,X,X,X,X,X,X,X,X,X,X,X,X,X,X,X,X,X,X,X,X,X,X,X,X,O,O,O,O,O,X,X,X,X,X,X,X,X,X,X,O,O,O,O,O,O,O,O,O,O,O,O,O,
	O,O,O,O,O,O,O,O,X,X,X,X,O,O,X,X,X,X,X,X,X,X,X,X,X,X,X,X,X,X,X,O,O,O,O,O,O,O,O,O,O,O,O,O,O,O,O,O,O,O,O,O,O,O,O,O,O,O,O,O,O,O,O,O,
	O,O,O,O,O,O,O,X,X,X,O,O,O,O,O,O,O,O,X,X,X,X,X,X,X,X,O,O,O,O,O,O,O,O,O,O,O,O,O,O,O,O,O,O,O,O,O,O,O,O,O,O,O,O,O,O,O,O,O,O,O,O,O,O,
	O,O,O,O,O,O,O,O,O,O,O,O,O,O,O,O,O,O,O,O,O,O,O,O,O,O,O,O,O,O,O,O,O,O,O,O,O,X,X,X,X,O,O,O,O,O,O,O,O,O,O,O,O,O,O,O,O,O,O,O,O,O,O,O,
	O,O,O,O,O,O,O,O,O,O,O,O,O,O,O,O,O,O,O,O,O,O,O,O,O,O,O,O,O,O,O,O,O,O,O,O,X,X,X,X,X,X,O,O,O,O,O,O,O,O,O,O,O,O,O,O,O,O,O,O,O,O,O,O,
	X,X,X,X,X,X,X,X,X,X,X,X,X,X,X,X,X,X,X,X,X,X,X,X,X,X,X,X,X,X,X,X,X,X,X,X,X,X,X,X,X,X,X,X,X,X,X,X,X,X,X,X,X,X,X,X,X,X,X,X,X,X,X,X,
	X,X,X,X,X,X,X,X,X,X,X,X,X,X,X,X,X,X,X,X,X,X,X,X,X,X,X,X,X,X,X,X,X,X,X,X,X,X,X,X,X,X,X,X,X,X,X,X,X,X,X,X,X,X,X,X,X,X,X,X,X,X,X,X,
	}

};

int mapKillCount[2] = { 2,3 };

//-- 맵 --//
Map::Map()
{
	td = new TileData(tileNumX, tileNumY);
	td->tiles = tile[mapCount];
}

Map::~Map()
{
	delete td;
}

void Map::paint(float dt)
{
	for (int i = 0; i < td->tileNum; i++)
	{
		Tile* t = &tile[mapCount][i];
		if (t->attr == 0) //0이면 백	
			continue;	

		switch (t->attr) {
		case 1: setRGBA(0, 0, 0, 1); break;//검정
		case 2: setRGBA(0.2, 0.3, 0.2, 1); break;
		}

		int x = TILE_WIDTH * (i % td->numW) + bg->off.x; 
		int y = TILE_HEIGHT * (i / td->numW) + bg->off.y;
		fillRect(x, y, TILE_WIDTH, TILE_HEIGHT);
	}

	setRGBA(1, 1, 1, 1);
}


//이동거리제한
int maxMoveR(iPoint p, iRect rt, bool player)
{
	int max = devSize.width*2;

	int x = ((int)p.x + rt.size.width / 2) / TILE_WIDTH;
	int sy = ((int)(p.y - rt.size.height)) / TILE_HEIGHT; //위쪽y
	int ey = (int)(p.y) / TILE_HEIGHT; //아래쪽y

	if (sy == ey) // 한칸보다 작을경우
		ey += 1;

	for (int i = x + 1; i < tileNumX; i++)
	{
		bool col = false;
		for (int j = sy; j < ey; j++)
		{
			Tile* t = &tile[mapCount][tileNumX * j + i];
			if (t->attr)
			{
				col = true;
				break;
			}
		}

		if (player == false)
		{
			Tile* t = &tile[mapCount][tileNumX * ey + i];
			if (t->attr == 0)
			{
				col = true;
			}

		}


		if (col)
		{
			max = TILE_WIDTH * i;
			break;
		}
	}

	max -= rt.size.width / 2 + 1;

	return max;
}

int maxMoveL(iPoint p, iRect rt, bool player)
{
	int max = 0;

	int x = ((int)p.x - rt.size.width / 2) / TILE_WIDTH;
	int sy = ((int)(p.y - rt.size.height)) / TILE_HEIGHT; //위쪽y
	int ey = (int)(p.y) / TILE_HEIGHT; //아래쪽y


	if (sy == ey) // 한칸보다 작을경우
		ey += 1;

	for (int i = x - 1; i > -1; i--)
	{
		bool col = false;
		for (int j = sy; j < ey; j++)
		{
			Tile* t = &tile[mapCount][tileNumX * j + i];
			if (t->attr)
			{
				col = true;
				break;
			}
		}

		if (player == false)
		{
			Tile* t = &tile[mapCount][tileNumX * ey + i];
			if (t->attr==0)
			{
				col = true;
			}

		}

		if (col)
		{
			max = TILE_WIDTH * (i + 1);
			break;
		}
	}

 	max += rt.size.width / 2;

	return max;
}

int maxMoveT(iPoint p, iRect rt)
{
	int max = rt.size.height + 1;//화면 바깥으로 못 나가도록

	int y = ((int)p.y - rt.size.height) / TILE_HEIGHT;
#if 1
	int sx = ((int)(p.x - rt.size.width / 2)) / TILE_WIDTH; //왼쪽 x
	int ex = ((int)(p.x + rt.size.width / 2)) / TILE_WIDTH; //오른쪽 x 

	for (int x = sx; x < ex + 1; x++)
	{
		Tile* t = &tile[mapCount][tileNumX * (y-1) + x];
		if (t->attr) //0이아닐때
		{
   			max = TILE_HEIGHT * y;
			max += rt.size.height + 1;
			break;
		}
	}
#endif 
	return max;
}

int maxMoveB(iPoint p, iRect rt)
{
	int max = devSize.height;

	int y = ((int)p.y) / TILE_HEIGHT;

	int sx = ((int)(p.x - rt.size.width / 2)) / TILE_WIDTH; //왼쪽 x
	int ex = ((int)(p.x + rt.size.width / 2)) / TILE_WIDTH; //오른쪽 x
	for (int x = sx; x < ex + 1; x++)
	{
		Tile* t = &tile[mapCount][tileNumX * y + x];
		if (t->attr) //0이아닐때
		{
			max = TILE_HEIGHT * y;
			break;
		}
	}
	
	return max;
}


///수정 전 맵
//Map::Map()
//{
//	td = new TileData(tileNumX, tileNumY);
//	td->tiles = tile;
//}
//
//Map::~Map()
//{
//	delete td;
//}
//
//void Map::paint(float dt)
//{
//
//	for (int i = 0; i < td->tileNum; i++)
//	{
//		Tile* t = &tile[i];
//		if (t->attr == 0) //0이면 백	
//			continue;
//
//		switch (t->attr) {
//		case 1: setRGBA(0, 0, 0, 1); break;//검정
//		case 2: setRGBA(0.2, 0.3, 0.2, 1); break;
//		}
//
//		int x = TILE_WIDTH * (i % td->numW) + bg->off.x;
//		int y = TILE_HEIGHT * (i / td->numW) + bg->off.y;
//		fillRect(x, y, TILE_WIDTH, TILE_HEIGHT);
//	}
//
//	setRGBA(1, 1, 1, 1);
//}
///