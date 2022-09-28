#pragma once

typedef signed char		int8;
typedef unsigned char	uint8;
typedef signed short	int16;
typedef unsigned short	uint16;
typedef signed int		int32;
typedef unsigned int	uint32;

typedef float			float32;
typedef double			float64;

struct Texture;
struct iColor4b;
struct iColor4f;
struct iPoint;
struct iSize;
struct iRect;

enum iKeyState
{
	iKeyStateBegan = 0,
	iKeyStateMoved,
	iKeyStateEnded
};

// anc
#define LEFT	1
#define RIGHT	2
#define HCENTER 4

#define TOP		8
#define BOTTOM	16
#define VCENTER	32

// reverse
#define REVERSE_NONE	1
#define REVERSE_WIDTH	2
#define REVERSE_HEIGHT	4

