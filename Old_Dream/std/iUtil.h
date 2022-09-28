#pragma once

// ================
// FILE IO
// ================
void saveFile(const char* fileName, char* buf, int len);
char* loadFile(const char* fileName, int& len);

// ================
// .... 
// ================
//float _sin(float degree);
//float _cos(float degree);
#define _sin(degree) sinf(M_PI / 180 * (degree))
#define _cos(degree) cosf(M_PI / 180 * (degree))

