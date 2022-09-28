#include "iArray.h"

#include "iStd.h"

iArray::iArray(MethodArray ma)
{
	node = NULL;
	count = 0;

	method = ma;
}

iArray::~iArray()
{
	removeAllObject();
}

void iArray::addObject(void* data)
{
	iNode* n = new iNode;
	n->data = data;
	n->prev = node;

	node = n;
	count++;
}

void iArray::addObject(int index, void* data)
{
	iNode* n = node;
	for (int i = count - 1; i > 0; i--)
	{
		if (i == index)
		{
			iNode* m = new iNode;
			m->data = data;
			m->prev = n->prev;

			n->prev = m;// = new 
			count++;
			return;
		}
		n = n->prev;
	}
	// index < 0 || index >= count

#if 1
	addObject(data);
#else
	if (index == count)
		addObject(data);
	else {}// if( index < 0 || index > count+1 )
#endif
}

void iArray::removeObject(int index)
{
	iNode* n = node;
	iNode* prevN = NULL;
	for (int i = count - 1; i > 0; i--)
	{
		if (i == index)
		{
			if (prevN)
				prevN->prev = n->prev;
			if (method)
				method(n->data);
			delete n;
			return;
		}
		prevN = n;
		n = n->prev;
	}
}

void iArray::removeObject(void* data)
{
	iNode* n = node;
	iNode* prevN = NULL;
	for (int i = count - 1; i > 0; i--)
	{
		if (n->data == data)//
		{
			if (prevN)
				prevN->prev = n->prev;
			if (method)
				method(n->data);
			delete n;
			//return;
		}
		prevN = n;
		n = n->prev;
	}
}

void iArray::removeAllObject()
{
	while (1)
	{
		iNode* n = node->prev;
		if (method)
			method(node->data);
		delete node;
		node = n;
		if (node == NULL)
			break;
	}
	count = 0;
}

void* iArray::objectAtIndex(int index)
{
	iNode* n = node;
	for (int i = 0; i < count; i++)
	{
		if (count - 1 - i == index)
			return n->data;
		n = n->prev;
	}

	return NULL;
}
