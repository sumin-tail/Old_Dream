#pragma once

#include <stdio.h>

struct iNode
{
	void* data;
	iNode* prev;
};

typedef void (*MethodArray)(void* data);

class iArray
{
public:
	iArray(MethodArray ma = NULL);
	virtual ~iArray();

	void addObject(void* data);
	void addObject(int index, void* data);

	void removeObject(int index);
	void removeObject(void* data);
	void removeAllObject();

	void* objectAtIndex(int index);

public:
	iNode* node;
	int count;

	MethodArray method;
};