#pragma once

#include "iStd.h"

#if 0

������Ʈ ������ �����Ұ�.
1. �߰� ���丮
2.	_CRT_SECURE_NO_WARNINGS
_USE_MATH_DEFINES
GDIPVER = 0x0110
GLEW_STATIC

glew.c ������Ʈ ����
���� ��������� �Ʒ���ο� ����
C : \Program Files(x86)\Windows Kits\10\Include\10.0.19041.0\um\gl
#endif

void loadOpenGL(void* hdc);
void freeOpenGL();
typedef void (*MethodDraw)(float dt);
void drawOpenGL(float dt, MethodDraw m);
void resizeOpenGL(int width, int height);

void loadCursor();
void freeCursor();
void setCursor(iPoint position);
bool updateCursor(bool inClient);
void drawCursor(float dt);
