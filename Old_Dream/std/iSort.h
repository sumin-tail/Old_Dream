#pragma once

struct SortData
{
	int index;
	int value;
};

class iSort
{
public:
	iSort();
	virtual ~iSort();

	void init();
	void add(int value);
	void update();

	int get(int index);

public:
	SortData* sd;
	int sdNum;
};

#define sdMax 300
