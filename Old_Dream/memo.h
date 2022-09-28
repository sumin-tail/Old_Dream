#pragma once


//함수 포인터 
typedef void (*MethodScene)(float dt);

// h
void addScene(MethodScene m);
MethodScene nextScene();

// cpp
MethodScene* ms;
int msNum, msIndex;
#define _msNum 100
MethodScene msCurr;

void loadSene()
{
	ms = new MethodScene[_msNum];
	msNum = 0;
	msIndex = 0;
	msCurr = 0;
}

void freeScene()
{
	delete ms;
}

void addScene(MethodScene m)
{
	if (msCurr == 0)
	{
		msCurr = m;
		msIndex = 0;
	}

	ms[msNum] = m;
	msNum++;
}

MethodScene nextScene()
{
	msIndex++;
	if (msIndex > msNum - 1)
		msIndex = 0;

	msCurr = ms[msIndex];
	return msCurr;
}
void drawScene(float dt)
{
	if (msCurr)
		msCurr(dt);
}
